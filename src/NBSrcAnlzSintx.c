//
//  NBSrcAnlzSintx.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzSintx.h"
#include "nb/core/NBString.h"
#include "nb/core/NBArray.h"
#include "nb/core/NBArraySorted.h"

//---------------------
// Compare
//---------------------

BOOL STSrcAnlzSintxPart_compare(const ENCompareMode mode, const void* data1, const void* data2, const UI32 dataSz){
	NBASSERT(data1 != NULL && data2 != NULL)
	NBASSERT(dataSz == sizeof(STSrcAnlzSintxPart))
	if(data1 != NULL && data2 != NULL && dataSz == sizeof(STSrcAnlzSintxPart)){
		const STSrcAnlzSintxPart* obj = (const STSrcAnlzSintxPart*)data1;
		const STSrcAnlzSintxPart* other = (const STSrcAnlzSintxPart*)data2;
		switch (mode) {
			case ENCompareMode_Equal:
				return (obj->type == other->type && obj->idx == other->idx) ? TRUE : FALSE;
			case ENCompareMode_Lower:
				return (obj->type < other->type || (obj->type == other->type && obj->idx < other->idx)) ? TRUE : FALSE;
			case ENCompareMode_LowerOrEqual:
				return (obj->type < other->type || (obj->type == other->type && obj->idx <= other->idx)) ? TRUE : FALSE;
			case ENCompareMode_Greater:
				return (obj->type > other->type || (obj->type == other->type && obj->idx > other->idx)) ? TRUE : FALSE;
			case ENCompareMode_GreaterOrEqual:
				return (obj->type > other->type || (obj->type == other->type && obj->idx >= other->idx)) ? TRUE : FALSE;
			default:
				break;
		}
	}
	NBASSERT(FALSE)
	return FALSE;
}

//---------------------
// Search
//---------------------

int SrcAnlzSintx_searchKeyword(const char* str, const int strSz, const char** keywordsArr, const int keywordsArrLen, const STSrcAnlzRange* keywsRngsArr, const int keywsRngsArrLen){
	int r = -1;
	if(strSz > 0 && strSz <= keywsRngsArrLen){
		STSrcAnlzRange rng = keywsRngsArr[strSz - 1];
		while(rng.count > 0){
			NBASSERT(NBString_strLenBytes(keywordsArr[rng.start]) == strSz)
			if(NBString_strIsEqualBytes(keywordsArr[rng.start], rng.count, str, strSz)){
				r = rng.start;
				break;
			}
			rng.start++; rng.count--;
		}
	}
	return r;
}

int SrcAnlzSintx_searchPunctuator(const char* str, const int strSz, const char** punctsArr, const int punctsArrLen, const STSrcAnlzRange* punctsRngsArr, const int punctsRngsArrLen){
	int r = -1;
	if(strSz > 0 && strSz <= punctsRngsArrLen){
		STSrcAnlzRange rng = punctsRngsArr[strSz - 1];
		while(rng.count > 0){
			NBASSERT(NBString_strLenBytes(punctsArr[rng.start]) == strSz)
			if(NBString_strIsEqualBytes(punctsArr[rng.start], rng.count, str, strSz)){
				r = rng.start;
				break;
			}
			rng.start++; rng.count--;
		}
	}
	return r;
}

//---------------------
// Generating sintax def structures from plain-text definitions
//---------------------

void SrcAnlzSintx_genBasicStructuresFromPlainDefs(const STSrcAnlzSintxPlainDef* defsArr, const int defsArrLen, const char** keywordsArr, const int keywordsArrLen, const STSrcAnlzRange* keywsRngsArr, const int keywsRngsArrLen,  const char** punctsArr, const int punctsArrLen, const STSrcAnlzRange* punctsRngsArr, const int punctsRngsArrLen){
	typedef struct STDef_ {
		UI32 firstCharName;	//first char at strBuffer
		UI32 qDefs;			//definitions
		UI32 qRefs;			//references (usages in definitions)
	} STDef;
	//
	STNBString strEnum;		NBString_init(&strEnum);
	STNBString strNames;	NBString_init(&strNames);
	STNBString strDefs;		NBString_init(&strDefs);
	STNBString strDefsRngs;	NBString_init(&strDefsRngs);
	STNBString strDefsSeqs;	NBString_init(&strDefsSeqs);
	//
	STNBString strTmp;		NBString_init(&strTmp);
	STNBString prevName;	NBString_init(&prevName);
	SI32 iNameSeq = 0;
	STNBString strBuff;		NBString_init(&strBuff);
	STNBArray defOrder;		NBArray_init(&defOrder, sizeof(SI32), NBCompareSI32);
	STNBArray defs;			NBArray_init(&defs, sizeof(STDef), NULL);
	const SI32 count = defsArrLen;
	SI32 i; for(i = 0; i < count; i++){
		const STSrcAnlzSintxPlainDef* plainDef = &defsArr[i];
		NBASSERT(NBString_strIndexOf(plainDef->name, " ", 0) == -1) //No space in definations names.
		NBASSERT(NBString_strLenBytes(plainDef->valPlain) > 0) //value must be not-empty
		NBASSERT(plainDef->valPlain[0] != ' ') //first char must be no-space
		NBASSERT(plainDef->valPlain[NBString_strLenBytes(plainDef->valPlain) - 1] != ' ') //last char must be no-space
		//PRINTF_INFO("Def: '%s' = '%s'.\n", plainDef->name, plainDef->valPlain);
		//Search-def
		SI32 iDef = -1;
		{
			SI32 i; for(i = (defs.use - 1); i >= 0; i--){
				STDef* data = NBArray_itmPtrAtIndex(&defs, STDef, i);
				if(NBString_strIsEqual(&strBuff.str[data->firstCharName], plainDef->name)){
					NBASSERT(NBString_isEqual(&prevName, plainDef->name) || data->qDefs == 0) //All definitions must be together
					data->qDefs++;
					iDef = i;
					break;
				}
			}
			if(i == -1){
				STDef data;
				data.firstCharName	= strBuff.length;
				data.qDefs			= 1;
				data.qRefs			= 0;
				NBString_concat(&strBuff, plainDef->name); NBString_concatByte(&strBuff, '\0');
				NBArray_addValue(&defs, data);
				iDef = defs.use - 1;
				NBASSERT(!NBString_isEqual(&prevName, plainDef->name)) //All definitions must be together
			}
		}
		NBASSERT(iDef != -1)
		NBString_set(&strTmp, plainDef->name);
		NBString_replaceByte(&strTmp, '-', '_');
		//Save order for ordered enum-definition
		if(!NBString_isEqual(&prevName, plainDef->name)){
			//Close prev reange
			if(i != 0){
				NBString_concatSI32(&strDefsRngs, (SI32)(iNameSeq + 1));
				NBString_concat(&strDefsRngs, " }\n");
			}
			//
			NBArray_addValue(&defOrder, iDef);
			iNameSeq = 0;
			//Enum
			if(strEnum.length > 0) NBString_concat(&strEnum, ", ");
			NBString_concat(&strEnum, "ENSrzAnlzSintxC_");
			NBString_concat(&strEnum, strTmp.str);
			if(i == 0) NBString_concat(&strEnum, " = 0");
			NBString_concatByte(&strEnum, '\n');
			//EnumName
			if(strNames.length > 0) NBString_concat(&strNames, ", ");
			NBString_concat(&strNames, "{ \"");
			NBString_concat(&strNames, strTmp.str);
			NBString_concat(&strNames, "\", NULL, 0 }\n");
			//Open new range
			if(strDefsRngs.length > 0) NBString_concat(&strDefsRngs, ", ");
			NBString_concat(&strDefsRngs, " { ");
			NBString_concatSI32(&strDefsRngs, (SI32)i);
			NBString_concat(&strDefsRngs, ", ");
		} else {
			iNameSeq++;
		}
		//Analyze value
		{
			//Start arr def
			{
				NBString_concat(&strDefsSeqs, "const STSrcAnlzSintxPart __globalSintxCSeq_");
				NBString_concat(&strDefsSeqs, strTmp.str);
				NBString_concat(&strDefsSeqs, "_");
				NBString_concatSI32(&strDefsSeqs, (SI32)iNameSeq);
				NBString_concat(&strDefsSeqs, "[] = {\n");
			}
			//Start def
			{
				if(i != 0) NBString_concat(&strDefs, ", ");
				NBString_concat(&strDefs, "{ ENSrzAnlzSintxC_");
				NBString_concat(&strDefs, strTmp.str);
				NBString_concat(&strDefs, ", __globalSintxCSeq_");
				NBString_concat(&strDefs, strTmp.str);
				NBString_concat(&strDefs, "_");
				NBString_concatSI32(&strDefs, (SI32)iNameSeq);
				NBString_concat(&strDefs, ", ");
			}
			//
			const SI32 valPlainLen = NBString_strLenBytes(plainDef->valPlain);
			SI32 iPrevSpace = -1, qsintxParts = 0;
			do {
				NBString_empty(&strTmp);
				const SI32 iCharSpace = NBString_strIndexOf(plainDef->valPlain, " ", (iPrevSpace + 1));
				if(iCharSpace == -1){
					NBString_concatBytes(&strTmp, &plainDef->valPlain[iPrevSpace + 1], (valPlainLen - iPrevSpace - 1));
					iPrevSpace = valPlainLen;
				} else {
					NBString_concatBytes(&strTmp, &plainDef->valPlain[iPrevSpace + 1], (iCharSpace - iPrevSpace - 1));
					iPrevSpace = iCharSpace;
				}
				NBASSERT(strTmp.length > 0)
				if(strTmp.length > 0){
					SI32 iKeyword = -1, iPunctuator = -1; BOOL isHardcoded = FALSE;
					//Search as  hardCoded value
					if(iKeyword == -1 && iPunctuator == -1 && !isHardcoded){
						if(NBString_isEqual(&strTmp, "identifier") /*|| NBString_isEqual(&strTmp, "typedef-name")*/ || NBString_isEqual(&strTmp, "constant") || NBString_isEqual(&strTmp, "string-literal")){
							isHardcoded = TRUE;
						}
					}
					//Search as keyword
					if(iKeyword == -1 && iPunctuator == -1 && !isHardcoded){
						iKeyword = SrcAnlzSintx_searchKeyword(strTmp.str, strTmp.length, keywordsArr, keywordsArrLen, keywsRngsArr, keywsRngsArrLen);
					}
					//Search as punctuator
					if(iKeyword == -1 && iPunctuator == -1 && !isHardcoded){
						iPunctuator = SrcAnlzSintx_searchPunctuator(strTmp.str, strTmp.length, punctsArr, punctsArrLen, punctsRngsArr, punctsRngsArrLen);
					}
					//PRINTF_INFO("Part: '%s' (%s).\n", strTmp.str, (iKeyword != -1 ? "keyword" : iPunctuator != -1 ? "punctuator" : isHardcoded ? "hardcoded" : "definition"));
					//Search-ref
					if(iKeyword == -1 && iPunctuator == -1 && !isHardcoded){
						SI32 iRef = -1;
						SI32 i; for(i = (defs.use - 1); i >= 0; i--){
							STDef* data = NBArray_itmPtrAtIndex(&defs, STDef, i);
							if(NBString_strIsEqual(&strBuff.str[data->firstCharName], strTmp.str)){
								data->qRefs++;
								iRef = i;
								break;
							}
						}
						if(i == -1){
							STDef data;
							data.firstCharName	= strBuff.length;
							data.qDefs			= 0;
							data.qRefs			= 1;
							NBString_concat(&strBuff, strTmp.str);
							NBString_concatByte(&strBuff, '\0');
							NBArray_addValue(&defs, data);
							iRef = defs.use - 1;
						}
						NBASSERT(iRef != -1)
					}
					//Add to list
					if(qsintxParts != 0) NBString_concat(&strDefsSeqs, ", ");
					if(iKeyword != -1){
						//Keyword
						NBString_concat(&strDefsSeqs, "{ ENSrzAnlzSintxPartType_Keyword, ");
						NBString_concatSI32(&strDefsSeqs, (SI32)iKeyword);
						NBString_concat(&strDefsSeqs, " }\n");
					} else if(iPunctuator != -1){
						//Punctuator
						NBString_concat(&strDefsSeqs, "{ ENSrzAnlzSintxPartType_Punctuator, ");
						NBString_concatSI32(&strDefsSeqs, (SI32)iPunctuator);
						NBString_concat(&strDefsSeqs, " }\n");
					} else if(isHardcoded){
						//Element
						if(NBString_isEqual(&strTmp, "identifier")){
							NBString_concat(&strDefsSeqs, "{ ENSrzAnlzSintxPartType_Identifier, -1 }\n");
						} else if(NBString_isEqual(&strTmp, "constant")){
							NBString_concat(&strDefsSeqs, "{ ENSrzAnlzSintxPartType_Constant, -1 }\n");
						} else if(NBString_isEqual(&strTmp, "string-literal")){
							NBString_concat(&strDefsSeqs, "{ ENSrzAnlzSintxPartType_StrLiteral, -1 }\n");
						} else {
							NBString_concat(&strDefsSeqs, "{ ENSrzAnlzSintxPartType_ERROR, -1 }\n");
							NBASSERT(FALSE)
						}
					} else {
						//Ref to definition
						NBString_replaceByte(&strTmp, '-', '_');
						NBString_concat(&strDefsSeqs, "{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_");
						NBString_concat(&strDefsSeqs, strTmp.str);
						NBString_concat(&strDefsSeqs, " }\n");
					}
					//
					qsintxParts++;
				}
			} while(iPrevSpace < valPlainLen);
			//End def
			{
				NBString_concatSI32(&strDefs, (SI32)qsintxParts);
				NBString_concat(&strDefs, ", NULL, 0 }\n");
			}
			//End of sequence
			{
				NBString_concat(&strDefsSeqs, " };\n");
			}
		}
		NBString_set(&prevName, plainDef->name);
	}
	//Close prev reange
	if(i != 0){
		NBString_concatSI32(&strDefsRngs, (SI32)(iNameSeq + 1));
		NBString_concat(&strDefsRngs, " }\n");
	}
	//
	//
	//
	//Analyze defs and refs
	{
		SI32 i; for(i = 0; i < defs.use; i++){
			STDef* data = NBArray_itmPtrAtIndex(&defs, STDef, i);
			PRINTF_INFO("#%d '%s': %d defs, %d refs%s.\n", (i+1), &strBuff.str[data->firstCharName], data->qDefs, data->qRefs, (data->qDefs <= 0 || data->qRefs <= 0 ? " (ERROR)" : ""));
			NBASSERT(data->qDefs > 0 && data->qRefs > 0)
		}
	}
	//print enumDefs
	PRINTF_INFO("\n\ntypedef enum ENSrzAnlzSintxC_ {\n%s, ENSrzAnlzSintxC_Count\n} ENSrzAnlzSintxC;\n\n\n", strEnum.str);
	//print enumNames
	PRINTF_INFO("\n\nstatic const STSrcAnlzSintx __globalSintxCDefs[] = {\n%s};\n\n\n", strNames.str);
	//print ranges
	PRINTF_INFO("\n\nconst STSrcAnlzRange __globalSintxCDefSeqsRngs[] = {\n%s};\n\n\n", strDefsRngs.str);
	//print defsSeqs
	PRINTF_INFO("\n\n%s\n\n\n", strDefsSeqs.str);
	//print defs
	PRINTF_INFO("\n\n//\n// Language sintax definitions (parts and deep-search arrays)\n//\n\nconst STSrcAnlzSintxSeq __globalSintxCDefSeqs[] = {\n%s};\n\n\n", strDefs.str);
	//
	NBArray_release(&defs);
	NBArray_release(&defOrder);
	NBString_release(&strBuff);
	NBString_release(&prevName);
	NBString_release(&strTmp);
	//
	NBString_release(&strDefsSeqs);
	NBString_release(&strDefsRngs);
	NBString_release(&strDefs);
	NBString_release(&strNames);
	NBString_release(&strEnum);
}

//
// Lang Def, Advanced Structures
//

void SrcAnlzSintx_genAdvancedStructuresFromBasicStructures_addDef(const STSrcAnlzSintxSeq* defsArr, const int defsArrLen, const SI32 iDef, STNBArraySorted* posibsFoundGrouped /*STSrcAnlzSintxPart*/, STNBArraySorted* posibsFound /*STSrcAnlzSintxPart*/, STNBArray* usedIdxs /*SI32*/){
	if(NBArray_indexOf(usedIdxs, &iDef, sizeof(iDef)) == -1){
		//Add to used-array to avoid recursivity
		NBArray_addValue(usedIdxs, iDef);
		//Process
		const STSrcAnlzSintxSeq* sintaxDefSeq = &defsArr[iDef];
		if(sintaxDefSeq->parts != NULL && sintaxDefSeq->partsSz > 0){
			const STSrcAnlzSintxPart* firstPart = &sintaxDefSeq->parts[0];
			//Add to seq-hints
			{
				//Specific sequence hints
				{
					//Add non-def posibility
					SI32 i; for(i = 0; i < posibsFound->use; i++){
						const STSrcAnlzSintxPart* found = NBArraySorted_itmPtrAtIndex(posibsFound, STSrcAnlzSintxPart, i);
						if(STSrcAnlzSintxPart_compare(ENCompareMode_Equal, found, firstPart, sizeof(*found))){
							break;
						}
					}
					//Add
					if(i == posibsFound->use){
						NBArraySorted_add(posibsFound, firstPart, sizeof(*firstPart));
					}
				}
				//Grouped definition all-sequences posibility
				{
					//Add non-def posibility
					SI32 i; for(i = 0; i < posibsFoundGrouped->use; i++){
						const STSrcAnlzSintxPart* found = NBArraySorted_itmPtrAtIndex(posibsFoundGrouped, STSrcAnlzSintxPart, i);
						if(STSrcAnlzSintxPart_compare(ENCompareMode_Equal, found, firstPart, sizeof(*found))){
							break;
						}
					}
					//Add
					if(i == posibsFoundGrouped->use){
						NBArraySorted_add(posibsFoundGrouped, firstPart, sizeof(*firstPart));
					}
				}
			}
			//Add all definitions of this part (recursive-call)
			if(firstPart->type == ENSrzAnlzSintxPartType_SintaxDef){
				NBASSERT(firstPart->idx >= 0)
				SI32 i; for(i = 0; i < defsArrLen; i++){
					const STSrcAnlzSintxSeq* sintaxDefSeq = &defsArr[i];
					if(sintaxDefSeq->iDefEnum == firstPart->idx){
						SrcAnlzSintx_genAdvancedStructuresFromBasicStructures_addDef(defsArr, defsArrLen, i, posibsFoundGrouped, posibsFound, usedIdxs);
					}
				}
			}
		}
	}
}
	
void SrcAnlzSintx_genAdvancedStructuresFromBasicStructures(const STSrcAnlzSintxSeq* defsArr, const int defsArrLen, const STSrcAnlzSintx* enumDefs){
	STNBString strPosibsGrouped;		NBString_init(&strPosibsGrouped);
	STNBString strPosibs;				NBString_init(&strPosibs);
	STNBString strMainArrGrp;			NBString_init(&strMainArrGrp);
	STNBString strMainArr;				NBString_init(&strMainArr);
	STNBArraySorted posibsFoundGrouped;	NBArraySorted_init(&posibsFoundGrouped, sizeof(STSrcAnlzSintxPart), NULL);
	STNBArraySorted posibsFound;		NBArraySorted_init(&posibsFound, sizeof(STSrcAnlzSintxPart), NULL);
	STNBArray usedIdxs;					NBArray_init(&usedIdxs, sizeof(SI32), NBCompareSI32);
	SI32 iDefSeq = 0, lastDefEnum = 0xFFFF;
	SI32 i; for(i = 0; i < defsArrLen; i++){
		const STSrcAnlzSintxSeq* sintaxDefSeq = &defsArr[i];
		//
		if(lastDefEnum != sintaxDefSeq->iDefEnum){
			//Print frouped posibilities
			if(lastDefEnum != 0xFFFF){
				//Build result
				NBASSERT(posibsFoundGrouped.use > 0)
				NBString_concat(&strPosibsGrouped, "const STSrcAnlzSintxPart __globalSintxCSeqHints_");
				NBString_concat(&strPosibsGrouped, enumDefs[lastDefEnum].name);
				NBString_concat(&strPosibsGrouped, "[] = {\n");
				SI32 i2; for(i2 = 0; i2 < posibsFoundGrouped.use; i2++){
					const STSrcAnlzSintxPart* found = NBArraySorted_itmPtrAtIndex(&posibsFoundGrouped, STSrcAnlzSintxPart, i2);
					if(i2 != 0) NBString_concat(&strPosibsGrouped, ", ");
					NBString_concat(&strPosibsGrouped, "{ ");
					switch(found->type){
						case ENSrzAnlzSintxPartType_Keyword: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_Keyword"); break;
						case ENSrzAnlzSintxPartType_Punctuator: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_Punctuator"); break;
						case ENSrzAnlzSintxPartType_Identifier: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_Identifier"); break;
						case ENSrzAnlzSintxPartType_Constant: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_Constant"); break;
						case ENSrzAnlzSintxPartType_StrLiteral: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_StrLiteral"); break;
						case ENSrzAnlzSintxPartType_SintaxDef: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_SintaxDef"); break;
						default: NBASSERT(FALSE); break;
					}
					NBString_concat(&strPosibsGrouped, ", ");
					NBString_concatSI32(&strPosibsGrouped, (SI32)found->idx);
					NBString_concat(&strPosibsGrouped, " }\n");
				}
				NBString_concat(&strPosibsGrouped, " };\n");
				//
				if(strMainArrGrp.length > 0) NBString_concat(&strMainArrGrp, ", ");
				NBString_concat(&strMainArrGrp, "{");
				NBString_concat(&strMainArrGrp, "\"");
				NBString_concat(&strMainArrGrp, enumDefs[lastDefEnum].name);
				NBString_concat(&strMainArrGrp, "\"");
				NBString_concat(&strMainArrGrp, ", __globalSintxCSeqHints_"); NBString_concat(&strMainArrGrp, enumDefs[lastDefEnum].name);
				NBString_concat(&strMainArrGrp, ", ");
				NBString_concatSI32(&strMainArrGrp, (SI32)posibsFoundGrouped.use);
				NBString_concat(&strMainArrGrp, "}\n");
				//
				NBArraySorted_empty(&posibsFoundGrouped);
			}
			iDefSeq = 0;
			lastDefEnum = sintaxDefSeq->iDefEnum;
		} else {
			iDefSeq++;
		}
		//Search posibilities
		NBArray_empty(&usedIdxs);
		NBArraySorted_empty(&posibsFound);
		SrcAnlzSintx_genAdvancedStructuresFromBasicStructures_addDef(defsArr, defsArrLen, i, &posibsFoundGrouped, &posibsFound, &usedIdxs);
		//Build result
		NBASSERT(posibsFound.use > 0)
		NBString_concat(&strPosibs, "const STSrcAnlzSintxPart __globalSintxCSeqHints_");
		NBString_concat(&strPosibs, enumDefs[sintaxDefSeq->iDefEnum].name);
		NBString_concat(&strPosibs, "_");
		NBString_concatSI32(&strPosibs, (SI32)iDefSeq);
		NBString_concat(&strPosibs, "[] = {\n");
		SI32 i2; for(i2 = 0; i2 < posibsFound.use; i2++){
			const STSrcAnlzSintxPart* found = NBArraySorted_itmPtrAtIndex(&posibsFound, STSrcAnlzSintxPart, i2);
			if(i2 != 0) NBString_concat(&strPosibs, ", ");
			NBString_concat(&strPosibs, "{ ");
			switch(found->type){
				case ENSrzAnlzSintxPartType_Keyword: NBString_concat(&strPosibs, "ENSrzAnlzSintxPartType_Keyword"); break;
				case ENSrzAnlzSintxPartType_Punctuator: NBString_concat(&strPosibs, "ENSrzAnlzSintxPartType_Punctuator"); break;
				case ENSrzAnlzSintxPartType_Identifier: NBString_concat(&strPosibs, "ENSrzAnlzSintxPartType_Identifier"); break;
				case ENSrzAnlzSintxPartType_Constant: NBString_concat(&strPosibs, "ENSrzAnlzSintxPartType_Constant"); break;
				case ENSrzAnlzSintxPartType_StrLiteral: NBString_concat(&strPosibs, "ENSrzAnlzSintxPartType_StrLiteral"); break;
				case ENSrzAnlzSintxPartType_SintaxDef: NBString_concat(&strPosibs, "ENSrzAnlzSintxPartType_SintaxDef"); break;
				default: NBASSERT(FALSE); break;
			}
			NBString_concat(&strPosibs, ", ");
			NBString_concatSI32(&strPosibs, (SI32)found->idx);
			NBString_concat(&strPosibs, " }\n");
		}
		NBString_concat(&strPosibs, " };\n");
		//
		//
		if(i != 0) NBString_concat(&strMainArr, " , ");
		NBString_concat(&strMainArr, "{ ENSrzAnlzSintxC_");
		NBString_concat(&strMainArr, enumDefs[sintaxDefSeq->iDefEnum].name);
		NBString_concat(&strMainArr, ", __globalSintxCSeq_");
		NBString_concat(&strMainArr, enumDefs[sintaxDefSeq->iDefEnum].name);
		NBString_concat(&strMainArr, "_");
		NBString_concatSI32(&strMainArr, (SI32)iDefSeq);
		NBString_concat(&strMainArr, ", ");
		NBString_concatSI32(&strMainArr, (SI32)sintaxDefSeq->partsSz);
		NBString_concat(&strMainArr, ", __globalSintxCSeqHints_");
		NBString_concat(&strMainArr, enumDefs[sintaxDefSeq->iDefEnum].name);
		NBString_concat(&strMainArr, "_");
		NBString_concatSI32(&strMainArr, (SI32)iDefSeq);
		NBString_concat(&strMainArr, ", ");
		NBString_concatSI32(&strMainArr, (SI32)posibsFound.use);
		NBString_concat(&strMainArr, " }\n");
	}
	if(lastDefEnum != 0xFFFF){
		//Build result
		NBASSERT(posibsFoundGrouped.use > 0)
		NBString_concat(&strPosibsGrouped, "const STSrcAnlzSintxPart __globalSintxCSeqHints_");
		NBString_concat(&strPosibsGrouped, enumDefs[lastDefEnum].name);
		NBString_concat(&strPosibsGrouped, "[] = {\n");
		SI32 i2; for(i2 = 0; i2 < posibsFoundGrouped.use; i2++){
			const STSrcAnlzSintxPart* found = NBArraySorted_itmPtrAtIndex(&posibsFoundGrouped, STSrcAnlzSintxPart, i2);
			if(i2 != 0) NBString_concat(&strPosibsGrouped, ", ");
			NBString_concat(&strPosibsGrouped, "{ ");
			switch(found->type){
				case ENSrzAnlzSintxPartType_Keyword: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_Keyword"); break;
				case ENSrzAnlzSintxPartType_Punctuator: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_Punctuator"); break;
				case ENSrzAnlzSintxPartType_Identifier: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_Identifier"); break;
				case ENSrzAnlzSintxPartType_Constant: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_Constant"); break;
				case ENSrzAnlzSintxPartType_StrLiteral: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_StrLiteral"); break;
				case ENSrzAnlzSintxPartType_SintaxDef: NBString_concat(&strPosibsGrouped, "ENSrzAnlzSintxPartType_SintaxDef"); break;
				default: NBASSERT(FALSE); break;
			}
			NBString_concat(&strPosibsGrouped, ", ");
			NBString_concatSI32(&strPosibsGrouped, (SI32)found->idx);
			NBString_concat(&strPosibsGrouped, " }\n");
		}
		NBString_concat(&strPosibsGrouped, " };\n");
		//
		if(strMainArrGrp.length > 0) NBString_concat(&strMainArrGrp, ", ");
		NBString_concat(&strMainArrGrp, "{");
		NBString_concat(&strMainArrGrp, "\"");
		NBString_concat(&strMainArrGrp, enumDefs[lastDefEnum].name);
		NBString_concat(&strMainArrGrp, "\"");
		NBString_concat(&strMainArrGrp, ", __globalSintxCSeqHints_");
		NBString_concat(&strMainArrGrp, enumDefs[lastDefEnum].name);
		NBString_concat(&strMainArrGrp, ", ");
		NBString_concatSI32(&strMainArrGrp, (SI32)posibsFoundGrouped.use);
		NBString_concat(&strMainArrGrp, "}\n");
		//
		NBArraySorted_empty(&posibsFoundGrouped);
	}
	//print defsSeqs
	PRINTF_INFO("\n\n//\n// Optimization: array of def posibilities\n// (pregenerated deep search arrays, no-definitions references)\n//\n\n%s\n\n\n", strPosibsGrouped.str);
	//print defs
	PRINTF_INFO("\n\n//\n// Language sintax definitions (parts and deep-search arrays)\n//\n\nconst STSrcAnlzSintx __globalSintxCDefs[] = {\n%s};\n\n\n", strMainArrGrp.str);
	//print defsSeqs
	PRINTF_INFO("\n\n//\n// Optimization: array of def posibilities\n// (pregenerated deep search arrays, no-definitions references)\n//\n\n%s\n\n\n", strPosibs.str);
	//print defs
	PRINTF_INFO("\n\n//\n// Language sintax definitions (parts and deep-search arrays)\n//\n\nconst STSrcAnlzSintxSeq __globalSintxCDefSeqs[] = {\n%s};\n\n\n", strMainArr.str);
	//
	NBArray_release(&usedIdxs);
	NBArraySorted_release(&posibsFound);
	NBArraySorted_release(&posibsFoundGrouped);
	NBString_release(&strPosibs);
	NBString_release(&strPosibsGrouped);
	NBString_release(&strMainArrGrp);
	NBString_release(&strMainArr);
}




