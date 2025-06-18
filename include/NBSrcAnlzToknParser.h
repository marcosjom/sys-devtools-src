//
//  NBSrcAnlzToknParser.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_TOKN_PARSE_H
#define NB_SRCANLZ_TOKN_PARSE_H

#include "nb/NBFrameworkDefs.h"
#include "nb/core/NBString.h"
#include "NBSrcAnlzTokn.h"		//for 'ENSrcAnlzToknType'
#include "NBSrcAnlzParserI.h"	//for 'STSrcAnlzParserI'

#ifdef __cplusplus
//extern "C" {
#endif

typedef struct STSrcAnlzToknParseType_ {
	STSrcAnlzToknParseTypeI		itf;				//Interface
	ENSrcAnlzToknParseResult	lastResult;			//Result of last evaluation
	UI32						lastValidLen;		//Last valid complete len
	SI32						lastValidVal;		//Last valid complete value (optional)
	ENSrcAnlzToknType			lastValidType;		//Last valid complete type (optional)
	SI8							lastValidSubType;	//Last valid complete subtype (optional)
} STSrcAnlzToknParseType;

// Code parsing pos
typedef struct STSrcAnlzToknParsePos_ {
	//Global pos
	UI32	iByte;		//Current byte index (or final count)
	UI32	iToken;		//Current token index (or final count)
	//Line pos
	UI32	iLine;		//Current line index (or final line count)
	UI32	iLineByte;	//Current line's byte index (or final byte count)
	UI32	iLineToken;	//Current line's token index (or final count)
} STSrcAnlzToknParsePos;

typedef struct STSrcAnlzToknParser_ {
	STSrcAnlzParserItfs*		itfs;
	//Parsing states
	STSrcAnlzToknParseType*		parseTypes;
	SI32						parseTypesSz;
	//
	char						lastStreamChar;
	//Reading token
	STNBString					strAcum;			//token acum
	STNBString					strAcum2;			//tmp string buffer
	BOOL						lastTokenWasSpace;	//
	//
	STSrcAnlzToknParsePos		streamPos;			//file or stream real position
} STSrcAnlzToknParser;

//Init and release
void SrcAnlzToknParser_init(STSrcAnlzToknParser* state, STSrcAnlzParserItfs* itfs);
void SrcAnlzToknParser_release(STSrcAnlzToknParser* state);

//Content processing
BOOL SrcAnlzToknParser_feed(STSrcAnlzToknParser* state, const char* data, const UI32 dataSz);
BOOL SrcAnlzToknParser_flush(STSrcAnlzToknParser* state);


#ifdef __cplusplus
//}
#endif

#endif
