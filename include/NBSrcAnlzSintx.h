//
//  NBSrcAnlzSintx.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_SINTX_H
#define NB_SRCANLZ_SINTX_H

#ifdef __cplusplus
//extern "C" {
#endif

#include "nb/NBFrameworkDefs.h"
#include "NBSrcAnlzBase.h"	//for 'STSrcAnlzRange'

//Language sintax plain definition (for auto-generating the sintax def structures)
typedef struct STSrcAnlzSintxPlainDef_ {
	const char* name;		//def name
	const char* valPlain;	//parts sequence
} STSrcAnlzSintxPlainDef;

//Type of part of a language sintax definition
typedef enum ENSrzAnlzSintxPartType_ {
	ENSrzAnlzSintxPartType_Keyword = 0		//Keyword
	, ENSrzAnlzSintxPartType_Punctuator		//Punctuator
	, ENSrzAnlzSintxPartType_Identifier		//Identifier (or equivalent)
	, ENSrzAnlzSintxPartType_Constant		//Constant (number, char)
	, ENSrzAnlzSintxPartType_StrLiteral		//Literal string (usually between '"', in case of header-name between '<' and '>')
	, ENSrzAnlzSintxPartType_SintaxDef		//Other definition reference
	, ENSrzAnlzSintxPartType_Count
} ENSrzAnlzSintxPartType;

#define ENSrzAnlzSintxPartType_STR(TYPE) (\
    TYPE == ENSrzAnlzSintxPartType_Keyword ? "ENSrzAnlzSintxPartType_Keyword" : \
    TYPE == ENSrzAnlzSintxPartType_Punctuator ? "ENSrzAnlzSintxPartType_Punctuator" : \
    TYPE == ENSrzAnlzSintxPartType_Identifier ? "ENSrzAnlzSintxPartType_Identifier" : \
    TYPE == ENSrzAnlzSintxPartType_Constant ? "ENSrzAnlzSintxPartType_Constant" : \
    TYPE == ENSrzAnlzSintxPartType_StrLiteral ? "ENSrzAnlzSintxPartType_StrLiteral" : \
    TYPE == ENSrzAnlzSintxPartType_SintaxDef ? "ENSrzAnlzSintxPartType_SintaxDef" : \
    "ENSrzAnlzSintxPartType_unknown" )

//Part of a language sintax definition
typedef struct STSrcAnlzSintxPart_ {
	ENSrzAnlzSintxPartType	type;
	SI32					idx;
	/*BOOL operator == (const struct STSrcAnlzSintxPart_ &other) const { return	(type == other.type && idx == other.idx); }
	BOOL operator != (const struct STSrcAnlzSintxPart_ &other) const { return	(type != other.type || idx != other.idx); }
	BOOL operator < (const struct STSrcAnlzSintxPart_ &other) const { return	(type < other.type || (type == other.type && idx < other.idx)); }
	BOOL operator <= (const struct STSrcAnlzSintxPart_ &other) const { return	(type < other.type || (type == other.type && idx <= other.idx)); }
	BOOL operator > (const struct STSrcAnlzSintxPart_ &other) const { return	(type > other.type || (type == other.type && idx > other.idx)); }
	BOOL operator >= (const struct STSrcAnlzSintxPart_ &other) const { return	(type > other.type || (type == other.type && idx >= other.idx)); }*/
} STSrcAnlzSintxPart;

BOOL STSrcAnlzSintxPart_compare(const ENCompareMode mode, const void* data1, const void* data2, const UI32 dataSz);

//Language sintax definition
typedef struct STSrcAnlzSintx_ {
	const char*					name;			//printable name
	//Hints to quick determine if the keyword, punctuator, other sintaxDef, etc...
	//can be the first part of any sequence for this definition.
	const STSrcAnlzSintxPart*	deepHints;
	int							deepHintsSz;
} STSrcAnlzSintx;

//Language sintax-sequence definition
typedef struct STSrcAnlzSintxSeq_ {
	int							iDefEnum;
	//Strict parts definitions
	const STSrcAnlzSintxPart*	parts;
	int							partsSz;
	//Hints to quick determine if the keyword, punctuator, etc.
	//can be the first part of this tree sequence
	const STSrcAnlzSintxPart*	deepHints;
	int							deepHintsSz;
} STSrcAnlzSintxSeq;

//Language sintax-definition
typedef struct STSrcAnlzSintxDefs_ {
	//Keywords
	const char**				keyws;
	SI32						keywsSz;
	const STSrcAnlzRange*		keywsRngs;
	SI32						keywsRngsSz;
	//Punctuators
	const char**				puncts;
	SI32						punctsSz;
	const STSrcAnlzRange*		punctsRngs;
	SI32						punctsRngsSz;
	//Sintax defs
	const STSrcAnlzSintx*		defs;
	SI32						defsSz;
	//Sintax sequences
	const STSrcAnlzSintxSeq*	defsSeqs;
	SI32						defsSeqsSz;
	const STSrcAnlzRange*		defsSeqsRngs;
	SI32						defsSeqsRngsSz;
} STSrcAnlzSintxDefs;

//Search

int SrcAnlzSintx_searchKeyword(const char* str, const int strSz, const char** keywordsArr, const int keywordsArrLen, const STSrcAnlzRange* keywsRngsArr, const int keywsRngsArrLen);
int SrcAnlzSintx_searchPunctuator(const char* str, const int strSz, const char** punctsArr, const int punctsArrLen, const STSrcAnlzRange* punctsRngsArr, const int punctsRngsArrLen);

//Generating sintax def structures from plain-text definitions

void SrcAnlzSintx_genBasicStructuresFromPlainDefs(const STSrcAnlzSintxPlainDef* defsArr, const int defsArrLen, const char** keywordsArr, const int keywordsArrLen, const STSrcAnlzRange* keywsRngsArr, const int keywsRngsArrLen, const char** punctsArr, const int punctsArrLen, const STSrcAnlzRange* punctsRngsArr, const int punctsRngsArrLen);
void SrcAnlzSintx_genAdvancedStructuresFromBasicStructures(const STSrcAnlzSintxSeq* defsArr, const int defsArrLen, const STSrcAnlzSintx* enumDefs);

#ifdef __cplusplus
//}
#endif

#endif
