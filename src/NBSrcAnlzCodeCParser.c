//
//  NBSrcAnlzCodeCParser.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzCodeCParser.h"
#include "NBSrcAnlzSintx.h"					//for 'SrcAnlzSintx_searchKeyword'
#include "NBSrcAnlzSintxC.h"				//for all the lang sintaxes
#include "NBSrcAnlzTokn.h"					//for 'ENSrcAnlzToknType'
#include "NBSrcAnlzCodeCParserConsume.h"	//for 'SrcAnlzCodeCParser_consume'
#include "NBSrcAnlzCodeCParserPProc.h"		//for 'SrcAnlzCodeCParser_pprocFeedDirective'

//-------------------
// Own interface
//-------------------

BOOL SrcAnlzCodeCParser_feedToknI(STSrcAnlzLangSntxI* itfObj, const char* toknStr, const ENSrcAnlzToknType toknType, const SI32 toknSubType, const SI32 toknTypeVal, const BOOL precededBySpace){
	BOOL r = TRUE;
	NBASSERT(itfObj != NULL && itfObj->obj != NULL)
	STSrcAnlzCodeCParser* state	= (STSrcAnlzCodeCParser*)itfObj->obj;
	if(toknType != ENSrcAnlzToknType_Comment){
		//Not-a-comment token
		STSrcAnlzSintxPart sintaxDefPart;
		switch(toknType){
			case ENSrcAnlzToknType_Keyword:
				sintaxDefPart.type	= ENSrzAnlzSintxPartType_Keyword;
				sintaxDefPart.idx	= toknTypeVal; NBASSERT(toknTypeVal != -1)
				NBASSERT(toknTypeVal >= 0 && toknTypeVal < state->sintxDefs.keywsSz)
				NBASSERT(NBString_strIsEqual(toknStr, state->sintxDefs.keyws[toknTypeVal]))
				break;
			case ENSrcAnlzToknType_Identifier:
				NBASSERT(toknTypeVal == -1)
				sintaxDefPart.idx	= SrcAnlzSintx_searchKeyword(toknStr, NBString_strLenBytes(toknStr), state->sintxDefs.keyws, state->sintxDefs.keywsSz, state->sintxDefs.keywsRngs, state->sintxDefs.keywsRngsSz);
				sintaxDefPart.type	= (sintaxDefPart.idx == -1 ? ENSrzAnlzSintxPartType_Identifier : ENSrzAnlzSintxPartType_Keyword);
				break;
			case ENSrcAnlzToknType_Constant:
				sintaxDefPart.type	= ENSrzAnlzSintxPartType_Constant;
				sintaxDefPart.idx	= toknTypeVal; NBASSERT(toknTypeVal == -1)
				break;
			case ENSrcAnlzToknType_StrLiteral:
				sintaxDefPart.type	= ENSrzAnlzSintxPartType_StrLiteral;
				sintaxDefPart.idx	= toknTypeVal; NBASSERT(toknTypeVal == -1)
				break;
			case ENSrcAnlzToknType_Punctuator:
				sintaxDefPart.type	= ENSrzAnlzSintxPartType_Punctuator;
				sintaxDefPart.idx	= toknTypeVal; NBASSERT(toknTypeVal != -1)
				NBASSERT(toknTypeVal >= 0 && toknTypeVal < state->sintxDefs.punctsSz)
				NBASSERT(NBString_strIsEqual(toknStr, state->sintxDefs.puncts[toknTypeVal]))
				break;
			case ENSrcAnlzToknType_Comment:
				sintaxDefPart.type	= ENSrzAnlzSintxPartType_Count;
				sintaxDefPart.idx	= toknTypeVal; NBASSERT(toknTypeVal == -1)
				break;
			case ENSrcAnlzToknType_HeaderName:
				sintaxDefPart.type	= ENSrzAnlzSintxPartType_StrLiteral;
				sintaxDefPart.idx	= toknTypeVal; NBASSERT(toknTypeVal == -1)
				break;
			case ENSrcAnlzToknType_Count:
				sintaxDefPart.type	= ENSrzAnlzSintxPartType_Count;
				sintaxDefPart.idx	= toknTypeVal; NBASSERT(toknTypeVal == -1)
				break;
			default:
				NBASSERT(FALSE)
				break;
		}
		r = SrcAnlzCodeCParser_feedTokn(state, toknStr, sintaxDefPart, precededBySpace);
	}
	return r;
}

BOOL SrcAnlzCodeCParser_flushI(STSrcAnlzLangSntxI* itfObj){
	NBASSERT(itfObj != NULL && itfObj->obj != NULL)
	STSrcAnlzCodeCParser* state	= (STSrcAnlzCodeCParser*)itfObj->obj;
	return SrcAnlzCodeCParser_flush(state);
}

BOOL SrcAnlzCodeCParser_endI(STSrcAnlzLangSntxI* itfObj){
	NBASSERT(itfObj != NULL && itfObj->obj != NULL)
	STSrcAnlzCodeCParser* state	= (STSrcAnlzCodeCParser*)itfObj->obj;
	return SrcAnlzCodeCParser_end(state);
}

//---------------------------
// Init and release
//---------------------------

void SrcAnlzCodeCParser_init(STSrcAnlzCodeCParser* state, STSrcAnlzParserItfs* itfs, STSrcAnlzCodeC* code){
	SrcAnlzSintxC_initDefs(&state->sintxDefs);
	state->itfs		= itfs;
	//Own sintax parser interface
	{
		state->ownSntxItf.obj			= state;
		state->ownSntxItf.feedToknFunc	= SrcAnlzCodeCParser_feedToknI;
		state->ownSntxItf.flushFunc		= SrcAnlzCodeCParser_flushI;
		state->ownSntxItf.endFunc		= SrcAnlzCodeCParser_endI;
		if(itfs != NULL){
			itfs->sintaxItf				= &state->ownSntxItf;
		}
	}
	state->code		= code;
	SrcAnlzCodeCParser_toknInit(&state->toknParse, &state->sintxDefs);
	//Own token parser interface
	{
		state->ownToknItf.itfsSz	= (sizeof(state->toknParse.itfs) / sizeof(state->toknParse.itfs[0]));
		state->ownToknItf.itfs		= (STSrcAnlzToknParseTypeI*)NBMemory_alloc(sizeof(STSrcAnlzToknParseTypeI) * state->ownToknItf.itfsSz);
		SI32 i; for(i = 0; i < state->ownToknItf.itfsSz; i++){
			state->ownToknItf.itfs[i] = state->toknParse.itfs[i];
		}
		if(itfs != NULL){
			itfs->toknItf			= &state->ownToknItf;
		}
	}
	//
	state->curBlqType		= ENSrcAnlzCodeCParseMode_Code;
	NBArray_init(&state->pprocCondPortionsStack, sizeof(STSrcAnlzCodeCondPortionState), NULL);
	//First code block state
	{
		STSrcAnlzCodeCondPortionState data;
		data.isParentActive			= TRUE;
		data.isPortionActive		= TRUE;
		data.isPortionActiveFound	= TRUE;
		data.isPortionElseFound		= TRUE;
		NBArray_addValue(&state->pprocCondPortionsStack, data);
	}
	//
	NBArray_init(&state->acumTokensPProc.defs, sizeof(STSrcAnlzTokn), NULL);
	NBString_init(&state->acumTokensPProc.strBuff);
	NBString_concatByte(&state->acumTokensPProc.strBuff, '\0');
	state->acumTokensCur			= NULL;
	//Included files
	NBArray_init(&state->includedStarts, sizeof(UI32), NBCompareUI32);
	NBString_init(&state->included);
	//Config: user and system include paths
	NBArray_init(&state->usrIncludesStarts, sizeof(UI32), NBCompareUI32);
	NBString_init(&state->usrIncludes);
	NBArray_init(&state->sysIncludesStarts, sizeof(UI32), NBCompareUI32);
	NBString_init(&state->sysIncludes);
	//defined MACROS
	SrcAnlzMacros_init(&state->macros);
	//
	NBArray_init(&state->parseScopesStack, sizeof(STSrcAnlzCodeCParseScope), NULL);
	//Push first (not valid) file scope (index 0 must be not-valid)
	{
		STSrcAnlzCodeCParseScope fScope;
		fScope.type						= ENSrcAnlzCodeCParseScopeType_Count;
		fScope.iSintx					= ENSrzAnlzSintxC_Count;
		fScope.iDeclScope				= 0;
		fScope.stats.qSintaxSeqs		= 0;
		fScope.stats.usrTypesRng.start	= 0;
		fScope.stats.usrTypesRng.count	= 0;
		fScope.stats.declarsRng.start	= 0;
		fScope.stats.declarsRng.count	= 0;
		fScope.stats.scopesRng.start	= 0;
		fScope.stats.scopesRng.count	= 0;
		fScope.stats.iLastActiveDecScope = 0;
		SrcAnlzSintxParseTree_init(&fScope.parseTree);
		NBArray_init(&fScope.acumTokens.defs, sizeof(STSrcAnlzTokn), NULL);
		NBString_init(&fScope.acumTokens.strBuff);
		NBArray_init(&fScope.charLvlStack, sizeof(char), NBComparechar);
		NBArray_addValue(&state->parseScopesStack, fScope);
	}
	//Push first (valid) file scope
	{
		//
		STSrcAnlzCodeCParseScope fScope;
		fScope.type						= ENSrcAnlzCodeCParseScopeType_Normal;
		fScope.iSintx					= ENSrzAnlzSintxC_Count;
		fScope.iDeclScope				= (code->defsDeclarsScopes.use - 1);
		fScope.stats.qSintaxSeqs		= 0;
		fScope.stats.usrTypesRng.start	= code->defsUsrTypes.use;
		fScope.stats.usrTypesRng.count	= 0;
		fScope.stats.declarsRng.start	= code->defsDeclars.use;
		fScope.stats.declarsRng.count	= 0;
		fScope.stats.scopesRng.start	= code->defsDeclarsScopes.use;
		fScope.stats.scopesRng.count	= 0;
		fScope.stats.iLastActiveDecScope = 0; //Last active declars-scope
		SrcAnlzSintxParseTree_init(&fScope.parseTree);
		NBArray_init(&fScope.acumTokens.defs, sizeof(STSrcAnlzTokn), NULL);
		NBString_init(&fScope.acumTokens.strBuff);
		NBString_concatByte(&fScope.acumTokens.strBuff, '\0');
		NBArray_init(&fScope.charLvlStack, sizeof(char), NBComparechar);
		//
		NBArray_addValue(&state->parseScopesStack, fScope);
	}
	//
	state->curParamsReadState	= NULL;
	//
	state->curLineTokensCount		= 0;
	//Set current acumTokens
	{
		NBASSERT(state->parseScopesStack.use == 2)
		state->acumTokensCur = (state->curBlqType == ENSrcAnlzCodeCParseMode_Code ? &NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, state->parseScopesStack.use - 1)->acumTokens : &state->acumTokensPProc);
	}
}

void SrcAnlzCodeCParser_release(STSrcAnlzCodeCParser* state){
	{
		state->ownSntxItf.obj			= NULL;
		state->ownSntxItf.feedToknFunc	= NULL;
		state->ownSntxItf.flushFunc		= NULL;
		state->ownSntxItf.endFunc		= NULL;
	}
	state->code				= NULL;
	{
		if(state->ownToknItf.itfs != NULL){
			NBMemory_free(state->ownToknItf.itfs);
		}
		state->ownToknItf.itfs		= NULL;
		state->ownToknItf.itfsSz	= 0;
	}
	SrcAnlzCodeCParser_toknRelease(&state->toknParse);
	//
	state->curBlqType		= ENSrcAnlzCodeCParseMode_Code;
	NBArray_release(&state->pprocCondPortionsStack);
	//
	NBArray_release(&state->acumTokensPProc.defs);
	NBString_release(&state->acumTokensPProc.strBuff);
	state->acumTokensCur		= NULL;
	//Included files
	NBArray_release(&state->includedStarts);
	NBString_release(&state->included);
	//Config: user and system include paths
	NBArray_release(&state->usrIncludesStarts);
	NBString_release(&state->usrIncludes);
	NBArray_release(&state->sysIncludesStarts);
	NBString_release(&state->sysIncludes);
	//defined MACROS
	SrcAnlzMacros_release(&state->macros);
	//
	{
		SI32 i; for(i = state->parseScopesStack.use - 1; i > 0; i--){
			NBASSERT(i != 0)
			STSrcAnlzCodeCParseScope* scope = NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, i);
			SrcAnlzSintxParseTree_release(&scope->parseTree);
			NBArray_release(&scope->acumTokens.defs);
			NBString_release(&scope->acumTokens.strBuff);
			NBArray_release(&scope->charLvlStack);
		}
		NBArray_empty(&state->parseScopesStack);
		NBArray_release(&state->parseScopesStack);
		
	}
	//
	if(state->curParamsReadState != NULL){
		{
			STSrcAnlzCodeCParseMacroParams* pState = state->curParamsReadState;
			NBString_release(&pState->strBuff);
			NBArray_release(&pState->tokens);
			NBArray_release(&pState->paramsRanges);
		}
		NBMemory_free(state->curParamsReadState);
		state->curParamsReadState = NULL;
	}
	//
	state->curLineTokensCount = 0;
}

//---------------------------
// Config
//---------------------------

BOOL SrcAnlzCodeCParser_addIncludePathUsr(STSrcAnlzCodeCParser* state, const char* path){
	BOOL r = FALSE;
	if(path != NULL){
		if(path[0] != '\0'){
			const char lastChar = path[NBString_strLenBytes(path) - 1];
			NBArray_addValueCopy(&state->usrIncludesStarts, UI32, state->usrIncludes.lenght);
			NBString_concat(&state->usrIncludes, path);
			//Add "/" at the end if necesary
			if(lastChar != '/' && lastChar != '\\') NBString_concatByte(&state->usrIncludes, '/');
			NBString_concatByte(&state->usrIncludes, '\0');
			r = TRUE;
		}
	}
	return r;
}

BOOL SrcAnlzCodeCParser_addIncludePathSys(STSrcAnlzCodeCParser* state, const char* path){
	BOOL r = FALSE;
	if(path != NULL){
		if(path[0] != '\0'){
			const char lastChar = path[NBString_strLenBytes(path) - 1];
			NBArray_addValueCopy(&state->sysIncludesStarts, UI32, state->sysIncludes.lenght);
			NBString_concat(&state->sysIncludes, path);
			//Add "/" at the end if necesary
			if(lastChar != '/' && lastChar != '\\') NBString_concatByte(&state->sysIncludes, '/');
			NBString_concatByte(&state->sysIncludes, '\0');
			r = TRUE;
		}
	}
	return r;
}

//---------------------------
// Code parse scope stack
//---------------------------

BOOL SrcAnlzCodeCParser_scopePush(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, const ENSrcAnlzCodeCParseScopeType parScopeType, const ENSrzAnlzSintxC iSintx, const UI32 iParentDeclarScope, const BOOL alsoMoveOldTopLastToken){
	BOOL r = FALSE;
	//
	NBASSERT(state->curBlqType == ENSrcAnlzCodeCParseMode_Code) //Tmp: must be in code mode
	NBASSERT(state->acumTokensCur != &state->acumTokensPProc) //Tmp: must be in code mode
	//
	NBASSERT(parScopeType < ENSrcAnlzCodeCParseScopeType_Count)
	NBASSERT(state->parseScopesStack.use > 1)
	if(state->parseScopesStack.use > 1){
		STSrcAnlzCodeCParseScope* oldTop = NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, state->parseScopesStack.use - 1); NBASSERT(oldTop->acumTokens.defs.use > 0)
/*#		ifdef NB_CONFIG_INCLUDE_ASSERTS
		//Verify scopes's ranges
		if(oldTop->iDeclScope != 0){
			UI32 iDeclScope2 = oldTop->iDeclScope;
			const STSrcAnlzCodeCDeclarsScope* scope2 = NBArray_itmPtrAtIndex(&code->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
			do {
				NBASSERT((scope2->usrTypesRng.start + scope2->usrTypesRng.count) == code->defsUsrTypes.use)		//Must be last declarScope
				NBASSERT((scope2->declarsRng.start + scope2->declarsRng.count) == code->defsDeclars.use)		//Must be last declarScope
				NBASSERT((scope2->scopesRng.start + scope2->scopesRng.count) == code->defsDeclarsScopes.use)	//Must be last declarScope
				//PRINTF_INFO("Verifying scope %d's range from(%d, +%d) to(%d, +%d).\n", iDeclScope2, scope2->scopesRng.start, scope2->scopesRng.count, scope2->scopesRng.start, (code->defsDeclarsScopes.use - scope2->scopesRng.start));
				if(scope2->iParentScope == 0){
					break;
				} else {
					iDeclScope2	= scope2->iParentScope;
					scope2		= NBArray_itmPtrAtIndex(&code->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
				}
			} while(TRUE);
		}
#		endif*/
		//Create the new declarScope
		UI32 iDeclScope = 0;
		if(iSintx < ENSrzAnlzSintxC_Count){
			const STSrcAnlzCodeCDeclarsScope* parentScope = NBArray_itmPtrAtIndex(&code->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iParentDeclarScope);
			iDeclScope					= code->defsDeclarsScopes.use;
			STSrcAnlzCodeCDeclarsScope dScope;
			dScope.iParentScope			= iParentDeclarScope;
			dScope.iDeepLvl				= (parentScope->iDeepLvl + 1);
			dScope.usrTypesRng.start	= code->defsUsrTypes.use;
			dScope.usrTypesRng.count	= 0;
			dScope.declarsRng.start		= code->defsDeclars.use;
			dScope.declarsRng.count		= 0;
			dScope.scopesRng.start		= code->defsDeclarsScopes.use + 1; //+1 to exclude itself
			dScope.scopesRng.count		= 0;
			NBArray_addValue(&code->defsDeclarsScopes, dScope);
		}
		//Create and push the new scope
		{
			STSrcAnlzCodeCParseScope scope;
			scope.type						= parScopeType;
			scope.iSintx					= iSintx;
			scope.iDeclScope				= iDeclScope;
			scope.stats.qSintaxSeqs			= 0;
			scope.stats.usrTypesRng.start	= code->defsUsrTypes.use;
			scope.stats.usrTypesRng.count	= 0;
			scope.stats.declarsRng.start	= code->defsDeclars.use;
			scope.stats.declarsRng.count	= 0;
			scope.stats.scopesRng.start		= code->defsDeclarsScopes.use;
			scope.stats.scopesRng.count		= 0;
			scope.stats.iLastActiveDecScope	= 0; //Last active declars-scope
			SrcAnlzSintxParseTree_init(&scope.parseTree);
			NBArray_init(&scope.acumTokens.defs, sizeof(STSrcAnlzTokn), NULL);
			NBString_init(&scope.acumTokens.strBuff);
			NBString_concatByte(&scope.acumTokens.strBuff, '\0');
			NBArray_init(&scope.charLvlStack, sizeof(char), NBComparechar);//stacks of chars that opened a inner level
			//Move old top last token to this scope
			if(alsoMoveOldTopLastToken){
				const STSrcAnlzTokn* oldTokenDef = NBArray_itmPtrAtIndex(&oldTop->acumTokens.defs, STSrcAnlzTokn, oldTop->acumTokens.defs.use - 1);
				const char* oldTokenStr		= &oldTop->acumTokens.strBuff.str[oldTokenDef->iAtBuff];
				const SI32 oldTokenLen		= NBString_strLenBytes(oldTokenStr);
				NBASSERT(oldTop->acumTokens.strBuff.lenght == (oldTokenDef->iAtBuff + oldTokenLen + 1)) //must be the last string in buffer
				//Add copy to new top
				STSrcAnlzTokn newToken;
				newToken.iAtBuff			= scope.acumTokens.strBuff.lenght;
				newToken.sintxPart			= oldTokenDef->sintxPart;
				newToken.isSpacePreceded	= oldTokenDef->isSpacePreceded;
				NBArray_addValue(&scope.acumTokens.defs, newToken);
				NBString_concatBytes(&scope.acumTokens.strBuff, oldTokenStr, oldTokenLen + 1);
				//Remove from old top
				NBString_removeLastBytes(&oldTop->acumTokens.strBuff, oldTokenLen + 1);
				NBArray_removeItemAtIndex(&oldTop->acumTokens.defs, oldTop->acumTokens.defs.use - 1);
			}
			//Add new scope
			NBArray_addValue(&state->parseScopesStack, scope);
			//Update acumToken link
			if(state->curBlqType == ENSrcAnlzCodeCParseMode_Code){
				state->acumTokensCur = &NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, state->parseScopesStack.use - 1)->acumTokens;
			}
		}
		PRINTF_INFO("SCOPE (#%d) PUSHED '%s' with declareScope(@%d)andParent(@%d).\n", state->parseScopesStack.use, (iSintx < ENSrzAnlzSintxC_Count ? state->sintxDefs.defs[iSintx].name : "noSintx"), iDeclScope, iParentDeclarScope);
		//
		r = TRUE;
	}
	return r;
}

BOOL SrcAnlzCodeCParser_scopePop(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, ENSrzAnlzSintxC* dstSintx, UI32* dstDeclarScope, const BOOL alsoMoveOldTopLastToken){
	BOOL r = FALSE;
	//
	NBASSERT(state->curBlqType == ENSrcAnlzCodeCParseMode_Code) //Tmp: must be in code mode
	NBASSERT(state->acumTokensCur != &state->acumTokensPProc) //Tmp: must be in code mode
	//
	NBASSERT(state->parseScopesStack.use > 2)
	if(state->parseScopesStack.use > 2){
		STSrcAnlzCodeCParseScope* oldTop = NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, state->parseScopesStack.use - 1);
		PRINTF_INFO("SCOPE (#%d) POPEP '%s' with declareScope(@%d).\n", state->parseScopesStack.use, (oldTop->iSintx < ENSrzAnlzSintxC_Count ? state->sintxDefs.defs[oldTop->iSintx].name : "noSintx"), oldTop->iDeclScope);
#		ifdef NB_CONFIG_INCLUDE_ASSERTS
		//Verify scopes's ranges
		if(oldTop->iDeclScope != 0){
			UI32 iDeclScope2 = oldTop->iDeclScope;
			STSrcAnlzCodeCDeclarsScope* scope2 = NBArray_itmPtrAtIndex(&code->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
			do {
				//ToDo: remove updates (only verify)
				scope2->usrTypesRng.count = (code->defsUsrTypes.use - scope2->usrTypesRng.start);
				scope2->declarsRng.count = (code->defsDeclars.use - scope2->declarsRng.start);
				scope2->scopesRng.count = (code->defsDeclarsScopes.use - scope2->scopesRng.start);
				NBASSERT((scope2->usrTypesRng.start + scope2->usrTypesRng.count) == code->defsUsrTypes.use)		//Must be last declarScope
				NBASSERT((scope2->declarsRng.start + scope2->declarsRng.count) == code->defsDeclars.use)		//Must be last declarScope
				//NBASSERT((scope2->scopesRng.start + scope2->scopesRng.count) == code->defsDeclarsScopes.use)	//Must be last declarScope
				//PRINTF_INFO("Verifying scope %d's range from(%d, +%d) to(%d, +%d).\n", iDeclScope2, scope2->scopesRng.start, scope2->scopesRng.count, scope2->scopesRng.start, (code->defsDeclarsScopes.use - scope2->scopesRng.start));
				if(scope2->iParentScope == 0){
					break;
				} else {
					iDeclScope2	= scope2->iParentScope;
					scope2		= NBArray_itmPtrAtIndex(&code->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
				}
			} while(TRUE);
		}
#		endif
		//Move old top last token to new top scope
		if(alsoMoveOldTopLastToken){
			NBASSERT(oldTop->acumTokens.defs.use > 0)
			const STSrcAnlzTokn* oldTokenDef = NBArray_itmPtrAtIndex(&oldTop->acumTokens.defs, STSrcAnlzTokn, oldTop->acumTokens.defs.use - 1);
			const char* oldTokenStr		= &oldTop->acumTokens.strBuff.str[oldTokenDef->iAtBuff];
			const SI32 oldTokenLen		= NBString_strLenBytes(oldTokenStr);
			NBASSERT(oldTop->acumTokens.strBuff.lenght == (oldTokenDef->iAtBuff + oldTokenLen + 1)) //must be the last string in buffer
			//Add copy to new top
			STSrcAnlzCodeCParseScope* newTop = NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, state->parseScopesStack.use - 2);
			STSrcAnlzTokn newToken;
			newToken.iAtBuff			= newTop->acumTokens.strBuff.lenght;
			newToken.sintxPart			= oldTokenDef->sintxPart;
			newToken.isSpacePreceded	= oldTokenDef->isSpacePreceded;
			NBArray_addValue(&newTop->acumTokens.defs, newToken);
			NBString_concatBytes(&newTop->acumTokens.strBuff, oldTokenStr, oldTokenLen + 1);
			//Remove from old top
			NBString_removeLastBytes(&oldTop->acumTokens.strBuff, oldTokenLen + 1);
			NBArray_removeItemAtIndex(&oldTop->acumTokens.defs, oldTop->acumTokens.defs.use - 1);
		}
		if(dstSintx != NULL) *dstSintx = oldTop->iSintx; oldTop->iSintx = ENSrzAnlzSintxC_Count;
		if(dstDeclarScope != NULL) *dstDeclarScope = oldTop->iDeclScope; oldTop->iDeclScope = 0;
		//Release
		NBArray_release(&oldTop->parseTree.nodes);
		NBArray_release(&oldTop->parseTree.freeNodesIdxs);
		NBArray_release(&oldTop->acumTokens.defs);
		NBString_release(&oldTop->acumTokens.strBuff);
		NBArray_release(&oldTop->charLvlStack);
		//Remove scope
		NBArray_removeItemAtIndex(&state->parseScopesStack, state->parseScopesStack.use - 1);
		//Update acumToken link
		if(state->curBlqType == ENSrcAnlzCodeCParseMode_Code){
			state->acumTokensCur = &NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, state->parseScopesStack.use - 1)->acumTokens;
		}
		//
		r = TRUE;
	}
	return r;
}

//---------------------------
// Scope feed
//---------------------------

//ParseScope (auto)
BOOL SrcAnlzCodeCParser_feedToknPriv(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue);
//ParseScope of normal code
BOOL SrcAnlzCodeCParser_feedToknPrivScopeNormal(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, STSrcAnlzCodeCParseScope* scope, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue);
//ParseScope of a __attribute (C/C++ extension)
BOOL SrcAnlzCodeCParser_feedToknPrivScopeAttribute(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, STSrcAnlzCodeCParseScope* scope, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue);
//ParseScope of __asm code (C/C++ extension)
BOOL SrcAnlzCodeCParser_feedToknPrivScopeASM(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, STSrcAnlzCodeCParseScope* scope, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue);

//ParseScope (auto)
BOOL SrcAnlzCodeCParser_feedToknPriv(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue){
	BOOL r = FALSE;
	if(state != NULL){
		r = TRUE;
		//
#		ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
		//Validate params (last token of current parScope must match with state->dbgCurTokenStr.str)
		if(partFound.type != ENSrzAnlzSintxPartType_SintaxDef){
			NBASSERT(partIdxValue < state->acumTokensCur->defs.use) //Index must be in range
			switch(partFound.type){
				case ENSrzAnlzSintxPartType_Keyword:
					NBASSERT(partFound.idx >= 0 && partFound.idx < state->sintxDefs.keywsSz)
					NBASSERT(NBString_strIsEqual(state->sintxDefs.keyws[partFound.idx], &state->acumTokensCur->strBuff.str[NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, partIdxValue)->iAtBuff]))
					break;
				case ENSrzAnlzSintxPartType_Punctuator:
					NBASSERT(partFound.idx >= 0 && partFound.idx < state->sintxDefs.punctsSz)
					NBASSERT(NBString_strIsEqual(state->sintxDefs.puncts[partFound.idx], &state->acumTokensCur->strBuff.str[NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, partIdxValue)->iAtBuff]))
					break;
				case ENSrzAnlzSintxPartType_Identifier: NBASSERT(partFound.idx == -1) break;
				case ENSrzAnlzSintxPartType_Constant: NBASSERT(partFound.idx == -1) break;
				case ENSrzAnlzSintxPartType_StrLiteral: NBASSERT(partFound.idx == -1) break;
				case ENSrzAnlzSintxPartType_SintaxDef: NBASSERT(partFound.idx != -1) break;
				default: NBASSERT(FALSE) break;
			}
		}
#		endif
		//Get top-active scope
		NBASSERT(state->parseScopesStack.use > 1)
		if(state->parseScopesStack.use <= 1){
			r = FALSE; NBASSERT(FALSE) //No active parsing scope
		} else {
			const UI32 iScope = (state->parseScopesStack.use - 1);
			STSrcAnlzCodeCParseScope* scope = NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, iScope);
			NBASSERT(scope->parseTree.nodes.use > 0)
			NBASSERT(SrcAnlzSintxParseTree_dbgValidate(&state->sintxDefs, &scope->parseTree));
			if(scope->type == ENSrcAnlzCodeCParseScopeType_ASM){
				//Currently in an ASM scope
				PRINTF_INFO("Token is at ASM scope.\n");
				if(!SrcAnlzCodeCParser_feedToknPrivScopeASM(state, code, scope, partFound, partIdxValue)){
					r = FALSE; NBASSERT(FALSE)
				}
			} else if(scope->type == ENSrcAnlzCodeCParseScopeType_Attribute){
				//Currently in an ATTRIBUTE scope
				PRINTF_INFO("Token is at ATTRIBUTE scope.\n");
				if(!SrcAnlzCodeCParser_feedToknPrivScopeAttribute(state, code, scope, partFound, partIdxValue)){
					r = FALSE; NBASSERT(FALSE)
				}
			} else {
				//Currently in an NORMAL scope
				PRINTF_INFO("Token is at NORMAL scope.\n");
				NBASSERT(scope->type == ENSrcAnlzCodeCParseScopeType_Normal)
				NBASSERT(scope->iDeclScope != 0) //Any Normal parseScope should have a declarScope
				BOOL tokenConsumed = FALSE;
				if(partFound.type == ENSrzAnlzSintxPartType_Identifier){
					NBASSERT(partIdxValue < state->acumTokensCur->defs.use) //Index must be in range
					const STSrcAnlzTokn* tokenDef = NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, partIdxValue);
					const char* tokenStr	= &state->acumTokensCur->strBuff.str[tokenDef->iAtBuff];
					const BOOL isAttribute	= (NBString_strIsEqual(tokenStr, "__attribute__") || NBString_strIsEqual(tokenStr, "__attribute"));
					const BOOL isASM		= NBString_strIsEqual(tokenStr, "__asm__") || NBString_strIsEqual(tokenStr, "__asm") || NBString_strIsEqual(tokenStr, "asm");
					if(isAttribute || isASM){
						NBASSERT(code->defsDeclarsScopes.use == (scope->stats.scopesRng.start + scope->stats.scopesRng.count))
						//Execute any partial-posibility (if available)
						UI32 iParentScope = scope->iDeclScope;
						const STSrcAnlzSintxParseTreeNode* rootNode = NBArray_itmPtrAtIndex(&scope->parseTree.nodes, STSrcAnlzSintxParseTreeNode, 0);
						//NBASSERT(!(scope->iDeclScope == 0 && rootNode->iNextPart != 0)) //Logic error, posibility found with no parent declarScope
						if(rootNode->iNextPart != 0){
							scope->stats.iLastActiveDecScope = scope->iDeclScope;
							const STSrcAnlzSintxParseTreeNode* firstPosib = NBArray_itmPtrAtIndex(&scope->parseTree.nodes, STSrcAnlzSintxParseTreeNode, rootNode->iNextPart); NBASSERT(firstPosib->iChildSeq != 0)
							const STSrcAnlzCodeCParseScopeStats parScopeStatsBkp = scope->stats;
							if(!SrcAnlzCodeCParser_consume(state, scope, state->code, firstPosib->iChildSeq)){
								r = FALSE; NBASSERT(FALSE)
							} else {
								NBASSERT(parScopeStatsBkp.iLastActiveDecScope > 0 && parScopeStatsBkp.iLastActiveDecScope < code->defsDeclarsScopes.use)
								NBASSERT(scope->stats.iLastActiveDecScope > 0 && scope->stats.iLastActiveDecScope < code->defsDeclarsScopes.use)
								NBASSERT(code->defsDeclarsScopes.use == (scope->stats.scopesRng.start + scope->stats.scopesRng.count)) //Validate before reseting stats
								//PRINTF_INFO("Restoring stats after partial sintaxSeq: from types(%d, +%d)-declrs(%d, +%d) -> to types(%d, +%d)-declrs(%d, +%d).\n", scope->stats.usrTypesRng.start, scope->stats.usrTypesRng.count, scope->stats.declarsRng.start, scope->stats.declarsRng.count, parScopeStatsBkp.usrTypesRng.start, parScopeStatsBkp.usrTypesRng.count, parScopeStatsBkp.declarsRng.start, parScopeStatsBkp.declarsRng.count);
								iParentScope		= scope->stats.iLastActiveDecScope;
								scope->stats		= parScopeStatsBkp;
							}
						}
						//Push parsing scope
						if(r){
							if(isASM){
								if(!SrcAnlzCodeCParser_scopePush(state, code, ENSrcAnlzCodeCParseScopeType_ASM, ENSrzAnlzSintxC_compound_statement, scope->iDeclScope, TRUE)){
									r = FALSE; NBASSERT(FALSE)
								} else {
									tokenConsumed = TRUE;
								}
							} else if(isAttribute){
								if(!SrcAnlzCodeCParser_scopePush(state, code, ENSrcAnlzCodeCParseScopeType_Attribute, ENSrzAnlzSintxC_Count, 0, TRUE)){
									r = FALSE; NBASSERT(FALSE)
								} else {
									tokenConsumed = TRUE;
								}
							} else {
								r = FALSE; NBASSERT(FALSE)
							}
						}
					}
				}
				//Feed
				if(r && !tokenConsumed){
					if(!SrcAnlzCodeCParser_feedToknPrivScopeNormal(state, code, scope, partFound, partIdxValue)){
						r = FALSE; NBASSERT(FALSE) //No active parsing scope
					}
				}
			}
		}
	}
	return r;
}

//ParseScope of a __attribute (C/C++ extension)
BOOL SrcAnlzCodeCParser_feedToknPrivScopeAttribute(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, STSrcAnlzCodeCParseScope* scope, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue){
	BOOL r = TRUE;
	//Dummy ATTRIBUTE processing, basically process until the last scope is closed.
	//Expecting something like:
	//__attribute__((aligned(8)))
	//__attribute__((noreturn))
	//__attribute__((format(printf,1,2)))
	if(partFound.type == ENSrzAnlzSintxPartType_Punctuator){
		BOOL popped = FALSE;
		NBASSERT(partIdxValue < state->acumTokensCur->defs.use) //Index must be in range
		NBASSERT(partFound.idx >= 0 && partFound.idx < state->sintxDefs.punctsSz)
		if(NBString_strIsEqual(state->sintxDefs.puncts[partFound.idx], "{")){
			NBArray_addValueCopy(&scope->charLvlStack, char, '{');
		} else if(NBString_strIsEqual(state->sintxDefs.puncts[partFound.idx], "}")){
			if(scope->charLvlStack.use == 0){
				r = FALSE; NBASSERT(FALSE) //No level to close
			} else if(NBArray_itmValueAtIndex(&scope->charLvlStack, char, scope->charLvlStack.use - 1) != '{'){
				r = FALSE; NBASSERT(FALSE) //Level opening unmatch
			} else {
				//Remove level
				NBArray_removeItemAtIndex(&scope->charLvlStack, scope->charLvlStack.use - 1);
				popped = TRUE;
			}
		} else if(NBString_strIsEqual(state->sintxDefs.puncts[partFound.idx], "(")){
			NBArray_addValueCopy(&scope->charLvlStack, char, '(');
		} else if(NBString_strIsEqual(state->sintxDefs.puncts[partFound.idx], ")")){
			if(scope->charLvlStack.use == 0){
				r = FALSE; NBASSERT(FALSE) //No level to close
			} else if(NBArray_itmValueAtIndex(&scope->charLvlStack, char, scope->charLvlStack.use - 1) != '('){
				r = FALSE; NBASSERT(FALSE) //Level opening unmatch
			} else {
				//Remove level
				NBArray_removeItemAtIndex(&scope->charLvlStack, scope->charLvlStack.use - 1);
				popped = TRUE;
			}
		}
		//Pop __attribute parScope
		if(popped && scope->charLvlStack.use == 0){
			ENSrzAnlzSintxC iSintx = ENSrzAnlzSintxC_Count; UI32 iDeclScope = 0;
			if(!SrcAnlzCodeCParser_scopePop(state, code, &iSintx, &iDeclScope, FALSE)){
				r = FALSE; NBASSERT(FALSE)
			} else {
				NBASSERT(iSintx == ENSrzAnlzSintxC_Count) //No sintax should be pushed
				NBASSERT(iDeclScope == 0) //No declarScope should be pushed
			}
		}
	}
	return r;
}

//ParseScope of __asm code (C/C++ extension)
BOOL SrcAnlzCodeCParser_feedToknPrivScopeASM(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, STSrcAnlzCodeCParseScope* scope, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue){
	BOOL r = TRUE;
	//Dummy ASM processing, basically process until the last scope is closed.
	//Expecting something like:
	//__asm__("nop;nop;nop")
	//__asm("nop;nop;nop")
	//asm("nop;nop;nop")
	if(partFound.type == ENSrzAnlzSintxPartType_Punctuator){
		BOOL popped = FALSE;
		NBASSERT(partIdxValue < state->acumTokensCur->defs.use) //Index must be in range
		NBASSERT(partFound.idx >= 0 && partFound.idx < state->sintxDefs.punctsSz)
		if(NBString_strIsEqual(state->sintxDefs.puncts[partFound.idx], "{")){
			NBArray_addValueCopy(&scope->charLvlStack, char, '{');
			PRINTF_INFO("ASM part: '%s' pushed to lvl-%d.\n", state->sintxDefs.puncts[partFound.idx], scope->charLvlStack.use);
		} else if(NBString_strIsEqual(state->sintxDefs.puncts[partFound.idx], "}")){
			if(scope->charLvlStack.use == 0){
				r = FALSE; NBASSERT(FALSE) //No level to close
			} else if(NBArray_itmValueAtIndex(&scope->charLvlStack, char, scope->charLvlStack.use - 1) != '{'){
				r = FALSE; NBASSERT(FALSE) //Level opening unmatch
			} else {
				//Remove level
				NBArray_removeItemAtIndex(&scope->charLvlStack, scope->charLvlStack.use - 1);
				PRINTF_INFO("ASM part: '%s' poped to lvl-%d.\n", state->sintxDefs.puncts[partFound.idx], scope->charLvlStack.use);
				popped = TRUE;
			}
		} else if(NBString_strIsEqual(state->sintxDefs.puncts[partFound.idx], "(")){
			NBArray_addValueCopy(&scope->charLvlStack, char, '(');
			PRINTF_INFO("ASM part: '%s' pushed to lvl-%d.\n", state->sintxDefs.puncts[partFound.idx], scope->charLvlStack.use);
		} else if(NBString_strIsEqual(state->sintxDefs.puncts[partFound.idx], ")")){
			if(scope->charLvlStack.use == 0){
				r = FALSE; NBASSERT(FALSE) //No level to close
			} else if(NBArray_itmValueAtIndex(&scope->charLvlStack, char, scope->charLvlStack.use - 1) != '('){
				r = FALSE; NBASSERT(FALSE) //Level opening unmatch
			} else {
				//Remove level
				NBArray_removeItemAtIndex(&scope->charLvlStack, scope->charLvlStack.use - 1);
				PRINTF_INFO("ASM part: '%s' poped to lvl-%d.\n", state->sintxDefs.puncts[partFound.idx], scope->charLvlStack.use);
				popped = TRUE;
			}
		}
		//Pop ASM scope
		if(popped && scope->charLvlStack.use == 0){
			PRINTF_INFO("ASM part: '%s' terminated ASM at lvl-%d.\n", state->sintxDefs.puncts[partFound.idx], scope->charLvlStack.use);
			ENSrzAnlzSintxC iSintx = ENSrzAnlzSintxC_Count; UI32 iDeclScope = 0;
			if(!SrcAnlzCodeCParser_scopePop(state, code, &iSintx, &iDeclScope, FALSE)){
				r = FALSE; NBASSERT(FALSE)
			} else {
				//The removed scope had ASM statements
				//Notify the new top scope about the part returned by the removed scope.
				NBASSERT(iSintx == ENSrzAnlzSintxC_compound_statement)
				NBASSERT(iDeclScope > 0)
				NBASSERT(state->parseScopesStack.use > 1)
				//Feed the part returned by the poped scope.
				STSrcAnlzSintxPart rPart;
				rPart.type	= ENSrzAnlzSintxPartType_SintaxDef;
				rPart.idx	= ENSrzAnlzSintxC_compound_statement;
				if(!SrcAnlzCodeCParser_feedToknPriv(state, code, rPart, iDeclScope)){
					r = FALSE; NBASSERT(FALSE)
				}
			}
		}
	}
	return r;
}

//ParseScope of normal code
BOOL SrcAnlzCodeCParser_feedToknPrivScopeNormal(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, STSrcAnlzCodeCParseScope* scope, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue){
	BOOL r = TRUE;
	const BOOL isDummyPart		= (partFound.type == ENSrzAnlzSintxPartType_SintaxDef && partFound.idx == ENSrzAnlzSintxC_Count) ? TRUE : FALSE;
	BOOL partConsumed			= FALSE;
	const char* partFoundStr	= (isDummyPart ? "" : (partFound.type == ENSrzAnlzSintxPartType_SintaxDef ? state->sintxDefs.defs[partFound.idx].name : &state->acumTokensCur->strBuff.str[NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, partIdxValue)->iAtBuff]));
	//Determine forbiden part (identifier is not a declared 'typedef-name'?)
	SI32 iForbSintaxDef = -1;
	if(!isDummyPart){
		if(partFound.type == ENSrzAnlzSintxPartType_Identifier){
			NBASSERT(partIdxValue < state->acumTokensCur->defs.use) //Index must be in range
			const STSrcAnlzTokn* tokenDef	= NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, partIdxValue);
			const char* tokenStr			= &state->acumTokensCur->strBuff.str[tokenDef->iAtBuff];
			const UI32 iTopDeclScope		= scope->iDeclScope; NBASSERT(iTopDeclScope > 0 && iTopDeclScope < code->defsDeclarsScopes.use)
			const STSrcAnlzCodeCDeclarsScope* dScope = NBArray_itmPtrAtIndex(&code->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iTopDeclScope);
			UI32 iDeclarDef = 0;
			if(!SrcAnlzCodeC_findDeclarByName(code, iTopDeclScope, TRUE, tokenStr, (dScope->declarsRng.start + dScope->declarsRng.count), &iDeclarDef)){
				r = FALSE; NBASSERT(FALSE)
			} else if(iDeclarDef == 0){
				//Not a already declared 'typedef-name' (forbiden)
				iForbSintaxDef = ENSrzAnlzSintxC_typedef_name;
				//PRINTF_INFO("FeedPartPriv, '%s' is not declared (not a 'typedef-name' at scope '#%d -> #%d; of %d').\n", tokenStr, (SI32)(dScope->declarsRng.start + 1), (SI32)(dScope->declarsRng.start + dScope->declarsRng.count), (SI32)state->code->defsDeclars.use);
			} else {
				const STSrcAnlzCodeCDeclar* declar = NBArray_itmPtrAtIndex(&code->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
				NBASSERT(NBString_strIsEqual(tokenStr, &code->strsBuff.str[declar->iNameFirstChar]))
				NBASSERT(declar->iDeepLvl <= dScope->iDeepLvl) //Program logic error ('SrcAnlzCodeC_findDeclarByName' should filter only same level declars)
				if((declar->type.specifiersMask & ENSrcAnlzDecSpecifiersBit_SC_Typedef) == 0){
					//Not a already declared 'typedef-name' (forbiden)
					iForbSintaxDef = ENSrzAnlzSintxC_typedef_name;
					//PRINTF_INFO("FeedPartPriv, '%s' is not a declared 'typedef-name' at scope '#%d -> #%d; of %d'.\n", tokenStr, (SI32)(dScope->declarsRng.start + 1), (SI32)(dScope->declarsRng.start + dScope->declarsRng.count), (SI32)state->code->defsDeclars.use);
				} else {
					//PRINTF_INFO("FeedPartPriv, '%s' is a declared 'typedef-name' at scope '#%d -> #%d; of %d'.\n", tokenStr, (SI32)(dScope->declarsRng.start + 1), (SI32)(dScope->declarsRng.start + dScope->declarsRng.count), (SI32)state->code->defsDeclars.use);
				}
			}
		}
	}
	//Process part
	if(scope->parseTree.nodes.use == 1){
		//------------------
		// Tree is empty
		//------------------
		if(!isDummyPart){
			//Start new posibilities
			ENSrzAnlzSintxC iSintaxDef = ENSrzAnlzSintxC_Count, iSintaxDef2 = ENSrzAnlzSintxC_Count;
			if(scope->iSintx == ENSrzAnlzSintxC_Count){
				//ENSrzAnlzSintxC_translation_unit
				iSintaxDef	= ENSrzAnlzSintxC_declaration;
				iSintaxDef2	= ENSrzAnlzSintxC_function_definition;
			} else if(scope->iSintx == ENSrzAnlzSintxC_block_item_list){
				iSintaxDef	= ENSrzAnlzSintxC_block_item;
			} else if(scope->iSintx == ENSrzAnlzSintxC_struct_declaration_list){
				iSintaxDef	= ENSrzAnlzSintxC_struct_declaration;
			}
			if(iSintaxDef == ENSrzAnlzSintxC_Count){
				r = FALSE; NBASSERT(FALSE)
			} else {
				if(!SrcAnlzSintxParseTree_addRootPosibilities(&state->sintxDefs, &scope->parseTree, iSintaxDef, partFound, partIdxValue)){
					r = FALSE; NBASSERT(FALSE)
				} else if(iSintaxDef2 != ENSrzAnlzSintxC_Count){
					if(!SrcAnlzSintxParseTree_addRootPosibilities(&state->sintxDefs, &scope->parseTree, iSintaxDef2, partFound, partIdxValue)){
						r = FALSE; NBASSERT(FALSE)
					}
				}
			}
			//Remove forbiden posibilities
			if(r && iForbSintaxDef >= 0){
				UI32 removedCount = 0;
				if(!SrcAnlzSintxParseTree_removePosibilitiesEndedByDefEnum(&state->sintxDefs, &scope->parseTree, iForbSintaxDef, &removedCount)){
					r = FALSE; NBASSERT(FALSE)
				} else if(removedCount != 0){
					//PRINTF_INFO("FeedPartPriv, %d posibilities were removed because ended in forbiden '%s'.\n", removedCount, state->sintxDefs.defs[iForbSintaxDef].name);
				}
			}
			//Analyze new posibilities
			if(r){
				const STSrcAnlzSintxParseTreeNode* rootNode = NBArray_itmPtrAtIndex(&scope->parseTree.nodes, STSrcAnlzSintxParseTreeNode, 0);
				if(rootNode->iNextPart != 0){
					//New posibilities started
					partConsumed = TRUE;
					//SrcAnlzSintxParseTree_print(&scope->parseTree);
					//PRINTF_INFO("Part '%s' started '%s' posibilities (%d nodes in array, %.2f%% active).\n", partFoundStr, state->sintxDefs.defs[iSintaxDef].name, scope->parseTree.nodes.use, 100.0f * (1.0f - ((float)(scope->parseTree.nodes.use - scope->parseTree.freeNodesIdxs.use) / (float)scope->parseTree.nodes.use)));
					//PRINTF_INFO("------------------\n");
				} else {
					//First part did not started any posibility (pop scope and try again at parent scope)
					const UI32 fullSeqsCount = scope->stats.qSintaxSeqs;
					ENSrzAnlzSintxC iSintx = ENSrzAnlzSintxC_Count; UI32 iDeclScope = 0;
					if(!SrcAnlzCodeCParser_scopePop(state, state->code, &iSintx, &iDeclScope, (partFound.type != ENSrzAnlzSintxPartType_SintaxDef))){
						r = FALSE; NBASSERT(FALSE)
					} else {
						if(fullSeqsCount == 0){
							//Remove empty declarsScope
							NBASSERT(fullSeqsCount == 0)
							NBASSERT(iDeclScope == (code->defsDeclarsScopes.use - 1)) //Program logic error (must be the last scope)
							const STSrcAnlzCodeCDeclarsScope* emptyScope = NBArray_itmPtrAtIndex(&code->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope);
							NBASSERT(emptyScope->usrTypesRng.count == 0) //Must be a empty declarScope
							NBASSERT(emptyScope->declarsRng.count == 0) //Must be a empty declarScope
							NBASSERT(emptyScope->scopesRng.count == 0) //Must be a empty declarScope
							NBArray_removeItemAtIndex(&code->defsDeclarsScopes, iDeclScope);
							PRINTF_INFO("Empty declarScope removed.\n");
						} else {
							//The removed scope had at least one completed sintax element (delaration, statement, etc..)
							//Notify the new top scope about the part returned by the removed scope.
							NBASSERT(iSintx == ENSrzAnlzSintxC_block_item_list || iSintx == ENSrzAnlzSintxC_struct_declaration_list)
							NBASSERT(state->parseScopesStack.use > 1)
							//Verify and update scopes's ranges
#							ifdef NB_CONFIG_INCLUDE_ASSERTS
							{
								UI32 iDeclScope2 = iDeclScope;
								STSrcAnlzCodeCDeclarsScope* scope2 = NBArray_itmPtrAtIndex(&code->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
								do {
									//ToDo: remove updates (only verify)
									scope2->usrTypesRng.count = (code->defsUsrTypes.use - scope2->usrTypesRng.start);
									scope2->declarsRng.count = (code->defsDeclars.use - scope2->declarsRng.start);
									scope2->scopesRng.count = (code->defsDeclarsScopes.use - scope2->scopesRng.start);
									NBASSERT((scope2->usrTypesRng.start + scope2->usrTypesRng.count) == code->defsUsrTypes.use) //Must be last declarScope
									NBASSERT((scope2->declarsRng.start + scope2->declarsRng.count) == code->defsDeclars.use) //Must be last declarScope
									NBASSERT((scope2->scopesRng.start + scope2->scopesRng.count) == code->defsDeclarsScopes.use) //Must be last declarScope
									//PRINTF_INFO("Verifying scope %d's range from(%d, +%d) to(%d, +%d).\n", iDeclScope2, scope2->scopesRng.start, scope2->scopesRng.count, scope2->scopesRng.start, (code->defsDeclarsScopes.use - scope2->scopesRng.start));
									if(scope2->iParentScope == 0){
										break;
									} else {
										iDeclScope2	= scope2->iParentScope;
										scope2		= NBArray_itmPtrAtIndex(&code->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
									}
								} while(TRUE);
							}
#							endif
							//Feed the part returned by the poped scope.
							STSrcAnlzSintxPart rPart;
							rPart.type	= ENSrzAnlzSintxPartType_SintaxDef;
							rPart.idx	= iSintx;
							if(!SrcAnlzCodeCParser_feedToknPriv(state, state->code, rPart, iDeclScope)){
								r = FALSE; NBASSERT(FALSE)
							}
						}
						//Recursive call (new top parsing scope, after poping)
						if(r){
							if(partFound.type == ENSrzAnlzSintxPartType_SintaxDef){
								//Same call
								if(!SrcAnlzCodeCParser_feedToknPriv(state, state->code, partFound, partIdxValue)){
									r = FALSE; NBASSERT(FALSE)
								}
							} else {
								//Call with the values from the new top-scope
								NBASSERT(state->parseScopesStack.use > 1)
								const UI32 iScope = (state->parseScopesStack.use - 1);
								STSrcAnlzCodeCParseScope* scope = NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, iScope);
								NBASSERT(scope->acumTokens.defs.use > 0)
								const UI32 partIdxNewValue = (scope->acumTokens.defs.use - 1);
								if(!SrcAnlzCodeCParser_feedToknPriv(state, state->code, partFound, partIdxNewValue)){
									r = FALSE; NBASSERT(FALSE)
								}
							}
						}
					}
				}
			}
		}
	} else {
		//------------------
		// Tree is not empty
		//------------------
		//Continue existing posibilities
		UI16 anyRootNodeAlreadyCompletedBeforePart = 0;
		if(!SrcAnlzSintxParseTree_continuePosibilities(&state->sintxDefs, &scope->parseTree, partFound, partIdxValue, &anyRootNodeAlreadyCompletedBeforePart)){
			r = FALSE; NBASSERT(FALSE)
		} else {
			//Remove forbiden posibilities
			if(iForbSintaxDef >= 0){
				UI32 removedCount = 0;
				if(!SrcAnlzSintxParseTree_removePosibilitiesEndedByDefEnum(&state->sintxDefs, &scope->parseTree, iForbSintaxDef, &removedCount)){
					r = FALSE; NBASSERT(FALSE)
				} else if(removedCount != 0){
					//PRINTF_INFO("FeedPartPriv, %d posibilities were removed because ended in forbiden '%s'.\n", removedCount, state->sintxDefs.defs[iForbSintaxDef].name);
				}
			}
			//Analyze continued posibilities
			if(r){
				if(NBArray_itmPtrAtIndex(&scope->parseTree.nodes, STSrcAnlzSintxParseTreeNode, 0)->iNextPart != 0){
					//This part didnt kill all psobilities.
					//SrcAnlzSintxParseTree_print(&scope->parseTree);
					//PRINTF_INFO("Token '%s' continued posibilities (%d nodes in array, %.2f%% active).\n", partFoundStr, scope->parseTree.nodes.use, 100.0f * (1.0f - ((float)(scope->parseTree.nodes.use - scope->parseTree.freeNodesIdxs.use) / (float)scope->parseTree.nodes.use)));
					//PRINTF_INFO("------------------\n");
					partConsumed = TRUE;
				} else {
					//This part killed all psobilities.
					if(anyRootNodeAlreadyCompletedBeforePart == 0){
						PRINTF_ERROR("FeedPartPriv, Token '%s' killed all posibilities before any was complete (%d nodes in array, %.2f%% active).\n", partFoundStr, scope->parseTree.nodes.use, 100.0f * (1.0f - ((float)(scope->parseTree.nodes.use - scope->parseTree.freeNodesIdxs.use) / (float)scope->parseTree.nodes.use)));
						r = FALSE; NBASSERT(FALSE)
					} else {
						scope->stats.qSintaxSeqs++;
						//PRINTF_INFO("FeedPartPriv, Token '%s' killed the last completed posibility (%d nodes in array, %.2f%% active), processing any completed sequence (id %d).\n", partFoundStr, scope->parseTree.nodes.use, 100.0f * (1.0f - ((float)(scope->parseTree.nodes.use - scope->parseTree.freeNodesIdxs.use) / (float)scope->parseTree.nodes.use)), anyRootNodeAlreadyCompletedBeforePart);
						//SrcAnlzSintxParseTree_printChilds(&state->sintxDefs, &scope->parseTree, anyRootNodeAlreadyCompletedBeforePart);
						//PRINTF_INFO("FeedPartPriv, ------------------\n");
						//Consume completed posibility
						if(!SrcAnlzCodeCParser_consume(state, scope, state->code, NBArray_itmPtrAtIndex(&scope->parseTree.nodes, STSrcAnlzSintxParseTreeNode, anyRootNodeAlreadyCompletedBeforePart)->iChildSeq)){
							r = FALSE; NBASSERT(FALSE)
						} else {
							//Reset tree
							SrcAnlzSintxParseTree_reset(&scope->parseTree);
							//PRINTF_INFO("FeedPartPriv, Stats after completed sintaxSeq: types(%d, +%d)-declrs(%d, +%d).\n", scope->stats.usrTypesRng.start, scope->stats.usrTypesRng.count, scope->stats.declarsRng.start, scope->stats.declarsRng.count);
							PRINTF_INFO("FeedPartPriv, --------------\n");
							if(!isDummyPart){
								//Recursive call (same parsing scope)
								if(!SrcAnlzCodeCParser_feedToknPriv(state, state->code, partFound, partIdxValue)){
									r = FALSE; NBASSERT(FALSE)
								}
							}
						}
					}
				}
			}
		}
	}
	//Analyze new scope depth opened by '{' (push)
	if(!isDummyPart){
		if(r && partConsumed && partFound.type == ENSrzAnlzSintxPartType_Punctuator){
			NBASSERT(partFound.idx >= 0 && partFound.idx < state->sintxDefs.punctsSz)
			NBASSERT(partIdxValue < state->acumTokensCur->defs.use) //Index must be in range
			if(NBString_strIsEqual(partFoundStr, "{")){
				UI16 partsReqsFoundCount = 0;
				STSrcAnlzSintxPart partsReqs[3];
				if(!SrcAnlzSintxParseTree_nextRequiredPosibsParts(&state->sintxDefs, &scope->parseTree, partsReqs, 3, &partsReqsFoundCount)){
					r = FALSE; NBASSERT(FALSE)
				} else if(partsReqsFoundCount == 0 || partsReqsFoundCount == 3){
					PRINTF_ERROR("FeedPartPriv, Language sintax logic error, expected a predectible part after '%s' but found %d posibles parts.\n", partFoundStr, partsReqsFoundCount);
					r = FALSE; NBASSERT(FALSE)
				} else {
					NBASSERT(partsReqsFoundCount == 1 || partsReqsFoundCount == 2)
					//Try to discard the '}' part (for posibles empty blocks)
					if(partsReqsFoundCount == 2){
						if(partsReqs[1].type == ENSrzAnlzSintxPartType_Punctuator){
							if(NBString_strIsEqual(state->sintxDefs.puncts[partsReqs[1].idx], "}")){
								partsReqsFoundCount--;
							}
						} else if(partsReqs[0].type == ENSrzAnlzSintxPartType_Punctuator){
							if(NBString_strIsEqual(state->sintxDefs.puncts[partsReqs[0].idx], "}")){
								partsReqs[0] = partsReqs[1];
								partsReqsFoundCount--;
							}
						}
					}
					if(partsReqsFoundCount != 1){
						PRINTF_ERROR("FeedPartPriv, Language sintax logic error, expected a predectible part after '%s' but found %d posibles parts.\n", partFoundStr, partsReqsFoundCount);
						r = FALSE; NBASSERT(FALSE)
					} else {
						const STSrcAnlzSintxPart* partDef = &partsReqs[0];
						if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							const BOOL wBlockItemList		= (partDef->idx == ENSrzAnlzSintxC_block_item_list);
							const BOOL wStructDecListList	= (partDef->idx == ENSrzAnlzSintxC_struct_declaration_list);
							if(wBlockItemList || wStructDecListList){
								//SrcAnlzSintxParseTree_print(&scope->parseTree);
								//PRINTF_INFO("FeedPartPriv, Token '%s' started new parsingScope with posibilities (%d nodes in array, %.2f%% active).\n", partFoundStr, scope->parseTree.nodes.use, 100.0f * (1.0f - ((float)(scope->parseTree.nodes.use - scope->parseTree.freeNodesIdxs.use) / (float)scope->parseTree.nodes.use)));
								//PRINTF_INFO("FeedPartPriv, ------------------\n");
								//Consume any current partial posibility (assuming all posibilities starts the same)
								scope->stats.iLastActiveDecScope = scope->iDeclScope;
								const STSrcAnlzSintxParseTreeNode* rootNode = NBArray_itmPtrAtIndex(&scope->parseTree.nodes, STSrcAnlzSintxParseTreeNode, 0); NBASSERT(rootNode->iNextPart != 0)
								const STSrcAnlzSintxParseTreeNode* firstPosib = NBArray_itmPtrAtIndex(&scope->parseTree.nodes, STSrcAnlzSintxParseTreeNode, rootNode->iNextPart); NBASSERT(firstPosib->iChildSeq != 0)
								const STSrcAnlzCodeCParseScopeStats parScopeStatsBkp = scope->stats;
								if(!SrcAnlzCodeCParser_consume(state, scope, state->code, firstPosib->iChildSeq)){
									r = FALSE; NBASSERT(FALSE)
								} else {
									NBASSERT(parScopeStatsBkp.iLastActiveDecScope > 0 && parScopeStatsBkp.iLastActiveDecScope < code->defsDeclarsScopes.use)
									NBASSERT(scope->stats.iLastActiveDecScope > 0 && scope->stats.iLastActiveDecScope < code->defsDeclarsScopes.use)
									//ToDo: remove this next assert; other inner scopes could be created before this inner-scope.
									//NBASSERT(code->defsDeclarsScopes.use == (scope->stats.scopesRng.start + scope->stats.scopesRng.count)) //Validate before reseting stats
									//Reset scope stats (this is a partial sintaxSeq)
									//PRINTF_INFO("Restoring stats after partial sintaxSeq: from types(%d, +%d)-declrs(%d, +%d) -> to types(%d, +%d)-declrs(%d, +%d).\n", scope->stats.usrTypesRng.start, scope->stats.usrTypesRng.count, scope->stats.declarsRng.start, scope->stats.declarsRng.count, parScopeStatsBkp.usrTypesRng.start, parScopeStatsBkp.usrTypesRng.count, parScopeStatsBkp.declarsRng.start, parScopeStatsBkp.declarsRng.count);
									const UI32 iParentScope		= scope->stats.iLastActiveDecScope;
									scope->stats				= parScopeStatsBkp;
									//Push parsing scope
									NBASSERT(partDef->idx >= 0 && partDef->idx < ENSrzAnlzSintxC_Count)
									if(!SrcAnlzCodeCParser_scopePush(state, state->code, ENSrcAnlzCodeCParseScopeType_Normal, (ENSrzAnlzSintxC)partDef->idx, iParentScope, FALSE)){
										r = FALSE; NBASSERT(FALSE)
									}
								}
							}
							//PRINTF_INFO("FeedPartPriv(%d), After '%s', expecting '%s'.\n", ciclesCount, partFoundStr, state->sintxDefs.defs[partDef->idx].name);
						}
					}
				}
			}
		}
	}
	return r;
}

//-------------------
// Feed
//-------------------

BOOL SrcAnlzParser_sintaxFlushPProcInvocTokens(STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseMacroParams* stateParams, const STSrcAnlzTokn* toknPend[3], SI8 toknPendParamRef[3], SI32* dstCountConsumed, const BOOL invocHasPresentVaArgs, BOOL* dstFirstIsSpacePreceded){
	BOOL r = TRUE;
	NBASSERT(toknPend[0] == NULL || (toknPend[0] != NULL && toknPend[1] == NULL) || (toknPend[0] != NULL && toknPend[1] != NULL && toknPend[2] == NULL) || (toknPend[0] != NULL && toknPend[1] != NULL && toknPend[2] != NULL))
	NBASSERT(toknPend[0] != NULL || toknPendParamRef[0] == -1) //paramRef should be -1 if no token is refered
	NBASSERT(toknPend[1] != NULL || toknPendParamRef[1] == -1) //paramRef should be -1 if no token is refered
	NBASSERT(toknPend[2] != NULL || toknPendParamRef[2] == -1) //paramRef should be -1 if no token is refered
	SI32 countConsumed = 0; if(dstCountConsumed != NULL) countConsumed = *dstCountConsumed;
	BOOL firstIsSpacePreceded = FALSE; if(dstFirstIsSpacePreceded != NULL) firstIsSpacePreceded = *dstFirstIsSpacePreceded;
	BOOL consumed = FALSE, isTextMode = FALSE;
	//Consume tokens concatenation ('token' + ## + 'token')
	if(toknPend[2] != NULL){
		NBASSERT(toknPendParamRef[0] >= -1 && toknPendParamRef[0] < stateParams->paramsRanges.use)
		NBASSERT(toknPendParamRef[1] >= -1 && toknPendParamRef[1] < stateParams->paramsRanges.use)
		NBASSERT(toknPendParamRef[2] >= -1 && toknPendParamRef[2] < stateParams->paramsRanges.use)
		//Three tokens accumulated
		//const STSrcAnlzTokn* toknDef		= NBArray_itmPtrAtIndex(&state->macros.valsTokns.tokns.defs, STSrcAnlzTokn, macroDef->valTokensFirst + i);
		//const SI8 toknParamRef			= NBArray_itmValueAtIndex(&state->macros.valsTokns.iParamRefs, SI8, macroDef->valTokensFirst + i);
		//NBASSERT(!NBString_strIsEqual("#", &state->macros.valsTokns.tokns.strBuff.str[toknDef->iAtBuff])) //'#' is not supported yet
		const SI8 toknPendParamRef0 = toknPendParamRef[0];
		const SI8 toknPendParamRef1 = toknPendParamRef[1];
		const SI8 toknPendParamRef2 = toknPendParamRef[2];
		const char* toknStr0 = &state->macros.valsTokns.tokns.strBuff.str[toknPend[0]->iAtBuff];
		const char* toknStr1 = &state->macros.valsTokns.tokns.strBuff.str[toknPend[1]->iAtBuff];
		const char* toknStr2 = &state->macros.valsTokns.tokns.strBuff.str[toknPend[2]->iAtBuff];
		if(NBString_strIsEqual(toknStr1, "##")){
			if((toknPendParamRef[0] != -1 && toknPendParamRef[0] >= stateParams->paramsRanges.use) || (toknPendParamRef[2] != -1 && toknPendParamRef[2] >= stateParams->paramsRanges.use)){
				//Special case, left or right side is the not-defined __VA_ARGS__ (or custom name).
				PRINTF_INFO("Ignoring tokens combination '%s'+'%s'+'%s' (one of its sides is not defined).\n", &state->macros.valsTokns.tokns.strBuff.str[toknPend[0]->iAtBuff], &state->macros.valsTokns.tokns.strBuff.str[toknPend[1]->iAtBuff], &state->macros.valsTokns.tokns.strBuff.str[toknPend[2]->iAtBuff]);
				toknPend[0] = toknPend[1] = toknPend[2] = NULL;
				consumed = TRUE;
			} else {
				const STSrcAnlzTokn* lToknDef	= toknPend[0];
				const SI8 lParamRef				= toknPendParamRef[0];
				const STSrcAnlzTokn* rToknDef	= toknPend[2];
				const SI8 rParamRef				= toknPendParamRef[2];
				NBASSERT(toknPend[0] != NULL)
				toknPend[0] = toknPend[1] = toknPend[2] = NULL;
				toknPendParamRef[0] = toknPendParamRef[1] = toknPendParamRef[2] = -1;
				BOOL mergeTokens = TRUE;
				if(lParamRef != -1){
					if(NBArray_itmPtrAtIndex(&stateParams->paramsRanges, STSrcAnlzRange, lParamRef)->count == 0){
						mergeTokens = FALSE; //Do not merge if any side is a paramRef with empty replacement list
					}
				}
				if(rParamRef != -1){
					if(NBArray_itmPtrAtIndex(&stateParams->paramsRanges, STSrcAnlzRange, rParamRef)->count == 0){
						mergeTokens = FALSE; //Do not merge if any side is a paramRef with empty replacement list
					}
				}
				if(!mergeTokens){
					//Direct translate parts
					if(r){
						if(lParamRef != -1){
							NBASSERT(lParamRef < stateParams->paramsRanges.use)
							const STSrcAnlzRange* toknsRng = NBArray_itmPtrAtIndex(&stateParams->paramsRanges, STSrcAnlzRange, lParamRef);
							SI32 i; for(i = 0; i < toknsRng->count; i++){
								NBASSERT((toknsRng->start + i) < stateParams->tokens.use)
								const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&stateParams->tokens, STSrcAnlzTokn, toknsRng->start + i);
								const char* toknStr = &stateParams->strBuff.str[toknDef->iAtBuff];
								//Process
								if(!SrcAnlzCodeCParser_feedTokn(state, toknStr, toknDef->sintxPart, (countConsumed == 0 ? firstIsSpacePreceded : toknDef->isSpacePreceded))){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
								countConsumed++;
							}
						} else {
							const char* toknStr = &state->macros.valsTokns.tokns.strBuff.str[lToknDef->iAtBuff];
							if(!SrcAnlzCodeCParser_feedTokn(state, toknStr, lToknDef->sintxPart, (countConsumed == 0 ? firstIsSpacePreceded : lToknDef->isSpacePreceded))){
								r = FALSE; NBASSERT(FALSE)
							}
							countConsumed++;
						}
					}
					if(r){
						if(rParamRef != -1){
							NBASSERT(rParamRef < stateParams->paramsRanges.use)
							const STSrcAnlzRange* toknsRng = NBArray_itmPtrAtIndex(&stateParams->paramsRanges, STSrcAnlzRange, rParamRef);
							SI32 i; for(i = 0; i < toknsRng->count; i++){
								NBASSERT((toknsRng->start + i) < stateParams->tokens.use)
								const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&stateParams->tokens, STSrcAnlzTokn, toknsRng->start + i);
								const char* toknStr = &stateParams->strBuff.str[toknDef->iAtBuff];
								//Process
								if(!SrcAnlzCodeCParser_feedTokn(state, toknStr, toknDef->sintxPart, (countConsumed == 0 ? firstIsSpacePreceded : toknDef->isSpacePreceded))){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
								countConsumed++;
							}
						} else {
							const char* toknStr = &state->macros.valsTokns.tokns.strBuff.str[rToknDef->iAtBuff];
							if(!SrcAnlzCodeCParser_feedTokn(state, toknStr, rToknDef->sintxPart, (countConsumed == 0 ? firstIsSpacePreceded : rToknDef->isSpacePreceded))){
								r = FALSE; NBASSERT(FALSE)
							}
							countConsumed++;
						}
					}
				} else {
					//Merge tokens and feed
					STNBString mergedStr; NBString_init(&mergedStr);
					if(lParamRef != -1){
						NBASSERT(lParamRef < stateParams->paramsRanges.use)
						const STSrcAnlzRange* toknsRng = NBArray_itmPtrAtIndex(&stateParams->paramsRanges, STSrcAnlzRange, lParamRef);
						NBASSERT(toknsRng->count > 0)
						SI32 i; for(i = 0; i < toknsRng->count; i++){
							NBASSERT((toknsRng->start + i) < stateParams->tokens.use)
							const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&stateParams->tokens, STSrcAnlzTokn, toknsRng->start + i);
							const char* toknStr = &stateParams->strBuff.str[toknDef->iAtBuff];
							if(i != 0 && toknDef->isSpacePreceded) NBString_concatByte(&mergedStr, ' ');
							NBString_concat(&mergedStr, toknStr);
						}
					} else {
						NBString_concat(&mergedStr, &state->macros.valsTokns.tokns.strBuff.str[lToknDef->iAtBuff]);
					}
					if(rParamRef != -1){
						NBASSERT(rParamRef < stateParams->paramsRanges.use)
						const STSrcAnlzRange* toknsRng = NBArray_itmPtrAtIndex(&stateParams->paramsRanges, STSrcAnlzRange, rParamRef);
						NBASSERT(toknsRng->count > 0)
						SI32 i; for(i = 0; i < toknsRng->count; i++){
							NBASSERT((toknsRng->start + i) < stateParams->tokens.use)
							const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&stateParams->tokens, STSrcAnlzTokn, toknsRng->start + i);
							const char* toknStr = &stateParams->strBuff.str[toknDef->iAtBuff];
							if(i != 0 && toknDef->isSpacePreceded) NBString_concatByte(&mergedStr, ' ');
							NBString_concat(&mergedStr, toknStr);
						}
					} else {
						NBString_concat(&mergedStr, &state->macros.valsTokns.tokns.strBuff.str[rToknDef->iAtBuff]);
					}
					//Feed
					NBASSERT(mergedStr.lenght > 0)
					if(mergedStr.lenght > 0){
						PRINTF_INFO("MACRO##, feeding merged content: '%s'\n", mergedStr.str);
						//push stream scope
						if(!(*state->itfs->parserItf->streamPushFunc)(state->itfs->parserItf, ENSrcAnlzParserSrcType_Virtual, NULL)){
							PRINTF_ERROR("Could not push stream for file.\n");
							r = FALSE; NBASSERT(FALSE)
						} else {
							//feed stream
							if(!(*state->itfs->parserItf->streamFeedFunc)(state->itfs->parserItf, (const char*)mergedStr.str, (UI32)mergedStr.lenght)){
								r = FALSE; NBASSERT(FALSE)
							}
							//pop stream scope
							if(r){
								if(!(*state->itfs->parserItf->streamPopFunc)(state->itfs->parserItf)){
									r = FALSE; NBASSERT(FALSE)
								}
							}
						}
						countConsumed++;
					}
					NBString_release(&mergedStr);
				}
			}
		}
	}
	//Consume string convertion ('#' + 'PARAM_NAME')
	if(r && !consumed && toknPend[0] != NULL && toknPend[1] != NULL){
		const char* toknStr0 = &state->macros.valsTokns.tokns.strBuff.str[toknPend[0]->iAtBuff];
		if(NBString_strIsEqual(toknStr0, "#")){
			if(toknPendParamRef[1] == -1){
				r = FALSE; NBASSERT(FALSE) //Expected a paramName after '#'
			} else {
				toknPend[0] = toknPend[1];
				toknPend[1] = toknPend[2];
				toknPend[2] = NULL;
				//
				toknPendParamRef[0] = toknPendParamRef[1];
				toknPendParamRef[1] = toknPendParamRef[2];
				toknPendParamRef[2] = -1;
				//
				isTextMode = TRUE;
				//Activate text mode (will be consumed next)
			}
		}
	}
	//Consume oldest token
	if(r && !consumed && toknPend[0] != NULL){
		const STSrcAnlzTokn* toknDef	= toknPend[0];
		const SI8 iParamRef				= toknPendParamRef[0];
		NBASSERT(!isTextMode || iParamRef != -1) //Program logic error, '#' must be followed by a paramName
		if(iParamRef == -1){
			//Token is not a paramRef (consume as it is)
			NBASSERT(!isTextMode)
			const char* toknStr = &state->macros.valsTokns.tokns.strBuff.str[toknDef->iAtBuff];
			if(!SrcAnlzCodeCParser_feedTokn(state, toknStr, toknDef->sintxPart, (countConsumed == 0 ? firstIsSpacePreceded : toknDef->isSpacePreceded))){
				r = FALSE; NBASSERT(FALSE)
			}
			countConsumed++;
		} else {
			if(isTextMode){
				//Convert to literal string
				STNBString strMerged;
				NBString_initWithStr(&strMerged, "\"");
				if(iParamRef < stateParams->paramsRanges.use){
					const STSrcAnlzRange* toknsRng = NBArray_itmPtrAtIndex(&stateParams->paramsRanges, STSrcAnlzRange, iParamRef);
					SI32 i; for(i = 0; i < toknsRng->count; i++){
						NBASSERT((toknsRng->start + i) < stateParams->tokens.use)
						const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&stateParams->tokens, STSrcAnlzTokn, toknsRng->start + i);
						const char* toknStr = &stateParams->strBuff.str[toknDef->iAtBuff];
						//Just add to text
						if(i != 0 && toknDef->isSpacePreceded) NBString_concatByte(&strMerged, ' ');
						//Special handling for producing the spelling of string literals
						//and character constants: a \ character is inserted before each " and \ character
						//of a character constant or string literal (including the delimiting " characters)
						//, except that it is implementation-defined whether a \ character is inserted before the \ character beginning a universal character name.
						while(*toknStr != '\0'){
							if(*toknStr == '\"' || *toknStr == '\\'){
								NBString_concatByte(&strMerged, '\\');
							}
							NBString_concatByte(&strMerged, *toknStr);
							toknStr++;
						}
					}
				}
				NBString_concat(&strMerged, "\"");
				PRINTF_INFO("Token's value converted to string %s.\n", strMerged.str);
				STSrcAnlzSintxPart sintxPart;
				sintxPart.type	= ENSrzAnlzSintxPartType_StrLiteral;
				sintxPart.idx	= -1;
				if(!SrcAnlzCodeCParser_feedTokn(state, strMerged.str, sintxPart, (countConsumed == 0 ? firstIsSpacePreceded : toknDef->isSpacePreceded))){
					r = FALSE; NBASSERT(FALSE)
				}
				countConsumed++;
				NBString_release(&strMerged);
			} else {
				//Translate param ref with corresponding values
				if(iParamRef >= stateParams->paramsRanges.use){
					//Param is not defined
					if(toknDef->isSpacePreceded){
						//Force the next part to be spacePreceded
						firstIsSpacePreceded = TRUE;
						countConsumed = 0;
					} else {
						//Force the next part to use its own 'isSpacePreceded' value
						countConsumed++;
					}
				} else {
					//Translate
					const STSrcAnlzRange* toknsRng = NBArray_itmPtrAtIndex(&stateParams->paramsRanges, STSrcAnlzRange, iParamRef);
					SI32 i; for(i = 0; i < toknsRng->count; i++){
						NBASSERT((toknsRng->start + i) < stateParams->tokens.use)
						const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&stateParams->tokens, STSrcAnlzTokn, toknsRng->start + i);
						const char* toknStr = &stateParams->strBuff.str[toknDef->iAtBuff];
						//Process
						if(!SrcAnlzCodeCParser_feedTokn(state, toknStr, toknDef->sintxPart, (countConsumed == 0 ? firstIsSpacePreceded : toknDef->isSpacePreceded))){
							r = FALSE; NBASSERT(FALSE)
							break;
						}
						countConsumed++;
					}
				}
			}
		}
		//Consume
		if(r){
			toknPend[0] = toknPend[1];
			toknPend[1] = toknPend[2];
			toknPend[2] = NULL;
			//
			toknPendParamRef[0] = toknPendParamRef[1];
			toknPendParamRef[1] = toknPendParamRef[2];
			toknPendParamRef[2] = -1;
		}
	}
	NBASSERT(toknPend[0] != NULL || toknPendParamRef[0] == -1) //paramRef should be -1 if no token is refered
	NBASSERT(toknPend[1] != NULL || toknPendParamRef[1] == -1) //paramRef should be -1 if no token is refered
	NBASSERT(toknPend[2] != NULL || toknPendParamRef[2] == -1) //paramRef should be -1 if no token is refered
	//
	if(dstCountConsumed != NULL) *dstCountConsumed = countConsumed;
	if(dstFirstIsSpacePreceded != NULL) *dstFirstIsSpacePreceded = firstIsSpacePreceded;
	return r;
}

BOOL SrcAnlzCodeCParser_feedTokn(STSrcAnlzCodeCParser* state, const char* toknStr, const STSrcAnlzSintxPart partFound, const BOOL isSpacePreceded){
	BOOL r = TRUE;
	const SI32 toknStrSz = NBString_strLenBytes(toknStr);
	NBASSERT(toknStrSz > 0)
	if(toknStrSz > 0){
		BOOL isSpace = FALSE;
		BOOL isLineChange = FALSE;
		if(toknStrSz == 1){
			const char c = toknStr[0];
			isSpace = (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\0' || c == '\r');
			isLineChange = (c == '\n');
		}
		BOOL curBlockCodeIsActive = TRUE;
		const SI32 prevCodeStateStackSz = state->pprocCondPortionsStack.use; NBASSERT(prevCodeStateStackSz > 0) //at least root state and one more
		if(prevCodeStateStackSz > 0){
			const STSrcAnlzCodeCondPortionState* lastState = NBArray_itmPtrAtIndex(&state->pprocCondPortionsStack, STSrcAnlzCodeCondPortionState, prevCodeStateStackSz - 1);
			curBlockCodeIsActive = lastState->isPortionActive;
		}
#		ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
		/*if(isSpace){
		 PRINTF_INFO("%s: [space] (%s, %s).\n", (state->curBlqType == ENSrcAnlzCodeCParseMode_PProc ? "pproc" : "code"), STR_CONTENT_TYPE(parState->curContentType), (curBlockCodeIsActive ? "active" : "inactive"));
		 } else if(isLineChange){
		 PRINTF_INFO("%s: [line-change] (%s, %s).\n", (state->curBlqType == ENSrcAnlzCodeCParseMode_PProc ? "pproc" : "code"), STR_CONTENT_TYPE(parState->curContentType), (curBlockCodeIsActive ? "active" : "inactive"));
		 } else {
		 PRINTF_INFO("%s: '%s' (%s, %s).\n", (state->curBlqType == ENSrcAnlzCodeCParseMode_PProc ? "pproc" : "code"), toknStr, STR_CONTENT_TYPE(parState->curContentType), (curBlockCodeIsActive ? "active" : "inactive"));
		 }*/
#		endif
		{
			//--------------------
			//- Process token (exclude espaces)
			//--------------------
			if(!isSpace){
				//PRINTF_INFO("Token: '%s'\n", toknStr);
				//Detect the start of a preprocessor directive?
				if(state->curBlqType == ENSrcAnlzCodeCParseMode_Code && partFound.type == ENSrzAnlzSintxPartType_Punctuator){
					NBASSERT(state->acumTokensCur != &state->acumTokensPProc)
					if(state->curLineTokensCount == 0){
						if(toknStrSz == 1){
							if(toknStr[0] == '#'){
								state->curBlqType		= ENSrcAnlzCodeCParseMode_PProc;
								state->acumTokensCur	= &state->acumTokensPProc;
							}
						}
					}
				}
				//If is still source code
				if(state->curBlqType == ENSrcAnlzCodeCParseMode_PProc){
					NBASSERT(state->acumTokensCur == &state->acumTokensPProc)
					//--------------------
					//Preprocessor directives ends after a new-line.
					//Note: '\' + new-line should be already omited.
					//--------------------
					BOOL addToken = TRUE;
					if(toknStrSz == 1){
						if(toknStr[0] == '\n'){
							addToken = FALSE;
							//Process directive
							if(!SrcAnlzCodeCParser_pprocFeedDirective(state)){
								r = FALSE; NBASSERT(FALSE)
							} else {
								//Clear pproc tokens buffer in case a deeper level is invoked
								NBArray_empty(&state->acumTokensPProc.defs);
								NBString_empty(&state->acumTokensPProc.strBuff);
								NBASSERT(state->parseScopesStack.use > 1)
								if(state->parseScopesStack.use <= 1){
									PRINTF_ERROR("No active scope.\n");
									r = FALSE; NBASSERT(FALSE)
								} else {
									STSrcAnlzCodeCParseScope* scope		= NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, state->parseScopesStack.use - 1);
									state->curBlqType		= ENSrcAnlzCodeCParseMode_Code;
									state->acumTokensCur	= &scope->acumTokens;
								}
							}
						}
					}
					if(addToken){
						STSrcAnlzTokn newToken;
						newToken.iAtBuff	= state->acumTokensCur->strBuff.lenght;
						newToken.sintxPart	= partFound;
						newToken.isSpacePreceded = isSpacePreceded;
						NBArray_addValue(&state->acumTokensCur->defs, newToken);
						NBString_concatBytes(&state->acumTokensCur->strBuff, toknStr, toknStrSz + 1); // +1 to include the '\0' char.
					}
				} else if(state->curBlqType == ENSrcAnlzCodeCParseMode_Code){
					NBASSERT(state->acumTokensCur != &state->acumTokensPProc)
					//--------------------
					// Source code
					//--------------------
					if(!isLineChange){
						NBASSERT(!isSpace)
						//Process code (only if is preprocessor-active block)
						if(curBlockCodeIsActive){
							BOOL addToken = TRUE;
							const char firstChar = toknStr[0];
							if(state->curParamsReadState != NULL){
								//NBASSERT(FALSE)
								//
								//Reading macro-params
								//
								STSrcAnlzCodeCParseMacroParams* pState = state->curParamsReadState;
								const STSrcAnlzMacro* macroDef = NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, pState->iMacro);
								if(pState->parentesisDepth == 0){
									//Expecting '(' for MACRO params
									if(firstChar == '(' && toknStrSz == 1 && !isSpacePreceded){
										pState->parentesisDepth++;
										//Open first param range
										STSrcAnlzRange pRange;
										pRange.start = pState->tokens.use;
										pRange.count = 0;
										NBArray_addValue(&pState->paramsRanges, pRange);
									} else {
										PRINTF_ERROR("Macro reference, expected first parenthesis of function-like MACRO's params.\n");
										r = FALSE; NBASSERT(FALSE)
									}
								} else {
									//Expecting MACRO params list
									NBASSERT(pState->paramsRanges.use > 0) //Program logic error
									if((firstChar == ',' && toknStrSz == 1) && pState->parentesisDepth == 1 && (!macroDef->params.isVaArgs || pState->paramsRanges.use < macroDef->params.count)){
										//Close prev param (comma found at level 1)
										STSrcAnlzRange* prevRange = NBArray_itmPtrAtIndex(&pState->paramsRanges, STSrcAnlzRange, pState->paramsRanges.use - 1);
										prevRange->count = pState->tokens.use - prevRange->start; NBASSERT(prevRange->start >= 0 && prevRange->start <= pState->tokens.use)
										//Open next param (comma found at level 1)
										STSrcAnlzRange pRange;
										pRange.start = pState->tokens.use;
										pRange.count = 0;
										NBArray_addValue(&pState->paramsRanges, pRange);
									} else if(!(firstChar == ')' && toknStrSz == 1 && pState->parentesisDepth == 1)){
										//Add param token
										STSrcAnlzTokn newToken;
										newToken.iAtBuff	= pState->strBuff.lenght;
										newToken.sintxPart	= partFound;
										newToken.isSpacePreceded = isSpacePreceded;
										NBArray_addValue(&pState->tokens, newToken);
										NBString_concatBytes(&pState->strBuff, toknStr, toknStrSz + 1); // +1 to include the '\0' char.
										if(toknStrSz == 1){
											if(firstChar == '('){
												NBASSERT(pState->parentesisDepth > 0)
												pState->parentesisDepth++;
											} else if(firstChar == ')'){
												NBASSERT(pState->parentesisDepth > 1)
												pState->parentesisDepth--;
											}
										}
									} else {
										//Close prev param
										if(pState->paramsRanges.use > 0){
											STSrcAnlzRange* prevRange = NBArray_itmPtrAtIndex(&pState->paramsRanges, STSrcAnlzRange, pState->paramsRanges.use - 1);
											prevRange->count = pState->tokens.use - prevRange->start;
											NBASSERT(prevRange->start >= 0 && (prevRange->start + prevRange->count) <= pState->tokens.use)
											//Remove first param if it is empty
											if(pState->paramsRanges.use == 1 && prevRange->count == 0){
												//Must be two tokens only: '(' + ')'
												NBASSERT(pState->tokens.use == 0)
												NBASSERT(pState->strBuff.lenght == 0)
												NBArray_empty(&pState->paramsRanges);
											}
										}
										//PRINTF_INFO("... end of macro-function-call reading (%d params, %d tokens), now translating...\n", pState->paramsRanges.use, pState->tokens.use);
										//Process macro with params
										if(!macroDef->params.areExplicit){
											r = FALSE; NBASSERT(FALSE) //Program logic error
										} else {
											PRINTF_INFO("Macro reference, translating macro-function call ...\n");
											//Disabling current param reading mode
											state->curParamsReadState = NULL;
											//Execute macro
											if(!(pState->paramsRanges.use == macroDef->params.count || (macroDef->params.isVaArgs && (pState->paramsRanges.use + 1) == macroDef->params.count))){
												PRINTF_ERROR("Macro reference, expected %d%s parameters, found %d at MACRO function call: '%s'.\n", macroDef->params.count, (macroDef->params.isVaArgs ? "+..." : ""), pState->paramsRanges.use, &state->macros.names.str[macroDef->nameStart]);
												r = FALSE; NBASSERT(FALSE)
											} else {
												//-------------------------
												//Note: '#' 'param' produces a string with the param's processed value
												//Note: '<token>' '##' '<token>' produces '<token><token>' (concatenation)
												//Note: '<token>' '##' <non-existing-token> produces nothing (concatenation)
												//-------------------------
												//Reset parsing validations
												const BOOL hasPresentVaArgs = (pState->paramsRanges.use > macroDef->params.count);
												SI32 countConsumed = 0; BOOL isSpacePrecededTmp = isSpacePreceded;
												const STSrcAnlzTokn* toknPend[3] = { NULL, NULL, NULL };
												SI8 toknPendParamRef[3] = {-1, -1, -1};
												//ToDo: enable/disable '__VA_OPT__' macro-func depending of 'hasPresentVaArgs' value.
												SI32 i; for(i = 0; i < macroDef->valTokensCount; i++){
													NBASSERT(state->macros.valsTokns.tokns.defs.use == state->macros.valsTokns.iParamRefs.use)
													const STSrcAnlzTokn* toknDef	= NBArray_itmPtrAtIndex(&state->macros.valsTokns.tokns.defs, STSrcAnlzTokn, macroDef->valTokensFirst + i);
													const SI8 toknParamRef			= NBArray_itmValueAtIndex(&state->macros.valsTokns.iParamRefs, SI8, macroDef->valTokensFirst + i);
													if(toknPend[0] == NULL){
														toknPend[0] = toknDef; toknPendParamRef[0] = toknParamRef; //Accum
													} else if(toknPend[1] == NULL){
														toknPend[1] = toknDef; toknPendParamRef[1] = toknParamRef; //Accum
													} else if(toknPend[2] == NULL){
														toknPend[2] = toknDef; toknPendParamRef[2] = toknParamRef; //Accum
													} else {
														//Flush
														if(!SrcAnlzParser_sintaxFlushPProcInvocTokens(state, pState, toknPend, toknPendParamRef, &countConsumed, hasPresentVaArgs, &isSpacePrecededTmp)){
															r = FALSE;
															break;
														}
														NBASSERT(toknPend[2] == NULL && toknPendParamRef[2] == -1) //At least one space should be replaced
														i--; //Process again
													}
												}
												//Flush pending tokens
												while(r && toknPend[0] != NULL){
													if(!SrcAnlzParser_sintaxFlushPProcInvocTokens(state, pState, toknPend, toknPendParamRef, &countConsumed, hasPresentVaArgs, &isSpacePrecededTmp)){
														r = FALSE;
														break;
													}
												}
												//Release param reading mode
												NBString_release(&pState->strBuff);
												NBArray_release(&pState->tokens);
												NBArray_release(&pState->paramsRanges);
												NBMemory_free(pState);
												pState = NULL;
												//PRINTF_INFO("... end of translating macro function call.\n");
											}
										}
									}
								}
								addToken = FALSE;
							} else {
								//
								//Reading translated code
								//
								//Detect MACRO identifier
								if(partFound.type == ENSrzAnlzSintxPartType_Identifier){
									//Search macro
									const SI32 iIdx = SrcAnlzMacros_findNameIdxByStr(&state->macros, toknStr);
									if(iIdx != -1){
										const STSrcAnlzStrIdx* idx = NBArraySorted_itmPtrAtIndex(&state->macros.namesIdx, STSrcAnlzStrIdx, iIdx);
										NBASSERT(idx->itmIndex >= 0 && idx->itmIndex < state->macros.defs.use)
										const STSrcAnlzMacro* macroDef = NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, idx->itmIndex);
										if(!macroDef->params.areExplicit){
											//MACRO value (direct translation)
#											ifdef NB_CONFIG_INCLUDE_ASSERTS
											{
												STNBString strTmp;
												NBString_init(&strTmp);
												SI32 i; for(i = 0; i < macroDef->valTokensCount; i++){
													NBASSERT(state->macros.valsTokns.tokns.defs.use == state->macros.valsTokns.iParamRefs.use)
													const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&state->macros.valsTokns.tokns.defs, STSrcAnlzTokn, macroDef->valTokensFirst + i);
													const char* toknStr2	= &state->macros.valsTokns.tokns.strBuff.str[toknDef->iAtBuff];
													if(strTmp.lenght != 0) NBString_concatByte(&strTmp, ' ');
													NBString_concat(&strTmp, toknStr2);
												}
												PRINTF_INFO("Macro reference, found: '%s', direct translation to %d tokens '%s' ...\n", toknStr, macroDef->valTokensCount, strTmp.str);
												NBString_release(&strTmp);
											}
#											endif
											//Reset parsing validations
											SI32 i; for(i = 0; i < macroDef->valTokensCount; i++){
												NBASSERT(state->macros.valsTokns.tokns.defs.use == state->macros.valsTokns.iParamRefs.use)
												const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&state->macros.valsTokns.tokns.defs, STSrcAnlzTokn, macroDef->valTokensFirst + i);
												const char* toknStr2	= &state->macros.valsTokns.tokns.strBuff.str[toknDef->iAtBuff];
												NBASSERT(NBArray_itmValueAtIndex(&state->macros.valsTokns.iParamRefs, SI8, macroDef->valTokensFirst + i) == -1) //Cannot be a param ref
												NBASSERT(toknStr2[0] != '\0') //Cannot be empty token
												//Append param token
												if(!SrcAnlzCodeCParser_feedTokn(state, toknStr2, toknDef->sintxPart, (i == 0 ? isSpacePreceded : toknDef->isSpacePreceded))){
													r = FALSE; NBASSERT(FALSE) break;
												}
												
											}
											//PRINTF_INFO("... end of macro reference translation.\n");
										} else {
											//MACRO function (get params before translating)
#											ifdef NB_CONFIG_INCLUDE_ASSERTS
											{
												STNBString strTmp;
												NBString_init(&strTmp);
												SI32 i; for(i = 0; i < macroDef->valTokensCount; i++){
													NBASSERT(state->macros.valsTokns.tokns.defs.use == state->macros.valsTokns.iParamRefs.use)
													const STSrcAnlzTokn* toknDef = NBArray_itmPtrAtIndex(&state->macros.valsTokns.tokns.defs, STSrcAnlzTokn, macroDef->valTokensFirst + i);
													const char* toknStr2	= &state->macros.valsTokns.tokns.strBuff.str[toknDef->iAtBuff];
													if(strTmp.lenght != 0) NBString_concatByte(&strTmp, ' ');
													NBString_concat(&strTmp, toknStr2);
												}
												PRINTF_INFO("Macro reference, call found: '%s'(+%d paramns), waiting for params before translating to %d tokens '%s'...\n", toknStr, macroDef->params.count, macroDef->valTokensCount, strTmp.str);
												NBString_release(&strTmp);
											}
#											endif
											//Activate macro-params-reading mode
											STSrcAnlzCodeCParseMacroParams* pState = (STSrcAnlzCodeCParseMacroParams*)NBMemory_alloc(sizeof(STSrcAnlzCodeCParseMacroParams));
											NBASSERT(state->curParamsReadState == NULL)
											pState->iMacro			= idx->itmIndex;
											pState->parentesisDepth	= 0; //0 when expecting first parenthesis. Commas not in level 1 must be ignored.
											NBString_init(&pState->strBuff); //strings buffer
											NBArray_init(&pState->tokens, sizeof(STSrcAnlzTokn), NULL); //tokens definitions
											NBArray_init(&pState->paramsRanges, sizeof(STSrcAnlzRange), NULL); //params (tokens ranges)
											state->curParamsReadState = pState;
										}
										addToken = FALSE;
									}
								}
							}
							//Add token to sintax
							if(r && addToken){
								PRINTF_INFO("Code token (pp-%s) '%s'\n", (curBlockCodeIsActive ? "on" : "off"), toknStr);
								const UI32 iTokenDef = state->acumTokensCur->defs.use;
								STSrcAnlzTokn newToken;
								newToken.iAtBuff	= state->acumTokensCur->strBuff.lenght;
								newToken.sintxPart	= partFound;
								newToken.isSpacePreceded = isSpacePreceded;
								NBArray_addValue(&state->acumTokensCur->defs, newToken);
								NBString_concatBytes(&state->acumTokensCur->strBuff, toknStr, toknStrSz + 1); // +1 to include the '\0' char.
								//Consume the part
								SrcAnlzCodeCParser_feedToknPriv(state, state->code, partFound, iTokenDef);
							}
						} else { //if(curBlockCodeIsActive)
							PRINTF_INFO("Code token (pp-%s) '%s'\n", (curBlockCodeIsActive ? "on" : "off"), toknStr);
						}
					} // if(!isLineChange)
				} else {
					PRINTF_ERROR("Undefined codeBlockType.\n");
					r = FALSE; NBASSERT(FALSE)
				}
				//Lines counters
				state->curLineTokensCount++;
			}
			//Lines counters
			if(isLineChange){
				state->curLineTokensCount = 0;
			}
		}
	}
	return r;
}
	
BOOL SrcAnlzCodeCParser_flush(STSrcAnlzCodeCParser* state){
	BOOL r = TRUE;
	//Finish any open PProc directive
	if(r){
		if(state->curBlqType == ENSrcAnlzCodeCParseMode_PProc){
			NBASSERT(state->acumTokensCur == &state->acumTokensPProc)
			//Process directive
			if(!SrcAnlzCodeCParser_pprocFeedDirective(state)){
				r = FALSE; NBASSERT(FALSE)
			} else {
				//Clear pproc tokens buffer in case a deeper level is invoked
				NBArray_empty(&state->acumTokensPProc.defs);
				NBString_empty(&state->acumTokensPProc.strBuff);
				NBASSERT(state->parseScopesStack.use > 1)
				if(state->parseScopesStack.use <= 1){
					PRINTF_ERROR("No active scope.\n");
					r = FALSE; NBASSERT(FALSE)
				} else {
					STSrcAnlzCodeCParseScope* scope		= NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, state->parseScopesStack.use - 1);
					state->curBlqType		= ENSrcAnlzCodeCParseMode_Code;
					state->acumTokensCur	= &scope->acumTokens;
				}
			}
		}
	}
	return r;
}

BOOL SrcAnlzCodeCParser_end(STSrcAnlzCodeCParser* state){
	BOOL r = TRUE;
	//ToDo, implement
	/*NBASSERT(state->codeParse.pprocCondPortionsStack.use == 1) //Only the root level must remain at the end
	 NBASSERT(state->codeParse.parseScopesStack.use == 2) //Only dummy and root scope must remain at the end
	 NBASSERT(state->streamsStack.use == 0) //No stream must remain active
	 if(state->codeParse.pprocCondPortionsStack.use == 1 && state->codeParse.parseScopesStack.use == 2){
	 NBASSERT(state->codeParse.parseScopesStack.use == 2)
	 if(state->codeParse.parseScopesStack.use == 2){
	 const STSrcAnlzCodeCParseScope* scope = NBArray_itmPtrAtIndex(&state->codeParse.parseScopesStack, STSrcAnlzCodeCParseScope, state->codeParse.parseScopesStack.use - 1);
	 NBASSERT(scope->parseTree.nodes.use == 1)
	 if(scope->parseTree.nodes.use == 1){
	 //Pop root scope (remove from stack)
	 //state->code.parseScopesStack->quitarElementoEnIndice(state->code.parseScopesStack->conteo - 1);
	 r = TRUE;
	 }
	 }
	 }*/
	
	//Feed an imposible/dummy part (the remaining posibility will be consumed)
	if(r){
		STSrcAnlzSintxPart dummyPart;
		dummyPart.type	= ENSrzAnlzSintxPartType_SintaxDef;
		dummyPart.idx	= ENSrzAnlzSintxC_Count;
		if(!SrcAnlzCodeCParser_feedToknPriv(state, state->code, dummyPart, 0)){
			r = FALSE; NBASSERT(FALSE)
		} else {
			r = TRUE;
		}
	}
	return r;
}


