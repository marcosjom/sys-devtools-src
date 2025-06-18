//
//  NBSrcAnlzCodeCParserTokn.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_CODE_C_PARSER_TOKN_H
#define NB_SRCANLZ_CODE_C_PARSER_TOKN_H

#include "nb/NBFrameworkDefs.h"
//
#include "nb/core/NBNumParser.h"
#include "NBSrcAnlzToknParser.h"	//STSrcAnlzToknParseTypeI

//Token parse types
typedef enum ENSrcAnlzToknParseType_ {
	ENSrcAnlzToknParseType_Punctuator = 0,	//punctuator (including multi-chars punctuator)
	ENSrcAnlzToknParseType_Identifier,		//identifier
	ENSrcAnlzToknParseType_Numeric,			//Numeric constant (intenger, floating point, binary, octal, hex)
	ENSrcAnlzToknParseType_Literal,			//Literal double-quote string, or single-quote char-constant
	ENSrcAnlzToknParseType_Comment,			//Monoline or multiline comments
	ENSrcAnlzToknParseType_HeaderNm,		//Header name
	ENSrcAnlzToknParseType_Count
} ENSrcAnlzToknParseType;

#define STR_PARSE_ELEM_TYPE(T) ( \
    T == ENSrcAnlzToknParseType_Punctuator ? "Punctuator" : \
    T == ENSrcAnlzToknParseType_Identifier ? "Identifier" : \
    T == ENSrcAnlzToknParseType_Numeric ? "Numeric" : \
    T == ENSrcAnlzToknParseType_Literal ? "Literal" : \
    T == ENSrcAnlzToknParseType_Comment ? "Comment" : \
    T == ENSrcAnlzToknParseType_HeaderNm ? "HeaderName" : \
    "ENSrcAnlzToknParseType_unknown")

//Token parse subtypes (constans)

typedef enum ENSrcAnlzToknParseConstType_ {
	ENSrcAnlzToknParseConstType_Integer = 0,	//Integer-constant
	ENSrcAnlzToknParseConstType_Float,			//Float-constant
	ENSrcAnlzToknParseConstType_Enum,			//Enumeration constant
	ENSrcAnlzToknParseConstType_Character,		//Character-constant
	ENSrcAnlzToknParseConstType_Count
} ENSrcAnlzToknParseConstType;

//Token parse subtypes (comments)

typedef enum ENSrcAnlzToknParseCommentType_ {
	ENSrcAnlzToknParseCommentType_MonoLine = 0,	//Integer-constant
	ENSrcAnlzToknParseCommentType_MultiLine,	//Float-constant
	ENSrcAnlzToknParseCommentType_Count
} ENSrcAnlzToknParseCommentType;

//Puctuator parsing state
typedef struct STSrcAnlzToknParsePunct_ {
	BOOL	isErr;
	const STSrcAnlzSintxDefs* sintxDefs;
	UI8		charsCount;
	UI16	lastValidPunctIdx;	// last valid complete punctuator index
} STSrcAnlzToknParsePunct;

//Identifier parsing state
typedef struct STSrcAnlzToknParseIdentf_ {
	BOOL	isErr;
	UI8		readingHexQuadChar;	//'0' if when expecting digit, non-digit, or '\', '1' when expecting 'u' or 'U', '2...5' when expecting hex digit
	UI16	charsCount;
} STSrcAnlzToknParseIdentf;

//Numeric-constant parsing state
typedef struct STSrcAnlzToknParseNumrc_ {
	BOOL			isErr;
	STNBNumParser	state;
	UI32			charsCount;
} STSrcAnlzToknParseNumrc;

//Char scape-seq type
typedef enum ENSrcAnlzToknScapeSeqType_ {
	ENSrcAnlzToknScapeSeqType_Plain = 0,
	ENSrcAnlzToknScapeSeqType_Octal,
	ENSrcAnlzToknScapeSeqType_Hex,
	ENSrcAnlzToknScapeSeqType_Count,
} ENSrcAnlzToknScapeSeqType;

//Literal or char-constant parsing state
typedef struct STSrcAnlzToknParseLiteral_ {
	BOOL	isErr;
	BOOL	isWide;				//Literal started with 'L' before quote
	BOOL	isQuoteOpen;		//Quote is currently open
	char	quoteChar;			//single or double quote char
	//
	UI32	curScapeSeqSz;		//current scape seq size
	ENSrcAnlzToknScapeSeqType curScapeSeqType; //current scape-seq type
	//
	UI32	logicCharsCount;	//logic chars count, after processing scape sequences
	UI32	pureCharsCount;		//pure chars count
} STSrcAnlzToknParseLiteral;

//Comment token type
typedef enum ENSrcAnlzToknCommentType_ {
	ENSrcAnlzToknCommentType_MonoLine = 0,
	ENSrcAnlzToknCommentType_MultiLine,
	ENSrcAnlzToknCommentType_Count,
} ENSrcAnlzToknCommentType;

//Comment parsing state
typedef struct STSrcAnlzToknParseComment_ {
	BOOL						isErr;
	BOOL						isOpen;
	ENSrcAnlzToknCommentType	type;
	UI8							borderCharsCount;	// chars found of opening/closing pattern
	UI32						charsCount;
} STSrcAnlzToknParseComment;

//Header name (only parsed withing #include and #pragma directives)
typedef struct STSrcAnlzToknParseHeadrName_ {
	BOOL					isErr;
	BOOL					isOpen;
	char					openingChar;		//'<' or double quote char
	UI32					charsCount;
} STSrcAnlzToknParseHeadrName;


typedef struct STSrcAnlzCodeCParserTokn_ {
	STSrcAnlzToknParseTypeI		itfs[ENSrcAnlzToknParseType_Count];
	//
	STSrcAnlzToknParsePunct		curPunctState;
	STSrcAnlzToknParseIdentf	curIdentfState;
	STSrcAnlzToknParseNumrc		curNumericState;
	STSrcAnlzToknParseLiteral	curLiteralState;
	STSrcAnlzToknParseComment	curCommentState;
	STSrcAnlzToknParseHeadrName	curHeadNameState;
} STSrcAnlzCodeCParserTokn;


//Init and release state
void SrcAnlzCodeCParser_toknInit(STSrcAnlzCodeCParserTokn* state, const STSrcAnlzSintxDefs* sintxDefs);
void SrcAnlzCodeCParser_toknRelease(STSrcAnlzCodeCParserTokn* state);

//Punctuator parser
void SrcAnlzCodeCParser_toknPunctReset(void* pState);
BOOL SrcAnlzCodeCParser_toknPunctFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType);

//Identifier parser
void SrcAnlzCodeCParser_toknIdentfReset(void* pState);
BOOL SrcAnlzCodeCParser_toknIdentfFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType);

//Numeric identifier
void SrcAnlzCodeCParser_toknNumrcReset(void* pState);
BOOL SrcAnlzCodeCParser_toknNumrcFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType);

//Literal or char-constant
void SrcAnlzCodeCParser_toknLiteralReset(void* pState);
BOOL SrcAnlzCodeCParser_toknLiteralFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType);

//Comments
void SrcAnlzCodeCParser_toknCommentReset(void* pState);
BOOL SrcAnlzCodeCParser_toknCommentFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType);

//Header name
void SrcAnlzCodeCParser_toknHeadrNameReset(void* pState);
BOOL SrcAnlzCodeCParser_toknHeadrNameFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType);

#ifdef __cplusplus
//}
#endif

#endif
