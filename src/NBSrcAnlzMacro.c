//
//  NBSrcAnlzMacro.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzMacro.h"

//--------------------------
// Init and release
//--------------------------
void SrcAnlzMacros_init(STSrcAnlzMacros* state){
	NBArray_init(&state->defs, sizeof(STSrcAnlzMacro), NULL);
	NBString_init(&state->names);
	NBArraySorted_init(&state->namesIdx, sizeof(STSrcAnlzStrIdx), SrcAnlzStrIdx_compare);
	NBArray_init(&state->params, sizeof(STSrcAnlzMacroParam), NULL);
	NBString_init(&state->paramsNames);
	{
		NBArray_init(&state->valsTokns.tokns.defs, sizeof(STSrcAnlzTokn), NULL);
		NBString_init(&state->valsTokns.tokns.strBuff);
		NBString_concatByte(&state->valsTokns.tokns.strBuff, '\0'); //index zero mut be a empty string (empty value)
		NBArray_init(&state->valsTokns.iParamRefs, sizeof(SI8), NBCompareSI8);
	}
	//Init strings
	NBString_concatByte(&state->names, '\0');
	NBString_concatByte(&state->paramsNames, '\0');
}

void SrcAnlzMacros_release(STSrcAnlzMacros* state){
	NBArray_release(&state->defs);
	NBString_release(&state->names);
	NBArraySorted_release(&state->namesIdx);
	NBArray_release(&state->params);
	NBString_release(&state->paramsNames);
	{
		NBArray_release(&state->valsTokns.tokns.defs);
		NBString_release(&state->valsTokns.tokns.strBuff);
		NBArray_release(&state->valsTokns.iParamRefs);
	}
}

//--------------------------
//Find
//--------------------------

SI32 SrcAnlzMacros_findNameIdx(const STSrcAnlzMacros* state, const STNBString* strNames, const UI32 iNameStart){
	STSrcAnlzStrIdx srchIdx;
	srchIdx.nameStart	= iNameStart;
	srchIdx.strings		= strNames;
	return NBArraySorted_indexOf(&state->namesIdx, &srchIdx, sizeof(srchIdx), NULL);
}

SI32 SrcAnlzMacros_findNameIdxByStr(const STSrcAnlzMacros* state, const char* strName){
	STNBString strNames;
	NBString_initWithStr(&strNames, strName);
	const SI32 r = SrcAnlzMacros_findNameIdx(state, &strNames, 0);
	NBString_release(&strNames);
	return r;
}

//--------------------------
// Set macro definition
//--------------------------

BOOL SrcAnlzMacros_macroCreateOrReset(STSrcAnlzMacros* state, STNBString* strNames, const UI32 iNameStart, STSrcAnlzMacro** dstDef){
	BOOL r = FALSE;
	if(state != NULL && strNames != NULL){
		STSrcAnlzStrIdx srchIdx;
		srchIdx.nameStart	= iNameStart;
		srchIdx.strings		= strNames;
		const SI32 iIdx		= NBArraySorted_indexOf(&state->namesIdx, &srchIdx, sizeof(srchIdx), NULL);
		if(iIdx != -1){
			const STSrcAnlzStrIdx* idx	= NBArraySorted_itmPtrAtIndex(&state->namesIdx, STSrcAnlzStrIdx, iIdx);
			STSrcAnlzMacro* macro	= NBArray_itmPtrAtIndex(&state->defs, STSrcAnlzMacro, idx->itmIndex); NBASSERT(NBString_strIsEqual(&strNames->str[iNameStart], &(state->names.str[macro->nameStart])))
			macro->params.start		= 0;
			macro->params.count		= 0;
			macro->params.areExplicit = FALSE;
			macro->params.isVaArgs	= FALSE;
			macro->valTokensFirst	= 0;
			macro->valTokensCount	= 0;
			macro->valParamsLinkedCount = 0;
			//PRINTF_INFO("Preprocessor: macro '%s' updated.\n", mcrName);
			if(dstDef != NULL) *dstDef = macro;
		} else {
			STSrcAnlzMacro data;
			data.nameStart			= state->names.lenght;
			data.params.start		= 0;
			data.params.count		= 0;
			data.params.areExplicit	= FALSE;
			data.params.isVaArgs	= FALSE;
			data.valTokensFirst		= 0;
			data.valTokensCount		= 0;
			data.valParamsLinkedCount = 0;
			NBArray_addValue(&state->defs, data);
			NBString_concat(&state->names, &strNames->str[iNameStart]);
			NBString_concatByte(&state->names, '\0');
			//Add search index
			STSrcAnlzStrIdx srchIdx;
			srchIdx.nameStart		= data.nameStart;
			srchIdx.strings			= &state->names;
			srchIdx.itmIndex		= state->defs.use - 1;
			NBArraySorted_addValue(&state->namesIdx, srchIdx);
			//
			if(dstDef != NULL) *dstDef = NBArray_itmPtrAtIndex(&state->defs, STSrcAnlzMacro, srchIdx.itmIndex);
			//PRINTF_INFO("Preprocessor: macro '%s' added.\n", mcrName);
		}
		r = TRUE;
	}
	return r;
}

BOOL SrcAnlzMacros_macroParamsStart(STSrcAnlzMacros* state, STSrcAnlzMacro* macro){
	BOOL r = FALSE;
	if(state != NULL && macro != NULL){
		NBASSERT(!macro->params.areExplicit)
		NBASSERT(!macro->params.isVaArgs)
		if(!macro->params.areExplicit){
			macro->params.areExplicit	= TRUE;
			macro->params.start			= state->params.use; NBASSERT(macro->params.count == 0)
			r = TRUE;
		}
	}
	return r;
}

BOOL SrcAnlzMacros_macroParamsAdd(STSrcAnlzMacros* state, STSrcAnlzMacro* macro, const char* paramName){
	BOOL r = FALSE;
	if(state != NULL && macro != NULL){
		NBASSERT(macro->params.areExplicit)	//'macroParamsStart' must be called before
		NBASSERT(!macro->params.isVaArgs)	//no param can be added after setting 'isVaArgs' to TRUE
		NBASSERT((macro->params.start + macro->params.count) == state->params.use) //All macro params must be set before setting other macro's param
		if(!macro->params.isVaArgs && macro->params.areExplicit && (macro->params.start + macro->params.count) == state->params.use){
			STSrcAnlzMacroParam newParam;
			newParam.nameStart = state->paramsNames.lenght;
			NBString_concat(&state->paramsNames, paramName);
			NBString_concatByte(&state->paramsNames, '\0');
			NBArray_addValue(&state->params, newParam);
			macro->params.count++;
			r = TRUE;
		}
	}
	return r;
}

BOOL SrcAnlzMacros_macroParamsAddVaArg(STSrcAnlzMacros* state, STSrcAnlzMacro* macro){
	BOOL r = FALSE;
	if(SrcAnlzMacros_macroParamsAdd(state, macro, "__VA_ARGS__")){
		macro->params.isVaArgs = TRUE;
		r = TRUE;
	}
	return r;
}

BOOL SrcAnlzMacros_macroParamsEnableVaArgAsLastParam(STSrcAnlzMacros* state, STSrcAnlzMacro* macro){
	BOOL r = FALSE;
	if(state != NULL && macro != NULL){
		NBASSERT(macro->params.areExplicit)	//'macroParamsStart' must be called before
		NBASSERT(!macro->params.isVaArgs)	//no param can be added after setting 'isVaArgs' to TRUE
		NBASSERT(macro->params.count > 0)	//must have at least one param
		if(!macro->params.isVaArgs && macro->params.count > 0){
			macro->params.isVaArgs = TRUE;
			r = TRUE;
		}
	}
	return r;
}

BOOL SrcAnlzMacros_macroValueAddToken(STSrcAnlzMacros* state, STSrcAnlzMacro* macro, const char* token, const STSrcAnlzSintxPart sintxPart, const BOOL isSpacePreceded){
	BOOL r = FALSE;
	if(state != NULL && macro != NULL && token != NULL){
		if(token[0] != '\0'){
			//Start value tokens range
			if(macro->valTokensCount == 0){
				macro->valTokensFirst = state->valsTokns.tokns.defs.use;
			}
			NBASSERT((macro->valTokensFirst + macro->valTokensCount) == state->valsTokns.tokns.defs.use) //Must be the last MACRO def
			if((macro->valTokensFirst + macro->valTokensCount) == state->valsTokns.tokns.defs.use){
				STSrcAnlzTokn newToken;
				newToken.iAtBuff	= state->valsTokns.tokns.strBuff.lenght;
				newToken.sintxPart	= sintxPart;
				newToken.isSpacePreceded = isSpacePreceded;
				NBString_concat(&state->valsTokns.tokns.strBuff, token);
				NBString_concatByte(&state->valsTokns.tokns.strBuff, '\0');
				NBArray_addValue(&state->valsTokns.tokns.defs, newToken);
				NBArray_addValueCopy(&state->valsTokns.iParamRefs, SI8, -1);
				macro->valTokensCount++;
				NBASSERT(state->valsTokns.tokns.defs.use == state->valsTokns.iParamRefs.use)
				r = TRUE;
			}
		}
	}
	return r;
}

BOOL SrcAnlzMacros_macroFinishBuild(STSrcAnlzMacros* state, STSrcAnlzMacro* macro){
	BOOL r = FALSE;
	if(state != NULL && macro != NULL){
		NBASSERT(state->valsTokns.tokns.defs.use == state->valsTokns.iParamRefs.use)
		if(macro->params.count > 0 && macro->valTokensCount > 0){
			SI8 i, i2;
			for(i = 0; i < macro->params.count; i++){
				const char* paramName = &state->paramsNames.str[NBArray_itmPtrAtIndex(&state->params, STSrcAnlzMacroParam, macro->params.start + i)->nameStart];
				for(i2 = 0; i2 < macro->valTokensCount; i2++){
					STSrcAnlzTokn* valToknDef = NBArray_itmPtrAtIndex(&state->valsTokns.tokns.defs, STSrcAnlzTokn, macro->valTokensFirst + i2);
					const char* valTokn = &state->valsTokns.tokns.strBuff.str[valToknDef->iAtBuff];
					if(NBString_strIsEqual(valTokn, paramName)){
						NBArray_setItemAt(&state->valsTokns.iParamRefs, macro->valTokensFirst + i2, &i, sizeof(i));
						macro->valParamsLinkedCount++;
						//PRINTF_INFO("Token '%s'(#%d) linked with param '%s' #%d.\n", valTokn, (i2 + 1), paramName, (i + 1));
					}
				}
			}
		}
		r = TRUE;
	}
	return r;
}
