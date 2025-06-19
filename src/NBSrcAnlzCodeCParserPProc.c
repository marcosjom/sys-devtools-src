//
//  NBSrcAnlzCodeCParserPProc.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzCodeCParserPProc.h"
#include "nb/core/NBNumParser.h"
#include <stdio.h>						//for FILE

//
//MACRO's execution params SINTAX def
// Ex: macro(x, y, (a,b,c))
// Param1: x
// Param2: y
// Param3: (a, b, c)
//

typedef enum ENSrcAnlzOpMacroParamChars_ {
	ENSrcAnlzOpMacroParamChars_GrpOpen = 0,	// '('
	ENSrcAnlzOpMacroParamChars_GrpClose,	// ')'
	ENSrcAnlzOpMacroParamChars_Comma,		// ','
	ENSrcAnlzOpMacroParamChars_Count
} ENSrcAnlzOpMacroParamChars;

typedef enum ENSrcAnlzOpMacroParamSintax_ {
	ENSrcAnlzOpMacroParamSintax_GrpOpen = 0,	// '('
	ENSrcAnlzOpMacroParamSintax_GrpClose,		// ')'
	ENSrcAnlzOpMacroParamSintax_Comma,			// ','
	ENSrcAnlzOpMacroParamSintax_Count
} ENSrcAnlzOpMacroParamSintax;

static const SI32 __globalOpMacroParamsSintaxChar1[] = {
	ENSrcAnlzOpMacroParamSintax_GrpOpen
	, ENSrcAnlzOpMacroParamSintax_GrpClose
	, ENSrcAnlzOpMacroParamSintax_Comma
};

static const STSrcAnlzExprOp __globalOpMacroParamsDefs[] = {
	//One char
	{ ENSrcAnlzOpMacroParamChars_GrpOpen,		{'(', '\0'}, {'\0', '\0'},	{')', '\0'},	ENSrcAnlzOpMacroParamSintax_GrpOpen, -1 }
	, { ENSrcAnlzOpMacroParamChars_GrpClose,	{')', '\0'}, {'(', '\0'},	{'\0', '\0'},	ENSrcAnlzOpMacroParamSintax_GrpClose, -1 }
	, { ENSrcAnlzOpMacroParamChars_Comma,		{',', '\0'}, {'\0', '\0'},	{'\0', '\0'},	ENSrcAnlzOpMacroParamSintax_Comma, -1 }
	//Others
};

static const STSrcAnlzExprOpSintx __globalOpMacroParamsSintax[] = {
	{ ENSrcAnlzOpMacroParamSintax_GrpOpen,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,	ENSrcAnlzSintxCOp_GrpOpen, ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpOpen, "ProgGrpOpen" }		// '('
	, { ENSrcAnlzOpMacroParamSintax_GrpClose,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,	ENSrcAnlzSintxCOp_GrpClose, ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpClose, "ProgGrpClose" }	// ')'
	, { ENSrcAnlzOpMacroParamSintax_Comma,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Comma, NULL,		ENSrcAnlzSintxCOp_IdxClose, ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_Comma, "ProgComma" }			// ','
};

const STSrcAnlzExprOpSintxMap __globalPProcMacroParamsSintaxMap = {
	//OpChars
	__globalOpMacroParamsDefs, (sizeof(__globalOpMacroParamsDefs) / sizeof(__globalOpMacroParamsDefs[0])) //ENSrcAnlzOpMacroParamChars_Count
	//OpSintaxs
	, __globalOpMacroParamsSintax, (sizeof(__globalOpMacroParamsSintax) / sizeof(__globalOpMacroParamsSintax[0])) //ENSrcAnlzOpMacroParamSintax_Count
	//Index by op lenghts
	, __globalOpMacroParamsSintaxChar1, (sizeof(__globalOpMacroParamsSintaxChar1) / sizeof(__globalOpMacroParamsSintaxChar1[0]))
	, NULL, 0
	, NULL, 0
	, NULL, 0
};

//
//Preprocesor expresions SINTAX def
//Ex: [#if/elif] (x > z) && (!x == ~x) || (defined(MY) && NEVER || defined OTHER)
//

typedef enum ENSrcAnlzOpPProcCondChars_ {
	//One char
	ENSrcAnlzOpPProcCondChars_GrpOpen = 0,	// '('
	ENSrcAnlzOpPProcCondChars_GrpClose,		// ')'
	ENSrcAnlzOpPProcCondChars_Exclamation,	// '!'
	ENSrcAnlzOpPProcCondChars_Ampersand,	// '&'
	ENSrcAnlzOpPProcCondChars_VertBar,		// '|'
	ENSrcAnlzOpPProcCondChars_GreatThan,	// '>'
	ENSrcAnlzOpPProcCondChars_LessThan,		// '<'
	ENSrcAnlzOpPProcCondChars_Plus,			// '+'
	ENSrcAnlzOpPProcCondChars_Minus,		// '-'
	ENSrcAnlzOpPProcCondChars_Asterisk,		// '*'
	ENSrcAnlzOpPProcCondChars_Slash,		// '/'
	ENSrcAnlzOpPProcCondChars_Percent,		// '%'
	ENSrcAnlzOpPProcCondChars_Tilde,		// '~'
	ENSrcAnlzOpPProcCondChars_Caret,		// '^'
	//Two chars
	ENSrcAnlzOpPProcCondChars_Ampersand2,	// '&&'
	ENSrcAnlzOpPProcCondChars_VertBar2,		// '||'
	ENSrcAnlzOpPProcCondChars_Equals2,		// '=='
	ENSrcAnlzOpPProcCondChars_Inequal,		// '!='
	ENSrcAnlzOpPProcCondChars_GreatEqual,	// '>='
	ENSrcAnlzOpPProcCondChars_LessEqual,	// '<='
	ENSrcAnlzOpPProcCondChars_GreatThan2,	// '>>'
	ENSrcAnlzOpPProcCondChars_LessThan2,	// '<<'
	//Others
	ENSrcAnlzOpPProcCondChars_Defined,		// 'defined'
	//
	ENSrcAnlzOpPProcCondChars_Count
} ENSrcAnlzOpPProcCondChars;

typedef enum ENSrcAnlzOpPProcCondSintax_ {
	ENSrcAnlzOpPProcCondSintax_Logic_And = 0,	// '&&'
	ENSrcAnlzOpPProcCondSintax_Logic_Or,		// '||'
	ENSrcAnlzOpPProcCondSintax_Logic_Neg,		// '!'
	//
	ENSrcAnlzOpPProcCondSintax_Relat_Equal,		// '=='
	ENSrcAnlzOpPProcCondSintax_Relat_Ineq,		// '!='
	ENSrcAnlzOpPProcCondSintax_Relat_Great,		// '>'
	ENSrcAnlzOpPProcCondSintax_Relat_Lower,		// '<'
	ENSrcAnlzOpPProcCondSintax_Relat_GrtEq,		// '>='
	ENSrcAnlzOpPProcCondSintax_Relat_LowEq,		// '<='
	//
	ENSrcAnlzOpPProcCondSintax_Arith_Add,		// '+'
	ENSrcAnlzOpPProcCondSintax_Arith_SignPos,	// '+'
	ENSrcAnlzOpPProcCondSintax_Arith_Sub,		// '-'
	ENSrcAnlzOpPProcCondSintax_Arith_SignNeg,	// '-'
	ENSrcAnlzOpPProcCondSintax_Arith_Mul,		// '*'
	ENSrcAnlzOpPProcCondSintax_Arith_Div,		// '/'
	ENSrcAnlzOpPProcCondSintax_Arith_Mod,		// '%'
	//
	ENSrcAnlzOpPProcCondSintax_Bitw_And,		// '&'
	ENSrcAnlzOpPProcCondSintax_Bitw_Or,			// '|'
	ENSrcAnlzOpPProcCondSintax_Bitw_Xor,		// '^' //binary complement (only activate bits activated in both operands)
	ENSrcAnlzOpPProcCondSintax_Bitw_Comp,		// '~' //binary complement (flipping bits values)
	ENSrcAnlzOpPProcCondSintax_Bitw_Right,		// '>>'
	ENSrcAnlzOpPProcCondSintax_Bitw_Left,		// '<<'
	//
	ENSrcAnlzOpPProcCondSintax_Prog_ParamOpen,	// '(', order is important (paramOpen must be first)
	ENSrcAnlzOpPProcCondSintax_Prog_ParamClose,	// ')'
	ENSrcAnlzOpPProcCondSintax_Prog_GrpOpen,	// '('
	ENSrcAnlzOpPProcCondSintax_Prog_GrpClose,	// ')'
	//
	ENSrcAnlzOpPProcCondSintax_Preprc_Def,		// 'defined'
	//
	ENSrcAnlzOpPProcCondSintax_Count
} ENSrcAnlzOpPProcCondSintax;

static const SI32 __globalOpPProcCondSintaxChar1[] = {
	ENSrcAnlzOpPProcCondChars_GrpOpen			// '('
	, ENSrcAnlzOpPProcCondChars_GrpClose		// ')'
	, ENSrcAnlzOpPProcCondChars_Exclamation		// '!'
	, ENSrcAnlzOpPProcCondChars_GreatThan		// '>'
	, ENSrcAnlzOpPProcCondChars_LessThan		// '<'
	, ENSrcAnlzOpPProcCondChars_Plus			// '+'
	, ENSrcAnlzOpPProcCondChars_Minus			// '-'
	, ENSrcAnlzOpPProcCondChars_Asterisk		// '*'
	, ENSrcAnlzOpPProcCondChars_Slash			// '/'
	, ENSrcAnlzOpPProcCondChars_Percent			// '%'
	, ENSrcAnlzOpPProcCondChars_Ampersand		// '&'
	, ENSrcAnlzOpPProcCondChars_VertBar			// '|'
	, ENSrcAnlzOpPProcCondChars_Caret			// '^'
	, ENSrcAnlzOpPProcCondChars_Tilde			// '~'
};

static const SI32 __globalOpPProcCondSintaxChar2[] = {
	ENSrcAnlzOpPProcCondChars_Ampersand2		// '&&'
	, ENSrcAnlzOpPProcCondChars_VertBar2		// '||'
	, ENSrcAnlzOpPProcCondChars_Equals2			// '!='
	, ENSrcAnlzOpPProcCondChars_Inequal			// '=='
	, ENSrcAnlzOpPProcCondChars_GreatEqual		// '>='
	, ENSrcAnlzOpPProcCondChars_LessEqual		// '<='
	, ENSrcAnlzOpPProcCondChars_GreatThan2		// '>>'
	, ENSrcAnlzOpPProcCondChars_LessThan2		// '<<'
};

static const SI32 __globalOpPProcCondSintaxCharN[] = {
	ENSrcAnlzOpPProcCondChars_Defined			// 'defined'
};

static const STSrcAnlzExprOp __globalOpMacroCondDefs[] = {
	//One char
	{ ENSrcAnlzOpPProcCondChars_GrpOpen,		{'(', '\0'}, {'\0', '\0'}, {')', '\0'}, ENSrcAnlzOpPProcCondSintax_Prog_ParamOpen, ENSrcAnlzOpPProcCondSintax_Prog_GrpOpen } //, order is important (paramOpen must be first)
	, { ENSrcAnlzOpPProcCondChars_GrpClose,		{')', '\0'}, {'(', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Prog_ParamClose, ENSrcAnlzOpPProcCondSintax_Prog_GrpClose }
	, { ENSrcAnlzOpPProcCondChars_Exclamation,	{'!', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Logic_Neg, -1 }
	, { ENSrcAnlzOpPProcCondChars_Ampersand,	{'&', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Bitw_And, -1 }
	, { ENSrcAnlzOpPProcCondChars_VertBar,		{'|', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Bitw_Or, -1 }
	, { ENSrcAnlzOpPProcCondChars_GreatThan,	{'>', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Relat_Great, -1 }
	, { ENSrcAnlzOpPProcCondChars_LessThan,		{'<', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Relat_Lower, -1 }
	, { ENSrcAnlzOpPProcCondChars_Plus,			{'+', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Arith_Add, ENSrcAnlzOpPProcCondSintax_Arith_SignPos }
	, { ENSrcAnlzOpPProcCondChars_Minus,		{'-', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Arith_Sub, ENSrcAnlzOpPProcCondSintax_Arith_SignNeg }
	, { ENSrcAnlzOpPProcCondChars_Asterisk,		{'*', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Arith_Mul, -1 }
	, { ENSrcAnlzOpPProcCondChars_Slash,		{'/', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Arith_Div, -1 }
	, { ENSrcAnlzOpPProcCondChars_Percent,		{'%', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Arith_Mod, -1 }
	, { ENSrcAnlzOpPProcCondChars_Tilde,		{'~', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Bitw_Comp, -1 }
	, { ENSrcAnlzOpPProcCondChars_Caret,		{'^', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Bitw_Xor, -1 }
	//Two chars
	, { ENSrcAnlzOpPProcCondChars_Ampersand2,	{'&', '&', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Logic_And, -1 }
	, { ENSrcAnlzOpPProcCondChars_VertBar2,		{'|', '|', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Logic_Or, -1 }
	, { ENSrcAnlzOpPProcCondChars_Equals2,		{'=', '=', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Relat_Equal, -1 }
	, { ENSrcAnlzOpPProcCondChars_Inequal,		{'!', '=', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Relat_Ineq, -1 }
	, { ENSrcAnlzOpPProcCondChars_GreatEqual,	{'>', '=', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Relat_GrtEq, -1 }
	, { ENSrcAnlzOpPProcCondChars_LessEqual,	{'<', '=', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Relat_LowEq, -1 }
	, { ENSrcAnlzOpPProcCondChars_GreatThan2,	{'>', '>', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Bitw_Right, -1 }
	, { ENSrcAnlzOpPProcCondChars_LessThan2,	{'<', '<', '\0'}, {'\0', '\0'}, {'\0', '\0'}, ENSrcAnlzOpPProcCondSintax_Bitw_Left, -1 }
	//Others
	, { ENSrcAnlzOpPProcCondChars_Defined,		{'d', 'e', 'f', 'i', 'n', 'e', 'd', '\0'}, {'\0'}, {'\0'}, ENSrcAnlzOpPProcCondSintax_Preprc_Def, -1 }
};

static const STSrcAnlzExprOpSintx __globalOpPProcCondSintax[] = {
	{ ENSrcAnlzOpPProcCondSintax_Logic_And,			ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_And, NULL,		ENSrcAnlzSintxCOp_Ampersand2,	ENSrcAnlzSintxCOpType_Logical, ENSrcAnlzSintxCOpLogic_And, "LogicAnd" }			// '&&'
	, { ENSrcAnlzOpPProcCondSintax_Logic_Or,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Or, NULL,		ENSrcAnlzSintxCOp_VertBar2,		ENSrcAnlzSintxCOpType_Logical, ENSrcAnlzSintxCOpLogic_Or, "LogicOr" }			// '||'
	, { ENSrcAnlzOpPProcCondSintax_Logic_Neg,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,	ENSrcAnlzSintxCOp_Exclamation,	ENSrcAnlzSintxCOpType_Logical, ENSrcAnlzSintxCOpLogic_Neg, "Neg" }				// '!'
	//
	, { ENSrcAnlzOpPProcCondSintax_Relat_Equal,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Equal, NULL,		ENSrcAnlzSintxCOp_Equals2,		ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_Equal, "RelatEqual" }		// '=='
	, { ENSrcAnlzOpPProcCondSintax_Relat_Ineq,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Equal, NULL,		ENSrcAnlzSintxCOp_Inequal,		ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_Ineq, "RelatIneq" }		// '!='
	, { ENSrcAnlzOpPProcCondSintax_Relat_Great,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Relat, NULL,		ENSrcAnlzSintxCOp_GreatThan,	ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_Great, "RelatGreat" }		// '>'
	, { ENSrcAnlzOpPProcCondSintax_Relat_Lower,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Relat, NULL,		ENSrcAnlzSintxCOp_LessThan,		ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_Lower, "RelatLower" }		// '<'
	, { ENSrcAnlzOpPProcCondSintax_Relat_GrtEq,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Relat, NULL,		ENSrcAnlzSintxCOp_GreatEqual,	ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_GrtEq, "RelatGrtEq" }		// '>='
	, { ENSrcAnlzOpPProcCondSintax_Relat_LowEq,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Relat, NULL,		ENSrcAnlzSintxCOp_LessEqual,	ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_LowEq, "RelatLowEq" }		// '<='
	//
	, { ENSrcAnlzOpPProcCondSintax_Arith_Add,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Add, NULL,		ENSrcAnlzSintxCOp_Plus,			ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Add, "ArithAdd" }			// '+'
	, { ENSrcAnlzOpPProcCondSintax_Arith_SignPos,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,	ENSrcAnlzSintxCOp_Plus,			ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Add, "ArithSignPos" }		// '+'
	, { ENSrcAnlzOpPProcCondSintax_Arith_Sub,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Add, NULL,		ENSrcAnlzSintxCOp_Minus,		ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Sub, "ArithSub" }			// '-'
	, { ENSrcAnlzOpPProcCondSintax_Arith_SignNeg,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,	ENSrcAnlzSintxCOp_Minus,		ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Sub, "ArithSignNeg" }		// '-'
	, { ENSrcAnlzOpPProcCondSintax_Arith_Mul,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Mult, NULL,		ENSrcAnlzSintxCOp_Asterisk,		ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Mul, "ArithMul"}			// '*'
	, { ENSrcAnlzOpPProcCondSintax_Arith_Div,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Mult, NULL,		ENSrcAnlzSintxCOp_Slash,		ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Div, "ArithDiv" }			// '/'
	, { ENSrcAnlzOpPProcCondSintax_Arith_Mod,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Mult, NULL,		ENSrcAnlzSintxCOp_Percent,		ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Mod, "ArithMod" }			// '%'
	//
	, { ENSrcAnlzOpPProcCondSintax_Bitw_And,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_BAnd, NULL,		ENSrcAnlzSintxCOp_Ampersand,	ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_And, "BitwAnd" }			// '&'
	, { ENSrcAnlzOpPProcCondSintax_Bitw_Or,			ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_BOr, NULL,		ENSrcAnlzSintxCOp_VertBar,		ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_Or, "BitwOr" }				// '|'
	, { ENSrcAnlzOpPProcCondSintax_Bitw_Xor,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_BXor, NULL,		ENSrcAnlzSintxCOp_Caret,		ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_Xor, "BitwXor" }			// '^'
	, { ENSrcAnlzOpPProcCondSintax_Bitw_Comp,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,	ENSrcAnlzSintxCOp_Tilde,		ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_Comp, "BitwComp" }			// '~'
	, { ENSrcAnlzOpPProcCondSintax_Bitw_Right,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Shift, NULL,		ENSrcAnlzSintxCOp_GreatThan2,	ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_Right, "BitwRight" }		// '>>'
	, { ENSrcAnlzOpPProcCondSintax_Bitw_Left,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Shift, NULL,		ENSrcAnlzSintxCOp_LessThan2,	ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_Left, "BitwLeft" }			// '<<'
	//
	, { ENSrcAnlzOpPProcCondSintax_Prog_ParamOpen,	ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Extends, ENSrcAnlzSintxCOpPriority_Postf, &__globalPProcMacroParamsSintaxMap, ENSrcAnlzSintxCOp_GrpOpen,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpOpen, "ProgGrpOpen" }	// '(', order is important (paramOpen must be first)
	, { ENSrcAnlzOpPProcCondSintax_Prog_ParamClose,	ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_GrpClose,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpClose, "ProgGrpClose" }	// ')'
	, { ENSrcAnlzOpPProcCondSintax_Prog_GrpOpen,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,	ENSrcAnlzSintxCOp_GrpOpen,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpOpen, "ProgGrpOpen" }		// '(', order is important (paramOpen must be first)
	, { ENSrcAnlzOpPProcCondSintax_Prog_GrpClose,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,	ENSrcAnlzSintxCOp_GrpClose,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpClose, "ProgGrpClose" }	// ')'
	//
	, { ENSrcAnlzOpPProcCondSintax_Preprc_Def,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,	ENSrcAnlzSintxCOp_Defined,		ENSrcAnlzSintxCOpType_Preproc, ENSrcAnlzSintxCOpPProc_def, "PreprcDef" }		// 'defined'
};

//Extern defined in the header
const STSrcAnlzExprOpSintxMap __globalPProcCondExprSintaxMap = {
	//OpChars
	__globalOpMacroCondDefs, (sizeof(__globalOpMacroCondDefs) / sizeof(__globalOpMacroCondDefs[0])) //ENSrcAnlzOpPProcCondChars_Count
	//OpSintaxs
	, __globalOpPProcCondSintax, (sizeof(__globalOpPProcCondSintax) / sizeof(__globalOpPProcCondSintax[0])) //ENSrcAnlzOpPProcCondSintax_Count
	//Index by op lenghts
	, __globalOpPProcCondSintaxChar1, (sizeof(__globalOpPProcCondSintaxChar1) / sizeof(__globalOpPProcCondSintaxChar1[0]))
	, __globalOpPProcCondSintaxChar2, (sizeof(__globalOpPProcCondSintaxChar2) / sizeof(__globalOpPProcCondSintaxChar2[0]))
	, NULL , 0
	, __globalOpPProcCondSintaxCharN, (sizeof(__globalOpPProcCondSintaxCharN) / sizeof(__globalOpPProcCondSintaxCharN[0]))
};

//
//Feed a complete preprocessor's directive
//

BOOL SrcAnlzCodeCParser_pprocFeedDirective(STSrcAnlzCodeCParser* state){
	BOOL r = FALSE;
	NBASSERT(state->pprocCondPortionsStack.use > 0) //at least root state and one more
	const STSrcAnlzCodeCondPortionState curBlkState = NBArray_itmValueAtIndex(&state->pprocCondPortionsStack, STSrcAnlzCodeCondPortionState, state->pprocCondPortionsStack.use - 1);
	//PARSE PREPROCESSOR DIRECTIVE
	const char* tokensBuff = state->acumTokensCur->strBuff.str;
	const SI32 tokensCount = state->acumTokensCur->defs.use;
	const STSrcAnlzTokn* tokensDefs = (const STSrcAnlzTokn*)state->acumTokensCur->defs._buffData;
	NBASSERT(tokensCount > 1)
	if(tokensCount > 1){
		const char* firstTokn = &(tokensBuff[tokensDefs[0].iAtBuff]);
		const char* secondTokn = &(tokensBuff[tokensDefs[1].iAtBuff]);
		NBASSERT(firstTokn[0] == '#' && firstTokn[1] == '\0')
		NBASSERT(secondTokn[0] != '\0')
#		ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
		//PRINT PREPROCESSOR DIRECTIVE
		SrcAnlzCodeCParser_pprocDbgPrintfTokens(state, (curBlkState.isPortionActive || (curBlkState.isParentActive && NBString_strIsEqual(secondTokn, "endif"))), NBString_strIsEqual(secondTokn, "endif"));
#		endif
		//
		//http://www.cplusplus.com/doc/tutorial/preprocessor/
		//
		if(
		   NBString_strIsEqual(secondTokn, "include")
		   || NBString_strIsEqual(secondTokn, "import")
		){
			//-------------------
			//#include ...
			//#import ...
			//-------------------
			if(tokensCount == 3){
				//Expected: (1)# (2)include (3)'"literal"'
				//Expected: (1)# (2)include (3)'<literal>'
				BOOL isSysPath = FALSE;
				BOOL isUsrPath = FALSE;
				const char* hNameStr	= &(tokensBuff[tokensDefs[2].iAtBuff]);
				const SI32 hNameLen		= NBString_strLenBytes(hNameStr);
				if(hNameLen > 2){ //At least one char between limits
					isSysPath	= (hNameStr[0] == '<' && hNameStr[hNameLen - 1] == '>');
					isUsrPath	= (hNameStr[0] == '\"' && hNameStr[hNameLen - 1] == '\"');
				}
				if(isUsrPath || isSysPath){
					if(!curBlkState.isPortionActive){
						r = TRUE; //do not "include" (code is disabled by #if/else/endif directives)
					} else {
						STNBString hNameOnly;
						NBString_initWithStrBytes(&hNameOnly, &hNameStr[1], hNameLen - 2);
						//Search in already included/imported list
						BOOL ignoreImported = FALSE;
						if(NBString_strIsEqual(secondTokn, "import")){
							SI32 i; const SI32 count = state->includedStarts.use;
							for(i = 0; i < count; i++){
								if(NBString_strIsEqual(hNameOnly.str, &state->included.str[NBArray_itmValueAtIndex(&state->includedStarts, UI32, i)])){
									ignoreImported = TRUE;
									break;
								}
							}
						}
						//
						if(ignoreImported){
							//Ignore #import (already imported)
							PRINTF_INFO("INCLUDE: %s ignoring #import (already imported)\n", hNameStr);
						} else {
							STNBString strFullPath; NBString_init(&strFullPath);
							//Search for include path
							FILE* stream = fopen(hNameOnly.str, "rb");
							if(stream != NULL){
								PRINTF_INFO("INCLUDE: %s (found without include paths)\n", hNameStr);
								NBString_empty(&strFullPath);
								//ToDo: concat local path
								NBString_concat(&strFullPath, hNameOnly.str);
							} else {
								if(isUsrPath){
									//First search file in include paths
									const char* pathsBuff = state->usrIncludes.str;
									const SI32 pathsCount = state->usrIncludesStarts.use;
									const UI32* pathsStarts = (const UI32*)state->usrIncludesStarts._buffData;
									SI32 i;
									for(i = 0; i < pathsCount; i++){
										NBString_empty(&strFullPath);
										NBString_concat(&strFullPath, &pathsBuff[pathsStarts[i]]); NBASSERT(strFullPath.str[strFullPath.length - 1] == '/' || strFullPath.str[strFullPath.length - 1] == '\\')
										NBString_concat(&strFullPath, hNameOnly.str);
										stream = fopen(strFullPath.str, "rb");
										if(stream != NULL){
											//PRINTF_INFO("INCLUDE: '%s' (found in userIncludePath '%s')\n", strPath.str, &pathsBuff[pathsStarts[i]]);
											break;
										}
									}
									if(i == pathsCount){
										//Then search system paths
										const char* pathsBuff = state->sysIncludes.str;
										const SI32 pathsCount = state->sysIncludesStarts.use;
										const UI32* pathsStarts = (const UI32*)state->sysIncludesStarts._buffData;
										SI32 i;
										for(i = 0; i < pathsCount; i++){
											NBString_empty(&strFullPath);
											NBString_concat(&strFullPath, &pathsBuff[pathsStarts[i]]); NBASSERT(strFullPath.str[strFullPath.length - 1] == '/' || strFullPath.str[strFullPath.length - 1] == '\\')
											NBString_concat(&strFullPath, hNameOnly.str);
											stream = fopen(strFullPath.str, "rb");
											if(stream != NULL){
												PRINTF_INFO("\n\nINCLUDE: %s (found in systemIncludePath '%s')\n", hNameStr, &pathsBuff[pathsStarts[i]]);
												break;
											}
										}
										if(i == pathsCount){
											//Not found
											PRINTF_ERROR("INCLUDE: %s (not found)\n", hNameStr);
											NBASSERT(FALSE)
										}
									}
								} else {
									NBASSERT(isSysPath)
									r = TRUE;
									//PRINTF_INFO("INCLUDE: '%s' (ignoring all systemIncludePath)\n", strPath.str);
									//First search file in system paths
									const char* pathsBuff = state->sysIncludes.str;
									const SI32 pathsCount = state->sysIncludesStarts.use;
									const UI32* pathsStarts = (const UI32*)state->sysIncludesStarts._buffData;
									SI32 i;
									for(i = 0; i < pathsCount; i++){
										NBString_empty(&strFullPath);
										NBString_concat(&strFullPath, &pathsBuff[pathsStarts[i]]); NBASSERT(strFullPath.str[strFullPath.length - 1] == '/' || strFullPath.str[strFullPath.length - 1] == '\\')
										NBString_concat(&strFullPath, hNameOnly.str);
										stream = fopen(strFullPath.str, "rb");
										if(stream != NULL){
											PRINTF_INFO("\n\nINCLUDE: %s (found in systemIncludePath '%s')\n", hNameStr, &pathsBuff[pathsStarts[i]]);
											break;
										}
									}
									if(i == pathsCount){
										//Then search user paths
										const char* pathsBuff = state->usrIncludes.str;
										const SI32 pathsCount = state->usrIncludesStarts.use;
										const UI32* pathsStarts = (const UI32*)state->usrIncludesStarts._buffData;
										SI32 i;
										for(i = 0; i < pathsCount; i++){
											NBString_empty(&strFullPath);
											NBString_concat(&strFullPath, &pathsBuff[pathsStarts[i]]); NBASSERT(strFullPath.str[strFullPath.length - 1] == '/' || strFullPath.str[strFullPath.length - 1] == '\\')
											NBString_concat(&strFullPath, hNameOnly.str);
											stream = fopen(strFullPath.str, "rb");
											if(stream != NULL){
												//PRINTF_INFO("INCLUDE: '%s' (found in userIncludePath '%s')\n", strPath.str, &pathsBuff[pathsStarts[i]]);
												break;
											}
										}
										if(i == pathsCount){
											//Not found
											PRINTF_ERROR("INCLUDE: %s (not found)\n", hNameStr);
											NBASSERT(FALSE)
										}
									}
								}
							}
							//Process include
							if(stream != NULL){
								//Add to imported list
								NBArray_addValueCopy(&state->includedStarts, UI32, state->included.length);
								NBString_concatBytes(&state->included, hNameOnly.str, hNameOnly.length + 1); // +1 to include the '\0' char.
								//Reset state
								NBASSERT(state->parseScopesStack.use > 1)
								if(state->parseScopesStack.use <= 1){
									PRINTF_ERROR("No active scope.\n");
									r = FALSE; NBASSERT(FALSE)
								} else {
									NBArray_empty(&state->acumTokensPProc.defs);
									NBString_empty(&state->acumTokensPProc.strBuff);
									STSrcAnlzCodeCParseScope* scope		= NBArray_itmPtrAtIndex(&state->parseScopesStack, STSrcAnlzCodeCParseScope, state->parseScopesStack.use - 1);
									state->curBlqType		= ENSrcAnlzCodeCParseMode_Code;
									state->acumTokensCur		= &scope->acumTokens;
									//push stream scope
									if(!(*state->itfs->parserItf->streamPushFunc)(state->itfs->parserItf, ENSrcAnlzParserSrcType_File, strFullPath.str)){
										PRINTF_ERROR("Could not push stream for file.\n");
										r = FALSE; NBASSERT(FALSE)
									} else {
#										ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
										const SI32 dbgBlkDepth = state->pprocCondPortionsStack.use;
#										endif
										//feed stream
										{
											r = TRUE;
											BYTE buffer[NB_SRCANLZ_PARSE_BUFFER_SIZE];
											PRINTF_INFO("----- PARSING INCLUDE FILE'S CONTENT: %s.\n", hNameStr);
											while(1){
												//Load data to buffer
												const size_t bytesRead = fread(buffer, 1, NB_SRCANLZ_PARSE_BUFFER_SIZE, stream);
												if(bytesRead <= 0){
													break;
												} else {
													if(!(*state->itfs->parserItf->streamFeedFunc)(state->itfs->parserItf, (const char*)buffer, (UI32)bytesRead)){
														r = FALSE; NBASSERT(FALSE)
														break;
													}
												}
											}
										}
										//flush stream
										if(r){
											if(!(*state->itfs->parserItf->streamFlushFunc)(state->itfs->parserItf)){
												r = FALSE; NBASSERT(FALSE)
											}
										}
										//-----------------------
										//http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf
										//A source file shall not end in a partial preprocessing token or in a partial comment.
										//Fix: push a newLine char at end of file (if necesary).
										//-----------------------
										/*if(r && (state->toknParseState->lastStreamChar != '\n' || state->toknParseState->strAcum2.length > 0)){
										 char lastChar = '\n';
										 PRINTF_WARNING("File's last char is not a newLine (adding newLine to stream): %s.\n", hNameStr);
										 if(!SrcAnlzToknParser_feed(state->toknParseState, &lastChar, sizeof(lastChar))){
										 r = FALSE; NBASSERT(FALSE)
										 }
										 }*/
										//pop stream scope
										if(r){
											if(!(*state->itfs->parserItf->streamPopFunc)(state->itfs->parserItf)){
												r = FALSE; NBASSERT(FALSE)
											}
										}
										NBASSERT(!r || dbgBlkDepth == state->pprocCondPortionsStack.use)
										PRINTF_INFO("----- END PARSING INCLUDE FILE'S CONTENT: %s.\n", hNameStr);
									}
								}
								fclose(stream);
							}
							NBString_release(&strFullPath);
						}
						NBString_release(&hNameOnly);
					}
				} else {
					PRINTF_ERROR("Preprocessor: #include directive has no valid header-name (%d tokens).\n", tokensCount);
				}
			} else {
				PRINTF_ERROR("Preprocessor: #include directive is not complete (%d tokens, 3 expected).\n", tokensCount);
			}
		} else if(
				  NBString_strIsEqual(secondTokn, "pragma")
				  || NBString_strIsEqual(secondTokn, "error")
				  || NBString_strIsEqual(secondTokn, "warning")
				  || NBString_strIsEqual(secondTokn, "line")
		){
			//-------------------
			//#pragma ...
			//#error ...
			//#warning ...
			//#line ...
			//-------------------
			if(tokensCount > 2){
				//Expected: (1)# (2)pragma ... (n)something
				//Expected: (1)# (2)error ... (n)something
				//Expected: (1)# (2)warning ... (n)something
				//Expected: (1)# (2)line ... (n)something
				const BOOL isError	= NBString_strIsEqual(secondTokn, "error");
				const BOOL isWarn	= NBString_strIsEqual(secondTokn, "warning");
				if(curBlkState.isPortionActive && (isError || isWarn)){
					STNBString strTmp; NBString_init(&strTmp);
					SI32 i; for(i = 2; i < tokensCount; i++){
						if(i != 2 && tokensDefs[i].isSpacePreceded) NBString_concatByte(&strTmp, ' ');
						NBString_concat(&strTmp, &(tokensBuff[tokensDefs[i].iAtBuff]));
					}
					if(isError){
						PRINTF_ERROR("Preprocessor active error-directive found: '%s'.\n", strTmp.str);
					} else {
						PRINTF_WARNING("Preprocessor active warning-directive found: '%s'.\n", strTmp.str);
						r = TRUE;
					}
					NBString_release(&strTmp);
				} else {
					r = TRUE;
				}
			} else {
				PRINTF_ERROR("Preprocessor: #%s directive is incomplete (%d tokens).\n", secondTokn, tokensCount);
			}
		} else if(
				  NBString_strIsEqual(secondTokn, "undef")
		){
			//-------------------
			//#undef ...
			//-------------------
			if(tokensCount > 2){
				//Expected: (1)# (2)undefine (3)name
				if(tokensCount == 3){
					if(!curBlkState.isPortionActive){
						r = TRUE; //do not "undefine" (code is disabled by #if/else/endif directives)
					} else {
						//Remove macro
						const SI32 iIdx = SrcAnlzMacros_findNameIdx(&state->macros, &state->acumTokensCur->strBuff, tokensDefs[2].iAtBuff);
						if(iIdx != -1){
							const STSrcAnlzStrIdx* idx = NBArraySorted_itmPtrAtIndex(&state->macros.namesIdx, STSrcAnlzStrIdx, iIdx);
							NBASSERT(idx->itmIndex >= 0 && idx->itmIndex < state->macros.defs.use)
							NBASSERT(NBString_strIsEqual(&(tokensBuff[tokensDefs[2].iAtBuff]), &(state->macros.names.str[NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, idx->itmIndex)->nameStart])))
							//NBArray_removeItemAtIndex(&state->macros.defs, idx->itmIndex); //Do not remove from buffer (to avoid index corruption)
							//Remove index
							NBArraySorted_removeItemAtIndex(&state->macros.namesIdx, iIdx);
							PRINTF_INFO("Preprocessor: macro '%s' removed.\n", &(tokensBuff[tokensDefs[2].iAtBuff]));
						} else {
							PRINTF_INFO("Preprocessor: macro '%s' was not listed.\n", &(tokensBuff[tokensDefs[2].iAtBuff]));
						}
						r = TRUE;
					}
				} else {
					PRINTF_ERROR("Preprocessor: #undef directive is not valid.\n");
				}
			} else {
				PRINTF_ERROR("Preprocessor: #undef directive is incomplete (%d tokens).\n", tokensCount);
			}
		} else if(
				  NBString_strIsEqual(secondTokn, "define")
		){
			//-------------------
			//#define ...
			//-------------------
			if(tokensCount > 2){
				//Expected: (1)# (2)define (3)name
				//Expected: (1)# (2)define (3)name (4)value
				//Expected: (1)# (2)define (3)name (4) '(' ... (n-1) ')' (n) value
				if(!curBlkState.isPortionActive){
					r = TRUE; //do not "define" (code is disabled by #if/else/endif directives)
				} else {
					STSrcAnlzMacro* macro = NULL;
					if(!SrcAnlzMacros_macroCreateOrReset(&state->macros, &state->acumTokensCur->strBuff, tokensDefs[2].iAtBuff, &macro)){
						NBASSERT(!r && FALSE)
					} else {
						r = TRUE;
						NBASSERT(macro != NULL)
						//Analyze macro params
						SI32 iToken = 3;
						if(r && iToken < tokensCount){
							const char* thirdTokn = &(tokensBuff[tokensDefs[iToken].iAtBuff]);
							if(thirdTokn[0] == '('){
								if(thirdTokn[1] == '\0'){
									if(!tokensDefs[iToken].isSpacePreceded){
										if(!SrcAnlzMacros_macroParamsStart(&state->macros, macro)){
											r = FALSE; NBASSERT(FALSE)
										} else {
											BOOL expectingParamName		= TRUE;
											//This MACRO has params (read param list)
											while(++iToken < tokensCount){
												//Must be a param name (or part of it), a comma or ')'
												const char* token = &(tokensBuff[tokensDefs[iToken].iAtBuff]);
												if(NBString_strIsEqual(token, ",")){
													if(expectingParamName){
														r = FALSE; NBASSERT(FALSE) //Comma without paramName before
														break;
													} else {
														expectingParamName = TRUE;
													}
												} else if(NBString_strIsEqual(token, ")")){
													if(expectingParamName && macro->params.count > 0){
														r = FALSE; NBASSERT(FALSE) //ClosingParenth without paramName before
														break;
													} else {
														expectingParamName = FALSE;
														iToken++;
														break;
													}
												} else {
													if(macro->params.isVaArgs){
														r = FALSE; NBASSERT(FALSE) //No params are allowed after "..."
														break;
													} else {
														if(!expectingParamName){
															if(NBString_strIsEqual(token, "...") && macro->params.count > 0){
																//Last param is __VAR_ARGS_'s explicit name
																if(!SrcAnlzMacros_macroParamsEnableVaArgAsLastParam(&state->macros, macro)){
																	r = FALSE; NBASSERT(FALSE) //Second param name found
																	break;
																}
															} else {
																r = FALSE; NBASSERT(FALSE) //Second param name found
																break;
															}
														} else {
															if(NBString_strIsEqual(token, "...")){
																if(!SrcAnlzMacros_macroParamsAddVaArg(&state->macros, macro)){
																	r = FALSE; NBASSERT(FALSE)
																	break;
																}
															} else {
																if(!SrcAnlzMacros_macroParamsAdd(&state->macros, macro, token)){
																	r = FALSE; NBASSERT(FALSE)
																	break;
																}
															}
															expectingParamName = FALSE;
														}
													}
												}
											}
										}
									}
								}
							}
						}
						//Analyze macro value
						if(r && iToken < tokensCount){
							//Add tokens
							while(iToken < tokensCount){
								const char* token = &(tokensBuff[tokensDefs[iToken].iAtBuff]);
								if(!SrcAnlzMacros_macroValueAddToken(&state->macros, macro, token, tokensDefs[iToken].sintxPart, tokensDefs[iToken].isSpacePreceded)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
								iToken++;
							}
							//Link tokens against params (optimization)
							if(r){
								if(!SrcAnlzMacros_macroFinishBuild(&state->macros, macro)){
									r = FALSE; NBASSERT(FALSE)
								}
							}
						}
						//if(r) PRINTF_INFO("Preprocessor (on): '# define '%s' %s (%d params) (%s).\n", &(tokensBuff[tokensDefs[2].iAtBuff]), (iIdx != -1 ? "updated" : "added"), macro->params.count, (macro->valTokensCount != 0 ? "explicit value" : " no value"));
					}
				}
			} else {
				PRINTF_ERROR("Preprocessor: #define directive is incomplete (%d tokens).\n", tokensCount);
			}
		} else if(
				  NBString_strIsEqual(secondTokn, "ifdef")
		){
			//-------------------
			//#ifdef ...
			//-------------------
			if(tokensCount == 3){
				//Expected: (1)# (2)ifdef (3)name
				if(!curBlkState.isPortionActive){
					//Code is disabled by parent
					STSrcAnlzCodeCondPortionState data;
					data.isParentActive			= FALSE;
					data.isPortionActive		= FALSE;
					data.isPortionActiveFound	= FALSE;
					data.isPortionElseFound		= FALSE;
					NBArray_addValue(&state->pprocCondPortionsStack, data);
					//PRINTF_INFO("Preprocessor: macro '%s' IGNORED (parent portion is no active).\n", &(tokensBuff[tokensDefs[2].iAtBuff]));
				} else {
					//Eval condifitonal
					const SI32 iIdx = SrcAnlzMacros_findNameIdx(&state->macros, &state->acumTokensCur->strBuff, tokensDefs[2].iAtBuff);
					//Push block state
					{
						STSrcAnlzCodeCondPortionState data;
						data.isParentActive			= TRUE;
						data.isPortionActive		= (iIdx != -1);
						data.isPortionActiveFound	= data.isPortionActive;
						data.isPortionElseFound		= FALSE;
						NBArray_addValue(&state->pprocCondPortionsStack, data);
					}
#					ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
					if(iIdx != -1){
						NBASSERT(NBString_strIsEqual(&(tokensBuff[tokensDefs[2].iAtBuff]), &(state->macros.names.str[NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, NBArraySorted_itmPtrAtIndex(&state->macros.namesIdx, STSrcAnlzStrIdx, iIdx)->itmIndex)->nameStart])))
						//PRINTF_INFO("Preprocessor: macro '%s' is defined.\n", &(tokensBuff[tokensDefs[2].iAtBuff]));
					} else {
						//PRINTF_INFO("Preprocessor: macro '%s' is not defined.\n", &(tokensBuff[tokensDefs[2].iAtBuff]));
					}
#					endif
				}
				r = TRUE;
			} else {
				PRINTF_ERROR("Preprocessor: #ifdef directive is not valid (%d tokens).\n", tokensCount);
			}
		} else if(
				  NBString_strIsEqual(secondTokn, "ifndef")
		){
			//-------------------
			//#ifndef ...
			//-------------------
			if(tokensCount == 3){
				//Expected: (1)# (2)ifndef (3)name
				if(!curBlkState.isPortionActive){
					//Code is disabled by parent
					STSrcAnlzCodeCondPortionState data;
					data.isParentActive			= FALSE;
					data.isPortionActive		= FALSE;
					data.isPortionActiveFound	= FALSE;
					data.isPortionElseFound		= FALSE;
					NBArray_addValue(&state->pprocCondPortionsStack, data);
					//PRINTF_INFO("Preprocessor: macro '%s' IGNORED (parent portion is no active).\n", &(tokensBuff[tokensDefs[2].iAtBuff]));
				} else {
					const SI32 iIdx = SrcAnlzMacros_findNameIdx(&state->macros, &state->acumTokensCur->strBuff, tokensDefs[2].iAtBuff);
					//Push block state
					{
						STSrcAnlzCodeCondPortionState data;
						data.isParentActive			= TRUE;
						data.isPortionActive		= (iIdx == -1);
						data.isPortionActiveFound	= data.isPortionActive;
						data.isPortionElseFound		= FALSE;
						NBArray_addValue(&state->pprocCondPortionsStack, data);
					}
#					ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
					if(iIdx != -1){
						NBASSERT(NBString_strIsEqual(&(tokensBuff[tokensDefs[2].iAtBuff]), &(state->macros.names.str[NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, NBArraySorted_itmPtrAtIndex(&state->macros.namesIdx, STSrcAnlzStrIdx, iIdx)->itmIndex)->nameStart])))
						//PRINTF_INFO("Preprocessor: macro '%s' is defined.\n", &(tokensBuff[tokensDefs[2].iAtBuff]));
					} else {
						//PRINTF_INFO("Preprocessor: macro '%s' is not defined.\n", &(tokensBuff[tokensDefs[2].iAtBuff]));
					}
#					endif
				}
				r = TRUE;
			} else {
				PRINTF_ERROR("Preprocessor: #ifndef directive is not valid (%d tokens).\n", tokensCount);
			}
		} else if(
				  NBString_strIsEqual(secondTokn, "else")
		){
			//-------------------
			//#else
			//-------------------
			if(tokensCount == 2){
				//Expected: (1)# (2)else
				//Change value of block state
				const SI32 statesCount = state->pprocCondPortionsStack.use;
				if(statesCount > 1){ //at least root-state and one more
					if(!curBlkState.isPortionElseFound){
						//Update last block state
						STSrcAnlzCodeCondPortionState* lastState = NBArray_itmPtrAtIndex(&state->pprocCondPortionsStack, STSrcAnlzCodeCondPortionState, statesCount - 1);
						NBASSERT(lastState->isParentActive == curBlkState.isParentActive)
						NBASSERT(lastState->isPortionActive == curBlkState.isPortionActive)
						NBASSERT(lastState->isPortionActiveFound == curBlkState.isPortionActiveFound)
						NBASSERT(lastState->isPortionElseFound == curBlkState.isPortionElseFound)
						if(lastState->isPortionActiveFound){
							lastState->isPortionActive		= FALSE;
							//PRINTF_INFO("PREPROC CONDITIONAL '%s' PUSHED A '%s' CONDIFITIONAL BLOCK.\n", secondTokn, (lastState->isPortionActive ? "active" : "inactive"));
						} else {
							lastState->isPortionActive		= lastState->isParentActive;
							lastState->isPortionActiveFound	= (lastState->isPortionActiveFound || lastState->isPortionActive);
							//PRINTF_INFO("PREPROC CONDITIONAL '%s' PUSHED A '%s' CONDIFITIONAL BLOCK.\n", secondTokn, (lastState->isPortionActive ? "active" : "inactive"));
						}
						lastState->isPortionElseFound	= TRUE;
						r = TRUE;
					} else {
						PRINTF_ERROR("Preprocessor: only one #else block is allowed (missing #endif?).\n");
					}
				} else {
					PRINTF_ERROR("Preprocessor: #else directive without an open #if (or equivalent).\n");
				}
			} else {
				PRINTF_ERROR("Preprocessor: #else directive is not valid (%d tokens).\n", tokensCount);
			}
		} else if(
				  NBString_strIsEqual(secondTokn, "endif")
		){
			//-------------------
			//#endif
			//-------------------
			if(tokensCount == 2){
				//Expected: (1)# (2)endif
				//Pop block state
				const SI32 statesCount = state->pprocCondPortionsStack.use;
				NBASSERT(statesCount > 1) //at least root state and one more
				if(statesCount > 1){
					NBArray_removeItemAtIndex(&state->pprocCondPortionsStack, statesCount - 1);
					r = TRUE;
				} else {
					PRINTF_ERROR("Preprocessor: #endif directive without an open #if (or equivalent).\n");
				}
			} else {
				PRINTF_ERROR("Preprocessor: #endif directive is not valid (%d tokens).\n", tokensCount);
			}
		} else if(
				  NBString_strIsEqual(secondTokn, "if")
				  || NBString_strIsEqual(secondTokn, "elif")
		){
			//-------------------
			//#if or elif
			//-------------------
			if(tokensCount > 2){
				//Expected: (1)# (2)if or elif ...
				//Example:  (1)# (2)if (3)_SOMETHING_
				//Example:  (1)# (2)if (3)defined (4)( (5)WIN32 (6)) (7)| (8)| (9)! (10)defined (11)( (12)_WIN32 (13))
				//Example:  # if defined ( WIN32 ) | | ! defined ( _WIN32 )
				const BOOL isIF	= (secondTokn[2] == '\0'); //'if' is two chars long, 'elif' is four.
				const SI32 statesCount = state->pprocCondPortionsStack.use;
				NBASSERT(statesCount > (isIF ? 0 : 1)) //at least root state and one more
				if(statesCount > (isIF ? 0 : 1)){
					BOOL condResult		= FALSE;
					const BOOL mustEval	= (curBlkState.isParentActive && (isIF || !curBlkState.isPortionActiveFound));
#					ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
					//PRINT PREPROCESSOR DIRECTIVE
					//PRINTF_INFO("\n\n");
					//SrcAnlzCodeCParser_pprocDbgPrintfTokens(state, mustEval, FALSE);
#					endif
					if(!mustEval){
						//PRINTF_INFO("PREPROC CONDITIONAL '%s' ignored (parent is %s, active portion %s).\n", secondTokn, (curBlkState.isParentActive ? "active" : "inactive"), curBlkState.isPortionActiveFound ? "already found":"not found yet");
						r = TRUE;
					} else {
						//const char* tokensBuff = state->acumTokensCur->strBuff.str;
						//const SI32 tokensCount = state->acumTokensCur->defs.use;
						//const STSrcAnlzTokn* tokensDefs = (const STSrcAnlzTokn*)state->acumTokensCur->defs._buffData;
						//Parse conditional's "constant-expression"
						//sada d
						/*STSrcAnlzSintxParseTree tree;
						SrcAnlzSintxParseTree_init(&tree);
						//Start posibilities
						const STSrcAnlzTokn* tknDef = &tokensDefs[2];
						if(!SrcAnlzSintxParseTree_addRootPosibilities(&state->sintxDefs, &tree, ENSrzAnlzSintxC_constant_expression, tknDef->sintxPart, 2)){
							PRINTF_ERROR("Error starting constant-expression at preprocessor conditional.\n");
						} else {
							SrcAnlzSintxParseTree_print(&state->sintxDefs, &tree);
							const STSrcAnlzSintxParseTreeNode* rootNode = NBArray_itmPtrAtIndex(&tree.nodes, STSrcAnlzSintxParseTreeNode, 0);
							if(rootNode->iNextPart == 0){
								PRINTF_ERROR("Expected a constant-expression at preprocessor conditional.\n");
							} else {
								UI32 i; for(i = 3; i < tokensCount; i++){
									const STSrcAnlzTokn* tknDef = &tokensDefs[i];
									if(!SrcAnlzSintxParseTree_continuePosibilities(&state->sintxDefs, &tree, tknDef->sintxPart, i, NULL)){
										PRINTF_ERROR("Error continuing constant-expression at preprocessor conditional.\n");
										break;
									} else {
										SrcAnlzSintxParseTree_print(&state->sintxDefs, &tree);
										const STSrcAnlzSintxParseTreeNode* rootNode = NBArray_itmPtrAtIndex(&tree.nodes, STSrcAnlzSintxParseTreeNode, 0);
										if(rootNode->iNextPart == 0){
											PRINTF_ERROR("Expected a constant-expression at preprocessor conditional.\n");
											break;
										}
									}
								}
								if(i == tokensCount){
									STSrcAnlzSintxPart dummyPart;
									dummyPart.type	= ENSrzAnlzSintxPartType_SintaxDef;
									dummyPart.idx	= ENSrzAnlzSintxC_Count;
									UI16 iPosibFound = 0;
									if(!SrcAnlzSintxParseTree_continuePosibilities(&state->sintxDefs, &tree, dummyPart, 0, &iPosibFound)){
										PRINTF_ERROR("Error ending constant-expression at preprocessor conditional.\n");
									} else if(iPosibFound == 0){
										PRINTF_ERROR("Expected a full constant-expression at preprocessor conditional.\n");
									} else {
										SrcAnlzSintxParseTree_printChilds(&state->sintxDefs, &tree, iPosibFound);
										r = TRUE;
									}
								}
							}
						}
						SrcAnlzSintxParseTree_release(&tree);*/
						STNBArray ops; NBArray_init(&ops, sizeof(STSrcAnlzExprParseNode), NULL);
						if(!SrcAnlzExprParser_feedTokns(state->acumTokensCur, &__globalPProcCondExprSintaxMap, 2, state->acumTokensCur->defs.use, &ops)){
							r = FALSE; NBASSERT(FALSE)
						} else {
#							ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
							//Print parsed conditional
							{
								STNBString strTmp;	NBString_init(&strTmp);
								STNBString strTmp2;	NBString_init(&strTmp2);
								SrcAnlzCodeCParser_pprocDbgAddfTokensToStr(state, &strTmp2, 2, TRUE, FALSE);
								SI32 iOp = 0; SrcAnlzExprParser_dbgAddOpsToStr(state->acumTokensCur, &strTmp, &iOp, (const STSrcAnlzExprParseNode*)ops._buffData, ops.use, TRUE);
								NBASSERT(iOp == ops.use)
								if(!NBString_isEqual(&strTmp, strTmp2.str)){
									PRINTF_ERROR("OPs does not match:\n------------\n%s //tokens\n%s //parsed\n------------\n", strTmp2.str, strTmp.str);
									STNBString strTmp3;
									NBString_init(&strTmp3);
									SI32 iOp = 0; SrcAnlzExprParser_dbgAddOpsDefsToStr(state->acumTokensCur, &strTmp3, &iOp, (const STSrcAnlzExprParseNode*)ops._buffData, ops.use, TRUE);
									PRINTF_ERROR("OPs defs:\n%s\n", strTmp3.str);
									NBString_release(&strTmp3);
									NBASSERT(FALSE)
								}
								NBString_release(&strTmp);
								NBString_release(&strTmp2);
							}
#							endif
							SI32 iOp = 0;
							STSrcAnlzValOpVal val; val.type = STSrcAnlzValOpValType_Count;
							if(!SrcAnlzCodeCParser_pprocCondExprResult(state, state->acumTokensCur, &val, &iOp, (const STSrcAnlzExprParseNode*)ops._buffData, ops.use)){
								r = FALSE; NBASSERT(FALSE)
							} else {
								if(!SrcAnlzCodeCParser_pprocCondExprOpValAsBoolean(state, &condResult, &val, STSrcAnlzEvalMode_Recursive)){
									r = FALSE; NBASSERT(FALSE)
								} else {
									//PRINTF_INFO("PREPROC CONDITIONAL '%s' resulted '%s' (parent is %s, active portion %s).\n", secondTokn, (condResult ? "TRUE" : "FALSE"), (curBlkState.isParentActive ? "active" : "inactive"), curBlkState.isPortionActiveFound ? "already found":"not found yet");
									r = TRUE;
								}
							}
						}
						NBArray_release(&ops);
					}
					//Apply result
					NBASSERT(r)
					if(r){
						if(isIF){
							//Push block state
							STSrcAnlzCodeCondPortionState data;
							data.isParentActive			= curBlkState.isPortionActive;
							data.isPortionActive		= (curBlkState.isPortionActive && condResult);
							data.isPortionActiveFound	= data.isPortionActive;
							data.isPortionElseFound		= FALSE;
							NBArray_addValue(&state->pprocCondPortionsStack, data);
							//PRINTF_INFO("PREPROC CONDITIONAL '%s' PUSHED A '%s' CONDIFITIONAL BLOCK.\n", secondTokn, (data.isPortionActive ? "active" : "inactive"));
							r = TRUE;
						} else {
							//Update last block state
							STSrcAnlzCodeCondPortionState* lastState = NBArray_itmPtrAtIndex(&state->pprocCondPortionsStack, STSrcAnlzCodeCondPortionState, statesCount - 1);
							NBASSERT(lastState->isParentActive == curBlkState.isParentActive)
							NBASSERT(lastState->isPortionActive == curBlkState.isPortionActive)
							NBASSERT(lastState->isPortionActiveFound == curBlkState.isPortionActiveFound)
							NBASSERT(lastState->isPortionElseFound == curBlkState.isPortionElseFound)
							NBASSERT(!condResult || !lastState->isPortionActiveFound) //only one #if #elif block must be executed.
							lastState->isPortionActive		= (lastState->isParentActive && condResult);
							lastState->isPortionActiveFound	= (lastState->isPortionActiveFound || lastState->isPortionActive);
							//PRINTF_INFO("PREPROC CONDITIONAL '%s' MOVED TO A '%s' CONDIFITIONAL BLOCK.\n", secondTokn, (lastState->isPortionActive ? "active" : "inactive"));
						}
					}
				} else {
					PRINTF_ERROR("Preprocessor: #%s directive without an open #if (or equivalent).\n", secondTokn);
				}
			} else {
				PRINTF_ERROR("Preprocessor: #%s directive is not valid (%d tokens).\n", secondTokn, tokensCount);
			}
		} else {
			PRINTF_ERROR("Preprocessor: unknown '%s' directive.\n", secondTokn);
		}
	}
	//Validate code portions state stack
#	ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
	{
		const SI32 newCodeStateStackSz = state->pprocCondPortionsStack.use;
		NBASSERT(newCodeStateStackSz > 0) //at least root state and one more
		{
			NBASSERT(NBArray_itmPtrAtIndex(&state->pprocCondPortionsStack, STSrcAnlzCodeCondPortionState, 0)->isParentActive) //root must be active
			NBASSERT(NBArray_itmPtrAtIndex(&state->pprocCondPortionsStack, STSrcAnlzCodeCondPortionState, 0)->isPortionActive) //root must be active
			SI32 i; BOOL prevIsActive = TRUE;
			for(i = 0; i < newCodeStateStackSz; i++){
				const STSrcAnlzCodeCondPortionState* cond = NBArray_itmPtrAtIndex(&state->pprocCondPortionsStack, STSrcAnlzCodeCondPortionState, i);
				NBASSERT(!(!cond->isParentActive && cond->isPortionActive)) //one block of code can't be active if parent block is inactive.
				NBASSERT(cond->isParentActive == prevIsActive)
				prevIsActive = cond->isPortionActive;
			}
		}
	}
#	endif
	//
	return r;
}

//
//Conditional expresions evaluations
//

BOOL SrcAnlzCodeCParser_pprocCondExprResult(STSrcAnlzCodeCParser* state, const STSrcAnlzTokns* tokns, STSrcAnlzValOpVal* dst, SI32* opIndex, const STSrcAnlzExprParseNode* ops, const SI32 opsCount){
	BOOL r = FALSE;
	SI32 iOP = *opIndex;
	if(iOP < opsCount){
		r = TRUE;
		//Left+right op
		typedef struct STSrcAnlzValOpResultsLR_ {
			SI32 idx;				//Op's index
			STSrcAnlzValOpVal valL;	//Left value
			STSrcAnlzValOpVal valR; //Right value
		} STSrcAnlzValOpResultsLR;
		//Index of unique left+right op.
		//Ooptimization, use this var instead the arrays
		//when theres only one (or none) left+right op.
		SI32 opIdx = -1, opsLRFound = 0;
		STNBArray opsResults;	NBArray_init(&opsResults, sizeof(STSrcAnlzValOpResultsLR), NULL); //Indexes of left+right ops (this are the 'operands' boundaries)
		STNBArray opsIdxsPend;	NBArray_init(&opsIdxsPend, sizeof(SI32), NBCompareSI32);//Indexes at 'opsIdxs' pending to process (to determine priorities)
		//Analyze this deep level
		//
		//Search for ANY left+right ops.
		//
		{
			iOP = *opIndex;
			while(iOP < opsCount){
				const STSrcAnlzExprParseNode* op = &ops[iOP];
				if(op->iCharDef != -1){
					const STSrcAnlzExprOp* opDef = &op->sintaxMap->opChars[op->iCharDef];
					NBASSERT(op->sintax.iDef == opDef->op1 || op->sintax.iDef == opDef->op2)
					NBASSERT(op->sintax.iDef != -1)
					if(op->sintax.iDef == -1){
						//operator should have a sintax
						PRINTF_ERROR("Operator-without-sintax #%d: '%s'.\n", iOP, opDef->chars);
						NBASSERT(FALSE)
						r = FALSE;
						break;
					} else {
						const STSrcAnlzExprOpSintxMap*	sintaxMap = op->sintaxMap;
						NBASSERT(sintaxMap != NULL)
						NBASSERT(op->sintax.iDef < sintaxMap->opSintaxsSz)
						const STSrcAnlzExprOpSintx* opSintax = &sintaxMap->opSintaxs[op->sintax.iDef];
						if(opSintax->sidesMask == ENSrcAnlzExprOpSideMask_Both){
							//Add to left+right ops list
							if(opIdx == -1){
								//Optimization: use this var instead the arrays
								//when theres only one (or none) left+right op.
								opIdx = iOP;
							} else {
								//Create the arrays (more than one left+right op found)
								STSrcAnlzValOpResultsLR d;
								d.valL.type = d.valR.type = STSrcAnlzValOpValType_Count;
								//Add the first op accumulated by optimization
								if(opsResults.use == 0){
									d.idx = opIdx;
									NBArray_addValueCopy(&opsIdxsPend, SI32, opsResults.use);
									NBArray_addValue(&opsResults, d);
								}
								//Add new op
								d.idx = iOP;
								NBArray_addValueCopy(&opsIdxsPend, SI32, opsResults.use);
								NBArray_addValue(&opsResults, d);
							}
							opsLRFound++;
							//PRINTF_INFO("Operator-left+right found #%d: '%s' (analyzing %d -> %d).\n", iOP, opDef->chars, *opIndex, (opsCount - 1));
						}
						//Jump the deep level
						if(op->sintax.rCount > 0) {
							iOP += op->sintax.rCount;
						}
					}
				}
				iOP++;
			}
		}
		//Process this depth level
		if(r){
			if(opsLRFound > 0){
				if(opsLRFound == 1){
					//
					//Only one left+right op.
					//
					NBASSERT(opIdx != -1)
					//PRINTF_INFO("Conditional level has %d left+right ops (no need for priority search): left(%d -> %d) right(%d -> %d).\n", opsLRFound, *opIndex, iOP + (opIdx - iOP) - 1, (opIdx + 1), opsCount - 1);
					STSrcAnlzValOpVal valL, valR;	//Leftand right values
					valL.type = valR.type = STSrcAnlzValOpValType_Count;
					SI32 iOP = *opIndex;
					//Process left side
					if(!SrcAnlzCodeCParser_pprocCondExprResult(state, tokns, &valL, &iOP, ops, iOP + (opIdx - iOP))){
						PRINTF_ERROR("Error executing LEFT value.\n");
						r = FALSE;
					} else {
						//Process right side
						SI32 iOP = opIdx + 1;
						if(!SrcAnlzCodeCParser_pprocCondExprResult(state, tokns, &valR, &iOP, ops, opsCount)){
							PRINTF_ERROR("Error executing RIGHT value.\n");
							r = FALSE;
						} else {
							//Execute operation
							const STSrcAnlzExprParseNode* op = &ops[opIdx];
							const STSrcAnlzExprOpSintxMap*	sintaxMap = op->sintaxMap;
							const STSrcAnlzExprOp* opDef = &sintaxMap->opChars[op->iCharDef];
							const STSrcAnlzExprOpSintx* opSintax = &sintaxMap->opSintaxs[op->sintax.iDef];
							//PRINTF_INFO("Operation is '%s' %s '%s'.\n", valL.value.str, opDef->chars, valR.value.str);
							if(!SrcAnlzCodeCParser_pprocCondExprEvalLR(state, dst, &valL, &valR, opSintax->opType, opSintax->opEnumValue)){
								PRINTF_ERROR("Could not evaluate left+right operation '%s'.\n", opDef->chars);
								r = FALSE; NBASSERT(FALSE)
							}
						}
					}
				} else {
					//
					//Multiples left+right op.
					//Must analyze execution priorities.
					//
					//PRINTF_INFO("Conditional level has %d left+right ops (must analyze execution priorities).\n", opsLRFound);
					while(opsIdxsPend.use > 0){
						//Analyze next operator to execute (by priority)
						SI32 pIndex = -1, pPriority = (ENSrcAnlzSintxCOpPriority_Count + 1);
						SI32 i; for(i = 0; i < opsIdxsPend.use; i++){
							const SI32 idx = NBArray_itmValueAtIndex(&opsIdxsPend, SI32, i);
							const STSrcAnlzExprParseNode* op = &ops[NBArray_itmPtrAtIndex(&opsResults, STSrcAnlzValOpResultsLR, idx)->idx];
							const STSrcAnlzExprOpSintxMap*	sintaxMap = op->sintaxMap;
							const STSrcAnlzExprOpSintx* opSintax = &sintaxMap->opSintaxs[op->sintax.iDef];
							if(pPriority > opSintax->priority){
								pIndex = i;
								pPriority = opSintax->priority;
							}
						}
						NBASSERT(pIndex != -1)
						if(pIndex == -1){
							PRINTF_ERROR("Could not define the op's priority!.\n");
							r = FALSE; NBASSERT(FALSE)
							break;
						} else {
							const SI32 idx = NBArray_itmValueAtIndex(&opsIdxsPend, SI32, pIndex);
							STSrcAnlzValOpResultsLR* result = NBArray_itmPtrAtIndex(&opsResults, STSrcAnlzValOpResultsLR, idx);
							const STSrcAnlzExprParseNode* op = &ops[result->idx];
							const STSrcAnlzExprOpSintxMap*	sintaxMap = op->sintaxMap;
							const STSrcAnlzExprOp* opDef = &sintaxMap->opChars[op->iCharDef];
							const STSrcAnlzExprOpSintx* opSintax = &sintaxMap->opSintaxs[op->sintax.iDef];
							//PRINTF_INFO("Next op to execute: '%s' (idx %d).\n", opDef->chars, NBArray_itmPtrAtIndex(&opsResults, STSrcAnlzValOpResultsLR, idx)->idx);
							//Obtain left result (if not available)
							if(result->valL.type == STSrcAnlzValOpValType_Count){
								SI32 iOP, iFirstOp = *opIndex;
								if(idx > 0){
									const STSrcAnlzValOpResultsLR* prevR = NBArray_itmPtrAtIndex(&opsResults, STSrcAnlzValOpResultsLR, idx - 1);
									iFirstOp = prevR->idx + 1;
									NBASSERT(iFirstOp < result->idx)
								}
								iOP = iFirstOp;
								if(!SrcAnlzCodeCParser_pprocCondExprResult(state, tokns, &result->valL, &iOP, ops, iFirstOp + (result->idx - iFirstOp))){
									PRINTF_ERROR("Error executing LEFT value.\n");
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
							//Obtain right result (if not available)
							if(result->valR.type == STSrcAnlzValOpValType_Count){
								SI32 iOP, iNextLastOp = opsCount;
								if((idx + 1) < opsResults.use){
									const STSrcAnlzValOpResultsLR* nextR = NBArray_itmPtrAtIndex(&opsResults, STSrcAnlzValOpResultsLR, idx + 1);
									iNextLastOp = nextR->idx;
									NBASSERT(iNextLastOp > result->idx && iNextLastOp <= opsCount)
								}
								iOP = (result->idx + 1);
								if(!SrcAnlzCodeCParser_pprocCondExprResult(state, tokns, &result->valR, &iOP, ops, iNextLastOp)){
									PRINTF_ERROR("Error executing RIGHT value.\n");
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
							//Eval operation
							if(r){
								STSrcAnlzValOpVal val; val.type = STSrcAnlzValOpValType_Count;
								NBASSERT(result->valL.type != STSrcAnlzValOpValType_Count)
								NBASSERT(result->valR.type != STSrcAnlzValOpValType_Count)
								if(!SrcAnlzCodeCParser_pprocCondExprEvalLR(state, &val, &result->valL, &result->valR, opSintax->opType, opSintax->opEnumValue)){
									PRINTF_ERROR("Could not evaluate left+right operation '%s'.\n", opDef->chars);
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									//Update prev's right value (if necesary)
									if(idx > 0){
										NBASSERT((idx - 1) < opsResults.use)
										STSrcAnlzValOpResultsLR* prevR = NBArray_itmPtrAtIndex(&opsResults, STSrcAnlzValOpResultsLR, idx - 1);
										if(prevR->valR.type == STSrcAnlzValOpValType_Count){
											prevR->valR = val;
											//PRINTF_INFO("Setting prev L+R op's right side with '%s' result.\n", STR_SrcAnlzValOpValType(val.type));
										}
									}
									//Update next's left value (if necesary)
									if((idx + 1) < opsResults.use){
										NBASSERT((idx + 1) < opsResults.use)
										STSrcAnlzValOpResultsLR* nextR = NBArray_itmPtrAtIndex(&opsResults, STSrcAnlzValOpResultsLR, idx + 1);
										if(nextR->valL.type == STSrcAnlzValOpValType_Count){
											nextR->valL = val;
											//PRINTF_INFO("Setting next L+R op's left side with '%s' result.\n", STR_SrcAnlzValOpValType(val.type));
										}
									}
									//Set final result (if this is the last remaining op)
									if(opsIdxsPend.use == 1 && dst != NULL){
										*dst = val;
									}
								}
							}
							//Remove from pending list
							NBArray_removeItemAtIndex(&opsIdxsPend, pIndex);
						}
					}
				}
			} else {
				//
				//Linear process: plain tokens, left-only and right-only operators.
				//
				iOP = *opIndex;
				//PRINTF_INFO("Conditional level has %d left+right ops (execute all from %d -> %d).\n", opsLRFound, iOP, (opsCount - 1));
				//
				while(iOP < opsCount){
					const STSrcAnlzExprParseNode* op = &ops[iOP++];
					if(op->iCharDef == -1){
						NBASSERT(op->iToken != -1)
						NBASSERT(dst->type == STSrcAnlzValOpValType_Count)
						//Determine content to analyze
						//Todo: to consider literal and plain values, ex: 0.1 should be diferent than 0."1" in code.
						//
						const char* firstTokn = NULL;
						//
						//Try to parse content as numeric
						//
						//TODO: remove this 'AUNumericoP' process.
						STNBNumParser numParseState;
						NBNumParser_init(&numParseState);
						{
							const STSrcAnlzTokn* tokensDefs = (const STSrcAnlzTokn*)tokns->defs._buffData;
							const char* tokensBuff = tokns->strBuff.str;
							firstTokn = &tokensBuff[tokensDefs[op->iToken].iAtBuff];
							if(!NBNumParser_feedBytes(&numParseState, firstTokn, NBString_strLenBytes(firstTokn))){
								NBASSERT(numParseState.isErr)
								numParseState.isErr = TRUE;
							}
							//ToDo: remove after testing
							/*if(tokensSrc == ENSrcAnlzExprSrc_MacrosValues){
								const STSrcAnlzMacroValTokn* tokensDefs = state->macros.valsTokensDefs->arreglo();
								const char* tokensBuff = state->macros.valsTokens.str;
								firstTokn = &tokensBuff[tokensDefs[op->iToken].toknStart];
								if(!AUNumericoP<UI64, char>::parserFeedUnsignedStr(&numParseState, firstTokn)){
									NBASSERT(numParseState.isErr)
									numParseState.isErr = TRUE;
								}
							} else {
								const STSrcAnlzTokn* tokensDefs = state->acumTokensCur->defs.arreglo();
								const char* tokensBuff = state->acumTokensCur->strBuff.str;
								firstTokn = &tokensBuff[tokensDefs[op->iToken].iAtBuff];
								if(!AUNumericoP<UI64, char>::parserFeedUnsignedStr(&numParseState, firstTokn)){
									NBASSERT(numParseState.isErr)
									numParseState.isErr = TRUE;
								}
							}*/
							//Close parser (final validations)
							if(!numParseState.isErr){
								if(!NBNumParser_end(&numParseState)){
									NBASSERT(numParseState.isErr)
									numParseState.isErr = TRUE;
								} else {
									NBASSERT(!numParseState.isErr)
								}
							}
						}
						//
						if(!numParseState.isErr){
							//Is numeric
							switch (numParseState.typeSub) {
								case ENNumericTypeSub_Int:
									//PRINTF_INFO("token('%s') is numeric: %d.\n", firstTokn, numParseState.valInt);
									dst->type = STSrcAnlzValOpValType_Int; dst->valIntt = numParseState.valInt; break;
								case ENNumericTypeSub_IntU:
									//PRINTF_INFO("token('%s') is numeric: %u.\n", firstTokn, numParseState.valIntU);
									dst->type = STSrcAnlzValOpValType_IntU; dst->valIntU = numParseState.valIntU; break;
								case ENNumericTypeSub_Long:
									//PRINTF_INFO("token('%s') is numeric: %ld.\n", firstTokn, numParseState.valLong);
									dst->type = STSrcAnlzValOpValType_Long; dst->valLong = numParseState.valLong; break;
								case ENNumericTypeSub_LongU:
									//PRINTF_INFO("token('%s') is numeric: %lu.\n", firstTokn, numParseState.valLongU);
									dst->type = STSrcAnlzValOpValType_LongU; dst->valLongU = numParseState.valLongU; break;
								case ENNumericTypeSub_LongLong:
									//PRINTF_INFO("token('%s') is numeric: %lld.\n", firstTokn, numParseState.valLongLong);
									dst->type = STSrcAnlzValOpValType_LongLong; dst->valLongLong = numParseState.valLongLong; break;
								case ENNumericTypeSub_LongLongU:
									//PRINTF_INFO("token('%s') is numeric: %llu.\n", firstTokn, numParseState.valLongLongU);
									dst->type = STSrcAnlzValOpValType_LongLongU; dst->valLongLongU = numParseState.valLongLongU; break;
								case ENNumericTypeSub_Float:
									//PRINTF_INFO("token('%s') is numeric: %f.\n", firstTokn, numParseState.valFloat);
									dst->type = STSrcAnlzValOpValType_Float;  dst->valFloat = numParseState.valFloat; break;
								case ENNumericTypeSub_Double:
									//PRINTF_INFO("token('%s') is numeric: %f.\n", firstTokn, numParseState.valDouble);
									dst->type = STSrcAnlzValOpValType_Double; dst->valDouble = numParseState.valDouble; break;
								case ENNumericTypeSub_DoubleLong:
									//PRINTF_INFO("token('%s') is numeric: %Lf.\n", firstTokn, numParseState.valDoubleLong);
									dst->type = STSrcAnlzValOpValType_DoubleLong; dst->valDoubleLong = numParseState.valDoubleLong; break;
								default:
									//PRINTF_ERROR("Unexpected numeric subType(%d).\n", (SI32)numParseState.typeSub);
									NBASSERT(FALSE)
									r = FALSE;
									break;
							}
						} else {
							//Not numeric
							NBASSERT(dst->type == STSrcAnlzValOpValType_Count)
							NBASSERT(op->iToken != -1)
							//Process identifier (not numeric)
							{
								const UI32 tokenNameStart = NBArray_itmPtrAtIndex(&tokns->defs, STSrcAnlzTokn, op->iToken)->iAtBuff;
								const STNBString* tokensStr = &tokns->strBuff;
								//ToDo: remove after testing
								/*if(tokensSrc == ENSrcAnlzExprSrc_MacrosValues){
									tokensStr		= state->macros.valsTokens;
									tokenNameStart	= NBArray_itmPtrAtIndex(&state->macros.valsTokensDefs, STSrcAnlzMacroValTokn, op->iToken)->toknStart;
								} else {
									tokensStr		= state->acumTokensCur->strBuff;
									tokenNameStart	= NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, op->iToken)->iAtBuff;
								}*/
								const SI32 iIdx = SrcAnlzMacros_findNameIdx(&state->macros, tokensStr, tokenNameStart);
								if(iIdx == -1){
									//MACRO not found (is a undefined IDENTIFIER)
									dst->type = STSrcAnlzValOpValType_IdNotMacro;
									dst->valIdNotDef = op->iToken;
									//PRINTF_INFO("Token is identifier: '%s'.\n", firstTokn);
								} else {
									//MACRO found (can be a FUNCTION or VALUE)
									const STSrcAnlzStrIdx* idx = NBArraySorted_itmPtrAtIndex(&state->macros.namesIdx, STSrcAnlzStrIdx, iIdx);
									dst->type			= STSrcAnlzValOpValType_Macro;
									dst->valMacroIdx	= idx->itmIndex;
									//PRINTF_INFO("Token is defined macro's name: '%s'.\n", firstTokn);
								}
							}
							//Operand are tokens (if not a MACRO nor IDENTIFIER)
							if(dst->type == STSrcAnlzValOpValType_Count){
								dst->type = STSrcAnlzValOpValType_Tokens;
								dst->valTokens.start = op->iToken;
								dst->valTokens.count = 1;
								//PRINTF_INFO("token('%s') is plain.\n", firstTokn);
							}
						}
					} else {
						NBASSERT(dst->type == STSrcAnlzValOpValType_Count)
						const STSrcAnlzExprOp* opDef = &op->sintaxMap->opChars[op->iCharDef];
						//PRINTF_INFO("Operator #%d: '%s'.\n", iOP, opDef->chars);
						if(op->sintax.iDef == -1){
							//operator should have a sintax
							PRINTF_ERROR("Operator-without-sintax #%d: '%s'.\n", iOP, opDef->chars);
							NBASSERT(FALSE)
							r = FALSE;
							break;
						} else {
							//explicit sintax definition
							const STSrcAnlzExprOpSintxMap*	sintaxMap = op->sintaxMap;
							const STSrcAnlzExprOpSintx* opSintax = &sintaxMap->opSintaxs[op->sintax.iDef];
							//PRINTF_INFO("Operator-with-sintax #%d: '%s'%s.\n", iOP, opDef->chars, (op->sintax.rCount > 0 ? " (pushing deeper level)": ""));
							NBASSERT(opSintax->sidesMask == ENSrcAnlzExprOpSideMask_Right) //TODO: add support to left-only operators
							//Get right result of this op
							STSrcAnlzValOpVal valR; valR.type = STSrcAnlzValOpValType_Count;
							if(!SrcAnlzCodeCParser_pprocCondExprResult(state, tokns, &valR, &iOP, ops, (op->sintax.rCount > 0 ? (iOP + op->sintax.rCount) : opsCount))){
								PRINTF_ERROR("Could not eval right side of operator #%d: '%s'.\n", iOP, opDef->chars);
								NBASSERT(FALSE)
								r = FALSE;
								break;
							} else {
								//Eval op wiht right operator
								if(!SrcAnlzCodeCParser_pprocCondExprEvalROnly(state, dst, &valR, opSintax->opType, opSintax->opEnumValue)){
									PRINTF_ERROR("Could not evaluate right-only operation '%s'.\n", opDef->chars);
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									//
								}
							}
						}
					}
				}
			}
		}
		NBArray_release(&opsIdxsPend);
		NBArray_release(&opsResults);
	}
	NBASSERT(r)
	*opIndex = iOP;
	return r;
}

BOOL SrcAnlzCodeCParser_pprocCondExprOpValAsBoolean(STSrcAnlzCodeCParser* state, BOOL* dst, const STSrcAnlzValOpVal* pVal, const STSrcAnlzEvalMode evalMode){
	BOOL r = FALSE;
	const STSrcAnlzValOpVal* val = pVal;
	//Process MACRO
	STSrcAnlzValOpVal localVal; localVal.type = STSrcAnlzValOpValType_Count;
	if(val->type == STSrcAnlzValOpValType_Macro){
		const STSrcAnlzMacro* macro = NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, val->valMacroIdx);
		if(macro->params.areExplicit){
			PRINTF_ERROR("MACRO is function-like while expecting value-only MACRO.\n");
			NBASSERT(FALSE)
			val = NULL;
		} else {
			NBASSERT(!macro->params.areExplicit)
			NBASSERT(macro->valParamsLinkedCount == 0)
			//Process the macro's value (no param is referenced inside the macro's value)
			if(!SrcAnlzCodeCParser_pprocMacroEvalNoParams(state, &localVal, macro, evalMode)){
				PRINTF_ERROR("Could not eval MACRO's direct value.\n");
				NBASSERT(FALSE)
				val = NULL;
			} else {
				val = &localVal;
			}
		}
	}
	//Eval as BOOLean
	if(val != NULL){
		switch(val->type) {
			case STSrcAnlzValOpValType_Bool: if(dst != NULL) *dst = val->valBool; r = TRUE; break;
			case STSrcAnlzValOpValType_Int: if(dst != NULL) *dst = (val->valIntt != 0); r = TRUE; break;
			case STSrcAnlzValOpValType_IntU: if(dst != NULL) *dst = (val->valIntU != 0); r = TRUE; break;
			case STSrcAnlzValOpValType_Long: if(dst != NULL) *dst = (val->valLong != 0); r = TRUE; break;
			case STSrcAnlzValOpValType_LongU: if(dst != NULL) *dst = (val->valLongU != 0); r = TRUE; break;
			case STSrcAnlzValOpValType_LongLong: if(dst != NULL) *dst = (val->valLongLong != 0); r = TRUE; break;
			case STSrcAnlzValOpValType_LongLongU: if(dst != NULL) *dst = (val->valLongLongU != 0); r = TRUE; break;
			case STSrcAnlzValOpValType_IdNotMacro: if(dst != NULL) *dst = FALSE; r = TRUE; break;
			default: NBASSERT(FALSE) break;
		}
	}
	return r;
}

#define NB_SRCANLZ_CONDITIONAL_EVAL_LR_LOGIC_OP(PTR_LEFT, PTR_RIGHT, PTR_DST, OP_ENUM_VAL, DATA_TYPE) \
	DATA_TYPE rL = 0, rR = 0; \
	/*Load left*/ \
	if(PTR_LEFT->type == STSrcAnlzValOpValType_Int) rL = (DATA_TYPE)PTR_LEFT->valIntt; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_IntU) rL = (DATA_TYPE)PTR_LEFT->valIntU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Long) rL = (DATA_TYPE)PTR_LEFT->valLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongU) rL = (DATA_TYPE)PTR_LEFT->valLongU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongLong) rL = (DATA_TYPE)PTR_LEFT->valLongLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongLongU) rL = (DATA_TYPE)PTR_LEFT->valLongLongU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Float) rL = (DATA_TYPE)PTR_LEFT->valFloat; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Double) rL = (DATA_TYPE)PTR_LEFT->valDouble; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_DoubleLong) rL = (DATA_TYPE)PTR_LEFT->valDoubleLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Bool) rL = (DATA_TYPE)(PTR_LEFT->valBool ? 1 : 0); \
	else { PRINTF_ERROR("RELATIONAL operators can only be aplied to integer, float, BOOLean and NotDefinedIdentifiers values\n"); NBASSERT(FALSE) break; } \
	/*Load right*/ \
	if(PTR_RIGHT->type == STSrcAnlzValOpValType_Int) rR = (DATA_TYPE)PTR_RIGHT->valIntt; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_IntU) rR = (DATA_TYPE)PTR_RIGHT->valIntU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Long) rR = (DATA_TYPE)PTR_RIGHT->valLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongU) rR = (DATA_TYPE)PTR_RIGHT->valLongU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongLong) rR = (DATA_TYPE)PTR_RIGHT->valLongLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongLongU) rR = (DATA_TYPE)PTR_RIGHT->valLongLongU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Float) rR = (DATA_TYPE)PTR_RIGHT->valFloat; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Double) rR = (DATA_TYPE)PTR_RIGHT->valDouble; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_DoubleLong) rR = (DATA_TYPE)PTR_RIGHT->valDoubleLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Bool) rR = (DATA_TYPE)(PTR_RIGHT->valBool ? 1 : 0); \
	else {  PRINTF_ERROR("RELATIONAL operators can only be aplied to integer, float, BOOLean and NotDefinedIdentifiers values\n"); NBASSERT(FALSE) break; } \
	/*Eval*/ \
	switch (OP_ENUM_VAL) { \
		case ENSrcAnlzSintxCOpRelat_Equal: PTR_DST->type = STSrcAnlzValOpValType_Bool; PTR_DST->valBool = (rL == rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpRelat_Ineq: PTR_DST->type = STSrcAnlzValOpValType_Bool; PTR_DST->valBool = (rL != rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpRelat_Great: PTR_DST->type = STSrcAnlzValOpValType_Bool; PTR_DST->valBool = (rL > rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpRelat_Lower: PTR_DST->type = STSrcAnlzValOpValType_Bool; PTR_DST->valBool = (rL < rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpRelat_GrtEq: PTR_DST->type = STSrcAnlzValOpValType_Bool; PTR_DST->valBool = (rL >= rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpRelat_LowEq: PTR_DST->type = STSrcAnlzValOpValType_Bool; PTR_DST->valBool = (rL <= rR); r = TRUE; break; \
		default: PRINTF_ERROR("Unexpected left+right RELATIONAL operation '%d'.\n", (SI32)OP_ENUM_VAL); NBASSERT(FALSE) break; \
	}


#define NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_FLOAT(PTR_LEFT, PTR_RIGHT, PTR_DST, OP_ENUM_VAL, DATA_TYPE, DATA_TYPE_ENUM, DATA_TYPE_VAR) \
	DATA_TYPE rL = 0, rR = 0; \
	/*Load left*/ \
	if(PTR_LEFT->type == STSrcAnlzValOpValType_Int) rL = (DATA_TYPE)PTR_LEFT->valIntt; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_IntU) rL = (DATA_TYPE)PTR_LEFT->valIntU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Long) rL = (DATA_TYPE)PTR_LEFT->valLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongU) rL = (DATA_TYPE)PTR_LEFT->valLongU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongLong) rL = (DATA_TYPE)PTR_LEFT->valLongLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongLongU) rL = (DATA_TYPE)PTR_LEFT->valLongLongU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Float) rL = (DATA_TYPE)PTR_LEFT->valFloat; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Double) rL = (DATA_TYPE)PTR_LEFT->valDouble; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_DoubleLong) rL = (DATA_TYPE)PTR_LEFT->valDoubleLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Bool) rL = (DATA_TYPE)(PTR_LEFT->valBool ? 1 : 0); \
	else { PRINTF_ERROR("RELATIONAL operators can only be aplied to integer, float, BOOLean and NotDefinedIdentifiers values\n"); NBASSERT(FALSE) break; } \
	/*Load right*/ \
	if(PTR_RIGHT->type == STSrcAnlzValOpValType_Int) rR = (DATA_TYPE)PTR_RIGHT->valIntt; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_IntU) rR = (DATA_TYPE)PTR_RIGHT->valIntU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Long) rR = (DATA_TYPE)PTR_RIGHT->valLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongU) rR = (DATA_TYPE)PTR_RIGHT->valLongU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongLong) rR = (DATA_TYPE)PTR_RIGHT->valLongLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongLongU) rR = (DATA_TYPE)PTR_RIGHT->valLongLongU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Float) rR = (DATA_TYPE)PTR_RIGHT->valFloat; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Double) rR = (DATA_TYPE)PTR_RIGHT->valDouble; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_DoubleLong) rR = (DATA_TYPE)PTR_RIGHT->valDoubleLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Bool) rR = (DATA_TYPE)(PTR_RIGHT->valBool ? 1 : 0); \
	else {  PRINTF_ERROR("RELATIONAL operators can only be aplied to integer, float, BOOLean and NotDefinedIdentifiers values\n"); NBASSERT(FALSE) break; } \
	/*Eval*/ \
	switch (OP_ENUM_VAL) { \
		case ENSrcAnlzSintxCOpArith_Add: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL + rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpArith_Sub: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL - rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpArith_Mul: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL * rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpArith_Div: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL / rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpArith_Mod: PRINTF_ERROR("'%%' operator cannot be applied to decimal value(s).\n"); NBASSERT(FALSE) break; \
		default: PRINTF_ERROR("Unexpected left+right ARITHMETIC operation '%d'.\n", (SI32)OP_ENUM_VAL); NBASSERT(FALSE) break; \
	}

#define NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_INT(PTR_LEFT, PTR_RIGHT, PTR_DST, OP_ENUM_VAL, DATA_TYPE, DATA_TYPE_ENUM, DATA_TYPE_VAR) \
	DATA_TYPE rL = 0, rR = 0; \
	/*Load left*/ \
	if(PTR_LEFT->type == STSrcAnlzValOpValType_Int) rL = (DATA_TYPE)PTR_LEFT->valIntt; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_IntU) rL = (DATA_TYPE)PTR_LEFT->valIntU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Long) rL = (DATA_TYPE)PTR_LEFT->valLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongU) rL = (DATA_TYPE)PTR_LEFT->valLongU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongLong) rL = (DATA_TYPE)PTR_LEFT->valLongLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongLongU) rL = (DATA_TYPE)PTR_LEFT->valLongLongU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Float) rL = (DATA_TYPE)PTR_LEFT->valFloat; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Double) rL = (DATA_TYPE)PTR_LEFT->valDouble; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_DoubleLong) rL = (DATA_TYPE)PTR_LEFT->valDoubleLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Bool) rL = (DATA_TYPE)(PTR_LEFT->valBool ? 1 : 0); \
	else { PRINTF_ERROR("ARITHMETIC operators can only be aplied to integer, float, BOOLean and NotDefinedIdentifiers values\n"); NBASSERT(FALSE) break; } \
	/*Load right*/ \
	if(PTR_RIGHT->type == STSrcAnlzValOpValType_Int) rR = (DATA_TYPE)PTR_RIGHT->valIntt; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_IntU) rR = (DATA_TYPE)PTR_RIGHT->valIntU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Long) rR = (DATA_TYPE)PTR_RIGHT->valLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongU) rR = (DATA_TYPE)PTR_RIGHT->valLongU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongLong) rR = (DATA_TYPE)PTR_RIGHT->valLongLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongLongU) rR = (DATA_TYPE)PTR_RIGHT->valLongLongU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Float) rR = (DATA_TYPE)PTR_RIGHT->valFloat; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Double) rR = (DATA_TYPE)PTR_RIGHT->valDouble; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_DoubleLong) rR = (DATA_TYPE)PTR_RIGHT->valDoubleLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Bool) rR = (DATA_TYPE)(PTR_RIGHT->valBool ? 1 : 0); \
	else {  PRINTF_ERROR("ARITHMETIC operators can only be aplied to integer, float, BOOLean and NotDefinedIdentifiers values\n"); NBASSERT(FALSE) break; } \
	/*Eval*/ \
	switch(OP_ENUM_VAL) { \
		case ENSrcAnlzSintxCOpArith_Add: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL + rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpArith_Sub: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL - rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpArith_Mul: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL * rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpArith_Div: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL / rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpArith_Mod: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL % rR); r = TRUE; break; \
		default: PRINTF_ERROR("Unexpected left+right ARITHMETIC operation '%d'.\n", (SI32)OP_ENUM_VAL); NBASSERT(FALSE) break; \
	}

#define NB_SRCANLZ_CONDITIONAL_EVAL_LR_BITWISE_OP(PTR_LEFT, PTR_RIGHT, PTR_DST, OP_ENUM_VAL, DATA_TYPE, DATA_TYPE_ENUM, DATA_TYPE_VAR) \
	DATA_TYPE rL = 0, rR = 0; \
	/*Load left*/ \
	if(PTR_LEFT->type == STSrcAnlzValOpValType_Int) rL = (DATA_TYPE)PTR_LEFT->valIntt; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_IntU) rL = (DATA_TYPE)PTR_LEFT->valIntU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Long) rL = (DATA_TYPE)PTR_LEFT->valLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongU) rL = (DATA_TYPE)PTR_LEFT->valLongU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongLong) rL = (DATA_TYPE)PTR_LEFT->valLongLong; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_LongLongU) rL = (DATA_TYPE)PTR_LEFT->valLongLongU; \
	else if(PTR_LEFT->type == STSrcAnlzValOpValType_Bool) rL = (DATA_TYPE)(PTR_LEFT->valBool ? 1 : 0); \
	else { PRINTF_ERROR("BITWISE operators can only be aplied to integer, BOOLean and NotDefinedIdentifiers values\n"); NBASSERT(FALSE) break; } \
	/*Load right*/ \
	if(PTR_RIGHT->type == STSrcAnlzValOpValType_Int) rR = (DATA_TYPE)PTR_RIGHT->valIntt; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_IntU) rR = (DATA_TYPE)PTR_RIGHT->valIntU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Long) rR = (DATA_TYPE)PTR_RIGHT->valLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongU) rR = (DATA_TYPE)PTR_RIGHT->valLongU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongLong) rR = (DATA_TYPE)PTR_RIGHT->valLongLong; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_LongLongU) rR = (DATA_TYPE)PTR_RIGHT->valLongLongU; \
	else if(PTR_RIGHT->type == STSrcAnlzValOpValType_Bool) rR = (DATA_TYPE)(PTR_RIGHT->valBool ? 1 : 0); \
	else {  PRINTF_ERROR("BITWISE operators can only be aplied to integer, BOOLean and NotDefinedIdentifiers values\n"); NBASSERT(FALSE) break; } \
	/*Eval*/ \
	switch (OP_ENUM_VAL) { \
		case ENSrcAnlzSintxCOpBitw_And: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL & rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpBitw_Or: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL | rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpBitw_Right: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL >> rR); r = TRUE; break; \
		case ENSrcAnlzSintxCOpBitw_Left: PTR_DST->type = DATA_TYPE_ENUM; PTR_DST->DATA_TYPE_VAR = (rL << rR); r = TRUE; break; \
		default: PRINTF_ERROR("Unexpected left+right BITWISE operation '%d'.\n", (SI32)OP_ENUM_VAL); NBASSERT(FALSE) break; \
	}

BOOL SrcAnlzCodeCParser_pprocCondExprEvalLR(STSrcAnlzCodeCParser* state, STSrcAnlzValOpVal* dst, const STSrcAnlzValOpVal* pLeft, const STSrcAnlzValOpVal* pRight, const SI32 opType, const SI32 opEnumValue){
	BOOL r = FALSE;
	NBASSERT(dst->type == STSrcAnlzValOpValType_Count)
	const STSrcAnlzValOpVal* left = pLeft;
	const STSrcAnlzValOpVal* right = pRight;
	//Translate MACROS identifiers (only if is not a function-like call)
	STSrcAnlzValOpVal localLeft, localRight; localLeft.type = localRight.type = STSrcAnlzValOpValType_Count;
	if(!(opType == ENSrcAnlzSintxCOpType_Prog && opEnumValue == ENSrcAnlzSintxCOpProg_GrpOpen)){
		//Translate left
		if(left != NULL){
			if(left->type == STSrcAnlzValOpValType_Macro){
				const STSrcAnlzMacro* macro = NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, left->valMacroIdx);
				if(macro->params.areExplicit){
					PRINTF_ERROR("LEFT MACRO is function-like while expecting value-only MACRO in L+R operation.\n");
					NBASSERT(FALSE)
					left = NULL;
				} else {
					NBASSERT(!macro->params.areExplicit)
					NBASSERT(macro->valParamsLinkedCount == 0)
					//Process the macro's value (no param is referenced inside the macro's value)
					if(!SrcAnlzCodeCParser_pprocMacroEvalNoParams(state, &localLeft, macro, STSrcAnlzEvalMode_Recursive)){
						PRINTF_ERROR("Could not eval LEFT MACRO's direct value in L+R operation.\n");
						NBASSERT(FALSE)
						left = NULL;
					} else {
						left = &localLeft;
					}
				}
			} else if(left->type == STSrcAnlzValOpValType_IdNotMacro){
				//In C, all undefined macros should be evaluated as '0'
				localLeft.type		= STSrcAnlzValOpValType_Int;
				localLeft.valIntt	= 0;
				left = &localLeft;
			}
		}
		//Translate right
		if(right != NULL){
			if(right->type == STSrcAnlzValOpValType_Macro){
				const STSrcAnlzMacro* macro = NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, right->valMacroIdx);
				if(macro->params.areExplicit){
					PRINTF_ERROR("MACRO is function-like while expecting value-only MACRO.\n");
					NBASSERT(FALSE)
					right = NULL;
				} else {
					NBASSERT(!macro->params.areExplicit)
					NBASSERT(macro->valParamsLinkedCount == 0)
					//Process the macro's value (no param is referenced inside the macro's value)
					if(!SrcAnlzCodeCParser_pprocMacroEvalNoParams(state, &localRight, macro, STSrcAnlzEvalMode_Recursive)){
						PRINTF_ERROR("Could not eval MACRO's direct value.\n");
						NBASSERT(FALSE)
						right = NULL;
					} else {
						right = &localRight;
					}
				}
			} else if(right->type == STSrcAnlzValOpValType_IdNotMacro){
				//In C, all undefined macros should be evaluated as '0'
				localRight.type		= STSrcAnlzValOpValType_Int;
				localRight.valIntt	= 0;
				right = &localLeft;
			}
		}
	}
	//
	if(left == NULL || right == NULL){
		PRINTF_ERROR("Missing operand(s) for LEFT+RIGHT operator.\n");
		NBASSERT(FALSE)
	} else {
		switch(opType) {
			case ENSrcAnlzSintxCOpType_Logical:
				switch (opEnumValue) {
					case ENSrcAnlzSintxCOpLogic_And:
					case ENSrcAnlzSintxCOpLogic_Or:
						{
							BOOL rL = FALSE, rR = FALSE;
							if(left->type == STSrcAnlzValOpValType_Bool) rL = left->valBool;
							else if(left->type == STSrcAnlzValOpValType_Int) rL = (left->valIntt != 0);
							else if(left->type == STSrcAnlzValOpValType_IntU) rL = (left->valIntU != 0);
							else if(left->type == STSrcAnlzValOpValType_Long) rL = (left->valLong != 0);
							else if(left->type == STSrcAnlzValOpValType_LongU) rL = (left->valLongU != 0);
							else if(left->type == STSrcAnlzValOpValType_LongLong) rL = (left->valLongLong != 0);
							else if(left->type == STSrcAnlzValOpValType_LongLongU) rL = (left->valLongLongU != 0);
							else { PRINTF_ERROR("LOGICAL operators can only be aplied to integer, BOOLean and NotDefinedIdentifiers values\n"); NBASSERT(FALSE) break; }
#							ifndef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
							//Early return (only when no debuging?)
							if((opEnumValue == ENSrcAnlzSintxCOpLogic_Or) == rL){ dst->type = STSrcAnlzValOpValType_Bool; dst->valBool = (opEnumValue == ENSrcAnlzSintxCOpLogic_Or); r = TRUE; break; }
#							endif
							//
							if(right->type == STSrcAnlzValOpValType_Bool) rR = right->valBool;
							else if(right->type == STSrcAnlzValOpValType_Int) rR = (right->valIntt != 0);
							else if(right->type == STSrcAnlzValOpValType_IntU) rR = (right->valIntU != 0);
							else if(right->type == STSrcAnlzValOpValType_Long) rR = (right->valLong != 0);
							else if(right->type == STSrcAnlzValOpValType_LongU) rR = (right->valLongU != 0);
							else if(right->type == STSrcAnlzValOpValType_LongLong) rR = (right->valLongLong != 0);
							else if(right->type == STSrcAnlzValOpValType_LongLongU) rR = (right->valLongLongU != 0);
							else {  PRINTF_ERROR("LOGICAL operators can only be aplied to integer, BOOLean and NotDefinedIdentifiers values\n"); NBASSERT(FALSE) break; }
							//
							dst->type = STSrcAnlzValOpValType_Bool; dst->valBool = (opEnumValue == ENSrcAnlzSintxCOpLogic_And ? (rL && rR) : (rL || rR));
							r = TRUE;
						}
						break;
					default:
						PRINTF_ERROR("Unexpected left+right LOGICAL operation '%d'.\n", (SI32)opEnumValue);
						NBASSERT(FALSE)
						break;
				}
				break;
			case ENSrcAnlzSintxCOpType_Relatnl:
				if(left->type == STSrcAnlzValOpValType_DoubleLong || right->type == STSrcAnlzValOpValType_DoubleLong){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_LOGIC_OP(left, right, dst, opEnumValue, long double);
				} else if(left->type == STSrcAnlzValOpValType_Double || right->type == STSrcAnlzValOpValType_Double){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_LOGIC_OP(left, right, dst, opEnumValue, double);
				} else if(left->type == STSrcAnlzValOpValType_Float || right->type == STSrcAnlzValOpValType_Float){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_LOGIC_OP(left, right, dst, opEnumValue, float);
				} else if(left->type == STSrcAnlzValOpValType_LongLongU || right->type == STSrcAnlzValOpValType_LongLongU){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_LOGIC_OP(left, right, dst, opEnumValue, long long unsigned);
				} else if(left->type == STSrcAnlzValOpValType_LongLong || right->type == STSrcAnlzValOpValType_LongLong){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_LOGIC_OP(left, right, dst, opEnumValue, long long);
				} else if(left->type == STSrcAnlzValOpValType_LongU || right->type == STSrcAnlzValOpValType_LongU){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_LOGIC_OP(left, right, dst, opEnumValue, long unsigned);
				} else if(left->type == STSrcAnlzValOpValType_Long || right->type == STSrcAnlzValOpValType_Long){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_LOGIC_OP(left, right, dst, opEnumValue, long);
				} else if(left->type == STSrcAnlzValOpValType_IntU || right->type == STSrcAnlzValOpValType_IntU){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_LOGIC_OP(left, right, dst, opEnumValue, int unsigned);
				} else {
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_LOGIC_OP(left, right, dst, opEnumValue, int);
				}
				break;
			case ENSrcAnlzSintxCOpType_Arithm:
				if(left->type == STSrcAnlzValOpValType_DoubleLong || right->type == STSrcAnlzValOpValType_DoubleLong){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_FLOAT(left, right, dst, opEnumValue, long double, STSrcAnlzValOpValType_DoubleLong, valDoubleLong);
				} else if(left->type == STSrcAnlzValOpValType_Double || right->type == STSrcAnlzValOpValType_Double){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_FLOAT(left, right, dst, opEnumValue, double, STSrcAnlzValOpValType_Double, valDouble);
				} else if(left->type == STSrcAnlzValOpValType_Float || right->type == STSrcAnlzValOpValType_Float){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_FLOAT(left, right, dst, opEnumValue, float, STSrcAnlzValOpValType_Float, valFloat);
				} else if(left->type == STSrcAnlzValOpValType_LongLongU || right->type == STSrcAnlzValOpValType_LongLongU){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_INT(left, right, dst, opEnumValue, long long unsigned, STSrcAnlzValOpValType_LongLongU, valLongLongU);
				} else if(left->type == STSrcAnlzValOpValType_LongLong || right->type == STSrcAnlzValOpValType_LongLong){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_INT(left, right, dst, opEnumValue, long long, STSrcAnlzValOpValType_LongLong, valLongLong);
				} else if(left->type == STSrcAnlzValOpValType_LongU || right->type == STSrcAnlzValOpValType_LongU){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_INT(left, right, dst, opEnumValue, long unsigned, STSrcAnlzValOpValType_LongU, valLongU);
				} else if(left->type == STSrcAnlzValOpValType_Long || right->type == STSrcAnlzValOpValType_Long){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_INT(left, right, dst, opEnumValue, long, STSrcAnlzValOpValType_Long, valLong);
				} else if(left->type == STSrcAnlzValOpValType_IntU || right->type == STSrcAnlzValOpValType_IntU){
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_INT(left, right, dst, opEnumValue, int unsigned, STSrcAnlzValOpValType_IntU, valIntU);
				} else {
					NB_SRCANLZ_CONDITIONAL_EVAL_LR_ARITHM_OP_INT(left, right, dst, opEnumValue, int, STSrcAnlzValOpValType_Int, valIntt);
				}
				break;
			case ENSrcAnlzSintxCOpType_Bitwise:
				{
					if(left->type == STSrcAnlzValOpValType_LongLongU || right->type == STSrcAnlzValOpValType_LongLongU){
						NB_SRCANLZ_CONDITIONAL_EVAL_LR_BITWISE_OP(left, right, dst, opEnumValue, long long unsigned, STSrcAnlzValOpValType_LongLongU, valLongLongU);
					} else if(left->type == STSrcAnlzValOpValType_LongLong || right->type == STSrcAnlzValOpValType_LongLong){
						NB_SRCANLZ_CONDITIONAL_EVAL_LR_BITWISE_OP(left, right, dst, opEnumValue, long long, STSrcAnlzValOpValType_LongLong, valLongLong);
					} else if(left->type == STSrcAnlzValOpValType_LongU || right->type == STSrcAnlzValOpValType_LongU){
						NB_SRCANLZ_CONDITIONAL_EVAL_LR_BITWISE_OP(left, right, dst, opEnumValue, long unsigned, STSrcAnlzValOpValType_LongU, valLongU);
					} else if(left->type == STSrcAnlzValOpValType_Long || right->type == STSrcAnlzValOpValType_Long){
						NB_SRCANLZ_CONDITIONAL_EVAL_LR_BITWISE_OP(left, right, dst, opEnumValue, long, STSrcAnlzValOpValType_Long, valLong);
					} else if(left->type == STSrcAnlzValOpValType_IntU || right->type == STSrcAnlzValOpValType_IntU){
						NB_SRCANLZ_CONDITIONAL_EVAL_LR_BITWISE_OP(left, right, dst, opEnumValue, int unsigned, STSrcAnlzValOpValType_IntU, valIntU);
					} else {
						NB_SRCANLZ_CONDITIONAL_EVAL_LR_BITWISE_OP(left, right, dst, opEnumValue, int, STSrcAnlzValOpValType_Int, valIntt);
					}
				}
				break;
			case ENSrcAnlzSintxCOpType_Prog:
				switch (opEnumValue) {
					case ENSrcAnlzSintxCOpProg_GrpOpen:
						//Function (left) + params (right)
						if(left->type == STSrcAnlzValOpValType_Macro){
							NBASSERT(left->valMacroIdx < state->macros.defs.use)
							const STSrcAnlzMacro* macro = NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, left->valMacroIdx);
							if(!macro->params.areExplicit){
								PRINTF_ERROR("MACRO is not function-like at left+right (FUNC+PARAM) operation '%d'.\n", (SI32)opEnumValue);
								NBASSERT(FALSE)
							} else {
								if(macro->valParamsLinkedCount == 0){
									//Process the macro's value (no param is referenced inside the macro's value)
									if(!SrcAnlzCodeCParser_pprocMacroEvalNoParams(state, dst, macro, STSrcAnlzEvalMode_Recursive)){
										PRINTF_ERROR("Could not eval MACRO's direct value at left+right (FUNC+PARAM) operation '%d'.\n", (SI32)opEnumValue);
										NBASSERT(FALSE)
									} else {
										r = TRUE;
									}
								} else {
									//Build a new parsing string replacing any param reference with the param's value.
									NBASSERT(FALSE)
								}
							}
						} else {
							PRINTF_ERROR("Unexpected left+right FUNC+PARAM operation '%d' (expeting a defined function-like MACRO).\n", (SI32)opEnumValue);
							NBASSERT(FALSE)
						}
						break;
						/*case ENSrcAnlzSintxCOpProg_GrpClose:
						 break;*/
					default:
						PRINTF_ERROR("Unexpected left+right PROGRAMMING operation '%d'.\n", (SI32)opEnumValue);
						NBASSERT(FALSE)
						break;
				}
				break;
			default:
				PRINTF_ERROR("Inexpected left+right operation type '%d'.\n", (SI32)opType);
				NBASSERT(FALSE)
				break;
		}
	}
	//
#	ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
	/*if(r && dst != NULL){
		switch(dst->type){
			case STSrcAnlzValOpValType_Bool: PRINTF_INFO("Operation left+right resulted: %s.\n", dst->valBool ? "TRUE" : "FALSE"); break;
			case STSrcAnlzValOpValType_Int: PRINTF_INFO("Operation left+right resulted: %d.\n", dst->valIntt); break;
			case STSrcAnlzValOpValType_IntU: PRINTF_INFO("Operation left+right resulted: %u.\n", dst->valIntU); break;
			case STSrcAnlzValOpValType_Long: PRINTF_INFO("Operation left+right resulted: %ld.\n", dst->valLong); break;
			case STSrcAnlzValOpValType_LongU: PRINTF_INFO("Operation left+right resulted: %lu.\n", dst->valLongU); break;
			case STSrcAnlzValOpValType_LongLong: PRINTF_INFO("Operation left+right resulted: %lld.\n", dst->valLongLong); break;
			case STSrcAnlzValOpValType_LongLongU: PRINTF_INFO("Operation left+right resulted: %llu.\n", dst->valLongLongU); break;
			case STSrcAnlzValOpValType_Float: PRINTF_INFO("Operation left+right resulted: %f.\n", dst->valFloat); break;
			case STSrcAnlzValOpValType_Double: PRINTF_INFO("Operation left+right resulted: %f.\n", dst->valDouble); break;
			case STSrcAnlzValOpValType_DoubleLong: PRINTF_INFO("Operation left+right resulted: %Lf.\n", dst->valDoubleLong); break;
			case STSrcAnlzValOpValType_Tokens: PRINTF_INFO("Operation left+right resulted: tokens(%d -> %d).\n", (SI32)dst->valTokens.start,  ((SI32)dst->valTokens.start +  (SI32)dst->valTokens.count - 1)); break;
			case STSrcAnlzValOpValType_IdNotMacro: PRINTF_INFO("Operation left+right resulted: idNotDef(%d).\n", dst->valIdNotDef); break;
			case STSrcAnlzValOpValType_Macro: PRINTF_INFO("Operation left+right resulted: macro(%d).\n", dst->valMacroIdx); break;
			default: NBASSERT(FALSE);
		}
	}*/
#	endif
	return r;
}

BOOL SrcAnlzCodeCParser_pprocCondExprEvalROnly(STSrcAnlzCodeCParser* state, STSrcAnlzValOpVal* dst, const STSrcAnlzValOpVal* pRight, const SI32 opType, const SI32 opEnumValue){
	BOOL r = FALSE;
	NBASSERT(dst->type == STSrcAnlzValOpValType_Count)
	const STSrcAnlzValOpVal* right = pRight;
	//Translate MACROS identifiers (only if is not a function-like call)
	STSrcAnlzValOpVal localRight; localRight.type = STSrcAnlzValOpValType_Count;
	if(!(opType == ENSrcAnlzSintxCOpType_Preproc && opEnumValue == ENSrcAnlzSintxCOpPProc_def) && !(opType == ENSrcAnlzSintxCOpType_Prog && opEnumValue == ENSrcAnlzSintxCOpProg_GrpOpen)){
		if(right != NULL){
			if(right->type == STSrcAnlzValOpValType_Macro){
				const STSrcAnlzMacro* macro = NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, right->valMacroIdx);
				//only translate if macro is function-like, do not translate.
				if(!macro->params.areExplicit){
					if(!SrcAnlzCodeCParser_pprocMacroEvalNoParams(state, &localRight, macro, STSrcAnlzEvalMode_Recursive)){
						PRINTF_ERROR("Could not eval MACRO's direct value.\n");
						NBASSERT(FALSE)
						right = NULL;
					} else {
						right = &localRight;
					}
				}
			} else if(right->type == STSrcAnlzValOpValType_IdNotMacro){
				//In C, all undefined macros should be evaluated as '0'
				localRight.type		= STSrcAnlzValOpValType_Int;
				localRight.valIntt	= 0;
				right = &localRight;
			}
		}
	}
	//Eval
	switch(opType) {
		case ENSrcAnlzSintxCOpType_Logical:
			switch (opEnumValue) {
				case ENSrcAnlzSintxCOpLogic_Neg:
					{
						BOOL rR = FALSE;
						if(right->type == STSrcAnlzValOpValType_Bool) rR = right->valBool;
						else if(right->type == STSrcAnlzValOpValType_Int) rR = (right->valIntt != 0);
						else if(right->type == STSrcAnlzValOpValType_IntU) rR = (right->valIntU != 0);
						else if(right->type == STSrcAnlzValOpValType_Long) rR = (right->valLong != 0);
						else if(right->type == STSrcAnlzValOpValType_LongU) rR = (right->valLongU != 0);
						else if(right->type == STSrcAnlzValOpValType_LongLong) rR = (right->valLongLong != 0);
						else if(right->type == STSrcAnlzValOpValType_LongLongU) rR = (right->valLongLongU != 0);
						else {  PRINTF_ERROR("LOGICAL operators can only be aplied to integer and BOOLean values\n"); NBASSERT(FALSE) break; }
						dst->type = STSrcAnlzValOpValType_Bool; dst->valBool = (!rR);
						r = TRUE;
					}
					break;
				default:
					PRINTF_ERROR("Unexpected left+right LOGICAL operation '%d'.\n", (SI32)opEnumValue);
					NBASSERT(FALSE)
					break;
			}
			break;
		case ENSrcAnlzSintxCOpType_Arithm:
			switch (opEnumValue) {
				case ENSrcAnlzSintxCOpArith_Add:
					if(right->type == STSrcAnlzValOpValType_Int){ dst->type = STSrcAnlzValOpValType_Int; dst->valIntt = right->valIntt; r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_IntU) { dst->type = STSrcAnlzValOpValType_IntU; dst->valIntU = right->valIntU; r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_Long) { dst->type = STSrcAnlzValOpValType_Long; dst->valLong = right->valLong; r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_LongU) { dst->type = STSrcAnlzValOpValType_LongU; dst->valLongU = right->valLongU; r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_LongLong) { dst->type = STSrcAnlzValOpValType_LongLong; dst->valLongLong = right->valLongLong; r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_LongLongU) { dst->type = STSrcAnlzValOpValType_LongLongU; dst->valLongLongU = right->valLongLongU; r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_Float) { dst->type = STSrcAnlzValOpValType_Float; dst->valFloat = right->valFloat; r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_Double) { dst->type = STSrcAnlzValOpValType_Double; dst->valDouble = right->valDouble; r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_DoubleLong) { dst->type = STSrcAnlzValOpValType_DoubleLong; dst->valDoubleLong = right->valDoubleLong; r = TRUE; }
					else { PRINTF_ERROR("ARITHMETIC POSITIVE SIGN operator can only be aplied to integer and decimal values\n"); NBASSERT(FALSE) }
					break;
				case ENSrcAnlzSintxCOpArith_Sub:
					if(right->type == STSrcAnlzValOpValType_Int){ dst->type = STSrcAnlzValOpValType_Int; dst->valIntt = (-right->valIntt); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_IntU) { dst->type = STSrcAnlzValOpValType_IntU; dst->valIntU = (-right->valIntU); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_Long) { dst->type = STSrcAnlzValOpValType_Long; dst->valLong = (-right->valLong); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_LongU) { dst->type = STSrcAnlzValOpValType_LongU; dst->valLongU = (-right->valLongU); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_LongLong) { dst->type = STSrcAnlzValOpValType_LongLong; dst->valLongLong = (-right->valLongLong); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_LongLongU) { dst->type = STSrcAnlzValOpValType_LongLongU; dst->valLongLongU = (-right->valLongLongU); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_Float) { dst->type = STSrcAnlzValOpValType_Float; dst->valFloat = (-right->valFloat); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_Double) { dst->type = STSrcAnlzValOpValType_Double; dst->valDouble = (-right->valDouble); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_DoubleLong) { dst->type = STSrcAnlzValOpValType_DoubleLong; dst->valDoubleLong = (-right->valDoubleLong); r = TRUE; }
					else { PRINTF_ERROR("ARITHMETIC NEGATIVE SIGN operator can only be aplied to integer and decimal values\n"); NBASSERT(FALSE) }
					break;
				default:
					PRINTF_ERROR("Unexpected right-only ARITHMETIC operation '%d'.\n", (SI32)opEnumValue);
					NBASSERT(FALSE)
					break;
			}
			break;
		case ENSrcAnlzSintxCOpType_Bitwise:
			switch (opEnumValue) {
					/*case ENSrcAnlzSintxCOpBitw_Xor:
					 if(right->type == STSrcAnlzValOpValType_Integer){
					 dst->type = STSrcAnlzValOpValType_Integer; dst->valInt = (^right->valInt); r = TRUE;
					 } else {
					 PRINTF_ERROR("BITWISE XOR operator can only be aplied to integer values\n"); NBASSERT(FALSE)
					 }
					 break;*/
				case ENSrcAnlzSintxCOpBitw_Comp:
					if(right->type == STSrcAnlzValOpValType_Int){ dst->type = STSrcAnlzValOpValType_Int; dst->valIntt = (~right->valIntt); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_IntU) { dst->type = STSrcAnlzValOpValType_IntU; dst->valIntU = (~right->valIntU); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_Long) { dst->type = STSrcAnlzValOpValType_Long; dst->valLong = (~right->valLong); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_LongU) { dst->type = STSrcAnlzValOpValType_LongU; dst->valLongU = (~right->valLongU); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_LongLong) { dst->type = STSrcAnlzValOpValType_LongLong; dst->valLongLong = (~right->valLongLong); r = TRUE; }
					else if(right->type == STSrcAnlzValOpValType_LongLongU) { dst->type = STSrcAnlzValOpValType_LongLongU; dst->valLongLongU = (~right->valLongLongU); r = TRUE; }
					else { PRINTF_ERROR("BITWISE COMPLEMENT operator can only be aplied to integer values\n"); NBASSERT(FALSE) }
					break;
				default:
					PRINTF_ERROR("Unexpected right-only BITWISE operation '%d'.\n", (SI32)opEnumValue);
					NBASSERT(FALSE)
					break;
			}
			break;
		case ENSrcAnlzSintxCOpType_Prog:
			switch (opEnumValue) {
				case ENSrcAnlzSintxCOpProg_GrpOpen:
					*dst = *right; //Return the same right side value
					r = TRUE;
					break;
					/*case ENSrcAnlzSintxCOpProg_GrpClose:
					 break;*/
				default:
					PRINTF_ERROR("Unexpected right-only PROGRAMMING operation '%d'.\n", (SI32)opEnumValue);
					NBASSERT(FALSE)
					break;
			}
			break;
		case ENSrcAnlzSintxCOpType_Preproc:
			switch (opEnumValue) {
				case ENSrcAnlzSintxCOpPProc_def:
					if(right->type == STSrcAnlzValOpValType_Macro){
						dst->type = STSrcAnlzValOpValType_Bool; dst->valBool = TRUE; r = TRUE;
						//PRINTF_INFO("defined retuned TRUE.\n")
					} else if(right->type == STSrcAnlzValOpValType_IdNotMacro) {
						dst->type = STSrcAnlzValOpValType_Bool; dst->valBool = FALSE; r = TRUE;
						//PRINTF_INFO("defined retuned FALSE.\n")
					} else {
						PRINTF_ERROR("PREPROCESOR DEFINED operator can only be aplied to MACRO-names and non-literal-tokens.\n"); NBASSERT(FALSE)
					}
					break;
				default:
					PRINTF_ERROR("Unexpected right-only PREPROCESOR operation '%d'.\n", (SI32)opEnumValue);
					NBASSERT(FALSE)
					break;
			}
			break;
		default:
			PRINTF_ERROR("Inexpected only ight operation type '%d'.\n", (SI32)opType);
			NBASSERT(FALSE)
			break;
	}
#	ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
	/*if(r && dst != NULL){
		switch(dst->type){
			case STSrcAnlzValOpValType_Bool: PRINTF_INFO("Operation right-only resulted: %s.\n", dst->valBool ? "TRUE" : "FALSE"); break;
			case STSrcAnlzValOpValType_Int: PRINTF_INFO("Operation right-only resulted: %d.\n", dst->valIntt); break;
			case STSrcAnlzValOpValType_IntU: PRINTF_INFO("Operation right-only resulted: %u.\n", dst->valIntU); break;
			case STSrcAnlzValOpValType_Long: PRINTF_INFO("Operation right-only resulted: %ld.\n", dst->valLong); break;
			case STSrcAnlzValOpValType_LongU: PRINTF_INFO("Operation right-only resulted: %lu.\n", dst->valLongU); break;
			case STSrcAnlzValOpValType_LongLong: PRINTF_INFO("Operation right-only resulted: %lld.\n", dst->valLongLong); break;
			case STSrcAnlzValOpValType_LongLongU: PRINTF_INFO("Operation right-only resulted: %llu.\n", dst->valLongLongU); break;
			case STSrcAnlzValOpValType_Float: PRINTF_INFO("Operation right-only resulted: %f.\n", dst->valFloat); break;
			case STSrcAnlzValOpValType_Double: PRINTF_INFO("Operation right-only resulted: %f.\n", dst->valDouble); break;
			case STSrcAnlzValOpValType_DoubleLong: PRINTF_INFO("Operation right-only resulted: %Lf.\n", dst->valDoubleLong); break;
			case STSrcAnlzValOpValType_Tokens: PRINTF_INFO("Operation right-only resulted: tokens(%d -> %d).\n", (SI32)dst->valTokens.start,  ((SI32)dst->valTokens.start +  (SI32)dst->valTokens.count - 1)); break;
			case STSrcAnlzValOpValType_IdNotMacro: PRINTF_INFO("Operation right-only resulted: idNotDef(%d).\n", dst->valIdNotDef); break;
			case STSrcAnlzValOpValType_Macro: PRINTF_INFO("Operation right-only resulted: macro(%d).\n", dst->valMacroIdx); break;
			default: NBASSERT(FALSE);
		}
	}*/
#	endif
	return r;
}

//
//Macros evaluations
//

BOOL SrcAnlzCodeCParser_pprocMacroEvalNoParams(STSrcAnlzCodeCParser* state, STSrcAnlzValOpVal* dst, const STSrcAnlzMacro* macro, const STSrcAnlzEvalMode evalMode){
	BOOL r = FALSE;
	if(state != NULL && macro != NULL){
		//
		//MACRO value
		//
		STNBArray ops; NBArray_init(&ops, sizeof(STSrcAnlzExprParseNode), NULL);
		if(!SrcAnlzExprParser_feedTokns(&state->macros.valsTokns.tokns, &__globalPProcCondExprSintaxMap, macro->valTokensFirst, (macro->valTokensFirst + macro->valTokensCount), &ops)){
			PRINTF_ERROR("Could not parse MACRO's value sintax.\n");
			NBASSERT(FALSE)
		} else {
#			ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
			//Print parsed macro value (as content of a conditional)
			{
				STNBString strTmp; NBString_init(&strTmp);
				SI32 iOp = 0; SrcAnlzExprParser_dbgAddOpsToStr(&state->macros.valsTokns.tokns, &strTmp, &iOp, (const STSrcAnlzExprParseNode*)ops._buffData, ops.use, FALSE);
				//PRINTF_INFO("Operations parsed from MACRO: '%s'.\n", strTmp.str);
				NBASSERT(iOp == ops.use)
				NBString_release(&strTmp);
			}
#			endif
			//Eval conditional value
			SI32 iOp = 0;
			if(!SrcAnlzCodeCParser_pprocCondExprResult(state, &state->macros.valsTokns.tokns, dst, &iOp, (const STSrcAnlzExprParseNode*)ops._buffData, ops.use)){
				PRINTF_ERROR("Could not eval MACRO value.\n");
				NBASSERT(FALSE)
			} else {
				if(dst->type == STSrcAnlzValOpValType_Macro){
					const STSrcAnlzMacro* subMacro = NBArray_itmPtrAtIndex(&state->macros.defs, STSrcAnlzMacro, dst->valMacroIdx);
					if(macro->params.areExplicit){
						PRINTF_ERROR("SUB MACRO is function-like while expecting value-only in parent MACRO eval.\n");
						NBASSERT(FALSE)
					} else {
						NBASSERT(!macro->params.areExplicit)
						NBASSERT(macro->valParamsLinkedCount == 0)
						//Process the macro's value (no param is referenced inside the macro's value)
						STSrcAnlzValOpVal localVal; localVal.type = STSrcAnlzValOpValType_Count;
						if(!SrcAnlzCodeCParser_pprocMacroEvalNoParams(state, &localVal, subMacro, STSrcAnlzEvalMode_Recursive)){
							PRINTF_ERROR("Could not eval LEFT MACRO's direct value in L+R operation.\n");
							NBASSERT(FALSE)
						} else {
							if(dst != NULL) *dst = localVal;
							r = TRUE;
						}
					}
				} else {
					r = TRUE;
				}
			}
		}
		NBArray_release(&ops);
	}
	NBASSERT(r)
	return r;
}

//
//Debug
//

#ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
void SrcAnlzCodeCParser_pprocDbgAddfTokensToStr(const STSrcAnlzCodeCParser* state, STNBString* dstStr, const SI32 iFirstTokn, const BOOL noSpaces, const BOOL oneDepthLess){
	NBASSERT(state->pprocCondPortionsStack.use > 0);
	if(!noSpaces){
		SI32 i; SI32 blkDepth = state->pprocCondPortionsStack.use - (oneDepthLess ? 1 : 0);
		for(i = 1; i < blkDepth; i++){
			NBString_concatSI32(dstStr, (SI32)i);
			NBString_concatByte(dstStr, '-');
		}
	}
	SI32 i; const SI32 count = state->acumTokensCur->defs.use;
	for(i = iFirstTokn; i < count; i++){
		const STSrcAnlzTokn* tokensDef = NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, i);
		if(i != 0 && tokensDef->isSpacePreceded && !noSpaces) NBString_concatByte(dstStr, ' ');
		NBString_concat(dstStr, &state->acumTokensCur->strBuff.str[tokensDef->iAtBuff]);
	}
}
#endif

#ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
void SrcAnlzCodeCParser_pprocDbgPrintfTokens(const STSrcAnlzCodeCParser* state, const BOOL curCodePortionActive, const BOOL oneDepthLess){
	STNBString strTmp; NBString_init(&strTmp);
	SrcAnlzCodeCParser_pprocDbgAddfTokensToStr(state, &strTmp, 0, FALSE, oneDepthLess);
	PRINTF_INFO("Preprocessor (%s): '%s'.\n", (curCodePortionActive ? "on" : "off"), strTmp.str);
	NBString_release(&strTmp);
}
#endif






