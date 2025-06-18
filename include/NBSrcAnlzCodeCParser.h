//
//  NBSrcAnlzCodeCParser.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_CODE_C_PARSE_H
#define NB_SRCANLZ_CODE_C_PARSE_H

#include "nb/NBFrameworkDefs.h"
#include "nb/core/NBString.h"
#include "NBSrcAnlzBase.h"				//for 'STSrcAnlzRange'
#include "NBSrcAnlzTokn.h"				//for 'STSrcAnlzTokns'
#include "NBSrcAnlzCodeC.h"				//for 'STSrcAnlzCodeC'
#include "NBSrcAnlzMacro.h"				//for 'STSrcAnlzMacros'
#include "NBSrcAnlzSintxParseTree.h"	//for 'STSrcAnlzSintxParseTree'
#include "NBSrcAnlzCodeCParserTokn.h"	//for 'STSrcAnlzCodeCParserTokn'
#include "NBSrcAnlzSintxC.h"			//for 'ENSrzAnlzSintxC'

#ifdef __cplusplus
//extern "C" {
#endif

// Code parsing scope stats

typedef struct STSrcAnlzCodeCParseScopeStats_ {
	UI32			qSintaxSeqs;	//Sintax sequences.
	STSrcAnlzRange	usrTypesRng;	//User types declared.
	STSrcAnlzRange	declarsRng;		//Declarations.
	STSrcAnlzRange	scopesRng;		//Subscopes (block-item-list)
	UI32			iLastActiveDecScope;	//Last active declars-scope
} STSrcAnlzCodeCParseScopeStats;

// Code parsing scope types

typedef enum ENSrcAnlzCodeCParseScopeType_ {
	ENSrcAnlzCodeCParseScopeType_Normal = 0,	//Normal code
	ENSrcAnlzCodeCParseScopeType_Attribute,		//Attribute like: __attribute__((aligned(8)))
	ENSrcAnlzCodeCParseScopeType_ASM,			//Assembler Instructions block like: __asm__("nop;nop;nop");
	ENSrcAnlzCodeCParseScopeType_Count
} ENSrcAnlzCodeCParseScopeType;

// Code parsing scope types

typedef struct STSrcAnlzCodeCParseScope_ {
	ENSrcAnlzCodeCParseScopeType	type;
	ENSrzAnlzSintxC					iSintx;			//Type of scope (enum-const-list, struct-dec-list, block-dec-list, etc).
	UI32							iDeclScope;	//Root declarations scope
	//
	STSrcAnlzCodeCParseScopeStats	stats;			//Parsing scope stats
	STSrcAnlzSintxParseTree			parseTree;		//Tree of posibilities in parsing language sintax
	STSrcAnlzTokns					acumTokens;		//Tokens accum in CODE scope
	STNBArray						charLvlStack;	//char, stacks of chars that opened a inner level
} STSrcAnlzCodeCParseScope;

// Code parsing reading params state

typedef struct STSrcAnlzCodeCParseMacroParams_ {
	UI32			iMacro;				//Macro function index
	UI32			parentesisDepth;	//0 when expecting first parenthesis. Commas not in level 1 must be ignored.
	STNBString		strBuff;			//strings buffer
	STNBArray		tokens;				//STSrcAnlzTokn, tokens definitions
	STNBArray		paramsRanges;		//STSrcAnlzRange, params (tokens ranges)
} STSrcAnlzCodeCParseMacroParams;

// State of a cond portion of code (between if, elif, else and end)

typedef struct STSrcAnlzCodeCondPortionState_ {
	BOOL	isParentActive;			//The parent-block is active?
	BOOL	isPortionActive;		//The current portion-of-block is active? (#if/#elif/#else/#endif)
	BOOL	isPortionActiveFound;	//one of the blocks inside a #if/#elif/#else/#endif was already activated (only one can be activated)
	BOOL	isPortionElseFound;		//The #else block was already found (only #endif is allowed after #else)
} STSrcAnlzCodeCondPortionState;

//Code modes
typedef enum ENSrcAnlzCodeCParseMode_ {
	ENSrcAnlzCodeCParseMode_Code = 0,	// int c = 0;
	ENSrcAnlzCodeCParseMode_PProc,		// #define ...
} ENSrcAnlzCodeCParseMode;

//Source code parsing
typedef struct STSrcAnlzCodeCParser_ {
	//
	STSrcAnlzSintxDefs			sintxDefs;
	STSrcAnlzParserItfs*		itfs;
	STSrcAnlzLangToknI			ownToknItf;
	STSrcAnlzLangSntxI			ownSntxItf;
	STSrcAnlzCodeC*				code;
	STSrcAnlzCodeCParserTokn	toknParse;
	//
	ENSrcAnlzCodeCParseMode		curBlqType;
	STNBArray					pprocCondPortionsStack;	//STSrcAnlzCodeCondPortionState
	//
	STSrcAnlzTokns				acumTokensPProc;	//Tokens accum in PREPROC scope
	STSrcAnlzTokns*				acumTokensCur;		//Current token acum (pointer to acumTokensPProc or acumTokensCode)
	//Included files
	STNBArray					includedStarts; //UI32, files #import first char pos at 'included'
	STNBString					included;		//files #import "..." separated by '\0'
	//Config: user and system include paths
	STNBArray					usrIncludesStarts;	//UI32, include paths first char pos at 'usrIncludes'
	STNBString					usrIncludes;	//includes paths separated by '\0'
	STNBArray					sysIncludesStarts;	//UI32, include paths first char pos at 'sysIncludes'
	STNBString					sysIncludes;	//includes paths separated by '\0'
	//
	//Macros definitions
	STSrcAnlzMacros				macros;
	STNBArray					parseScopesStack;	//STSrcAnlzCodeCParseScope, scopes stack
	//
	STSrcAnlzCodeCParseMacroParams*	curParamsReadState; //Current params reading //Not NULL when expecting to read MACRO-func-params
	//
	UI32						curLineTokensCount;
} STSrcAnlzCodeCParser;

//Init and release
void SrcAnlzCodeCParser_init(STSrcAnlzCodeCParser* state, STSrcAnlzParserItfs* itfs, STSrcAnlzCodeC* code);
void SrcAnlzCodeCParser_release(STSrcAnlzCodeCParser* state);

//Config
BOOL SrcAnlzCodeCParser_addIncludePathUsr(STSrcAnlzCodeCParser* state, const char* path);
BOOL SrcAnlzCodeCParser_addIncludePathSys(STSrcAnlzCodeCParser* state, const char* path);

//Code parse scope stack
BOOL SrcAnlzCodeCParser_scopePush(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, const ENSrcAnlzCodeCParseScopeType parScopeType, const ENSrzAnlzSintxC iSintx, const UI32 iParentDeclarScope, const BOOL alsoMoveOldTopLastToken);
BOOL SrcAnlzCodeCParser_scopePop(STSrcAnlzCodeCParser* state, STSrcAnlzCodeC* code, ENSrzAnlzSintxC* dstSintx, UI32* dstDeclarScope, const BOOL alsoMoveOldTopLastToken);

//Feed
BOOL SrcAnlzCodeCParser_feedTokn(STSrcAnlzCodeCParser* state, const char* toknStr, const STSrcAnlzSintxPart partFound, const BOOL precededBySpace);
BOOL SrcAnlzCodeCParser_flush(STSrcAnlzCodeCParser* state);
BOOL SrcAnlzCodeCParser_end(STSrcAnlzCodeCParser* state);

//Feed
//BOOL SrcAnlzParser_codeFeedExpression(void* pGState);


#ifdef __cplusplus
//}
#endif

#endif
