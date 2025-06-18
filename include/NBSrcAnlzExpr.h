//
//  NBSrcAnlzExpr.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_EXPR_H
#define NB_SRCANLZ_EXPR_H

#include "nb/NBFrameworkDefs.h"

// Expression operator's sides mask

typedef enum ENSrcAnlzExprOpSideMask_ {
	ENSrcAnlzExprOpSideMask_None	= 0,
	ENSrcAnlzExprOpSideMask_Left	= 1,
	ENSrcAnlzExprOpSideMask_Right	= 2,
	ENSrcAnlzExprOpSideMask_Both	= 3
} ENSrcAnlzExprOpSideMask;

// Expression operator's priority

typedef struct STSrcAnlzExprOpPriority_ {
	SI32		uniqueId;
	const char*	name;
} STSrcAnlzExprOpPriority;

// Expression operator's definition

typedef struct STSrcAnlzExprOp_ {
	SI32			uniqueID;		//unique id
	char			chars[8];		// op string: '++', '-', '*', ...
	char			pushChars[8];	// op string that pushed one depth level (if depth level)
	char			popChars[8];	// op string that pops one depth level back (if depth level)
	//
	SI32			op1;			//Operation posibility 1 (the same chars can be multiple type or operations, ex: '++x' vs 'x++', 'x * y' vs '*ptr', ...)
	SI32			op2;			//Operation posibility 2 (the same chars can be multiple type or operations, ex: '++x' vs 'x++', 'x * y' vs '*ptr', ...)
} STSrcAnlzExprOp;

#define NBSRC_ANLZ_OP_CHARS_REQ_SIDES1(MAP, CHARSDEF, MASK) (CHARSDEF->op1 != -1 && (MAP->opSintaxs[CHARSDEF->op1].sidesMask & MASK))
#define NBSRC_ANLZ_OP_CHARS_REQ_SIDES2(MAP, CHARSDEF, MASK) (CHARSDEF->op2 != -1 && (MAP->opSintaxs[CHARSDEF->op2].sidesMask & MASK))

typedef struct STSrcAnlzExprOpSintxMap_ STSrcAnlzExprOpSintxMap;

typedef enum ENSrcAnlzExprOpSeqType_ {
	ENSrcAnlzExprOpSeqType_Unique = 0,		//The op defines a unique operator (normal)
	ENSrcAnlzExprOpSeqType_Extends,			//The op starts an extension of previous op (ex: params definition: ' funcName' + '(' + 'params' .... )
	ENSrcAnlzExprOpSeqType_Count
} ENSrcAnlzExprOpSeqType;

typedef struct STSrcAnlzExprOpSintx_ {
	SI32						uniqueID;		//unique Id
	ENSrcAnlzExprOpSideMask		sidesMask;		//Operands requiered mask (example: left < right, or !right, ~right, ++right, left++, ...)
	ENSrcAnlzExprOpSeqType		seqBehavior;	//Operands is unique or extends previous op?
	SI32						priority;		//sidesMask priority when evaluated
	const STSrcAnlzExprOpSintxMap*	pushMap;		//Sintax map to use when pushing one level depper with this char
	//Redundant data
	SI32						opChars;		//Op chars definition index
	SI32						opType;			//Op type '*OpBitw', '*OpLogic', '*OpArith', ...
	SI32						opEnumValue;	//Op value in type enumeration
	const char*					opName;			//Ref name
} STSrcAnlzExprOpSintx;

typedef struct STSrcAnlzExprOpSintxMap_ {
	const STSrcAnlzExprOp*		opChars;		//'STSrcAnlzExprOp' array
	SI32						opCharsSz;
	//
	const STSrcAnlzExprOpSintx*	opSintaxs;		//'STSrcAnlzExprOpSintx' array
	SI32						opSintaxsSz;
	//
	const SI32*					opsChar1;		//index of 1-char-len elements at 'opSintaxs'
	SI32						opsChar1Sz;
	const SI32*					opsChars2;		//index of 2-chars-len elements at 'opSintaxs'
	SI32						opsChars2Sz;
	const SI32*					opsChars3;		//index of 3-chars-len elements at 'opSintaxs'
	SI32						opsChars3Sz;
	const SI32*					opsCharsN;		//index of n-chars-len elements at 'opSintaxs'
	SI32						opsCharsNSz;
} STSrcAnlzExprOpSintxMap;

#ifdef __cplusplus
//}
#endif

#endif
