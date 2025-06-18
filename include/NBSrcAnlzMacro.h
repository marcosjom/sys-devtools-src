//
//  NBSrcAnlzMacro.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_MACRO_H
#define NB_SRCANLZ_MACRO_H

#include "nb/NBFrameworkDefs.h"
#include "nb/core/NBString.h"
#include "nb/core/NBArray.h"
#include "nb/core/NBArraySorted.h"
#include "NBSrcAnlzBase.h"	//for 'STSrcAnlzRange'
#include "NBSrcAnlzTokn.h"	//for 'STSrcAnlzTokns'
#include "NBSrcAnlzSintx.h"	//for 'STSrcAnlzSintxPart'

#ifdef __cplusplus
//extern "C" {
#endif

//Macro param name definition
typedef struct STSrcAnlzMacroParam_ {
	UI32	nameStart;		//param name's first char index
} STSrcAnlzMacroParam;

//Macro params defs
typedef struct STSrcAnlzMacroParams_ {
	UI32	start;			//params's first def index
	UI32	count;			//params's count def index
	BOOL	areExplicit;	//param's are explicit (is function-like macro?)
	BOOL	isVaArgs;		//param's input lenght is flexible (last param is '...'?)
} STSrcAnlzMacroParams;

//Macro definition
typedef struct STSrcAnlzMacro_ {
	UI32					nameStart;				//macro name's first char index
	STSrcAnlzMacroParams	params;					//params definitions
	UI32					valTokensFirst;			//value's first token index
	UI32					valTokensCount;			//value's tokens count
	UI32					valParamsLinkedCount;	//times a param appears in the macro's value (optimization to determine if the value can be directly processed or must replace params references with their values)
} STSrcAnlzMacro;

typedef struct STSrcAnlzMacroTokns_ {
	STSrcAnlzTokns	tokns;		//Tokens defs
	STNBArray		iParamRefs;	//SI8, For each token the index to the param that references
} STSrcAnlzMacroTokns;

//Macros state
typedef struct STSrcAnlzMacros_ {
	STNBArray				defs;			//STSrcAnlzMacro, macros list
	STNBString				names;			//macros names string
	STNBArraySorted			namesIdx;		//STSrcAnlzStrIdx, macros search index by names
	STNBArray				params;			//STSrcAnlzMacroParam, macros params list
	STNBString				paramsNames;	//macros params names string
	STSrcAnlzMacroTokns		valsTokns;
} STSrcAnlzMacros;

//Init and release state
void SrcAnlzMacros_init(STSrcAnlzMacros* state);
void SrcAnlzMacros_release(STSrcAnlzMacros* state);

//Find
SI32 SrcAnlzMacros_findNameIdx(const STSrcAnlzMacros* state, const STNBString* strNames, const UI32 iNameStart);
SI32 SrcAnlzMacros_findNameIdxByStr(const STSrcAnlzMacros* state, const char* strName);

//Set macro definition
BOOL SrcAnlzMacros_macroCreateOrReset(STSrcAnlzMacros* state, STNBString* strNames, const UI32 iNameStart, STSrcAnlzMacro** dstDef);
BOOL SrcAnlzMacros_macroParamsStart(STSrcAnlzMacros* state, STSrcAnlzMacro* macro);
BOOL SrcAnlzMacros_macroParamsAdd(STSrcAnlzMacros* state, STSrcAnlzMacro* macro, const char* paramName);
BOOL SrcAnlzMacros_macroParamsAddVaArg(STSrcAnlzMacros* state, STSrcAnlzMacro* macro);
BOOL SrcAnlzMacros_macroParamsEnableVaArgAsLastParam(STSrcAnlzMacros* state, STSrcAnlzMacro* macro);
BOOL SrcAnlzMacros_macroValueAddToken(STSrcAnlzMacros* state, STSrcAnlzMacro* macro, const char* token, const STSrcAnlzSintxPart sintxPart, const BOOL isSpacePreceded);
BOOL SrcAnlzMacros_macroFinishBuild(STSrcAnlzMacros* state, STSrcAnlzMacro* macro);

#ifdef __cplusplus
//}
#endif

#endif
