//
//  NBSrcAnlzCodeCParserPProc.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_CODE_C_PARSER_PPROC_H
#define NB_SRCANLZ_CODE_C_PARSER_PPROC_H

#include "nb/NBFrameworkDefs.h"
//
#include "NBSrcAnlzCodeCParser.h"	//for 'STSrcAnlzCodeCParser'
#include "NBSrcAnlzExprParse.h"		//for 'ENSrcAnlzExprSrc'
#include "NBSrcAnlzSintxC.h"		//for 'ENSrcAnlzSintxCOpType'

#ifdef __cplusplus
//extern "C" {
#endif

//
//Preprocesor expresions SINTAX map
//

extern const STSrcAnlzExprOpSintxMap __globalPProcCondExprSintaxMap;	//Sintax for reading expresions
extern const STSrcAnlzExprOpSintxMap __globalPProcMacroParamsSintaxMap;	//Sintax for reading macro's excution params

//
//Preprocesor expresions EVAL
//

typedef enum STSrcAnlzValOpValType_ {
	STSrcAnlzValOpValType_Bool = 0,		//Boolean value
	STSrcAnlzValOpValType_Int,			//integer signed
	STSrcAnlzValOpValType_IntU,			//integer unsigned
	STSrcAnlzValOpValType_Long,			//long signed
	STSrcAnlzValOpValType_LongU,		//long unsigned
	STSrcAnlzValOpValType_LongLong,		//long long signed
	STSrcAnlzValOpValType_LongLongU,	//long long unsigned
	STSrcAnlzValOpValType_Float,		//float
	STSrcAnlzValOpValType_Double,		//double
	STSrcAnlzValOpValType_DoubleLong,	//long double
	STSrcAnlzValOpValType_Tokens,		//Multiple tokens (special chars, identifiers and/or literal)
	STSrcAnlzValOpValType_IdNotMacro,	//One identifier, not defined (yet)
	STSrcAnlzValOpValType_Macro,		//One identifier, a defined MACRO
	//
	STSrcAnlzValOpValType_Count
} STSrcAnlzValOpValType;

#define STR_SrcAnlzValOpValType(TYPE) ( \
    TYPE == STSrcAnlzValOpValType_Bool ? "BOOL" : \
    TYPE == STSrcAnlzValOpValType_Int ? "int" : \
    TYPE == STSrcAnlzValOpValType_IntU ? "intU" : \
    TYPE == STSrcAnlzValOpValType_Long ? "long" : \
    TYPE == STSrcAnlzValOpValType_LongU ? "longU" : \
    TYPE == STSrcAnlzValOpValType_LongLong ? "longLong" : \
    TYPE == STSrcAnlzValOpValType_LongLongU ? "longlongU" : \
    TYPE == STSrcAnlzValOpValType_Float ? "float" : \
    TYPE == STSrcAnlzValOpValType_Double ? "double" : \
    TYPE == STSrcAnlzValOpValType_DoubleLong ? "doubleLong" : \
    TYPE == STSrcAnlzValOpValType_Tokens ? "tokens" : \
    TYPE == STSrcAnlzValOpValType_IdNotMacro ? "identfNotMacro" : \
    TYPE == STSrcAnlzValOpValType_Macro ? "macro" : \
    "unknownType")


typedef struct STSrcAnlzValOpVal_ {
	STSrcAnlzValOpValType	type;
	union {
		BOOL				valBool;		//Boolean value.
		int					valIntt;		//
		unsigned int		valIntU;		//
		long				valLong;		//
		unsigned long		valLongU;		//
		long long			valLongLong;	//
		unsigned long long	valLongLongU;	//
		float				valFloat;		//
		double				valDouble;		//
		long double			valDoubleLong;	//
		STSrcAnlzRange		valTokens;		//Tokens range at 'parCodeState->tokensAcumDefs' array.
		SI32				valIdNotDef;	//Token position
		SI32				valMacroIdx;	//Macro's position at 'state->macros' array.
	};
} STSrcAnlzValOpVal;

// Evaluating an expresion

typedef enum STSrcAnlzEvalMode_ {
	STSrcAnlzEvalMode_Recursive = 0,	//Eval with recursivity
	STSrcAnlzEvalMode_Single,			//Eval only one level/time.
	//
	STSrcAnlzEvalMode_Count
} STSrcAnlzEvalMode;

//Feed a complete preprocessor's directive
BOOL SrcAnlzCodeCParser_pprocFeedDirective(STSrcAnlzCodeCParser* state);

//Conditional expresions evaluations
BOOL SrcAnlzCodeCParser_pprocCondExprResult(STSrcAnlzCodeCParser* state, const STSrcAnlzTokns* tokns, STSrcAnlzValOpVal* dst, SI32* opIndex, const STSrcAnlzExprParseNode* ops, const SI32 opsCount);
BOOL SrcAnlzCodeCParser_pprocCondExprOpValAsBoolean(STSrcAnlzCodeCParser* state, BOOL* dst, const STSrcAnlzValOpVal* pVal, const STSrcAnlzEvalMode evalMode);
BOOL SrcAnlzCodeCParser_pprocCondExprEvalLR(STSrcAnlzCodeCParser* state, STSrcAnlzValOpVal* dst, const STSrcAnlzValOpVal* pLeft, const STSrcAnlzValOpVal* pRight, const SI32 opType, const SI32 opEnumValue);
BOOL SrcAnlzCodeCParser_pprocCondExprEvalROnly(STSrcAnlzCodeCParser* state, STSrcAnlzValOpVal* dst, const STSrcAnlzValOpVal* pRight, const SI32 opType, const SI32 opEnumValue);

//Macros evaluations
BOOL SrcAnlzCodeCParser_pprocMacroEvalNoParams(STSrcAnlzCodeCParser* state, STSrcAnlzValOpVal* dst, const STSrcAnlzMacro* macro, const STSrcAnlzEvalMode evalMode);

#ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
//Debug
void SrcAnlzCodeCParser_pprocDbgAddfTokensToStr(const STSrcAnlzCodeCParser* state, STNBString* dstStr, const SI32 iFirstTokn, const BOOL noSpaces, const BOOL oneDepthLess);
void SrcAnlzCodeCParser_pprocDbgPrintfTokens(const STSrcAnlzCodeCParser* state, const BOOL curCodePortionActive, const BOOL oneDepthLess);
#endif

#ifdef __cplusplus
//}
#endif

#endif
