//
//  NBSrcAnlzExprParse.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzExprParse.h"

BOOL SrcAnlzExprParser_feedTokns(const STSrcAnlzTokns* tokns, const STSrcAnlzExprOpSintxMap* map, const SI32 pFirstTokn, const SI32 tokensCount, STNBArray* dstOperations /*STSrcAnlzExprParseNode*/){
	BOOL r = FALSE;
	SI32 iToken = pFirstTokn;
	if(!SrcAnlzExprParser_feedToknsPlain(tokns, map, &iToken, tokensCount, STSrcAnlzExprParseReadMode_Level, NULL, 0, dstOperations)){
		PRINTF_ERROR("Could not parse EXPRESSION's sintax.\n");
		NBASSERT(FALSE)
	} else if(iToken != tokensCount){
		PRINTF_ERROR("Logic error after parsing EXPRESSION's sintax (not all tokens were processed).\n");
		NBASSERT(FALSE)
	} else {
		//
		//Final step: merge operators that are
		//defined to extend previous operands,
		//and operators that are RightOnly.
		//By example: functions params currently are defined as:
		//
		// '!'        'funcName'     '('                'funcParams'
		// R-only     op              L+R new level     ops
		//
		// After this, they should be defined as:
		//
		// '!'         (             'funcNam'    '('                'funcParams'
		// R-only      new-level    op           L+R new level      ops
		//
		SI32 i; for(i = 0; i < dstOperations->use; i++){
			STSrcAnlzExprParseNode* op = NBArray_itmPtrAtIndex(dstOperations, STSrcAnlzExprParseNode, i);
			NBASSERT(op->sintaxMap != NULL)
			//PRINTF_INFO("OP #%d / %d: '%s' (+%d %s)\n", (i + 1), dstOperations->use, (op->iCharDef != -1 ? "operator" : "operand"), (op->iCharDef != -1 ? op->sintax.rCount : 1), (op->iCharDef != -1 ? "ops" : "tokens"));
			if(op->iCharDef != -1){
				//Is operator
				NBASSERT(op->sintax.iDef != -1)
				const STSrcAnlzExprOpSintx* sintax = &op->sintaxMap->opSintaxs[op->sintax.iDef];
				//Merge this right-only op with its right's side
				if(sintax->sidesMask == ENSrcAnlzExprOpSideMask_Right && op->sintax.rCount == 0){
					if((i + 1) < dstOperations->use){
						const STSrcAnlzExprParseNode* nextOp = NBArray_itmPtrAtIndex(dstOperations, STSrcAnlzExprParseNode, i + 1);
						if(nextOp->iCharDef == -1){
							op->sintax.rCount = 1;
						} else {
							op->sintax.rCount = 1 + nextOp->sintax.rCount;
						}
						//PRINTF_INFO("OP #%d / %d: '%s' (+%d %s) [AFTER MERGING WITH NEXT OP]\n", (i + 1), dstOperations->use, (op->iCharDef != -1 ? "operator" : "operand"), (op->iCharDef != -1 ? op->sintax.rCount : 1), (op->iCharDef != -1 ? "ops" : "tokens"));
					}
				}
				//Merge this op with previous,
				//and extends any previous op that touches this range
				if(sintax->seqBehavior == ENSrcAnlzExprOpSeqType_Extends){
					if(i > 0){
						SI32 rngStart = i - 1;
						const SI32 rngEnd = (i + op->sintax.rCount);
						NBASSERT(NBArray_itmPtrAtIndex(dstOperations, STSrcAnlzExprParseNode, i - 1)->iCharDef == -1) //Prev op must be a operand
						//Update prevs ranges
						SI32 i2; for(i2 = (i - 2); i2 >= 0; i2--){ //-2 asuming prev is an operand
							STSrcAnlzExprParseNode* prevOp = NBArray_itmPtrAtIndex(dstOperations, STSrcAnlzExprParseNode, i2);
							if(prevOp->iCharDef != -1){
								const SI32 prevEnd = i2 + prevOp->sintax.rCount;
								if(prevEnd >= rngStart && prevEnd < rngEnd){
									prevOp->sintax.rCount = rngEnd - i2;
									//PRINTF_INFO("OP #%d / %d: '%s' (+%d %s) [AFTER EXTENDING]\n", (i2 + 1), dstOperations->use, (prevOp->iCharDef != -1 ? "operator" : "operand"), (prevOp->iCharDef != -1 ? prevOp->sintax.rCount : 1), (prevOp->iCharDef != -1 ? "ops" : "tokens"));
								}
							}
						}
					}
				}
			}
		}
		r = TRUE;
	}
	return r;
}

BOOL SrcAnlzExprParser_feedToknsPlain(const STSrcAnlzTokns* tokns, const STSrcAnlzExprOpSintxMap* map, SI32* firstTokn, const SI32 tokensCount, const STSrcAnlzExprParseReadMode readMode, const char* popLevelChars, const SI32 depthLevel, STNBArray* dstOperations /*STSrcAnlzExprParseNode*/){
	BOOL r = FALSE;
	//
	STSrcAnlzExprParseNode curOp;	//Current definition
	curOp.sintaxMap			= map;
	curOp.iCharDef			= -1;
	curOp.iToken			= -1;
	BOOL explicitPop		= FALSE;
	SI32 qOpsAtLevel		= 0;	//Quantity of operands added at this depth level
	//
	STSrcAnlzTokn* toknDefs	= (STSrcAnlzTokn*)tokns->defs._buffData;
	const char* tokensBuff	= tokns->strBuff.str; //ToDo: remove after testing:(tokensSrc == ENSrcAnlzExprSrc_MacrosValues ? state->macros.valsTokens.str : parCodeState->acumTokensCur->strBuff.str);
	//
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	//char dbgSpacesBuff[128];
#	endif
	//
	SI32 iToken = *firstTokn;
	NBASSERT(iToken >= 0)
	if(iToken >= 0){
		r = TRUE;
		while(iToken < tokensCount && r){
			const SI32 iFirstTokn	= iToken++;
			const SI32 iAtBuff		= toknDefs[iFirstTokn].iAtBuff; //ToDo: remove after testing: (tokensSrc == ENSrcAnlzExprSrc_MacrosValues ? NBArray_itmPtrAtIndex(&state->macros.valsTokensDefs, STSrcAnlzMacroValTokn, iFirstTokn)->iAtBuff : NBArray_itmPtrAtIndex(&parCodeState->acumTokensCur->defs, STSrcAnlzTokn, iFirstTokn)->iAtBuff);
			const char* token		= &tokensBuff[iAtBuff];
			const SI32 tokenLen		= NBString_strLenBytes(token);
#			ifdef NB_CONFIG_INCLUDE_ASSERTS
			//PRINTF_INFO("%sOp-Token: '%s'.\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), token);
#			endif
			//---------------
			//- Pop one level
			//---------------
			if(popLevelChars != NULL){
				NBASSERT(popLevelChars[0] != '\0')
				if(popLevelChars[0] != '\0'){
					if(NBString_strIsEqual(token, popLevelChars)){
						if(readMode == STSrcAnlzExprParseReadMode_Operand){
							//If this is an operand level, the
							//pop-chars must be processed by parent level.
							iToken--;
						}
						//Return to POP in recursivity.
						explicitPop = TRUE;
						break;
					}
				}
			}
			//-----------------
			//- Is an operator?
			//-----------------
			SI32 opIndex = -1;
			{
				//Search 1-char operators
				if(tokenLen == 1 && map->opsChar1 != NULL){
					SI32 i; const SI32 count = map->opsChar1Sz;
					for(i = 0; i < count; i++){
						const SI32 iOp = map->opsChar1[i]; NBASSERT(iOp >= 0 && iOp < map->opCharsSz)
						const STSrcAnlzExprOp* opDef = &map->opChars[iOp]; NBASSERT(opDef->uniqueID == iOp)
						NBASSERT(opDef->chars[0] != '\0' && opDef->chars[1] == '\0')
						if(opDef->chars[0] == token[0]){
							opIndex = iOp;
							break;
						}
					}
				}
				//Search 2-chars operators
				if(tokenLen == 2 && map->opsChars2 != NULL){
					SI32 i; const SI32 count = map->opsChars2Sz;
					for(i = 0; i < count; i++){
						const SI32 iOp = map->opsChars2[i]; NBASSERT(iOp >= 0 && iOp < map->opCharsSz)
						const STSrcAnlzExprOp* opDef = &map->opChars[iOp];
						NBASSERT(opDef->uniqueID == iOp)
						NBASSERT(opDef->chars[0] != '\0' && opDef->chars[1] != '\0' && opDef->chars[2] == '\0')
						if(opDef->chars[0] == token[0] && opDef->chars[1] == token[1]){
							opIndex = iOp;
							break;
						}
					}
				}
				//Search 3-chars operators
				if(tokenLen == 3 && map->opsChars3 != NULL){
					SI32 i; const SI32 count = map->opsChars3Sz;
					for(i = 0; i < count; i++){
						const SI32 iOp = map->opsChars3[i]; NBASSERT(iOp >= 0 && iOp < map->opCharsSz)
						const STSrcAnlzExprOp* opDef = &map->opChars[iOp];
						NBASSERT(opDef->uniqueID == iOp)
						NBASSERT(opDef->chars[0] != '\0' && opDef->chars[1] != '\0' && opDef->chars[2] != '\0' && opDef->chars[3] == '\0')
						if(opDef->chars[0] == token[0] && opDef->chars[1] == token[1] && opDef->chars[2] == token[2]){
							opIndex = iOp;
							break;
						}
					}
				}
				//Search n-chars operators
				if(tokenLen > 3 && map->opsCharsN != NULL){
					SI32 i; const SI32 count = map->opsCharsNSz;
					for(i = 0; i < count; i++){
						const SI32 iOp = map->opsCharsN[i]; NBASSERT(iOp >= 0 && iOp < map->opCharsSz)
						const STSrcAnlzExprOp* opDef = &map->opChars[iOp];
						NBASSERT(opDef->uniqueID == iOp)
						if(NBString_strIsEqual(opDef->chars, token)){
							opIndex = iOp;
							break;
						}
					}
				}
			}
			//---------------------------------
			//- Process token (operand or operator)
			//---------------------------------
			{
				const BOOL isOperator		= (opIndex != -1);
				const BOOL prevIsOperator	= (curOp.iCharDef != -1);
				const BOOL hasLeftOperand	= (qOpsAtLevel != 0);
				NBASSERT(iToken == (iFirstTokn + 1))
				if(!isOperator){
					//
					//Process operand
					//
					if(!prevIsOperator){
						//
						//add content to prev (or new) operand
						//
						NBASSERT(curOp.iCharDef == -1)
						if(curOp.iToken == -1){
							curOp.iToken = iFirstTokn;
						}
					} else {
						//
						//determine prev operator sintax
						//
						NBASSERT(curOp.iCharDef != -1)
						NBASSERT(curOp.sintax.iDef == -1)
						NBASSERT(curOp.sintax.rCount == 0)
						if(curOp.sintax.iDef != -1){
							PRINTF_ERROR("Logic error, operand found after closed-operator.\n");
							r = FALSE; NBASSERT(FALSE);
							break;
						} else {
							//Define operator sintax
							const STSrcAnlzExprOp* opDef = &map->opChars[curOp.iCharDef]; NBASSERT(opDef->uniqueID == curOp.iCharDef)
							if(hasLeftOperand){
								//Prev operator must be left+right (both sides are required when left operand exists)
								if(opDef->op1 != -1 && map->opSintaxs[opDef->op1].sidesMask == ENSrcAnlzExprOpSideMask_Both){
									curOp.sintax.iDef = opDef->op1;
#									ifdef NB_CONFIG_INCLUDE_ASSERTS
									//PRINTF_INFO("%sSetting operand with sintax1(%d) '%s%s%s'.\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), opDef->op1, NBSRC_ANLZ_OP_CHARS_REQ_SIDES1(map, opDef, ENSrcAnlzExprOpSideMask_Left) ? "left ": "...", opDef->chars, NBSRC_ANLZ_OP_CHARS_REQ_SIDES1(map, opDef, ENSrcAnlzExprOpSideMask_Right) ? " right" : " ...");
#									endif
								}
								if(curOp.sintax.iDef == -1){
									if(opDef->op2 != -1 && map->opSintaxs[opDef->op2].sidesMask == ENSrcAnlzExprOpSideMask_Both){
										curOp.sintax.iDef = opDef->op2;
#										ifdef NB_CONFIG_INCLUDE_ASSERTS
										//PRINTF_INFO("%sSetting operand with sintax2(%d) '%s%s%s'.\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), opDef->op2, NBSRC_ANLZ_OP_CHARS_REQ_SIDES2(map, opDef, ENSrcAnlzExprOpSideMask_Left) ? "left ": "...", opDef->chars, NBSRC_ANLZ_OP_CHARS_REQ_SIDES2(map, opDef, ENSrcAnlzExprOpSideMask_Right) ? " right" : " ...");
#										endif
									}
								}
								if(curOp.sintax.iDef == -1){
									PRINTF_ERROR("Unexpected operand after '%s' operator (operador does not suppoer left+right sintax).\n", opDef->chars);
									r = FALSE; NBASSERT(FALSE);
									break;
								}
							} else {
								//Prev operator must be right-only (right required when left operand does not exists)
								if(opDef->op1 != -1 && map->opSintaxs[opDef->op1].sidesMask == ENSrcAnlzExprOpSideMask_Right){
									curOp.sintax.iDef = opDef->op1;
#									ifdef NB_CONFIG_INCLUDE_ASSERTS
									//PRINTF_INFO("%sSetting operand with sintax1(%d) '%s%s%s'.\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), opDef->op1, NBSRC_ANLZ_OP_CHARS_REQ_SIDES1(map, opDef, ENSrcAnlzExprOpSideMask_Left) ? "left ": "...", opDef->chars, NBSRC_ANLZ_OP_CHARS_REQ_SIDES1(map, opDef, ENSrcAnlzExprOpSideMask_Right) ? " right" : " ...");
#									endif
								}
								if(curOp.sintax.iDef == -1){
									if(opDef->op2 != -1 && map->opSintaxs[opDef->op2].sidesMask == ENSrcAnlzExprOpSideMask_Right){
										curOp.sintax.iDef = opDef->op2;
#										ifdef NB_CONFIG_INCLUDE_ASSERTS
										//PRINTF_INFO("%sSetting operand with sintax2(%d) '%s%s%s'.\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), opDef->op2, NBSRC_ANLZ_OP_CHARS_REQ_SIDES2(map, opDef, ENSrcAnlzExprOpSideMask_Left) ? "left ": "...", opDef->chars, NBSRC_ANLZ_OP_CHARS_REQ_SIDES2(map, opDef, ENSrcAnlzExprOpSideMask_Right) ? " right" : " ...");
#										endif
									}
								}
								if(curOp.sintax.iDef == -1){
									PRINTF_ERROR("Inexpected operand after '%s' operator (operador does not suppoer left+right sintax).\n", opDef->chars);
									r = FALSE; NBASSERT(FALSE);
									break;
								}
							}
							NBASSERT(curOp.sintax.iDef != -1)
							if(curOp.sintax.iDef == -1){
								PRINTF_ERROR("Could not define operator's sintax before '%s'.\n", opDef->chars);
								r = FALSE; NBASSERT(FALSE);
								break;
							} else {
								//Add prev operator
								NBASSERT(curOp.sintaxMap != NULL)
								NBASSERT(curOp.iCharDef != -1)
								NBASSERT(curOp.sintax.iDef != -1)
#								ifdef NB_CONFIG_INCLUDE_ASSERTS
								//PRINTF_INFO("%sAdding operator ('%s') found before operand.\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), &tokensBuff[toknDefs[curOp.iToken].iAtBuff]);
#								endif
								NBArray_addValue(dstOperations, curOp);
								//Start operand
								curOp.iCharDef	= -1;
								curOp.iToken	= iFirstTokn;
							}
						}
					}
				} else {
					//
					//Process operand
					//
					ENSrcAnlzExprOpSideMask prevOpSidesMask	= ENSrcAnlzExprOpSideMask_None;
					if(!prevIsOperator){
						//
						//Close prev operand (if available)
						//
						NBASSERT(curOp.iCharDef	== -1)
						if(curOp.iToken != -1){
							NBASSERT(curOp.sintaxMap != NULL)
#							ifdef NB_CONFIG_INCLUDE_ASSERTS
							//PRINTF_INFO("%sAdding operand ('%s') found before operator.\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), &tokensBuff[toknDefs[curOp.iToken].iAtBuff]);
#							endif
							NBArray_addValue(dstOperations, curOp);
							qOpsAtLevel++;
							//Reading only one operand?
							if(readMode == STSrcAnlzExprParseReadMode_Operand){
								//Reset op
								curOp.iCharDef	= -1;
								curOp.iToken	= -1;
								//Reset position to this operator (the one that closed the operand)
								iToken = iFirstTokn;
								break;
							}
						}
					} else {
						//
						//Found second-continous-operator
						//Close the prev operator
						//
						NBASSERT(curOp.sintax.iDef == -1)
						const STSrcAnlzExprOp* prevDef = &map->opChars[curOp.iCharDef]; NBASSERT(prevDef->uniqueID == curOp.iCharDef)
						//Found second operator, try to close prev as right-only
						if(prevDef->op1 != -1 && map->opSintaxs[prevDef->op1].sidesMask == ENSrcAnlzExprOpSideMask_Right){
							curOp.sintax.iDef = prevDef->op1;
						}
						if(curOp.sintax.iDef == -1){
							if(prevDef->op2 != -1 && map->opSintaxs[prevDef->op2].sidesMask == ENSrcAnlzExprOpSideMask_Right){
								curOp.sintax.iDef = prevDef->op2;
							}
						}
						if(curOp.sintax.iDef == -1 && hasLeftOperand){
							//Found second operator, with a left operand, try to close prev as left-only operator (sufix has higher priority)
							if(prevDef->op1 != -1 && map->opSintaxs[prevDef->op1].sidesMask == ENSrcAnlzExprOpSideMask_Left){
								curOp.sintax.iDef = prevDef->op1;
							}
							if(curOp.sintax.iDef == -1){
								if(prevDef->op2 != -1 && map->opSintaxs[prevDef->op2].sidesMask == ENSrcAnlzExprOpSideMask_Left){
									curOp.sintax.iDef = prevDef->op2;
								}
							}
							//Could not close as left-only, try to close prev as left+right operator (if not left-only, both sides are required when left operand exists)
							if(curOp.sintax.iDef == -1){
								if(prevDef->op1 != -1 && map->opSintaxs[prevDef->op1].sidesMask == ENSrcAnlzExprOpSideMask_Both){
									curOp.sintax.iDef = prevDef->op1;
								}
								if(curOp.sintax.iDef == -1){
									if(prevDef->op2 != -1 && map->opSintaxs[prevDef->op2].sidesMask == ENSrcAnlzExprOpSideMask_Both){
										curOp.sintax.iDef = prevDef->op2;
									}
								}
							}
						}
						//Results of closing previous open-operator
						if(curOp.sintax.iDef == -1){
							PRINTF_ERROR("Missing operand between operator '%s%s%s' and '%s'.\n", (hasLeftOperand ? "left ":"(left-optional)"), prevDef->chars, (!hasLeftOperand ? "right":"(right-optional)"), map->opChars[opIndex].chars);
							r = FALSE; NBASSERT(FALSE);
							break;
						} else {
							prevOpSidesMask = map->opSintaxs[curOp.sintax.iDef].sidesMask;
							//Add prev operator
							NBASSERT(curOp.sintaxMap != NULL)
							NBASSERT(curOp.sintax.iDef != -1)
							NBASSERT(curOp.iCharDef != -1)
#							ifdef NB_CONFIG_INCLUDE_ASSERTS
							//PRINTF_INFO("%sAdding nested operator '%s' followeb by '%s' {'%s%s%s' iToken(%d)}.\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), (curOp.sintax.iDef != -1 ? map->opSintaxs[curOp.sintax.iDef].opName : "none"), map->opChars[opIndex].chars, (prevOpSidesMask & ENSrcAnlzExprOpSideMask_Left ? "left ": "..."), prevDef->chars, (prevOpSidesMask & ENSrcAnlzExprOpSideMask_Right ? " right": "..."), curOp.iToken);
#							endif
							NBArray_addValue(dstOperations, curOp);
						}
					}
					const BOOL prevIsOperand = (curOp.iCharDef == -1 && qOpsAtLevel != 0);
					//Set new operator
					curOp.iCharDef		= opIndex;
					curOp.sintax.iDef	= -1;
					curOp.sintax.rCount	= 0;
					//Push one level depper (if necesary) and read right side
					{
						const STSrcAnlzExprOp* opDef = &map->opChars[curOp.iCharDef]; NBASSERT(opDef->uniqueID == curOp.iCharDef)
						const BOOL isPushDepthOp	= (opDef->popChars[0] != '\0');
						const BOOL prevOpNeedsRight = (prevIsOperator && (prevOpSidesMask & ENSrcAnlzExprOpSideMask_Right));
						if(!(isPushDepthOp || prevOpNeedsRight)){
#							ifdef NB_CONFIG_INCLUDE_ASSERTS
							//PRINTF_INFO("%sFound operator('%s').\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), opDef->chars);
#							endif
						} else {
							//Set sintax (to determine the next sintax's map)
							{
								NBASSERT(curOp.sintax.iDef == -1)
								//Determining testing order
								//When previous was an operand, test (left+right) first: ex: myFunc(a)
								//When previous was an operator, test (left+right) first: ex: a + (b * c)
								ENSrcAnlzExprOpSideMask firstMask = (prevIsOperand ? ENSrcAnlzExprOpSideMask_Both : ENSrcAnlzExprOpSideMask_Right);
								ENSrcAnlzExprOpSideMask secndMask = (prevIsOperand ? ENSrcAnlzExprOpSideMask_Right : ENSrcAnlzExprOpSideMask_Both);
								//Try first
								if(opDef->op1 != -1 && map->opSintaxs[opDef->op1].sidesMask == firstMask){
									curOp.sintax.iDef = opDef->op1;
								}
								if(curOp.sintax.iDef == -1){
									if(opDef->op2 != -1 && map->opSintaxs[opDef->op2].sidesMask == firstMask){
										curOp.sintax.iDef = opDef->op2;
									}
								}
								//Test second
								if(curOp.sintax.iDef == -1 && hasLeftOperand){
									//Close this as left+right operator (both sides are required)
									if(opDef->op1 != -1 && map->opSintaxs[opDef->op1].sidesMask == secndMask){
										curOp.sintax.iDef = opDef->op1;
									}
									if(curOp.sintax.iDef == -1){
										if(opDef->op2 != -1 && map->opSintaxs[opDef->op2].sidesMask == secndMask){
											curOp.sintax.iDef = opDef->op2;
										}
									}
								}
								if(curOp.sintax.iDef == -1){
									PRINTF_ERROR("New operator '%s', could not determine sintax's.\n", opDef->chars);
									r = FALSE; NBASSERT(FALSE);
									break;
								}
								NBASSERT(curOp.sintax.iDef != -1)
							}
							//Define new level map
							NBASSERT(curOp.sintax.iDef >= 0 && curOp.sintax.iDef < map->opSintaxsSz)
							const STSrcAnlzExprOpSintxMap* newLevelMap = map->opSintaxs[curOp.sintax.iDef].pushMap;
							if(newLevelMap == NULL){
								newLevelMap = map; //use current map
							}
							//Add operator before addin the new level
							NBASSERT(curOp.sintaxMap != NULL)
							NBASSERT(curOp.iCharDef != -1)
							NBASSERT(curOp.sintax.iDef != -1)
							const SI32 thisOpIndex = dstOperations->use;
#							ifdef NB_CONFIG_INCLUDE_ASSERTS
							//PRINTF_INFO("%sAdding new depper operator('%s').\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), map->opSintaxs[curOp.sintax.iDef].opName);
#							endif
							NBArray_addValue(dstOperations, curOp);
#							ifdef NB_CONFIG_INCLUDE_ASSERTS
							//PRINTF_INFO("%s+pushing operator('%s%s%s') depper one %s (to level %d, with %s map).\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), (map->opSintaxs[curOp.sintax.iDef].sidesMask & ENSrcAnlzExprOpSideMask_Left ? "left ": "..."), opDef->chars, (map->opSintaxs[curOp.sintax.iDef].sidesMask & ENSrcAnlzExprOpSideMask_Right ? " right": "..."), (isPushDepthOp ? "level" : "operator"), (depthLevel + 1), (newLevelMap != map ? "NEW" : "SAME"));
#							endif
							if(!SrcAnlzExprParser_feedToknsPlain(tokns, newLevelMap, &iToken, tokensCount, (isPushDepthOp ? STSrcAnlzExprParseReadMode_Level : STSrcAnlzExprParseReadMode_Operand), (isPushDepthOp ? opDef->popChars : popLevelChars), (depthLevel + 1), dstOperations)){
								r = FALSE; NBASSERT(FALSE);
								break;
							} else {
								//Update parent OP's right-ops count
								{
									STSrcAnlzExprParseNode* thisParentOp = NBArray_itmPtrAtIndex(dstOperations, STSrcAnlzExprParseNode, thisOpIndex);
									NBASSERT(thisParentOp->sintaxMap == curOp.sintaxMap)
									NBASSERT(thisParentOp->iCharDef == curOp.iCharDef)
									NBASSERT(thisParentOp->sintax.iDef == curOp.sintax.iDef)
									NBASSERT(thisParentOp->sintax.rCount == 0)
									thisParentOp->sintax.rCount = (dstOperations->use - thisOpIndex - 1); NBASSERT(thisParentOp->sintax.rCount >= 0)
								}
#								ifdef NB_CONFIG_INCLUDE_ASSERTS
								//PRINTF_INFO("%s+poped operator('%s%s%s') with %d child operations.\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), (map->opSintaxs[curOp.sintax.iDef].sidesMask & ENSrcAnlzExprOpSideMask_Left ? "left ": "..."), opDef->chars, (map->opSintaxs[curOp.sintax.iDef].sidesMask & ENSrcAnlzExprOpSideMask_Right ? " right": "..."), (dstOperations->use - thisOpIndex));
#								endif
								//Reset op
								curOp.iCharDef		= -1;
								curOp.iToken		= -1;
								qOpsAtLevel++;
								//Note: the content of the new level is considerated an operand
								//Reading only one operator?
								if(readMode == STSrcAnlzExprParseReadMode_Operand){
									break;
								}
							}
						}
					}
				}
			}
		} //while
	} //if
	//
	// Process remaining content
	//
	if(r){
		if(curOp.iCharDef == -1){
			//Last operand
			if(curOp.iToken != -1){
				NBASSERT(curOp.sintaxMap != NULL)
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
				//PRINTF_INFO("%sAdding remainig-operand ('%s').\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), &tokensBuff[toknDefs[curOp.iToken].iAtBuff]);
#				endif
				NBArray_addValue(dstOperations, curOp);
				qOpsAtLevel++;
			}
		} else if(qOpsAtLevel != 0){
			//Last operator (must be a left side only)
			NBASSERT(curOp.sintax.iDef == -1)
			NBASSERT(curOp.sintax.rCount == 0)
			const STSrcAnlzExprOp* opDef = &map->opChars[curOp.iCharDef]; NBASSERT(opDef->uniqueID == curOp.iCharDef)
			//Prev operator must be left+right (both sides are required when left operand exists)
			if(opDef->op1 != -1 && map->opSintaxs[opDef->op1].sidesMask == ENSrcAnlzExprOpSideMask_Left){
				curOp.sintax.iDef = opDef->op1;
				//PRINTF_INFO("Setting last-operator with sintax1(%d) '%s%s%s'.\n", opDef->op1, NBSRC_ANLZ_OP_CHARS_REQ_SIDES1(map, opDef, ENSrcAnlzExprOpSideMask_Left) ? "left ": "...", opDef->chars, NBSRC_ANLZ_OP_CHARS_REQ_SIDES1(map, opDef, ENSrcAnlzExprOpSideMask_Right) ? " right" : " ...");
			}
			if(curOp.sintax.iDef == -1){
				if(opDef->op2 != -1 && map->opSintaxs[opDef->op2].sidesMask == ENSrcAnlzExprOpSideMask_Left){
					curOp.sintax.iDef = opDef->op2;
					//PRINTF_INFO("Setting last-operator with sintax2(%d) '%s%s%s'.\n", opDef->op2, NBSRC_ANLZ_OP_CHARS_REQ_SIDES2(map, opDef, ENSrcAnlzExprOpSideMask_Left) ? "left ": "...", opDef->chars, NBSRC_ANLZ_OP_CHARS_REQ_SIDES2(map, opDef, ENSrcAnlzExprOpSideMask_Right) ? " right" : " ...");
				}
			}
			if(curOp.sintax.iDef == -1){
				PRINTF_ERROR("Unexpected operator after '%s' operator (operador does not suppoer left-only sintax).\n", opDef->chars);
				r = FALSE; NBASSERT(FALSE);
			} else {
				NBASSERT(curOp.sintaxMap != NULL)
				NBASSERT(curOp.iCharDef != -1)
				NBASSERT(curOp.sintax.iDef != -1)
				NBASSERT(curOp.sintax.rCount == 0)
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
				//PRINTF_INFO("%sAdding remainig-operator ('%s').\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), &tokensBuff[toknDefs[curOp.iToken].iAtBuff]);
#				endif
				NBArray_addValue(dstOperations, curOp);
			}
		}
		//
		if((readMode == STSrcAnlzExprParseReadMode_Level && (!explicitPop && popLevelChars != NULL))){
			//Explicit pop is required
			PRINTF_ERROR("Expected EXPLICIT-POP at scope in #%d deeplevel.\n", depthLevel);
			r = FALSE; NBASSERT(FALSE);
		} else if(readMode == STSrcAnlzExprParseReadMode_Operand && qOpsAtLevel == 0){
			PRINTF_ERROR("Empty scope found in EXPRESSION at #%d deeplevel.\n", depthLevel);
			r = FALSE; NBASSERT(FALSE);
		} else {
#			ifdef NB_CONFIG_INCLUDE_ASSERTS
			//PRINTF_INFO("%s-popping back (to level %d).\n", NBString_strRepeatByte(dbgSpacesBuff, '\t', depthLevel + 1), (depthLevel - 1));
#			endif
			r = TRUE;
		}
	}
	*firstTokn = iToken;
	return r;
}

//
//Debug
//

#ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
BOOL SrcAnlzExprParser_dbgAddOpsToStr(const STSrcAnlzTokns* tokns, STNBString* dstStr, SI32* opIndex, const STSrcAnlzExprParseNode* ops, const SI32 opsCount, const BOOL noSpaces){
	BOOL r = FALSE;
	//
	STSrcAnlzTokn* toknDefs	= (STSrcAnlzTokn*)tokns->defs._buffData;
	const char* tokensBuff	= tokns->strBuff.str; //ToDo: remove after testing: (tokensSrc == ENSrcAnlzExprSrc_MacrosValues ? state->macros.valsTokens.str : parCodeState->acumTokensCur->strBuff.str);
	//
	SI32 iOP = *opIndex;
	if(iOP >= 0 && iOP < opsCount){
		r = TRUE;
		//Process this depth level
		while(iOP < opsCount){
			const STSrcAnlzExprParseNode* op = &ops[iOP++];
			if(op->iCharDef == -1){
				//plain text operand
				NBASSERT(op->iCharDef == -1)
				NBASSERT(op->iToken != -1)
				if(op->iToken == -1){
					//should not be a empty operand
					//PRINTF_INFO("Operand-plain-empty #%d iToken(%d).\n", iOP, op->iToken);
					NBASSERT(FALSE)
					r = FALSE;
					break;
				} else {
					//An operand can have multiple tokens
					const char* name = &(tokensBuff[toknDefs[op->iToken].iAtBuff]);
					if(!noSpaces) NBString_concatByte(dstStr, ' ');
					NBString_concat(dstStr, name);
					//ToDo: remove after testing:
					/*if(tokensSrc == ENSrcAnlzExprSrc_MacrosValues){
						const STSrcAnlzMacroValTokn* toknDef = NBArray_itmPtrAtIndex(&state->macros.valsTokensDefs, STSrcAnlzMacroValTokn, op->iToken);
						const char* name = &(tokensBuff[toknDef->iAtBuff]);
						if(!noSpaces) NBString_concatByte(&dstStr, ' ');
						NBString_concat(&dstStr, name);
					} else {
						const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&parCodeState->acumTokensCur->defs, STSrcAnlzTokn, op->iToken);
						const char* name = &(tokensBuff[toknDef->iAtBuff]);
						if(!noSpaces) NBString_concatByte(&dstStr, ' ');
						NBString_concat(&dstStr, name);
					}*/
				}
			} else {
				//operator
				NBASSERT(op->sintaxMap != NULL)
				NBASSERT(op->iCharDef != -1)
				NBASSERT(op->sintax.iDef != -1)
				const STSrcAnlzExprOp* opDef = &op->sintaxMap->opChars[op->iCharDef];
				NBASSERT(op->sintax.iDef == opDef->op1 || op->sintax.iDef == opDef->op2)
				NBASSERT(op->sintax.iDef != -1)
				if(op->sintax.iDef == -1){
					//operator should have a sintax
					//PRINTF_INFO("Operator-without-sintax #%d: '%s'.\n", iOP, opDef->chars);
					NBASSERT(FALSE)
					r = FALSE;
					break;
				} else {
					//explicit sintax definition
					if(!noSpaces) NBString_concatByte(dstStr, ' ');
					//const STSrcAnlzExprOpSintx* opSintax = &op->sintaxMap->opSintaxs[op->sintax.iDef];
					//NBString_concat(&strTmp, __globalSintxCOpPriorities[opSintax->priority].name);
					//Opening
					NBString_concat(dstStr, opDef->chars);
					//PRINTF_INFO("Operator-with-sintax #%d: '%s'%s.\n", iOP, opDef->chars, (op->sintax.rCount > 0 ? " (pushing deeper level)": ""));
					//Child operators
					if(op->sintax.rCount > 0){
						if(!SrcAnlzExprParser_dbgAddOpsToStr(tokns, dstStr, &iOP, ops, (iOP + op->sintax.rCount), noSpaces)){
							//PRINTF_INFO("Operator-depper level could not be proceesed #%d: '%s'.\n", iOP, opDef->chars);
							NBASSERT(FALSE)
							r = FALSE;
							break;
						}
						//PRINTF_INFO("Popping one level.\n");
					}
					//Closing char
					if(opDef->popChars[0] != '\0'){
						NBString_concat(dstStr, opDef->popChars);
					}
				}
			}
		}
	}
	NBASSERT(r)
	*opIndex = iOP;
	return r;
}
#endif

#ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
BOOL SrcAnlzExprParser_dbgAddOpsDefsToStr(const STSrcAnlzTokns* tokns, STNBString* dstStr, SI32* opIndex, const STSrcAnlzExprParseNode* ops, const SI32 opsCount, const BOOL noSpaces){
	BOOL r = FALSE;
	//
	STSrcAnlzTokn* toknDefs	= (STSrcAnlzTokn*)tokns->defs._buffData;
	const char* tokensBuff	= tokns->strBuff.str; //ToDo: remove after testing: (tokensSrc == ENSrcAnlzExprSrc_MacrosValues ? state->macros.valsTokens.str : parCodeState->acumTokensCur->strBuff.str);
	//
	SI32 iOP = *opIndex;
	if(iOP >= 0 && iOP < opsCount){
		r = TRUE;
		//Process this depth level
		while(iOP < opsCount){
			const STSrcAnlzExprParseNode* op = &ops[iOP++];
			if(op->iCharDef == -1){
				//plain text operand
				NBASSERT(op->iCharDef == -1)
				NBASSERT(op->iToken != -1)
				if(op->iToken == -1){
					//should not be a empty operand
					//PRINTF_INFO("Operand-plain-empty #%d iToken(%d).\n", iOP, op->iToken);
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					//An operand can have multiple tokens
					const char* name = &(tokensBuff[toknDefs[op->iToken].iAtBuff]);
					if(!noSpaces) NBString_concatByte(dstStr, ' ');
					NBString_concat(dstStr, name);
					//ToDo: remove after testing:
					/*if(tokensSrc == ENSrcAnlzExprSrc_MacrosValues){
					 const STSrcAnlzMacroValTokn* toknDef = NBArray_itmPtrAtIndex(&state->macros.valsTokensDefs, STSrcAnlzMacroValTokn, op->iToken);
					 const char* name = &(tokensBuff[toknDef->iAtBuff]);
					 if(!noSpaces) NBString_concatByte(&dstStr, ' ');
					 NBString_concat(&dstStr, name);
					 } else {
					 const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&parCodeState->acumTokensCur->defs, STSrcAnlzTokn, op->iToken);
					 const char* name = &(tokensBuff[toknDef->iAtBuff]);
					 if(!noSpaces) NBString_concatByte(&dstStr, ' ');
					 NBString_concat(&dstStr, name);
					 }*/
				}
			} else {
				//operator
				NBASSERT(op->sintaxMap != NULL)
				NBASSERT(op->iCharDef != -1)
				NBASSERT(op->sintax.iDef != -1)
				const STSrcAnlzExprOp* opDef = &op->sintaxMap->opChars[op->iCharDef];
				NBASSERT(op->sintax.iDef == opDef->op1 || op->sintax.iDef == opDef->op2)
				NBASSERT(op->sintax.iDef != -1)
				if(op->sintax.iDef == -1){
					//operator should have a sintax
					//PRINTF_INFO("Operator-without-sintax #%d: '%s'.\n", iOP, opDef->chars);
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					//explicit sintax definition
					if(!noSpaces) NBString_concatByte(dstStr, ' ');
					//const STSrcAnlzExprOpSintx* opSintax = &op->sintaxMap->opSintaxs[op->sintax.iDef];
					//NBString_concat(&strTmp, __globalSintxCOpPriorities[opSintax->priority].name);
					if(op->sintax.rCount > 0){
						NBString_concat(dstStr, " { ");
					}
					//Opening
					NBString_concat(dstStr, opDef->chars);
					//PRINTF_INFO("Operator-with-sintax #%d: '%s'%s.\n", iOP, opDef->chars, (op->sintax.rCount > 0 ? " (pushing deeper level)": ""));
					//Child operators
					if(op->sintax.rCount > 0){
						if(!SrcAnlzExprParser_dbgAddOpsToStr(tokns, dstStr, &iOP, ops, (iOP + op->sintax.rCount), noSpaces)){
							//PRINTF_INFO("Operator-depper level could not be proceesed #%d: '%s'.\n", iOP, opDef->chars);
							r = FALSE; NBASSERT(FALSE)
							break;
						}
						//PRINTF_INFO("Popping one level.\n");
					}
					//Closing char
					if(opDef->popChars[0] != '\0'){
						NBString_concat(dstStr, opDef->popChars);
					}
					if(op->sintax.rCount > 0){
						NBString_concat(dstStr, " } ");
					}
				}
			}
		}
	}
	NBASSERT(r)
	*opIndex = iOP;
	return r;
}
#endif



