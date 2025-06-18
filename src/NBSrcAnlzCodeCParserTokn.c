//
//  NBSrcAnlzCodeCParserTokn.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzCodeCParserTokn.h"
#include "NBSrcAnlzTokn.h"	//for 'ENSrcAnlzToknType'
#include "nb/core/NBNumParser.h"

//Init and release state
void SrcAnlzCodeCParser_toknInit(STSrcAnlzCodeCParserTokn* state, const STSrcAnlzSintxDefs* sintxDefs){
	SrcAnlzCodeCParser_toknPunctReset(&state->curPunctState);	state->curPunctState.sintxDefs = sintxDefs;
	SrcAnlzCodeCParser_toknIdentfReset(&state->curIdentfState);
	SrcAnlzCodeCParser_toknNumrcReset(&state->curNumericState);
	SrcAnlzCodeCParser_toknLiteralReset(&state->curLiteralState);
	SrcAnlzCodeCParser_toknCommentReset(&state->curCommentState);
	SrcAnlzCodeCParser_toknHeadrNameReset(&state->curHeadNameState);
	{
		SI32 i; for(i = 0; i < ENSrcAnlzToknParseType_Count; i++){
			state->itfs[ENSrcAnlzToknParseType_Punctuator].obj			= &state->curPunctState;
			state->itfs[ENSrcAnlzToknParseType_Punctuator].funcReset	= SrcAnlzCodeCParser_toknPunctReset;
			state->itfs[ENSrcAnlzToknParseType_Punctuator].funcFeed		= SrcAnlzCodeCParser_toknPunctFeed;
			//
			state->itfs[ENSrcAnlzToknParseType_Identifier].obj			= &state->curIdentfState;
			state->itfs[ENSrcAnlzToknParseType_Identifier].funcReset	= SrcAnlzCodeCParser_toknIdentfReset;
			state->itfs[ENSrcAnlzToknParseType_Identifier].funcFeed		= SrcAnlzCodeCParser_toknIdentfFeed;
			//
			state->itfs[ENSrcAnlzToknParseType_Numeric].obj				= &state->curNumericState;
			state->itfs[ENSrcAnlzToknParseType_Numeric].funcReset		= SrcAnlzCodeCParser_toknNumrcReset;
			state->itfs[ENSrcAnlzToknParseType_Numeric].funcFeed		= SrcAnlzCodeCParser_toknNumrcFeed;
			//
			state->itfs[ENSrcAnlzToknParseType_Literal].obj				= &state->curLiteralState;
			state->itfs[ENSrcAnlzToknParseType_Literal].funcReset		= SrcAnlzCodeCParser_toknLiteralReset;
			state->itfs[ENSrcAnlzToknParseType_Literal].funcFeed		= SrcAnlzCodeCParser_toknLiteralFeed;
			//
			state->itfs[ENSrcAnlzToknParseType_Comment].obj				= &state->curCommentState;
			state->itfs[ENSrcAnlzToknParseType_Comment].funcReset		= SrcAnlzCodeCParser_toknCommentReset;
			state->itfs[ENSrcAnlzToknParseType_Comment].funcFeed		= SrcAnlzCodeCParser_toknCommentFeed;
			//
			state->itfs[ENSrcAnlzToknParseType_HeaderNm].obj			= &state->curHeadNameState;
			state->itfs[ENSrcAnlzToknParseType_HeaderNm].funcReset		= SrcAnlzCodeCParser_toknHeadrNameReset;
			state->itfs[ENSrcAnlzToknParseType_HeaderNm].funcFeed		= SrcAnlzCodeCParser_toknHeadrNameFeed;
			//
			NBASSERT(ENSrcAnlzToknParseType_Count == 6)
		}
	}
}

void SrcAnlzCodeCParser_toknRelease(STSrcAnlzCodeCParserTokn* state){
	{
		SI32 i; for(i = 0; i < ENSrcAnlzToknParseType_Count; i++){
			state->itfs[i].obj			= NULL;
			state->itfs[i].funcReset	= NULL;
			state->itfs[i].funcFeed	= NULL;
		}
	}
	//
	SrcAnlzCodeCParser_toknPunctReset(&state->curPunctState);
	SrcAnlzCodeCParser_toknIdentfReset(&state->curIdentfState);
	SrcAnlzCodeCParser_toknNumrcReset(&state->curNumericState);
	SrcAnlzCodeCParser_toknLiteralReset(&state->curLiteralState);
	SrcAnlzCodeCParser_toknCommentReset(&state->curCommentState);
	SrcAnlzCodeCParser_toknHeadrNameReset(&state->curHeadNameState);
}

//Punctuator (parsing)

void SrcAnlzCodeCParser_toknPunctReset(void* pState){
	STSrcAnlzToknParsePunct* state= (STSrcAnlzToknParsePunct*)pState;
	state->isErr				= FALSE;
	state->charsCount			= 0;
	state->lastValidPunctIdx	= 0;
}

BOOL SrcAnlzCodeCParser_toknPunctFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType){
	ENSrcAnlzToknParseResult r = ENSrcAnlzToknParseResult_Error;
	STSrcAnlzToknParsePunct* state= (STSrcAnlzToknParsePunct*)pState;
	if(!state->isErr){
		const SI32 rngsSz = state->sintxDefs->punctsRngsSz;
		STSrcAnlzRange rng;
		if(strSz > 0 && strSz <= rngsSz){
			//Analyze all ranges with len equal or larger than current string
			SI32 i; for(i = (strSz - 1); i < rngsSz && !r; i++){
				NBASSERT(i < rngsSz)
				rng = state->sintxDefs->punctsRngs[i];
				while(rng.count > 0){
					NBASSERT(NBString_strLenBytes(state->sintxDefs->puncts[rng.start]) >= strSz)
					if(NBString_strIsEqualBytes(state->sintxDefs->puncts[rng.start], rng.count, str, strSz)){
						//Found a matching (or partial) punctuator
						if((i + 1) == strSz){
							//Found the exact punctuator
							NBASSERT(r != ENSrcAnlzToknParseResult_Complete) //Second compelte found (error in punctutors defs)
							if(dstVal != NULL) *dstVal = rng.start;
							if(dstType != NULL) *dstType = ENSrcAnlzToknType_Punctuator;
							if(dstSubType != NULL) * dstSubType = 0;
							r = ENSrcAnlzToknParseResult_Complete;
						} else if(r != ENSrcAnlzToknParseResult_Complete){
							r = ENSrcAnlzToknParseResult_Partial;
						}
						break;
					}
					rng.start++; rng.count--;
				}
			}
		}
		state->isErr = (r == ENSrcAnlzToknParseResult_Error);
	}
	if(dstResult != NULL) *dstResult = r;
	return TRUE;
}

//Identifier (parsing)

void SrcAnlzCodeCParser_toknIdentfReset(void* pState){
	STSrcAnlzToknParseIdentf* state= (STSrcAnlzToknParseIdentf*)pState;
	state->isErr				= FALSE;
	state->readingHexQuadChar	= 0;
	state->charsCount			= 0;
}

BOOL SrcAnlzCodeCParser_toknIdentfFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType){
	ENSrcAnlzToknParseResult r = ENSrcAnlzToknParseResult_Error;
	STSrcAnlzToknParseIdentf* state= (STSrcAnlzToknParseIdentf*)pState;
	if(!state->isErr){
		switch(state->readingHexQuadChar){
			case 0:
				//Expecting digit, non-digit, '_' or '\'
				if(c == '_' || (c > 64 && c < 91) || (c > 96 && c < 123) || (state->charsCount != 0 && c > 47 && c < 58)){ //'A' = 65, 'Z' = 90, 'a' = 97, 'z' = 123, '0' = 48, '9' = 57
					state->charsCount++;
					r = ENSrcAnlzToknParseResult_Partial;
				} else if(c == '\\'){
					state->readingHexQuadChar++;
					state->charsCount++;
					r = ENSrcAnlzToknParseResult_Partial;
				}
				break;
			case 1:
				//Expecting 'u' or 'U'
				if(c == 'u' || c == 'U'){
					state->readingHexQuadChar++;
					state->charsCount++;
					r = ENSrcAnlzToknParseResult_Partial;
				}
				break;
			case 2: case 3: case 4: case 5:
				//Expecting hex-digit
				if((c > 64 && c < 71) || (c > 96 && c < 103) || (c > 47 && c < 58)){
					if(state->readingHexQuadChar == 5) state->readingHexQuadChar = 0;
					else state->readingHexQuadChar++;
					state->charsCount++;
					r = ENSrcAnlzToknParseResult_Partial;
				}
				break;
		}
		//Save last completed parsing size
		if(r == ENSrcAnlzToknParseResult_Partial){
			if(state->readingHexQuadChar == 0 && state->charsCount > 0){
				if(dstVal != NULL) *dstVal = -1;
				if(dstType != NULL) *dstType = ENSrcAnlzToknType_Identifier;
				if(dstSubType != NULL) * dstSubType = 0;
				r = ENSrcAnlzToknParseResult_Complete;
			}
		}
		//
		state->isErr = (r == ENSrcAnlzToknParseResult_Error);
	}
	if(dstResult != NULL) *dstResult = r;
	return TRUE;
}

//Numeric identifier (parsing)

void SrcAnlzCodeCParser_toknNumrcReset(void* pState){
	STSrcAnlzToknParseNumrc* state= (STSrcAnlzToknParseNumrc*)pState;
	state->isErr		= FALSE;
	NBNumParser_init(&state->state);
	state->charsCount	= 0;
}

BOOL SrcAnlzCodeCParser_toknNumrcFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType){
	ENSrcAnlzToknParseResult r = ENSrcAnlzToknParseResult_Error;
	STSrcAnlzToknParseNumrc* state= (STSrcAnlzToknParseNumrc*)pState;
	if(!state->isErr){
		NBNumParser_feedByte(&state->state, c);
		state->charsCount++;
		state->isErr = state->state.isErr;
	}
	NBASSERT(state->isErr == state->state.isErr)
	//Save last completed parsing size
	if(!state->isErr){
		r = ENSrcAnlzToknParseResult_Partial;
		ENNumericType numType = ENNumericType_Count;
		if(NBNumParser_hasValidEnd(&state->state, &numType)){
			NBASSERT(numType == ENNumericType_Floating || numType == ENNumericType_Integer)
			if(dstVal != NULL) *dstVal = -1;
			if(dstType != NULL) *dstType = ENSrcAnlzToknType_Constant;
			if(dstSubType != NULL) * dstSubType = (numType == ENNumericType_Floating ? ENSrcAnlzToknParseConstType_Float : ENSrcAnlzToknParseConstType_Integer);
			r = ENSrcAnlzToknParseResult_Complete;
		}
	}
	if(dstResult != NULL) *dstResult = r;
	return TRUE; //(!state->isErr);
}

//
//Literal or char-constant
//

void SrcAnlzCodeCParser_toknLiteralReset(void* pState){
	STSrcAnlzToknParseLiteral* state= (STSrcAnlzToknParseLiteral*)pState;
	state->isErr			= FALSE;
	state->isWide			= FALSE;	//Literal started with 'L' before quote
	state->isQuoteOpen		= FALSE;
	state->quoteChar		= '\0';		//single or double quote char
	//
	state->curScapeSeqSz	= 0;		//current scape seq size
	state->curScapeSeqType	= ENSrcAnlzToknScapeSeqType_Count; //current scape-seq type
	//
	state->logicCharsCount	= 0;		//logic chars count, after processing scape sequences
	state->pureCharsCount	= 0;		//pure chars count
}

BOOL SrcAnlzCodeCParser_toknLiteralFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType){
	ENSrcAnlzToknParseResult r = ENSrcAnlzToknParseResult_Error;
	STSrcAnlzToknParseLiteral* state= (STSrcAnlzToknParseLiteral*)pState;
	if(!state->isErr){
		if(state->pureCharsCount == 0){
			//First char
			switch (c) {
				case 'L': state->isWide = TRUE; r = ENSrcAnlzToknParseResult_Partial; break;
				case '\"': case '\'': state->isQuoteOpen = TRUE; state->quoteChar = c; r = ENSrcAnlzToknParseResult_Partial; break;
			}
		} else if(state->pureCharsCount == 1 && state->isWide){
			//Second char after 'L'
			NBASSERT(!state->isQuoteOpen)
			NBASSERT(state->quoteChar == '\0')
			switch (c) {
				case '\"': case '\'': state->isQuoteOpen = TRUE; state->quoteChar = c; r = ENSrcAnlzToknParseResult_Partial; break;
			}
		} else if(state->isQuoteOpen){
			switch (state->curScapeSeqType) {
				case ENSrcAnlzToknScapeSeqType_Count:
					//Reading plain char
					if(c == state->quoteChar){
						state->isQuoteOpen = FALSE;
						r = ENSrcAnlzToknParseResult_Partial;
					} else if(c == '\\'){
						NBASSERT(state->curScapeSeqSz == 0)
						state->curScapeSeqSz++;
						state->curScapeSeqType = ENSrcAnlzToknScapeSeqType_Plain;
						r = ENSrcAnlzToknParseResult_Partial;
					} else if(c != '\n'){
						state->logicCharsCount++;
						r = ENSrcAnlzToknParseResult_Partial;
					}
					break;
				case ENSrcAnlzToknScapeSeqType_Plain:
					//Reading scape first char
					NBASSERT(state->curScapeSeqSz == 1)
					switch(c){
						case '\'': case '\"': case '?': case '\\': case 'a': case 'b': case 'f': case 'n': case 'r': case 't': case 'v':
							state->curScapeSeqSz	= 0;
							state->curScapeSeqType	= ENSrcAnlzToknScapeSeqType_Count;
							state->logicCharsCount++;
							r = ENSrcAnlzToknParseResult_Partial;
							break;
						case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
							state->curScapeSeqSz++;
							state->curScapeSeqType	= ENSrcAnlzToknScapeSeqType_Octal;
							r = ENSrcAnlzToknParseResult_Partial;
							break;
						case 'x': case 'X':
							state->curScapeSeqSz++;
							state->curScapeSeqType	= ENSrcAnlzToknScapeSeqType_Hex;
							r = ENSrcAnlzToknParseResult_Partial;
							break;
					}
					break;
				case ENSrcAnlzToknScapeSeqType_Octal:
					NBASSERT(state->curScapeSeqSz > 0)
					switch(c){
						case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
							state->curScapeSeqSz++;
							if(state->curScapeSeqSz >= 4){ //max: 3 octal digits: "\ octal-digit octal-digit octal-digit"
								state->curScapeSeqSz	= 0;
								state->curScapeSeqType	= ENSrcAnlzToknScapeSeqType_Count;
								state->logicCharsCount++;
							}
							r = ENSrcAnlzToknParseResult_Partial;
							break;
						default:
							if(c == state->quoteChar){
								state->curScapeSeqSz	= 0;
								state->curScapeSeqType	= ENSrcAnlzToknScapeSeqType_Count;
								state->logicCharsCount++;
								state->isQuoteOpen		= FALSE;
								r = ENSrcAnlzToknParseResult_Partial;
							} else if(c == '\\'){
								state->curScapeSeqSz	= 1;
								state->curScapeSeqType = ENSrcAnlzToknScapeSeqType_Plain;
								state->logicCharsCount++;
								r = ENSrcAnlzToknParseResult_Partial;
							} else if(c != '\n'){
								state->curScapeSeqSz	= 0;
								state->curScapeSeqType	= ENSrcAnlzToknScapeSeqType_Count;
								state->logicCharsCount++;
								state->logicCharsCount++;
								r = ENSrcAnlzToknParseResult_Partial;
							}
							break;
					}
					break;
				case ENSrcAnlzToknScapeSeqType_Hex:
					NBASSERT(state->curScapeSeqSz > 0)
					switch(c){
						case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
						case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
						case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
							state->curScapeSeqSz++;
							r = ENSrcAnlzToknParseResult_Partial;
							break;
						default:
							if(c == state->quoteChar){
								if(state->curScapeSeqSz > 2){ //at least one hex-char: "\xF"
									state->curScapeSeqSz	= 0;
									state->curScapeSeqType	= ENSrcAnlzToknScapeSeqType_Count;
									state->logicCharsCount++;
									state->isQuoteOpen		= FALSE;
									r = ENSrcAnlzToknParseResult_Partial;
								}
							} else if(c == '\\'){
								if(state->curScapeSeqSz > 2){ //at least one hex-char: "\xF"
									state->curScapeSeqSz	= 1;
									state->curScapeSeqType = ENSrcAnlzToknScapeSeqType_Plain;
									state->logicCharsCount++;
									r = ENSrcAnlzToknParseResult_Partial;
								}
							} else if(c != '\n'){
								if(state->curScapeSeqSz > 2){ //at least one hex-char: "\xF"
									state->curScapeSeqSz	= 0;
									state->curScapeSeqType	= ENSrcAnlzToknScapeSeqType_Count;
									state->logicCharsCount++;
									state->logicCharsCount++;
									r = ENSrcAnlzToknParseResult_Partial;
								}
							}
							break;
					}
					break;
				default:
					NBASSERT(FALSE) //Unknown 'ENSrcAnlzToknScapeSeqType'
					break;
			}
		}
		state->pureCharsCount++;
		//Save last completed parsing size
		if(r == ENSrcAnlzToknParseResult_Partial){
			if(state->quoteChar != '\0' && !state->isQuoteOpen){
				NBASSERT(state->quoteChar == '\"' || state->quoteChar == '\'')
				if(state->quoteChar == '\"'){
					if(dstVal != NULL) *dstVal = -1;
					if(dstType != NULL) *dstType = ENSrcAnlzToknType_StrLiteral;
					if(dstSubType != NULL) * dstSubType = 0;
					r = ENSrcAnlzToknParseResult_Complete;
				} else if(state->quoteChar == '\''){
					if(dstVal != NULL) *dstVal = -1;
					if(dstType != NULL) *dstType = ENSrcAnlzToknType_Constant;
					if(dstSubType != NULL) * dstSubType = ENSrcAnlzToknParseConstType_Character;
					r = ENSrcAnlzToknParseResult_Complete;
				} else {
					r = ENSrcAnlzToknParseResult_Error; NBASSERT(FALSE)
				}
			}
		} else {
			r = ENSrcAnlzToknParseResult_Error;
		}
		state->isErr = (r == ENSrcAnlzToknParseResult_Error);
	}
	if(dstResult != NULL) *dstResult = r;
	return TRUE;
}

//Comments (parsing)

void SrcAnlzCodeCParser_toknCommentReset(void* pState){
	STSrcAnlzToknParseComment* state= (STSrcAnlzToknParseComment*)pState;
	state->isErr			= FALSE;
	state->isOpen			= FALSE;
	state->type				= ENSrcAnlzToknCommentType_Count;
	state->borderCharsCount	= 0;	// chars found of opening/closing pattern
	state->charsCount		= 0;
}

BOOL SrcAnlzCodeCParser_toknCommentFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType){
	ENSrcAnlzToknParseResult r = ENSrcAnlzToknParseResult_Error;
	STSrcAnlzToknParseComment* state= (STSrcAnlzToknParseComment*)pState;
	if(!state->isErr){
		switch (state->type) {
			case ENSrcAnlzToknCommentType_Count:
				//Defining type of comment
				if(state->borderCharsCount == 0){
					if(c == '/'){
						state->borderCharsCount++;
						r = ENSrcAnlzToknParseResult_Partial;
					}
				} else if(state->borderCharsCount == 1){
					if(c == '/'){
						state->isOpen	= TRUE;
						state->type	= ENSrcAnlzToknCommentType_MonoLine;
						state->borderCharsCount = 0;
						r = ENSrcAnlzToknParseResult_Complete;
					} else if(c == '*'){
						state->isOpen	= TRUE;
						state->type	= ENSrcAnlzToknCommentType_MultiLine;
						state->borderCharsCount = 0;
						r = ENSrcAnlzToknParseResult_Partial;
					}
				}
				break;
			case ENSrcAnlzToknCommentType_MonoLine:
				//Read until new-line
				if(state->isOpen){
					if(c == '\n'){
						state->isOpen = FALSE; //do not include the new line
					} else {
						r = ENSrcAnlzToknParseResult_Complete;
					}
				}
				break;
			case ENSrcAnlzToknCommentType_MultiLine:
				//Read until closing combination
				if(state->isOpen){
					if(state->borderCharsCount == 0){
						if(c == '*') state->borderCharsCount++;
						r = ENSrcAnlzToknParseResult_Partial;
					} else {
						NBASSERT(state->borderCharsCount == 1)
						if(c == '/') state->isOpen = FALSE;
						state->borderCharsCount = 0;
						r = ENSrcAnlzToknParseResult_Complete;
					}
				}
				break;
		}
		state->charsCount++;
		if(r == ENSrcAnlzToknParseResult_Complete){
			if(state->type == ENSrcAnlzToknCommentType_MonoLine){
				if(dstVal != NULL) *dstVal = -1;
				if(dstType != NULL) *dstType = ENSrcAnlzToknType_Comment;
				if(dstSubType != NULL) * dstSubType = ENSrcAnlzToknParseCommentType_MonoLine;
			} else if(state->type == ENSrcAnlzToknCommentType_MultiLine){
				if(dstVal != NULL) *dstVal = -1;
				if(dstType != NULL) *dstType = ENSrcAnlzToknType_Comment;
				if(dstSubType != NULL) * dstSubType = ENSrcAnlzToknParseCommentType_MultiLine;
			} else {
				r = ENSrcAnlzToknParseResult_Error; NBASSERT(FALSE)
			}
		}
		state->isErr = (r == ENSrcAnlzToknParseResult_Error);
	}
	if(dstResult != NULL) *dstResult = r;
	return TRUE;
}

//Header name (parsing)

void SrcAnlzCodeCParser_toknHeadrNameReset(void* pState){
	STSrcAnlzToknParseHeadrName* state= (STSrcAnlzToknParseHeadrName*)pState;
	state->isErr			= FALSE;
	state->isOpen			= FALSE;
	state->openingChar		= '\0';		//'<' or double quote char
	state->charsCount		= 0;
}

BOOL SrcAnlzCodeCParser_toknHeadrNameFeed(void* pState, const char* str, const SI32 strSz, const char c, ENSrcAnlzToknParseResult* dstResult, SI32* dstVal, ENSrcAnlzToknType* dstType, SI8* dstSubType){
	ENSrcAnlzToknParseResult r = ENSrcAnlzToknParseResult_Error;
	STSrcAnlzToknParseHeadrName* state= (STSrcAnlzToknParseHeadrName*)pState;
	if(!state->isErr){
		switch(state->openingChar){
			case '\0':
				//Expecting '<' or double quote char
				if(c == '<' || c == '\"'){
					state->openingChar = c;
					state->isOpen = TRUE;
					r = ENSrcAnlzToknParseResult_Partial;
				}
				break;
			case '\"':
			case '<':
				if(state->isOpen && c != '\n'){ //new-line char is forbiden
					if((state->openingChar == '\"' && c == '\"') || (state->openingChar == '<' && c == '>')){
						state->isOpen = FALSE;
						if(dstVal != NULL) *dstVal = -1;
						if(dstType != NULL) *dstType = ENSrcAnlzToknType_HeaderName;
						if(dstSubType != NULL) * dstSubType = 0;
						r = ENSrcAnlzToknParseResult_Complete;
					} else {
						r = ENSrcAnlzToknParseResult_Partial;
					}
				}
				break;
			default:
				NBASSERT(FALSE) //Program logic error
				break;
		}
		state->charsCount++;
		state->isErr = (r == ENSrcAnlzToknParseResult_Error);
	}
	if(dstResult != NULL) *dstResult = r;
	return TRUE;
}


