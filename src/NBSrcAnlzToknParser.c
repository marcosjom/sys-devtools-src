//
//  NBSrcAnlzToknParser.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzToknParser.h"

//-------------------------
// Init and release
//-------------------------

void SrcAnlzToknParser_init(STSrcAnlzToknParser* state, STSrcAnlzParserItfs* itfs){
	state->itfs				= itfs;
	state->parseTypes		= NULL;
	state->parseTypesSz		= 0;
	if(itfs != NULL){
		if(itfs->toknItf != NULL){
			STSrcAnlzLangToknI* toknItf = itfs->toknItf;
			if(toknItf->itfs != NULL && toknItf->itfsSz > 0){
				state->parseTypesSz	= toknItf->itfsSz;
				state->parseTypes	= (STSrcAnlzToknParseType*)NBMemory_alloc(sizeof(STSrcAnlzToknParseType) * toknItf->itfsSz);
				SI32 i; for(i = 0; i < toknItf->itfsSz; i++){
					state->parseTypes[i].itf			= toknItf->itfs[i];
					state->parseTypes[i].lastResult		= ENSrcAnlzToknParseResult_Count;
					state->parseTypes[i].lastValidLen	= 0;
					state->parseTypes[i].lastValidVal	= 0;
					state->parseTypes[i].lastValidType	= ENSrcAnlzToknType_Count;
					state->parseTypes[i].lastValidSubType = 0;
					//Reset state
					if(state->parseTypes[i].itf.funcReset != NULL){
						(*state->parseTypes[i].itf.funcReset)(state->parseTypes[i].itf.obj);
					}
				}
			}
		}
	}
	//
	state->lastStreamChar		= '\0';
	NBString_init(&state->strAcum);
	NBString_init(&state->strAcum2);
	state->lastTokenWasSpace	= FALSE;
	state->streamPos.iByte		= 0;
	state->streamPos.iToken		= 0;
	state->streamPos.iLine		= 0;
	state->streamPos.iLineByte	= 0;
	state->streamPos.iLineToken	= 0;
}

void SrcAnlzToknParser_release(STSrcAnlzToknParser* state){
	if(state->parseTypes != NULL) NBMemory_free(state->parseTypes); state->parseTypes = NULL;
	state->parseTypesSz		= 0;
	//
	state->lastStreamChar	= '\0';
	NBString_release(&state->strAcum);
	NBString_release(&state->strAcum2);
	state->lastTokenWasSpace	= FALSE;
	state->streamPos.iByte		= 0;
	state->streamPos.iToken		= 0;
	state->streamPos.iLine		= 0;
	state->streamPos.iLineByte	= 0;
	state->streamPos.iLineToken	= 0;
	//
	state->itfs				= NULL;
}

//-------------------------
// Content processing
//-------------------------

BOOL SrcAnlzToknParser_feed(STSrcAnlzToknParser* state, const char* data, const UI32 dataSz){
	BOOL r = TRUE;
	{
		//Process buffer
		char c = '\0'; BOOL cCameFromBuffer = FALSE, cIgnore = FALSE;
		SI32 posibleTypesCount = 0, largerSize = 0, largerTokenVal = -1;
		ENSrcAnlzToknType largerTokenType = ENSrcAnlzToknType_Count; SI32 largerTokenSubtype = 0;
		STNBString largerTokenStr; NBString_init(&largerTokenStr);
		//Process buffer
		UI32 buffPos = 0;
		while(buffPos < dataSz /*&& state->strAcum2.lenght == 0*/){
			//Load char
			cIgnore = FALSE;
			if(state->strAcum2.lenght > 0 && state->lastStreamChar != '\\'){
				//Load from 'pending buffer'
				c = state->strAcum2.str[0];
				NBString_removeFirstBytes(&state->strAcum2, 1);
				cCameFromBuffer = FALSE;
			} else {
				//Load from 'data'
				c = data[buffPos++];
				cCameFromBuffer = TRUE;
				//Analyze '\ + new-line' combination (must be ignored)
				if(state->lastStreamChar == '\\'){
					if(c != '\n'){
						//Add previously-ignored char
						NBString_concatByte(&state->strAcum2, state->lastStreamChar);
						//Also add current char (if it's not '\\')
						if(c != '\\') NBString_concatByte(&state->strAcum2, c);
					}
					cIgnore = TRUE;
				}
				//Analyze '\' new char
				if(c == '\\') cIgnore = TRUE;
			}
			//Ignore new-lines
			if(!cIgnore){
				//
				//Add char
				//
				NBString_concatByte(&state->strAcum, c);
				//
				//Analyze parsing results
				//
				posibleTypesCount = largerSize = largerTokenSubtype = 0;
				largerTokenType = ENSrcAnlzToknType_Count; largerTokenVal = -1;
				{
					const char* toknStr	= state->strAcum.str;
					const SI32 toknLen	= state->strAcum.lenght;
					SI32 i; for(i = 0; i< state->parseTypesSz; i++){
						STSrcAnlzToknParseType* pType = &state->parseTypes[i];
						if(pType->lastResult != ENSrcAnlzToknParseResult_Error){
							if(pType->itf.funcFeed != NULL){
								ENSrcAnlzToknParseResult result = ENSrcAnlzToknParseResult_Error;
								SI32 val = 0; ENSrcAnlzToknType type = ENSrcAnlzToknType_Count; SI8 subType = 0;
								if(!(pType->itf.funcFeed)(pType->itf.obj, toknStr, toknLen, c, &result, &val, &type, &subType)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									pType->lastResult	= result;
									if(result == ENSrcAnlzToknParseResult_Complete){
										pType->lastValidLen		= toknLen;
										pType->lastValidVal		= val;
										pType->lastValidType	= type;
										pType->lastValidSubType	= subType;
										//PRINTF_INFO("Token is complete: '%s' (parser %d).\n", toknStr, i);
										posibleTypesCount++;
									} else if(result == ENSrcAnlzToknParseResult_Partial){
										//PRINTF_INFO("Token is partial: '%s' (parser %d).\n", toknStr, i);
										posibleTypesCount++;
									}
								}
							}
						}
						//
						if(largerSize < pType->lastValidLen){
							largerSize			= pType->lastValidLen;
							largerTokenVal		= pType->lastValidVal;
							largerTokenType		= pType->lastValidType;
							largerTokenSubtype	= pType->lastValidSubType;
						}
					}
					//PRINTF_INFO("%d states evaluated: '%s' (larger %d).\n", i, toknStr, largerSize);
					if(!r) break;
				}
				//
				//Purge content
				//
				//PRINTF_INFO("posibleTypesCount: %d.\n", posibleTypesCount);
				if(posibleTypesCount == 0){
					NBASSERT((largerSize == 0 && largerTokenType == ENSrcAnlzToknType_Count) || (largerSize != 0 && largerTokenType < ENSrcAnlzToknType_Count))
					//Reset tokn parsing states
					{
						SI32 i; for(i = 0; i < state->parseTypesSz; i++){
							STSrcAnlzToknParseType* pType = &state->parseTypes[i];
							pType->lastResult		= ENSrcAnlzToknParseResult_Count;
							pType->lastValidLen		= 0;
							pType->lastValidVal		= -1;
							pType->lastValidType	= ENSrcAnlzToknType_Count;
							pType->lastValidSubType	= 0;
							if(pType->itf.funcReset != NULL){
								(*pType->itf.funcReset)(pType->itf.obj);
							}
						}
						//PRINTF_INFO("%d states reseted.\n", state->parseTypesSz);
					}
					//PROCESS
					if(largerTokenType >= ENSrcAnlzToknType_Count){
						if(!(NBString_isEqual(&state->strAcum, "\n") || NBString_isEqual(&state->strAcum, "\t") || NBString_isEqual(&state->strAcum, " "))){
							PRINTF_WARNING("Unidentfied content: '%s'.\n", state->strAcum.str);
						}
						//Feed
						{
							//Swap buffers
							STNBString tmpPtr = largerTokenStr;
							largerTokenStr	= state->strAcum;
							state->strAcum	= tmpPtr;
							NBString_empty(&state->strAcum);
						}
						if(!(*state->itfs->sintaxItf->feedToknFunc)(state->itfs->sintaxItf, largerTokenStr.str, largerTokenType, largerTokenSubtype, largerTokenVal, state->lastTokenWasSpace)){
							r = FALSE; NBASSERT(FALSE)
							break;
						}
						state->lastTokenWasSpace = TRUE;
					} else {
						NBASSERT(largerSize <= state->strAcum.lenght)
						//Move remaining content to 're-process' buffer
						if(state->strAcum2.lenght > 0) NBString_concat(&state->strAcum, state->strAcum2.str);
						NBString_set(&state->strAcum2, &state->strAcum.str[largerSize]);
						if(state->strAcum.lenght > largerSize) NBString_removeLastBytes(&state->strAcum, state->strAcum.lenght - largerSize);
						//PRINT
						/*{
							switch(largerTokenType){
								case ENSrcAnlzToknType_Keyword:
									PRINTF_INFO("Token keyword: '%s' (%d).\n", state->strAcum.str, largerTokenVal);
									break;
								case ENSrcAnlzToknType_Identifier:
									PRINTF_INFO("Token identifier: '%s' (%d).\n", state->strAcum.str, largerTokenVal);
									break;
								case ENSrcAnlzToknType_Constant:
									PRINTF_INFO("Token constant: '%s' (%d) (subtype %d).\n", state->strAcum.str, largerTokenVal, largerTokenSubtype);
									/ *switch(largerTokenSubtype){
										case ENSrcAnlzToknParseConstType_Integer:
											PRINTF_INFO("Token const-integer '%s'.\n", state->strAcum.str);
											break;
										case ENSrcAnlzToknParseConstType_Float:
											PRINTF_INFO("Token const-float '%s'.\n", state->strAcum.str);
											break;
										case ENSrcAnlzToknParseConstType_Enum:
											PRINTF_INFO("Token const-enum '%s'.\n", state->strAcum.str);
											break;
										case ENSrcAnlzToknParseConstType_Character:
											PRINTF_INFO("Token const-char '%s'.\n", state->strAcum.str);
											break;
										default:
											PRINTF_INFO("Unknown constant-type token '%s'.\n", state->strAcum.str);
											break;
									}* /
									break;
								case ENSrcAnlzToknType_StrLiteral:
									PRINTF_INFO("Token string-literal '%s' (%d).\n", state->strAcum.str, largerTokenVal);
									break;
								case ENSrcAnlzToknType_Punctuator:
									PRINTF_INFO("Token punctuator '%s' (%d).\n", state->strAcum.str, largerTokenVal);
									break;
								case ENSrcAnlzToknType_Comment:
									PRINTF_INFO("Token comment: '%s' (%d) (subtype %d).\n", state->strAcum.str, largerTokenVal, largerTokenSubtype);
									/ *switch(largerTokenSubtype){
										case ENSrcAnlzToknParseCommentType_MonoLine:
											//PRINTF_INFO("Token comment-monoline '%s'.\n", state->strAcum.str);
											break;
										case ENSrcAnlzToknParseCommentType_MultiLine:
											//PRINTF_INFO("Token comment-multiline '%s'.\n", state->strAcum.str);
											break;
										default:
											PRINTF_INFO("Unkown comment-type token '%s'.\n", state->strAcum.str);
											break;
									}* /
									break;
								case ENSrcAnlzToknType_HeaderName:
									PRINTF_INFO("Token header-name '%s' (%d).\n", state->strAcum.str, largerTokenVal);
									break;
								default:
									//PRINTF_INFO("Unknown token type '%s' (%d).\n", state->strAcum.str, largerTokenVal)
									break;
							}
						}*/
						//PRINTF_INFO("Chars %d of %d are %s: '%s'.\n", largerSize, (state->strAcum.lenght + state->strAcum2.lenght), STR_TOKEN_TYPE(largerTokenType), state->strAcum.str);
						//PRINTF_INFO("Remaining: '%s'\n", state->strAcum2.str);
						//PRINTF_INFO("-----------\n");
						//Feed
						{
							//Swap buffers
							STNBString tmpPtr = largerTokenStr;
							largerTokenStr	= state->strAcum;
							state->strAcum	= tmpPtr;
							NBString_empty(&state->strAcum);
						}
						if(!(*state->itfs->sintaxItf->feedToknFunc)(state->itfs->sintaxItf, largerTokenStr.str, largerTokenType, largerTokenSubtype, largerTokenVal, state->lastTokenWasSpace)){
							r = FALSE; NBASSERT(FALSE)
							break;
						}
						//
						state->streamPos.iToken++;
						state->streamPos.iLineToken++;
						state->lastTokenWasSpace = FALSE;
					}
				}
			}
			//Physical byte and line counter
			if(cCameFromBuffer){
				//Lines counters
				if(c == '\n'){
					state->streamPos.iLine++;
					state->streamPos.iLineByte = 0;
					state->streamPos.iLineToken = 0;
				} else {
					state->streamPos.iLineByte++;
				}
				//Bytes counter
				state->streamPos.iByte++;
				state->lastStreamChar = c;
			}
		} //while
		NBString_release(&largerTokenStr);
	}
	return r;
}

BOOL SrcAnlzToknParser_flush(STSrcAnlzToknParser* state){
	BOOL r = FALSE;
	//Process remaining 'pending buffer'
	if(!SrcAnlzToknParser_feed(state, NULL, 0)){
		r = FALSE; NBASSERT(FALSE)
	} else {
		//Process remaining source code state
		if(state->itfs->sintaxItf->flushFunc != NULL){
			if(!(*state->itfs->sintaxItf->flushFunc)(state->itfs->sintaxItf)){
				r = FALSE; NBASSERT(FALSE)
			} else {
				r = TRUE;
			}
		}
	}
	return r;
}
