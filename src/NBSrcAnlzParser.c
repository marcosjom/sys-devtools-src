//
//  NBSrcAnlzParser.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzParser.h"

//Stream stack
BOOL SrcAnlzParser_streamPushI(STSrcAnlzParserI* itf, const ENSrcAnlzParserSrcType srcType, const char* srcPath){
	NBASSERT(itf != NULL && itf->obj != NULL)
	return SrcAnlzParser_streamPush((STSrcAnlzParser*)itf->obj, srcType, srcPath);
}

BOOL SrcAnlzParser_streamFeedI(STSrcAnlzParserI* itf, const char* data, const UI32 dataSz){
	NBASSERT(itf != NULL && itf->obj != NULL)
	return SrcAnlzParser_streamFeed((STSrcAnlzParser*)itf->obj, data, dataSz);
}

BOOL SrcAnlzParser_streamFlushI(STSrcAnlzParserI* itf){
	NBASSERT(itf != NULL && itf->obj != NULL)
	return SrcAnlzParser_streamFlush((STSrcAnlzParser*)itf->obj);
}

BOOL SrcAnlzParser_streamPopI(STSrcAnlzParserI* itf){
	NBASSERT(itf != NULL && itf->obj != NULL)
	return SrcAnlzParser_streamPop((STSrcAnlzParser*)itf->obj);
}

//-------------------------
//- File state
//-------------------------

void SrcAnlzParser_init(STSrcAnlzParser* state, STSrcAnlzParserItfs* itfs){
	state->itfs			= itfs;
	//Own global parser interface
	{
		state->ownParserItf.obj				= state;
		state->ownParserItf.streamPushFunc	= SrcAnlzParser_streamPushI;
		state->ownParserItf.streamFeedFunc	= SrcAnlzParser_streamFeedI;
		state->ownParserItf.streamFlushFunc	= SrcAnlzParser_streamFlushI;
		state->ownParserItf.streamPopFunc	= SrcAnlzParser_streamPopI;
		if(itfs != NULL){
			itfs->parserItf = &state->ownParserItf;
		}
	}
	//
	state->stream			= NULL;
	//Streams stack
	NBArray_init(&state->streamsStack, sizeof(STSrcAnlzParserStream*), NULL);
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	state->dbgTopStream		= NULL;
#	endif
}

void SrcAnlzParser_release(STSrcAnlzParser* state){
	//Streams stack
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	state->dbgTopStream		= NULL;
#	endif
	{
		NBASSERT(state->streamsStack.use == 0)
		{
			UI32 i; for(i = 0; i < state->streamsStack.use; i++){
				STSrcAnlzParserStream* stream = NBArray_itmValueAtIndex(&state->streamsStack, STSrcAnlzParserStream*, i);
				SrcAnlzToknParser_release(&stream->toknParser);
				NBString_release(&stream->srcPath);
			}
			NBArray_empty(&state->streamsStack);
		}
		NBArray_release(&state->streamsStack);
	}
	//
	state->stream	= NULL;
	//Own global parser interface
	{
		state->ownParserItf.obj				= NULL;
		state->ownParserItf.streamPushFunc	= NULL;
		state->ownParserItf.streamFeedFunc	= NULL;
		state->ownParserItf.streamFlushFunc	= NULL;
		state->ownParserItf.streamPopFunc	= NULL;
	}
	//Language sintax parser interface
	state->itfs		= NULL;
}

//Streams stack
BOOL SrcAnlzParser_streamPush(STSrcAnlzParser* state, const ENSrcAnlzParserSrcType srcType, const char* srcPath){
	BOOL r = TRUE;
	STSrcAnlzParserStream* stream = (STSrcAnlzParserStream*)NBMemory_alloc(sizeof(STSrcAnlzParserStream));
	{
		stream->type		= srcType;
		NBString_init(&stream->srcPath);
		if(srcPath != NULL){
			if(srcPath[0] != '\0'){
				//Setting explicit name
				NBString_concat(&stream->srcPath, srcPath);
			} else if(state->streamsStack.use > 0){
				//Setting parent's name
				STSrcAnlzParserStream* topStream = NBArray_itmValueAtIndex(&state->streamsStack, STSrcAnlzParserStream*, state->streamsStack.use - 1);
				NBString_concat(&stream->srcPath, topStream->srcPath.str);
			}
		} else if(state->streamsStack.use > 0){
			//Setting parent's name
			STSrcAnlzParserStream* topStream = NBArray_itmValueAtIndex(&state->streamsStack, STSrcAnlzParserStream*, state->streamsStack.use - 1);
			NBString_concat(&stream->srcPath, topStream->srcPath.str);
		}
		//Tokens parsing state
		SrcAnlzToknParser_init(&stream->toknParser, state->itfs);
	}
	NBArray_add(&state->streamsStack, &stream, sizeof(stream));
	PRINTF_INFO("SrcStream, pushed to '%s'.\n", srcPath);
	//Set dbg top stream (for debugging)
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	if(state->streamsStack.use == 0){
		state->dbgTopStream		= NULL;
	} else {
		state->dbgTopStream		= NBArray_itmValueAtIndex(&state->streamsStack, STSrcAnlzParserStream*, state->streamsStack.use - 1);
	}
#	endif
	return r;
}

BOOL SrcAnlzParser_streamFeed(STSrcAnlzParser* state, const char* data, const UI32 dataSz){
	BOOL r = FALSE;
	NBASSERT(state->streamsStack.use > 0)
	if(state->streamsStack.use > 0){
		r = TRUE;
		STSrcAnlzParserStream* lastStream = NBArray_itmValueAtIndex(&state->streamsStack, STSrcAnlzParserStream*, state->streamsStack.use - 1);
		if(dataSz > 0){
			NBASSERT(data != NULL)
			if(data == NULL){
				r = FALSE; NBASSERT(FALSE)
			} else {
				if(!SrcAnlzToknParser_feed(&lastStream->toknParser, data, dataSz)){
					r = FALSE; NBASSERT(FALSE)
				}
			}
		}
	}
	return r;
}

BOOL SrcAnlzParser_streamFlush(STSrcAnlzParser* state){
	BOOL r = FALSE;
	NBASSERT(state->streamsStack.use > 0)
	if(state->streamsStack.use > 0){
		STSrcAnlzParserStream* lastStream = NBArray_itmValueAtIndex(&state->streamsStack, STSrcAnlzParserStream*, state->streamsStack.use - 1);
		if(!SrcAnlzToknParser_flush(&lastStream->toknParser)){
			r = FALSE; NBASSERT(FALSE)
		} else {
			r = TRUE;
		}
	}
	return r;
}

BOOL SrcAnlzParser_streamPop(STSrcAnlzParser* state){
	BOOL r = FALSE;
	NBASSERT(state->streamsStack.use > 0)
	if(state->streamsStack.use > 0){
		STSrcAnlzParserStream* lastStream = NBArray_itmValueAtIndex(&state->streamsStack, STSrcAnlzParserStream*, state->streamsStack.use - 1);
		PRINTF_INFO("SrcStream, popping from '%s'.\n", lastStream->srcPath.str);
		//Release parser
		SrcAnlzToknParser_release(&lastStream->toknParser);
		//Release srcPath
		NBString_release(&lastStream->srcPath);
		//Remove path desc from stack
		NBArray_removeItemAtIndex(&state->streamsStack, state->streamsStack.use - 1);
		NBMemory_free(lastStream);
		//Set dbg top stream (for debugging)
#		ifdef NB_CONFIG_INCLUDE_ASSERTS
		if(state->streamsStack.use == 0){
			state->dbgTopStream		= NULL;
		} else {
			state->dbgTopStream		= NBArray_itmValueAtIndex(&state->streamsStack, STSrcAnlzParserStream*, state->streamsStack.use - 1);
		}
#		endif
		r = TRUE;
	}
	return r;
}

//File processing
BOOL SrcAnlzParser_processFilePath(STSrcAnlzParser* state, const char* filePath){
	BOOL r = FALSE;
	FILE* stream = fopen(filePath, "rb");
	if(stream != NULL){
#		ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
		//const SI32 dbgBlkDepth = state->codeParse.pprocCondPortionsStack->conteo;
#		endif
		//Push stream
		if(SrcAnlzParser_streamPush(state, ENSrcAnlzParserSrcType_File, filePath)){
			r = TRUE;
			//Read content
			BYTE buffer[NB_SRCANLZ_PARSE_BUFFER_SIZE];
			PRINTF_INFO("----- PARSING FILE'S CONTENT.\n")
			while(1){
				//Load data to buffer
				const size_t bytesRead = fread(buffer, 1, NB_SRCANLZ_PARSE_BUFFER_SIZE, stream);
				if(bytesRead <= 0){
					break;
				} else {
					if(!SrcAnlzParser_streamFeed(state, (const char*)buffer, (UI32)bytesRead)){
						r = FALSE;
						break;
					}
				}
			}
			//Flush
			if(r){
				if(!SrcAnlzParser_streamFlush(state)){
					r = FALSE;
				}
			}
			//Last char must be new-line
			if(r){
				//-----------------------
				//http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf
				//A source file shall not end in a partial preprocessing token or in a partial comment.
				//Fix: push a newLine char at end of file (if necesary).
				//-----------------------
				/*if(r && (topStream->toknParser.lastStreamChar != '\n' || topStream->toknParser.strAcum2.length > 0)){
					PRINTF_WARNING("File's last char is not a newLine (adding newLine to stream): '%s'.\n", filePath);
					char lastChar = '\n';
					if(!SrcAnlzParser_streamFeed(state, &lastChar, sizeof(lastChar))){
						r = FALSE;
					} else if(!SrcAnlzParser_streamFlush(state)){
						r = FALSE;
					}
				}*/
			}
			//Pop stream
			if(!SrcAnlzParser_streamPop(state)){
				r = FALSE;
			}
		}
		fclose(stream);
		//NBASSERT(!r || dbgBlkDepth == state->codeParse.pprocCondPortionsStack->conteo)
	}
	NBASSERT(r)
	return r;
}

//Note: the caller of this method must deal with non-newLine ended streams.
//If the last char of the stream is not a newLine char, the last preprocessor directive or code-token will not be processed.
BOOL SrcAnlzParser_processStream(STSrcAnlzParser* state, FILE* stream){
	BOOL r = FALSE;
	if(stream != NULL){
		//Push stream
		if(SrcAnlzParser_streamPush(state, ENSrcAnlzParserSrcType_Virtual, NULL)){
			r = TRUE;
			BYTE buffer[NB_SRCANLZ_PARSE_BUFFER_SIZE];
			PRINTF_INFO("----- PARSING FILE'S CONTENT.\n")
			while(1){
				//Load data to buffer
				const size_t bytesRead = fread(buffer, 1, NB_SRCANLZ_PARSE_BUFFER_SIZE, stream);
				if(bytesRead <= 0){
					break;
				} else {
					if(!SrcAnlzParser_streamFeed(state, (const char*)buffer, (UI32)bytesRead)){
						r = FALSE;
						break;
					}
				}
			}
			//Flush
			if(r){
				if(!SrcAnlzParser_streamFlush(state)){
					r = FALSE;
				}
			}
			//Last char must be new-line
			if(r){
				//-----------------------
				//http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf
				//A source file shall not end in a partial preprocessing token or in a partial comment.
				//Fix: push a newLine char at end of file (if necesary).
				//-----------------------
				/*if(r && (topStream->toknParser.lastStreamChar != '\n' || topStream->toknParser.strAcum2.length > 0)){
					PRINTF_WARNING("File's last char is not a newLine (adding newLine to stream): '%s'.\n", NULL);
					char lastChar = '\n';
					if(!SrcAnlzParser_streamFeed(state, &lastChar, sizeof(lastChar))){
						r = FALSE;
					} else if(!SrcAnlzParser_streamFlush(state)){
						r = FALSE;
					}
				}*/
			}
			//Pop stream
			if(!SrcAnlzParser_streamPop(state)){
				r = FALSE;
			}
		}
	}
	return r;
}

BOOL SrcAnlzParser_processLines(STSrcAnlzParser* state, const char* strLines){
	BOOL r = FALSE;
	if(strLines != NULL){
		r = TRUE;
		PRINTF_INFO("----- PARSING CONTENT LINES.\n")
		const SI32 strLen = NBString_strLenBytes(strLines);
		if(strLen > 0){
#			ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
			//const SI32 dbgBlkDepth = state->codeParse.pprocCondPortionsStack->conteo;
#			endif
			//Push stream
			if(SrcAnlzParser_streamPush(state, ENSrcAnlzParserSrcType_Virtual, NULL)){
				if(!SrcAnlzParser_streamFeed(state, strLines, strLen)){
					r = FALSE;
				} else {
					//Flush
					if(!SrcAnlzParser_streamFlush(state)){
						r = FALSE;
					} else {
						//Last char must be new-line
						//-----------------------
						//http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf
						//A source file shall not end in a partial preprocessing token or in a partial comment.
						//Fix: push a newLine char at end of file (if necesary).
						//-----------------------
						/*if(topStream->toknParser.lastStreamChar != '\n' || topStream->toknParser.strAcum2.length > 0){
							PRINTF_WARNING("ContentLines's last char is not a newLine (adding newLine to stream).\n");
							char lastChar = '\n';
							if(!SrcAnlzParser_streamFeed(state, &lastChar, sizeof(lastChar))){
								r = FALSE;
							} else if(!SrcAnlzParser_streamFlush(state)){
								r = FALSE;
							}
						}*/
					}
				}
				//Pop stream
				if(!SrcAnlzParser_streamPop(state)){
					r = FALSE;
				}
			}
			//NBASSERT(!r || dbgBlkDepth == state->codeParse.pprocCondPortionsStack->conteo)
		}
	}
	return r;
}

BOOL SrcAnlzParser_end(STSrcAnlzParser* state){
	BOOL r = FALSE;
	if(state->itfs->sintaxItf->endFunc != NULL){
		if(!(*state->itfs->sintaxItf->endFunc)(state->itfs->sintaxItf)){
			r = FALSE; NBASSERT(FALSE)
		} else {
			r = TRUE;
		}
	}
	return r;
}


