//
//  NBSrcAnlzParser.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_PARSE_H
#define NB_SRCANLZ_PARSE_H

#include "nb/NBFrameworkDefs.h"
//
#include <stdio.h>					//for FILE
#include "NBSrcAnlzParserI.h"		//for 'STSrcAnlzParserI'
#include "NBSrcAnlzToknParser.h"	//for 'STSrcAnlzToknParser'
//
#ifdef __cplusplus
//extern "C" {
#endif

//A 'stream' can be a file content, a string,
//or a virtual content produced by an internal parsing stage.
typedef struct STSrcAnlzParserStream_ {
	ENSrcAnlzParserSrcType	type;		//stream type (file or virtual)
	STNBString				srcPath;	//stream source path
	STSrcAnlzToknParser		toknParser;	//token parser of this stream
} STSrcAnlzParserStream;

//Parse state
typedef struct STSrcAnlzParser_ {
	STSrcAnlzParserItfs*		itfs;
	STSrcAnlzParserI			ownParserItf;
	//File stream
	FILE*						stream;
	//File stack
	STNBArray					streamsStack;	//STSrcAnlzParserStream*
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	const STSrcAnlzParserStream* dbgTopStream;
#	endif
} STSrcAnlzParser;

//Init and release
void SrcAnlzParser_init(STSrcAnlzParser* state, STSrcAnlzParserItfs* itfs);
void SrcAnlzParser_release(STSrcAnlzParser* state);

//Stream stack
BOOL SrcAnlzParser_streamPush(STSrcAnlzParser* state, const ENSrcAnlzParserSrcType srcType, const char* srcPath);
BOOL SrcAnlzParser_streamFeed(STSrcAnlzParser* state, const char* data, const UI32 dataSz);
BOOL SrcAnlzParser_streamFlush(STSrcAnlzParser* state);
BOOL SrcAnlzParser_streamPop(STSrcAnlzParser* state);

//Processing
BOOL SrcAnlzParser_processFilePath(STSrcAnlzParser* state, const char* filePath);
BOOL SrcAnlzParser_processStream(STSrcAnlzParser* state, FILE* stream); //Note: the caller of this method must deal with non-newLine ended streams.
BOOL SrcAnlzParser_processLines(STSrcAnlzParser* state, const char* strLines); //Note: the caller of this method must deal with non-newLine ended streams.
BOOL SrcAnlzParser_end(STSrcAnlzParser* state);

#ifdef __cplusplus
//}
#endif

#endif
