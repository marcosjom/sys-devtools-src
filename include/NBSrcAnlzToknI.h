//
//  NBSrcAnlzTokn.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_TOKN_I_H
#define NB_SRCANLZ_TOKN_I_H

#include "nb/NBFrameworkDefs.h"
//
#include "nb/core/NBString.h"
#include "nb/core/NBArray.h"
#include "NBSrcAnlzSintx.h"	//for 'STSrcAnlzSintxPart'

//Token types (specific type)

typedef enum ENSrcAnlzToknType_ {
	ENSrcAnlzToknType_Keyword = 0,		//Keyword
	ENSrcAnlzToknType_Identifier,		//Identifier
	ENSrcAnlzToknType_Constant,			//Constant (integer, float, enum, char, ...)
	ENSrcAnlzToknType_StrLiteral,		//String literal
	ENSrcAnlzToknType_Punctuator,		//Punctuator
	ENSrcAnlzToknType_Comment,			//Comment
	ENSrcAnlzToknType_HeaderName,		//Header name (only parsed in #include and #pragma directives)
	ENSrcAnlzToknType_Count
} ENSrcAnlzToknType;

#define STR_TOKEN_TYPE(T) ( \
    T == ENSrcAnlzToknType_Keyword ? "Keyword" : \
    T == ENSrcAnlzToknType_Identifier ? "Identifier" : \
    T == ENSrcAnlzToknType_Constant ? "Constant" : \
    T == ENSrcAnlzToknType_StrLiteral ? "StrLiteral" : \
    T == ENSrcAnlzToknType_Punctuator ? "Punctuator" : \
    T == ENSrcAnlzToknType_Comment ? "Comment" : \
    T == ENSrcAnlzToknType_HeaderName ? "HeaderName" : \
    "unknown_token_type")

typedef enum ENSrcAnlzToknParseResult_ {
	ENSrcAnlzToknParseResult_Error = 0,		//The portion is not a token
	ENSrcAnlzToknParseResult_Partial,		//The portion could be atoken
	ENSrcAnlzToknParseResult_Complete,		//The portin is a valid token
	ENSrcAnlzToknParseResult_Count
} ENSrcAnlzToknParseResult;

typedef void (*ParseToknResetFunc)	(void* obj);
typedef BOOL (*ParseToknResetFeed)	(void* obj, const char* str, const SI32 strSz, const char newChar, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType);

typedef struct STSrcAnlzToknParseTypeI_ {
	void*					obj;
	ParseToknResetFunc		funcReset;	//Func to reset the parse state
	ParseToknResetFeed		funcFeed;	//Func to feed a char to the parse state
} STSrcAnlzToknParseTypeI;

#ifdef __cplusplus
//}
#endif

#endif
