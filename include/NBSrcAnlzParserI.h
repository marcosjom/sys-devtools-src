//
//  NBSrcAnlzParser.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_PARSER_I_H
#define NB_SRCANLZ_PARSER_I_H

#include "nb/NBFrameworkDefs.h"
#include "NBSrcAnlzToknI.h"			//for 'ENSrcAnlzToknType'

//
#ifdef __cplusplus
//extern "C" {
#endif

// Specific lang's token parser interface

typedef struct STSrcAnlzLangToknI_ {
	STSrcAnlzToknParseTypeI*	itfs;
	UI32						itfsSz;
} STSrcAnlzLangToknI;

// Specific lang's sintax parser interface

struct STSrcAnlzLangSntxI_;
typedef BOOL (*SrcAnlzSintxParseFeedToknFunc)	(struct STSrcAnlzLangSntxI_* itfObj, const char* toknStr, const ENSrcAnlzToknType toknType, const SI32 toknSubType, const SI32 toknTypeVal, const BOOL precededBySpace);
typedef BOOL (*SrcAnlzSintxParseFlushFunc)		(struct STSrcAnlzLangSntxI_* itfObj);
typedef BOOL (*SrcAnlzSintxParseEndFunc)		(struct STSrcAnlzLangSntxI_* itfObj);

typedef struct STSrcAnlzLangSntxI_ {
	void*							obj;					//obscure sintxParse object.
	//Token feed
	SrcAnlzSintxParseFeedToknFunc	feedToknFunc;			//to feed a parsed token.
	SrcAnlzSintxParseFlushFunc		flushFunc;				//to consume the remaining of the token
	SrcAnlzSintxParseEndFunc		endFunc;				//to consume the remaining of the tree
} STSrcAnlzLangSntxI;

// Global parser interface

typedef enum ENSrcAnlzParserSrcType_ {
	ENSrcAnlzParserSrcType_File = 0,
	ENSrcAnlzParserSrcType_Virtual,
	ENSrcAnlzParserSrcType_Count
} ENSrcAnlzParserSrcType;

struct STSrcAnlzParserI_;
typedef BOOL (*SrcAnlzParserStreamPush)		(struct STSrcAnlzParserI_* itfObj, const ENSrcAnlzParserSrcType srcType, const char* srcPath);
typedef BOOL (*SrcAnlzParserStreamFeed)		(struct STSrcAnlzParserI_* itfObj, const char* data, const UI32 dataSz);
typedef BOOL (*SrcAnlzParserStreamFlush)	(struct STSrcAnlzParserI_* itfObj);
typedef BOOL (*SrcAnlzParserStreamPop)		(struct STSrcAnlzParserI_* itfObj);

typedef struct STSrcAnlzParserI_ {
	void*						obj;
	SrcAnlzParserStreamPush		streamPushFunc;
	SrcAnlzParserStreamFeed		streamFeedFunc;
	SrcAnlzParserStreamFlush	streamFlushFunc;
	SrcAnlzParserStreamPop		streamPopFunc;
} STSrcAnlzParserI;

// Global interface

typedef struct STSrcAnlzParserItfs_ {
	STSrcAnlzLangToknI*		toknItf;
	STSrcAnlzLangSntxI*		sintaxItf;
	STSrcAnlzParserI*		parserItf;
} STSrcAnlzParserItfs;

#ifdef __cplusplus
//}
#endif

#endif
