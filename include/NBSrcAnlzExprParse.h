//
//  NBSrcAnlzExprParse.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_EXPR_PARSE_H
#define NB_SRCANLZ_EXPR_PARSE_H

#include "nb/NBFrameworkDefs.h"
//
#include "NBSrcAnlzTokn.h"		//for 'STSrcAnlzTokns'
#include "NBSrcAnlzExpr.h"		//for 'STSrcAnlzExprOpSintxMap'

#ifdef __cplusplus
//extern "C" {
#endif

// 
// Parsing an expresion
//

typedef enum STSrcAnlzExprParseReadMode_ {
	STSrcAnlzExprParseReadMode_Level = 0,	//Reading one level and depper
	STSrcAnlzExprParseReadMode_Operand,		//Reading only one operator
	STSrcAnlzExprParseReadMode_Count
} STSrcAnlzExprParseReadMode;

typedef struct STSrcAnlzExprParseOp_ {
	SI32 iDef;		//Operator sintex index
	SI32 rCount;	//Ops at right that are one level depper (right operands of this operator)
} STSrcAnlzExprParseOp;

typedef struct STSrcAnlzExprParseNode_ {
	const STSrcAnlzExprOpSintxMap*	sintaxMap;	//sintax map used
	SI32						iCharDef;		//charDef index at sintax map (-1 for operands, a valid index for operators)
	union {
		STSrcAnlzExprParseOp	sintax;			//operator's sintaxDef index at map (when 'iCharDef != -1')
		SI32					iToken;			//operand token (when 'iCharDef == -1')
	};
} STSrcAnlzExprParseNode;

//
//Feed
//

BOOL SrcAnlzExprParser_feedTokns(const STSrcAnlzTokns* tokns, const STSrcAnlzExprOpSintxMap* map, const SI32 firstTokn, const SI32 toknsCount, STNBArray* dstOperations /*STSrcAnlzExprParseNode*/);
BOOL SrcAnlzExprParser_feedToknsPlain(const STSrcAnlzTokns* tokns, const STSrcAnlzExprOpSintxMap* map, SI32* firstTokn, const SI32 toknsCount, const STSrcAnlzExprParseReadMode readMode, const char* popLevelChars, const SI32 depthLevel, STNBArray* dstOperations /*STSrcAnlzExprParseNode*/);

#ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
BOOL SrcAnlzExprParser_dbgAddOpsToStr(const STSrcAnlzTokns* tokns, STNBString* dstStr, SI32* opIndex, const STSrcAnlzExprParseNode* ops, const SI32 opsCount, const BOOL noSpaces);
BOOL SrcAnlzExprParser_dbgAddOpsDefsToStr(const STSrcAnlzTokns* tokns, STNBString* dstStr, SI32* opIndex, const STSrcAnlzExprParseNode* ops, const SI32 opsCount, const BOOL noSpaces);
#endif


#ifdef __cplusplus
//}
#endif

#endif
