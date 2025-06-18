//
//  NBSourceTranslator.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef tool_sourcetranslator_NBSourceTranslator_h
#define tool_sourcetranslator_NBSourceTranslator_h

#include "AUFrameworkBaseStdAfx.h"
#include "AUFrameworkBase.h"

#ifdef __cplusplus
extern "C" {
#endif




//------------------------
//-- Translation parameters
//------------------------

//Actions on a file
typedef enum ENSrcAction_ {
	ENSrcAction_ignore = 0,
	ENSrcAction_copy,
	ENSrcAction_process,
	ENSrcAction_count
} ENSrcAction;

typedef enum ENSrcFileFormat_ {
	ENSrcFileFormat_plain,	//
	ENSrcFileFormat_c,		//
	ENSrcFileFormat_cpp,	//
	ENSrcFileFormat_objc,	//
	ENSrcFileFormat_objcpp,	//
	ENSrcFileFormat_php,	//
	ENSrcFileFormat_java,	//
	ENSrcFileFormat_mysql,	//
	ENSrcFileFormat_count
} ENSrcFileFormat;
	
//Specific configuration for an file-extension
typedef struct STSrcExtConfig_ {
	SI32			priorityWeight;
	const char*		extension;
	ENSrcFileFormat	fileFormat;
	ENSrcAction		action;
	UI32			optionsMask;
} STSrcExtConfig;

//Specific configuration for a path (file or folder)
typedef struct STSrcPathConfig_ {
	SI32		priorityWeight;
	const char* virtualPath;
	ENSrcAction	action;
	UI32		optionsMask;
} STSrcPathConfig;

//Translation task definition
typedef struct STSrcTraslationTask_ {
	const char* taskName;
	const char* srcFolder;
	const char* dstFolder;
	const char* pathPairsCsv;
	SI32		defaultPriorityWeight;
	ENSrcAction	defaultAction;
	UI32		defaultOptionsMask;
	//
	const STSrcExtConfig* extraExtsConfg;
	const SI32 extraExtsConfgCount;
	//
	const STSrcPathConfig* pathsConfg;
	const SI32 pathsConfgCount;
} STSrcTraslationTask;

//Translations main parameters definition
typedef struct STSrcTranslatorParams_ {
	const STSrcTraslationTask* transTasks;
	const SI32 transTasksCount;
	//
	const STSrcExtConfig* extsConfg;
	const SI32 extsConfgCount;
} STSrcTranslatorParams;





//------------------------
//-- Strings pairs
//------------------------

typedef enum ENStringRelevance_ {
	ENStringRelevance_none,			//Any text found
	ENStringRelevance_literal,		//Any literal string
	ENStringRelevance_comment,		//Any literal commentary
	ENStringRelevance_fileName,		//File name
	ENStringRelevance_folderName,	//Folder name
	ENStringRelevance_memberName,	//Class/struct member names
	ENStringRelevance_enumName,		//Enum names
	ENStringRelevance_structName,	//Struct names
	ENStringRelevance_className,	//Class names
	ENStringRelevance_macro,		//Macro or Constant
	ENStringRelevance_count
} ENStringRelevance;
	
typedef struct STItmCollectionStats_ {
	ENStringRelevance	maxRelevance;		//Word max relevance found
	int					timesFound;			//times found
	int					firstApprRefStart;	//first file, index of first char at 'firstsAppareancesRefs' string
	int					lastApprRefStart;	//last file, index of first char at 'firstsAppareancesRefs' string
} STItmCollectionStats;

void ItmCollectionStats_init(STItmCollectionStats* obj);
void ItmCollectionStats_finalize(STItmCollectionStats* obj);

typedef struct STStringPair_ {
	UI32				iOrig;		//Old string
	UI32				iNew;		//Replacement string (or zero when the old string must be preserved)
	bool				ignore;		//Ignore word
	bool				isPathWord;	//Is part of a path (filename or foldername)
	//Collection stats (ignored on load, filled during words collection action)
	STItmCollectionStats collStats;
} STStringPair;

//Hard-search index, word must be the same to match
typedef struct STStringSrchHardIndex_ {
	UI32			strStart;	//first char of this string
	AUCadenaLarga8*	strings;	//pool of strings
	UI32			indexPos;	//position at STStringPair array.
	bool operator==(const STStringSrchHardIndex_ &other) const { return (AUCadenaLarga8::cadenasSonIguales(&strings->str()[strStart], &other.strings->str()[other.strStart])); }
	bool operator!=(const STStringSrchHardIndex_ &other) const { return (!AUCadenaLarga8::cadenasSonIguales(&strings->str()[strStart], &other.strings->str()[other.strStart])); }
	bool operator<(const STStringSrchHardIndex_ &other) const { return (AUCadenaLarga8::esMenor(&strings->str()[strStart], &other.strings->str()[other.strStart])); }
	bool operator>(const STStringSrchHardIndex_ &other) const { return (AUCadenaLarga8::esMayor(&strings->str()[strStart], &other.strings->str()[other.strStart])); }
	bool operator<=(const STStringSrchHardIndex_ &other) const { return (AUCadenaLarga8::esMenorIgual(&strings->str()[strStart], &other.strings->str()[other.strStart])); }
	bool operator>=(const STStringSrchHardIndex_ &other) const { return (AUCadenaLarga8::esMayorIgual(&strings->str()[strStart], &other.strings->str()[other.strStart])); }
} STStringSrchHardIndex;
	
typedef struct STStringsPairs_ {
	AUArregloNativoMutableP<STStringPair>* pairs;	//Start of each string
	AUCadenaLargaMutable8* strings;					//All strings
	//Search optimization
	AUArregloNativoOrdenadoMutableP<STStringSrchHardIndex>* _srchIndexByOrig;
	//Storage for first and last appareances references
	int firstsAppareancesRefsLastRefStart;	//Optimization, save the last word, to avoid adding it several times
	int lastsAppareancesRefsLastRefStart;	//Optimization, save the last word, to avoid adding it several times
	AUCadenaLargaMutable8* firstsAppareancesRefs;
	AUCadenaLargaMutable8* lastsAppareancesRefs;
} STStringsPairs;

void StringsPairs_init(STStringsPairs* obj);
void StringsPairs_finalize(STStringsPairs* obj);
UI32 StringsPairs_add(STStringsPairs* obj, const char* str, const ENStringRelevance relevance, const char* curFile);
UI32 StringsPairs_addIfNew(STStringsPairs* obj, AUCadenaLarga8* strs, const UI32 iStrStart, const ENStringRelevance relevance, const char* curFile, bool incrementAppareance);
UI32 StringsPairs_indexOfOrig(const STStringsPairs* obj, AUCadenaLarga8* strs, const UI32 iStrStart);
//
bool StringsPairs_loadFromFileCsv(STStringsPairs* obj, const char* filePath);
bool StringsPairs_saveToFileCsv(STStringsPairs* obj, const char* filePath, bool includeCollStats);
void StringsPairs_saveToFileCsvRow(FILE* file, STStringsPairs* pairs, STStringPair* pair, bool includeCollStats, AUCadenaLargaMutable8* strTmp2);




//------------------------
//-- Strings library
//------------------------

typedef struct STStrLibIndex_ {
	SI32 iArray;
	SI32 iItem;
} STStrLibIndex;

typedef struct STStringsLib_ {
	STStringsPairs*	pairsArr;		//Pairs-lists array
	SI32			pairsArrSize;	//Pairs-lists array's size
	SI32			curPairsArr;	//Current pairs-list array been filled
} STStringsLib;
	
void StringLib_init(STStringsLib* obj);
void StringLib_finalize(STStringsLib* obj);
UI32 StringLib_openNewList(STStringsLib* obj);
void StringLib_setFillingList(STStringsLib* obj, const SI32 listIndex);
UI32 StringLib_size(STStringsLib* obj);
STStrLibIndex StringLib_add(STStringsLib* obj, const char* str, const ENStringRelevance relevance, const char* curFile);
STStrLibIndex StringLib_addIfNew(STStringsLib* obj, AUCadenaLarga8* strs, const UI32 iStrStart, const ENStringRelevance relevance, const char* curFile, bool incrementAppareance);
STStrLibIndex StringLib_addOrUpdate(STStringsLib* obj, AUCadenaLarga8* strs, const UI32 iStrStart, const ENStringRelevance relevance, const char* curFile, bool incrementAppareance);
STStrLibIndex StringLib_indexOfOrig(const STStringsLib* obj, AUCadenaLarga8* strs, const UI32 iStrStart);
UI32 StringLib_loadMissingTranslationsFromPairsCsv(STStringsLib* obj, const char* filePath);
void StringLib_findDuplicatedTranslations(STStringsLib* obj, AUArregloNativoMutableP<STStrLibIndex>* dst, bool onlyExplicitTranslations);



//------------------------
//-- Translated string
//------------------------

typedef enum ENTranslatedStrType_ {
	ENTranslatedStrType_Code = 0,			// content
	ENTranslatedStrType_LiteralSingle,		// 'content'
	ENTranslatedStrType_LiteralDouble,		// "content"
	ENTranslatedStrType_CommentMonoLine,	// //content
	ENTranslatedStrType_CommentMultiLine	// /*content*/
} ENTranslatedStrType;
	
typedef struct STTranslatedString_ {
	ENTranslatedStrType origType;	//type of the original string
	UI32	iOrig;					//start of original string
	UI32	iFinal;					//start of translated string
} STTranslatedString;

//------------------------
//-- Translating line (strings sequence)
//-- (words and separators on a line, literal text, or comment)
//------------------------

typedef struct STTranslatedLine_ {
	AUArregloNativoMutableP<STTranslatedString>* strSeq;
} STTranslatedLine;

void TranslatedLine_init(STTranslatedLine* obj);
void TranslatedLine_finalize(STTranslatedLine* obj);
void TranslatedLine_addStrSeq(STTranslatedLine* obj, AUCadenaLargaMutable8* strsStorage, const char* strOrig, const ENTranslatedStrType origType);
void TranslatedLine_updateStrSeqFinal(STTranslatedLine* obj, AUCadenaLargaMutable8* strsStorage, const UI32 strIndex, const char* strFinal);
UI32 TranslatedLine_flushToFile(STTranslatedLine* obj, AUCadenaLargaMutable8* strsStorage, FILE* file);

//------------------------
//-- Actions stats
//------------------------

typedef struct STTranslStat_ {
	UI32 countFiles;			//All files
	UI32 countFilesIgnore;		//Files ignored (or to be ignored)
	UI32 countFilesCopy;		//Files copied (or to be copied)
	UI32 countFilesProcess;		//Files processed (or to be processed)
	//
	UI32 countStrs;				//Chars sequences found
	UI32 countStrsNewFound;		//Chars sequences found fot he first time
	UI32 countStrsTranslated;	//Chars sequences changed for a translation
	//
	UI64 countBytesRead;		//Bytes read.
	UI64 countBytesWritten;		//Bytes written.
	//
	UI32 countLinesRead;		//Lines read.
	UI32 countLinesWritten;		//Lines written.
} STTranslStat;

void TranslStat_init(STTranslStat* obj);
void TranslStat_restart(STTranslStat* obj);
void TranslStat_add(STTranslStat* obj, const STTranslStat* toAdd);

//------------------------
//-- Extension acumulated stats
//------------------------

typedef struct STTranslStatGrp_ {
	char*			grpName;
	STTranslStat	stats;
	//
	bool operator==(const STTranslStatGrp_ &other) const { return (AUCadenaLarga8::cadenasSonIguales(grpName, other.grpName)); }
	bool operator!=(const STTranslStatGrp_ &other) const { return (!AUCadenaLarga8::cadenasSonIguales(grpName, other.grpName)); }
	bool operator<(const STTranslStatGrp_ &other) const { return (AUCadenaLarga8::esMenor(grpName, other.grpName)); }
	bool operator>(const STTranslStatGrp_ &other) const { return (AUCadenaLarga8::esMayor(grpName, other.grpName)); }
	bool operator<=(const STTranslStatGrp_ &other) const { return (AUCadenaLarga8::esMenorIgual(grpName, other.grpName)); }
	bool operator>=(const STTranslStatGrp_ &other) const { return (AUCadenaLarga8::esMayorIgual(grpName, other.grpName)); }
} STTranslStatGrp;
	
void TranslStatGrp_init(STTranslStatGrp* obj, const char* grpName);
void TranslStatGrp_finalize(STTranslStatGrp* obj);





//------------------------
//-- Grouped stats (usefull for configuration analysis)
//------------------------

typedef struct STTranslStatsGrouped_ {
	AUArregloNativoOrdenadoMutableP<STTranslStatGrp>*	byExt;
	AUArregloNativoOrdenadoMutableP<STTranslStatGrp>*	byFolder;
	AUArregloNativoOrdenadoMutableP<STTranslStatGrp>*	byFile;
} STTranslStatsGrouped;

void TranslStatsGrouped_init(STTranslStatsGrouped* obj);
void TranslStatsGrouped_finalize(STTranslStatsGrouped* obj);
void TranslStatsGrouped_add(STTranslStatsGrouped* obj, const STTranslStatsGrouped* toAdd);
void TranslStatsGrouped_printf(STTranslStatsGrouped* obj);





//------------------------
//-- Grouped stats per task (usefull for configuration analysis)
//------------------------
	
typedef struct STTranslStatsPerTask_ {
	AUArregloNativoMutableP<STTranslStatsGrouped>*	grpdStatsPerTask;
} STTranslStatsPerTask;

void TranslStatsPerTask_init(STTranslStatsPerTask* obj);
void TranslStatsPerTask_empty(STTranslStatsPerTask* obj);
void TranslStatsPerTask_finalize(STTranslStatsPerTask* obj);





//------------------------
//-- File parser
//------------------------

#define PARSER_BUFFER_SIZE				4096

//Translate actions
#define TRANSLATE_ACTION_NONE			0
#define TRANSLATE_ACTION_COLLECT_WORDS	1
#define TRANSLATE_ACTION_TRANSLATE		2	//translate all strings-sequences

	
//Translate options
#define TRANSLATE_OPT_NONE							0
#define TRANSLATE_OPT_COLLECT_LITERALS_SINGLE		1
#define TRANSLATE_OPT_COLLECT_LITERALS_DOUBLE		2
#define TRANSLATE_OPT_COLLECT_LITERALS (TRANSLATE_OPT_COLLECT_LITERALS_SINGLE | TRANSLATE_OPT_COLLECT_LITERALS_DOUBLE)
//
#define TRANSLATE_OPT_COLLECT_COMMENTS_C_MONOLINE	4
#define TRANSLATE_OPT_COLLECT_COMMENTS_C_MULTILINE	8
#define TRANSLATE_OPT_COLLECT_COMMENTS_C (TRANSLATE_OPT_COLLECT_COMMENTS_C_ONELINE | TRANSLATE_OPT_COLLECT_COMMENTS_C_MULTILINE)
//
#define TRANSLATE_OPT_TRANSLATE_ONLY_PATHS			16	//Only translates paths
#define TRANSLATE_OPT_TRANSLATE_OUTPUT_FILENAME		32	//Create new files using the translated filename (preserving the extension)
#define TRANSLATE_OPT_TRANSLATE_STRS_IN_LITERALS	64	//Translate all words inside literals.
#define TRANSLATE_OPT_TRANSLATE_PATHS_IN_LITERALS	128	//Translate only file/folder names inside literals.
#define TRANSLATE_OPT_TRANSLATE_STRS_IN_COMMENTS	256	//Translate all words inside commentaries.
#define TRANSLATE_OPT_TRANSLATE_PATHS_IN_COMMENTS	512	//Translate only file/folder names inside commentaries.

typedef struct STParsingFileState_ {
	const char* curFileName;
	char		charCurrnt;
	char		charBefore;
	bool		literalCharScaped;	//Determine if the '/' was consumed in cases like "Hi \"my\" friend", to avoid missparsing on cases like "\\".
	//
	ENTranslatedStrType		readingBlqType;
	SI32					readingBlqTypeDefIndex;			//If reading monoline or multiline comment, the definition index
	AUCadenaLargaMutable8*	readingLastValidWord;
	AUCadenaLargaMutable8*	readingStrAcum;
	//Optional, storage for this file in memory
	//(not necesary for just collecting words)
	AUArregloNativoMutableP<STTranslatedLine>* linesSeq;	//optional, can be null
	AUCadenaLargaMutable8* linesStorage;					//strings buffer
	//Optional, file stats
	//(not necesary)
	STTranslStat*	stats;
} STParsingFileState;

void ParsingFileState_init(STParsingFileState* obj, const char* curFileName, bool initLinesBuffers, bool createStatsRecord);
void ParsingFileState_finalize(STParsingFileState* obj);
STTranslatedLine* ParsingFileState_lastLine(STParsingFileState* obj);
STTranslatedLine* ParsingFileState_addNewLine(STParsingFileState* obj);
bool ParsingFileState_flushToFile(STParsingFileState* obj, FILE* fileDst);
//
void Parser_init();
void Parser_finalize();
//
bool Parser_loadPairsFromCSVs(const STSrcTranslatorParams* params, STStringsLib* strlib);
bool Parser_savePairsToCSVs(const STSrcTranslatorParams* params, STStringsLib* strlib, bool includeCollStats);
UI32 Parser_applyIgnoresFromXml(STStringsLib* strlib, const char* filepathXml);
void Parser_printfGroupedStatsPerTask(const STSrcTranslatorParams* params, STTranslStatsPerTask* grpStatsPerTask);
//
STTranslStat Parser_processTasks(const STSrcTranslatorParams* params, STStringsLib* strlib, const SI32 parsingActionsMask, STTranslStatsGrouped* grpdStats, STTranslStatsPerTask* grpStatsPerTask);
STTranslStat Parser_processFolder(const STSrcTranslatorParams* params, STStringsLib* strlib, const SI32 parsingActionsMask, STTranslStatsGrouped* grpdStats, const STSrcTraslationTask* task, const char* virtualPathOrg, const char* virtualPathDst, const SI32 defaultPriorityWeight, const ENSrcAction defaultAction, const UI32 defaultOptionsMask);
bool Parser_processFileParseStringsSeqs(STParsingFileState* fileState, STStringsLib* strlib, const ENSrcFileFormat formatType, const SI32 parsingActionsMask, const UI32 parsingOptionsMask, const char* filePath);
bool Parser_processFileCopy(const char* filePath, FILE* fileDst);

//
bool Parser_processStringsSeqsInit(STParsingFileState* fileState, STStringsLib* strlib, const SI32 parsingActionsMask, const UI32 parsingOptionsMask);
bool Parser_processStringsSeqsFromBuffer(STParsingFileState* fileState, STStringsLib* strlib, const ENSrcFileFormat formatType, const SI32 parsingActionsMask, const UI32 parsingOptionsMask, const BYTE* buffer, const UI32 bufferInitialPos, const UI32 bufferBytesData, const bool processAllAsCode);
bool Parser_processStringsMustIgnoreSequence(const char* strArr, const UI32 strSize);
void Parser_processStringsFlush(STParsingFileState* fileState, STStringsLib* strlib, const ENSrcFileFormat formatType, const SI32 parsingActionsMask, STTranslatedLine* dstLine, const ENTranslatedStrType blqType, const ENStringRelevance newRelevance, AUCadenaLargaMutable8* lastValidWord, AUCadenaLargaMutable8* strAcum, const UI32 parsingOptionsMask);
bool Parser_processStringsSeqsFinish(STParsingFileState* fileState, STStringsLib* strlib, const ENSrcFileFormat formatType, const SI32 parsingActionsMask, const UI32 parsingOptionsMask);
UI32 Parser_processStringsTranslate(STParsingFileState* fileState, STStringsLib* strlib, const ENSrcFileFormat formatType, const UI32 parsingOptionsMask);
UI32 Parser_processStringsTranslateSubBlock(STParsingFileState* fileState, STStringsLib* strlib, const bool onlyPaths);



#ifdef __cplusplus
}
#endif

#endif
