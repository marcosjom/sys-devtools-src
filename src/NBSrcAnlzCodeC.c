//
//  NBSrcAnlzCodeC.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzCodeC.h"

//
//C-source code operators sintax rules
//

//Code blocks (to ignore)

/*typedef enum ENSrcAnlzOpCodeBlockChars_ {
	ENSrcAnlzOpCodeBlockChars_GrpOpen = 0,	// '('
	//ENSrcAnlzOpCodeBlockChars_GrpClose,	// ')'
	ENSrcAnlzOpCodeBlockChars_IdxOpen,		// '['
	//ENSrcAnlzOpCodeBlockChars_IdxClose,	// ']'
	ENSrcAnlzOpCodeBlockChars_BlckOpen,		// '{'
	//ENSrcAnlzOpCodeBlockChars_BlckClose,	// '}'
	ENSrcAnlzOpCodeBlockChars_Count
} ENSrcAnlzOpCodeBlockChars;*/

/*typedef enum ENSrcAnlzOpCodeBlockSintax_ {
	ENSrcAnlzOpCodeBlockSintax_GrpOpen = 0,	// '('
	//ENSrcAnlzOpCodeBlockSintax_GrpClose,	// ')'
	ENSrcAnlzOpCodeBlockSintax_IdxOpen,		// '['
	//ENSrcAnlzOpCodeBlockSintax_IdxClose,	// ']'
	ENSrcAnlzOpCodeBlockSintax_BlckOpen,	// '{'
	//ENSrcAnlzOpCodeBlockSintax_BlckClose,	// '}'
	ENSrcAnlzOpCodeBlockSintax_Count
} ENSrcAnlzOpCodeBlockSintax;*/

/*static const SI32 __globalOpCodeBlockSintaxChar1[] = {
	ENSrcAnlzOpCodeBlockSintax_GrpOpen
	//, ENSrcAnlzOpCodeBlockSintax_GrpClose
	, ENSrcAnlzOpCodeBlockSintax_IdxOpen
	//, ENSrcAnlzOpCodeBlockSintax_IdxClose
	, ENSrcAnlzOpCodeBlockSintax_BlckOpen
	//, ENSrcAnlzOpCodeBlockSintax_BlckClose
};*/

/*static const STSrcAnlzExprOp __globalOpCodeBlockDefs[] = {
	//One char
	{ ENSrcAnlzOpCodeBlockChars_GrpOpen,		{'(', NULL}, {NULL, NULL},	{')', NULL},	ENSrcAnlzOpCodeBlockSintax_GrpOpen, -1 }
	//, { ENSrcAnlzOpCodeBlockChars_GrpClose,	{')', NULL}, {'(', NULL},	{NULL, NULL},	ENSrcAnlzOpCodeBlockSintax_GrpClose, -1 }
	, { ENSrcAnlzOpCodeBlockChars_IdxOpen,		{'[', NULL}, {NULL, NULL},	{']', NULL},	ENSrcAnlzOpCodeBlockSintax_IdxOpen, -1 }
	//, { ENSrcAnlzOpCodeBlockChars_IdxClose,	{']', NULL}, {'[', NULL},	{NULL, NULL},	ENSrcAnlzOpCodeBlockSintax_IdxClose, -1 }
	, { ENSrcAnlzOpCodeBlockChars_BlckOpen,		{'{', NULL}, {NULL, NULL},	{'}', NULL},	ENSrcAnlzOpCodeBlockSintax_BlckOpen, -1 }
	//, { ENSrcAnlzOpCodeBlockChars_BlckClose,	{'}', NULL}, {'{', NULL},	{NULL, NULL},	ENSrcAnlzOpCodeBlockSintax_BlckClose, -1 }
	//Others
};*/

/*static const STSrcAnlzExprOpSintx __globalOpCodeBlockSintax[] = {
	{ ENSrcAnlzOpCodeBlockSintax_GrpOpen,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,	ENSrcAnlzSintxCOp_GrpOpen, ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpOpen, "ProgGrpOpen" }	// '('
	//, { ENSrcAnlzOpCodeBlockSintax_GrpClose,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,	ENSrcAnlzSintxCOp_GrpClose, ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpClose, "ProgGrpClose" }	// ')'
	, { ENSrcAnlzOpCodeBlockChars_IdxOpen,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,	ENSrcAnlzSintxCOp_IdxOpen, ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_IdxOpen, "ProgIdxOpen" }	// '['
	//, { ENSrcAnlzOpCodeBlockChars_IdxClose,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,	ENSrcAnlzSintxCOp_IdxClose, ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_IdxClose, "ProgIdxClose" }	// ']'
	, { ENSrcAnlzOpCodeBlockChars_BlckOpen,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,	ENSrcAnlzSintxCOp_BlckOpen, ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_BlckOpen, "ProgBlckOpen" }	// '{'
	//, { ENSrcAnlzOpCodeBlockChars_BlckClose,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,	ENSrcAnlzSintxCOp_BlckClose, ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_BlckClose, "ProgBlckClose" }	// '}'
};*/

/*const STSrcAnlzExprOpSintxMap __globalCodeBlockSintaxMap = {
	//OpChars
	__globalOpCodeBlockDefs, (sizeof(__globalOpCodeBlockDefs) / sizeof(__globalOpCodeBlockDefs[0])) //ENSrcAnlzOpCodeBlockChars_Count
	//OpSintaxs
	, __globalOpCodeBlockSintax, (sizeof(__globalOpCodeBlockSintax) / sizeof(__globalOpCodeBlockSintax[0])) //ENSrcAnlzOpCodeBlockSintax_Count
	//Index by op lenghts
	, __globalOpCodeBlockSintaxChar1, (sizeof(__globalOpCodeBlockSintaxChar1) / sizeof(__globalOpCodeBlockSintaxChar1[0]))
	, NULL, 0
	, NULL, 0
	, NULL, 0
};*/

// Code expresions

/*typedef enum ENSrcAnlzOpCodeChars_ {
	//One char
	ENSrcAnlzOpCodeChars_GrpOpen = 0,	// '('
	//ENSrcAnlzOpCodeChars_GrpClose,	// ')'
	ENSrcAnlzOpCodeChars_IdxOpen,		// '[' //new
	//ENSrcAnlzOpCodeChars_IdxClose,	// ']' //new
	ENSrcAnlzOpCodeChars_Point,			// '.' //new
	ENSrcAnlzOpCodeChars_Exclamation,	// '!'
	ENSrcAnlzOpCodeChars_Ampersand,		// '&'
	ENSrcAnlzOpCodeChars_VertBar,		// '|'
	ENSrcAnlzOpCodeChars_GreatThan,		// '>'
	ENSrcAnlzOpCodeChars_LessThan,		// '<'
	ENSrcAnlzOpCodeChars_Plus,			// '+'
	ENSrcAnlzOpCodeChars_Minus,			// '-'
	ENSrcAnlzOpCodeChars_Asterisk,		// '*'
	ENSrcAnlzOpCodeChars_Slash,			// '/'
	ENSrcAnlzOpCodeChars_Percent,		// '%'
	ENSrcAnlzOpCodeChars_Tilde,			// '~'
	ENSrcAnlzOpCodeChars_Caret,			// '^'
	ENSrcAnlzOpCodeChars_CondStart,		// '?' //new
	ENSrcAnlzOpCodeChars_CondMid,		// ':' //new
	ENSrcAnlzOpCodeChars_Assign,		// '=' //new
	ENSrcAnlzOpCodeChars_Comma,			// ',' //new
	//Two chars
	ENSrcAnlzOpCodeChars_Arrow,			// '->' //new
	ENSrcAnlzOpCodeChars_PlusPlus,		// '++' //new
	ENSrcAnlzOpCodeChars_MinusMinus,	// '--' //new
	ENSrcAnlzOpCodeChars_Ampersand2,	// '&&'
	ENSrcAnlzOpCodeChars_VertBar2,		// '||'
	ENSrcAnlzOpCodeChars_Equals2,		// '=='
	ENSrcAnlzOpCodeChars_Inequal,		// '!='
	ENSrcAnlzOpCodeChars_GreatEqual,	// '>='
	ENSrcAnlzOpCodeChars_LessEqual,		// '<='
	ENSrcAnlzOpCodeChars_GreatThan2,	// '>>'
	ENSrcAnlzOpCodeChars_LessThan2,		// '<<'
	ENSrcAnlzOpCodeChars_AssignPlus,	// '+=' //new
	ENSrcAnlzOpCodeChars_AssignLess,	// '-=' //new
	ENSrcAnlzOpCodeChars_AssignMult,	// '*=' //new
	ENSrcAnlzOpCodeChars_AssignDiv,		// '/=' //new
	ENSrcAnlzOpCodeChars_AssignMod,		// '%=' //new
	ENSrcAnlzOpCodeChars_AssignAnd,		// '&=' //new
	ENSrcAnlzOpCodeChars_AssignOr,		// '|=' //new
	ENSrcAnlzOpCodeChars_AssignXor,		// '^=' //new
	//Three chars
	ENSrcAnlzOpCodeChars_AssignBitwR,	// '>>=' //new
	ENSrcAnlzOpCodeChars_AssignBitwL,	// '<<=' //new
	//Others
	ENSrcAnlzOpCodeChars_SizeOf,		// 'sizeof' //new
	//
	ENSrcAnlzOpCodeChars_Count
} ENSrcAnlzOpCodeChars;*/

/*typedef enum ENSrcAnlzOpCodeSintax_ {
	ENSrcAnlzOpCodeSintax_Logic_And = 0,	// '&&'
	ENSrcAnlzOpCodeSintax_Logic_Or,			// '||'
	ENSrcAnlzOpCodeSintax_Logic_Neg,		// '!'
	//
	ENSrcAnlzOpCodeSintax_Relat_Equal,		// '=='
	ENSrcAnlzOpCodeSintax_Relat_Ineq,		// '!='
	ENSrcAnlzOpCodeSintax_Relat_Great,		// '>'
	ENSrcAnlzOpCodeSintax_Relat_Lower,		// '<'
	ENSrcAnlzOpCodeSintax_Relat_GrtEq,		// '>='
	ENSrcAnlzOpCodeSintax_Relat_LowEq,		// '<='
	//
	ENSrcAnlzOpCodeSintax_Arith_Add,		// '+'
	ENSrcAnlzOpCodeSintax_Arith_SignPos,	// '+'
	ENSrcAnlzOpCodeSintax_Arith_Sub,		// '-'
	ENSrcAnlzOpCodeSintax_Arith_SignNeg,	// '-'
	ENSrcAnlzOpCodeSintax_Arith_Mul,		// '*'
	ENSrcAnlzOpCodeSintax_Arith_Div,		// '/'
	ENSrcAnlzOpCodeSintax_Arith_Mod,		// '%'
	ENSrcAnlzOpCodeSintax_Arith_IncPre,		// '++'
	ENSrcAnlzOpCodeSintax_Arith_IncPos,		// '++'
	ENSrcAnlzOpCodeSintax_Arith_DecPre,		// '--'
	ENSrcAnlzOpCodeSintax_Arith_DecPos,		// '--'
	//
	ENSrcAnlzOpCodeSintax_Bitw_And,			// '&'
	ENSrcAnlzOpCodeSintax_Bitw_Or,			// '|'
	ENSrcAnlzOpCodeSintax_Bitw_Xor,			// '^' //binary complement (only activate bits activated in both operands)
	ENSrcAnlzOpCodeSintax_Bitw_Comp,		// '~' //binary complement (flipping bits values)
	ENSrcAnlzOpCodeSintax_Bitw_Right,		// '>>'
	ENSrcAnlzOpCodeSintax_Bitw_Left,		// '<<'
	//
	ENSrcAnlzOpCodeSintax_Assign_Set,		// '='
	ENSrcAnlzOpCodeSintax_Assign_Add,		// '+='
	ENSrcAnlzOpCodeSintax_Assign_Sub,		// '-='
	ENSrcAnlzOpCodeSintax_Assign_Mult,		// '*='
	ENSrcAnlzOpCodeSintax_Assign_Div,		// '/='
	ENSrcAnlzOpCodeSintax_Assign_Mod,		// '%='
	ENSrcAnlzOpCodeSintax_Assign_BitsAnd,	// '&='
	ENSrcAnlzOpCodeSintax_Assign_BitsOr,	// '|='
	ENSrcAnlzOpCodeSintax_Assign_BitsXor,	// '^='
	ENSrcAnlzOpCodeSintax_Assign_BitsR,		// '>>='
	ENSrcAnlzOpCodeSintax_Assign_BitsL,		// '<<='
	//
	ENSrcAnlzOpCodeSintax_Prog_ValOf,		// '*'
	ENSrcAnlzOpCodeSintax_Prog_PtrOf,		// '&'
	ENSrcAnlzOpCodeSintax_Prog_SizeOf,		// 'sizeof'
	ENSrcAnlzOpCodeSintax_Prog_MbrOf,		// '.'
	ENSrcAnlzOpCodeSintax_Prog_MbrOfPtr,	// '->'
	ENSrcAnlzOpCodeSintax_Prog_ParamOpen,	// '('	//order is important (first paramOpen tha GrpOpen)
	//ENSrcAnlzOpCodeSintax_Prog_ParamClose,// ')'
	ENSrcAnlzOpCodeSintax_Prog_GrpOpen,		// '('
	//ENSrcAnlzOpCodeSintax_Prog_GrpClose,	// ')'
	ENSrcAnlzOpCodeSintax_Prog_IdxOpen,		// '['
	//ENSrcAnlzOpCodeSintax_Prog_IdxClose,	// ']'
	ENSrcAnlzOpCodeSintax_Prog_BDefOpen,	// '{' //order is important (first BlockDefOpen than BlockOpen)
	//ENSrcAnlzOpCodeSintax_Prog_BDefClose,	// '}'
	ENSrcAnlzOpCodeSintax_Prog_BlckOpen,	// '{'
	//ENSrcAnlzOpCodeSintax_Prog_BlckClose,	// '}'
	ENSrcAnlzOpCodeSintax_Prog_Comma,		// ','
	ENSrcAnlzOpCodeSintax_Prog_CondStart,	// '?'
	ENSrcAnlzOpCodeSintax_Prog_CondMid,		// ':'
	//
	ENSrcAnlzOpCodeSintax_Count
} ENSrcAnlzOpCodeSintax;*/

/*static const SI32 __globalOpCodeSintaxChar1[] = {
	ENSrcAnlzOpCodeChars_GrpOpen,		// '('
	//ENSrcAnlzOpCodeChars_GrpClose,	// ')'
	ENSrcAnlzOpCodeChars_IdxOpen,		// '[' //new
	//ENSrcAnlzOpCodeChars_IdxClose,	// ']' //new
	ENSrcAnlzOpCodeChars_Point,			// '.' //new
	ENSrcAnlzOpCodeChars_Exclamation,	// '!'
	ENSrcAnlzOpCodeChars_Ampersand,		// '&'
	ENSrcAnlzOpCodeChars_VertBar,		// '|'
	ENSrcAnlzOpCodeChars_GreatThan,		// '>'
	ENSrcAnlzOpCodeChars_LessThan,		// '<'
	ENSrcAnlzOpCodeChars_Plus,			// '+'
	ENSrcAnlzOpCodeChars_Minus,			// '-'
	ENSrcAnlzOpCodeChars_Asterisk,		// '*'
	ENSrcAnlzOpCodeChars_Slash,			// '/'
	ENSrcAnlzOpCodeChars_Percent,		// '%'
	ENSrcAnlzOpCodeChars_Tilde,			// '~'
	ENSrcAnlzOpCodeChars_Caret,			// '^'
	ENSrcAnlzOpCodeChars_CondStart,		// '?' //new
	ENSrcAnlzOpCodeChars_CondMid,		// ':' //new
	ENSrcAnlzOpCodeChars_Assign,		// '=' //new
	ENSrcAnlzOpCodeChars_Comma			// ',' //new
};*/

/*static const SI32 __globalOpCodeSintaxChar2[] = {
	ENSrcAnlzOpCodeChars_Arrow,			// '->' //new
	ENSrcAnlzOpCodeChars_PlusPlus,		// '++' //new
	ENSrcAnlzOpCodeChars_MinusMinus,	// '--' //new
	ENSrcAnlzOpCodeChars_Ampersand2,	// '&&'
	ENSrcAnlzOpCodeChars_VertBar2,		// '||'
	ENSrcAnlzOpCodeChars_Equals2,		// '=='
	ENSrcAnlzOpCodeChars_Inequal,		// '!='
	ENSrcAnlzOpCodeChars_GreatEqual,	// '>='
	ENSrcAnlzOpCodeChars_LessEqual,		// '<='
	ENSrcAnlzOpCodeChars_GreatThan2,	// '>>'
	ENSrcAnlzOpCodeChars_LessThan2,		// '<<'
	ENSrcAnlzOpCodeChars_AssignPlus,	// '+=' //new
	ENSrcAnlzOpCodeChars_AssignLess,	// '-=' //new
	ENSrcAnlzOpCodeChars_AssignMult,	// '*=' //new
	ENSrcAnlzOpCodeChars_AssignDiv,		// '/=' //new
	ENSrcAnlzOpCodeChars_AssignMod,		// '%=' //new
	ENSrcAnlzOpCodeChars_AssignAnd,		// '&=' //new
	ENSrcAnlzOpCodeChars_AssignOr,		// '|=' //new
	ENSrcAnlzOpCodeChars_AssignXor		// '^=' //new
};*/

/*static const SI32 __globalOpCodeSintaxChar3[] = {
	ENSrcAnlzOpCodeChars_AssignBitwR,	// '>>=' //new
	ENSrcAnlzOpCodeChars_AssignBitwL	// '<<=' //new
};*/

/*static const SI32 __globalOpCodeSintaxCharN[] = {
	ENSrcAnlzOpCodeChars_SizeOf			// 'sizeof' //new
};*/

/*static const STSrcAnlzExprOp __globalOpCodeDefs[] = {
	//One char
	{ ENSrcAnlzOpCodeChars_GrpOpen,			{'(', NULL}, {NULL, NULL}, {')', NULL}, ENSrcAnlzOpCodeSintax_Prog_ParamOpen, ENSrcAnlzOpCodeSintax_Prog_GrpOpen } //, order is important (paramOpen must be first)
	//, { ENSrcAnlzOpCodeChars_GrpClose,	{')', NULL}, {'(', NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Prog_ParamClose, ENSrcAnlzOpCodeSintax_Prog_GrpClose }
	, { ENSrcAnlzOpCodeChars_IdxOpen,		{'[', NULL}, {NULL, NULL}, {']', NULL}, ENSrcAnlzOpCodeSintax_Prog_IdxOpen, -1 } //, order is important (idxOpen must be first)
	//, { ENSrcAnlzOpCodeChars_IdxClose,	{']', NULL}, {'[', NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Prog_IdxClose, -1 }
	, { ENSrcAnlzOpCodeChars_Point,			{'.', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Prog_MbrOf, -1 }
	, { ENSrcAnlzOpCodeChars_Exclamation,	{'!', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Logic_Neg, -1 }
	, { ENSrcAnlzOpCodeChars_Ampersand,		{'&', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Bitw_And, ENSrcAnlzOpCodeSintax_Prog_PtrOf }
	, { ENSrcAnlzOpCodeChars_VertBar,		{'|', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Bitw_Or, -1 }
	, { ENSrcAnlzOpCodeChars_GreatThan,		{'>', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Relat_Great, -1 }
	, { ENSrcAnlzOpCodeChars_LessThan,		{'<', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Relat_Lower, -1 }
	, { ENSrcAnlzOpCodeChars_Plus,			{'+', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Arith_Add, ENSrcAnlzOpCodeSintax_Arith_SignPos }
	, { ENSrcAnlzOpCodeChars_Minus,			{'-', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Arith_Sub, ENSrcAnlzOpCodeSintax_Arith_SignNeg }
	, { ENSrcAnlzOpCodeChars_Asterisk,		{'*', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Arith_Mul, ENSrcAnlzOpCodeSintax_Prog_ValOf }
	, { ENSrcAnlzOpCodeChars_Slash,			{'/', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Arith_Div, -1 }
	, { ENSrcAnlzOpCodeChars_Percent,		{'%', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Arith_Mod, -1 }
	, { ENSrcAnlzOpCodeChars_Tilde,			{'~', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Bitw_Comp, -1 }
	, { ENSrcAnlzOpCodeChars_Caret,			{'^', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Bitw_Xor, -1 }
	, { ENSrcAnlzOpCodeChars_CondStart,		{'?', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Prog_CondStart, -1 }
	, { ENSrcAnlzOpCodeChars_CondMid,		{':', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Prog_CondMid, -1 }
	, { ENSrcAnlzOpCodeChars_Assign,		{'=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_Set, -1 }
	, { ENSrcAnlzOpCodeChars_Comma,			{',', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Prog_Comma, -1 }
	//Two chars
	, { ENSrcAnlzOpCodeChars_Arrow,			{'-', '>', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Prog_MbrOfPtr, -1 }
	, { ENSrcAnlzOpCodeChars_PlusPlus,		{'+', '+', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Arith_IncPre, ENSrcAnlzOpCodeSintax_Arith_IncPos }
	, { ENSrcAnlzOpCodeChars_MinusMinus,	{'-', '-', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Arith_DecPre, ENSrcAnlzOpCodeSintax_Arith_DecPos }
	, { ENSrcAnlzOpCodeChars_Ampersand2,	{'&', '&', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Logic_And, -1 }
	, { ENSrcAnlzOpCodeChars_VertBar2,		{'|', '|', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Logic_Or, -1 }
	, { ENSrcAnlzOpCodeChars_Equals2,		{'=', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Relat_Equal, -1 }
	, { ENSrcAnlzOpCodeChars_Inequal,		{'!', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Relat_Ineq, -1 }
	, { ENSrcAnlzOpCodeChars_GreatEqual,	{'>', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Relat_GrtEq, -1 }
	, { ENSrcAnlzOpCodeChars_LessEqual,		{'<', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Relat_LowEq, -1 }
	, { ENSrcAnlzOpCodeChars_GreatThan2,	{'>', '>', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Bitw_Right, -1 }
	, { ENSrcAnlzOpCodeChars_LessThan2,		{'<', '<', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Bitw_Left, -1 }
	, { ENSrcAnlzOpCodeChars_AssignPlus,	{'+', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_Add, -1 }
	, { ENSrcAnlzOpCodeChars_AssignLess,	{'-', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_Sub, -1 }
	, { ENSrcAnlzOpCodeChars_AssignMult,	{'*', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_Mult, -1 }
	, { ENSrcAnlzOpCodeChars_AssignDiv,		{'/', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_Div, -1 }
	, { ENSrcAnlzOpCodeChars_AssignMod,		{'%', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_Mod, -1 }
	, { ENSrcAnlzOpCodeChars_AssignAnd,		{'&', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_BitsAnd, -1 }
	, { ENSrcAnlzOpCodeChars_AssignOr,		{'|', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_BitsOr, -1 }
	, { ENSrcAnlzOpCodeChars_AssignXor,		{'^', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_BitsXor, -1 }
	//Three chars
	, { ENSrcAnlzOpCodeChars_AssignBitwR,	{'>', '>', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_BitsR, -1 }
	, { ENSrcAnlzOpCodeChars_AssignBitwL,	{'<', '<', '=', NULL}, {NULL, NULL}, {NULL, NULL}, ENSrcAnlzOpCodeSintax_Assign_BitsL, -1 }
	//Others
	, { ENSrcAnlzOpCodeChars_SizeOf,		{'s', 'i', 'z', 'e', 'o', 'f', NULL}, {NULL}, {NULL}, ENSrcAnlzOpCodeSintax_Prog_SizeOf, -1 }
};*/

/*static const STSrcAnlzExprOpSintx __globalOpCodeSintax[] = {
	{ ENSrcAnlzOpCodeSintax_Logic_And,			ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_And, NULL,			ENSrcAnlzSintxCOp_Ampersand2,	ENSrcAnlzSintxCOpType_Logical, ENSrcAnlzSintxCOpLogic_And, "LogicAnd" }		// '&&'
	, { ENSrcAnlzOpCodeSintax_Logic_Or,			ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Or, NULL,			ENSrcAnlzSintxCOp_VertBar2,		ENSrcAnlzSintxCOpType_Logical, ENSrcAnlzSintxCOpLogic_Or, "LogicOr" }		// '||'
	, { ENSrcAnlzOpCodeSintax_Logic_Neg,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,		ENSrcAnlzSintxCOp_Exclamation,	ENSrcAnlzSintxCOpType_Logical, ENSrcAnlzSintxCOpLogic_Neg, "LogicNeg" }		// '!'
	//
	, { ENSrcAnlzOpCodeSintax_Relat_Equal,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Equal, NULL,		ENSrcAnlzSintxCOp_Equals2,		ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_Equal, "RelatEqual" }	// '=='
	, { ENSrcAnlzOpCodeSintax_Relat_Ineq,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Equal, NULL,		ENSrcAnlzSintxCOp_Inequal,		ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_Ineq, "RelatIneq" }	// '!='
	, { ENSrcAnlzOpCodeSintax_Relat_Great,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Relat, NULL,		ENSrcAnlzSintxCOp_GreatThan,		ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_Great, "RelatGreat" }	// '>'
	, { ENSrcAnlzOpCodeSintax_Relat_Lower,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Relat, NULL,		ENSrcAnlzSintxCOp_LessThan,		ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_Lower, "RelatLower" }	// '<'
	, { ENSrcAnlzOpCodeSintax_Relat_GrtEq,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Relat, NULL,		ENSrcAnlzSintxCOp_GreatEqual,	ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_GrtEq, "RelatGrtEq" }	// '>='
	, { ENSrcAnlzOpCodeSintax_Relat_LowEq,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Relat, NULL,		ENSrcAnlzSintxCOp_LessEqual,		ENSrcAnlzSintxCOpType_Relatnl, ENSrcAnlzSintxCOpRelat_LowEq, "RelatLowEq" }	// '<='
	//
	, { ENSrcAnlzOpCodeSintax_Arith_Add,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Add, NULL,			ENSrcAnlzSintxCOp_Plus,			ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Add, "ArithAdd" }		// '+'
	, { ENSrcAnlzOpCodeSintax_Arith_SignPos,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,		ENSrcAnlzSintxCOp_Plus,			ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Add, "ArithSignPos" }	// '+'
	, { ENSrcAnlzOpCodeSintax_Arith_Sub,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Add, NULL,			ENSrcAnlzSintxCOp_Minus,			ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Sub, "ArithSub" }		// '-'
	, { ENSrcAnlzOpCodeSintax_Arith_SignNeg,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,		ENSrcAnlzSintxCOp_Minus,			ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Sub, "ArithSignNeg" }	// '-'
	, { ENSrcAnlzOpCodeSintax_Arith_Mul,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Mult, NULL,		ENSrcAnlzSintxCOp_Asterisk,		ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Mul, "ArithMul"}		// '*'
	, { ENSrcAnlzOpCodeSintax_Arith_Div,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Mult, NULL,		ENSrcAnlzSintxCOp_Slash,			ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Div, "ArithDiv" }		// '/'
	, { ENSrcAnlzOpCodeSintax_Arith_Mod,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Mult, NULL,		ENSrcAnlzSintxCOp_Percent,		ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Mod, "ArithMod" }		// '%'
	, { ENSrcAnlzOpCodeSintax_Arith_IncPre,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,		ENSrcAnlzSintxCOp_Plus2,			ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Inc, "ArithIncPre" }	// '++'
	, { ENSrcAnlzOpCodeSintax_Arith_IncPos,		ENSrcAnlzExprOpSideMask_Left, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_Plus2,			ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Inc, "ArithIncPos" }	// '++'
	, { ENSrcAnlzOpCodeSintax_Arith_DecPre,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,		ENSrcAnlzSintxCOp_Minus2,		ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Dec, "ArithDecPre" }	// '--'
	, { ENSrcAnlzOpCodeSintax_Arith_DecPos,		ENSrcAnlzExprOpSideMask_Left, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_Minus2,		ENSrcAnlzSintxCOpType_Arithm, ENSrcAnlzSintxCOpArith_Dec, "ArithDecPos" }	// '__'
	//
	, { ENSrcAnlzOpCodeSintax_Bitw_And,			ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_BAnd, NULL,		ENSrcAnlzSintxCOp_Ampersand,		ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_And, "BitwAnd" }		// '&'
	, { ENSrcAnlzOpCodeSintax_Bitw_Or,			ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_BOr, NULL,			ENSrcAnlzSintxCOp_VertBar,		ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_Or, "BitwOr" }			// '|'
	, { ENSrcAnlzOpCodeSintax_Bitw_Xor,			ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_BXor, NULL,		ENSrcAnlzSintxCOp_Caret,			ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_Xor, "BitwXor" }		// '^'
	, { ENSrcAnlzOpCodeSintax_Bitw_Comp,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,		ENSrcAnlzSintxCOp_Tilde,			ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_Comp, "BitwComp" }		// '~'
	, { ENSrcAnlzOpCodeSintax_Bitw_Right,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Shift, NULL,		ENSrcAnlzSintxCOp_GreatThan2,	ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_Right, "BitwRight" }	// '>>'
	, { ENSrcAnlzOpCodeSintax_Bitw_Left,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Shift, NULL,		ENSrcAnlzSintxCOp_LessThan2,		ENSrcAnlzSintxCOpType_Bitwise, ENSrcAnlzSintxCOpBitw_Left, "BitwLeft" }		// '<<'
	//
	, { ENSrcAnlzOpCodeSintax_Assign_Set,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_Equals,		ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_Set, "AssignSet" }	// '='
	, { ENSrcAnlzOpCodeSintax_Assign_Add,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_AssignAdd,		ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_Add, "AssignAdd" }	// '+='
	, { ENSrcAnlzOpCodeSintax_Assign_Sub,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_AssignSub,		ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_Sub, "AssignSub" }	// '-='
	, { ENSrcAnlzOpCodeSintax_Assign_Mult,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_AssignMult,	ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_Mult, "AssignMult" }	// '*='
	, { ENSrcAnlzOpCodeSintax_Assign_Div,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_AssignDiv,		ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_Div, "AssignDiv" }	// '/='
	, { ENSrcAnlzOpCodeSintax_Assign_Mod,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_AssignMod,		ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_Mod, "AssignMod" }	// '%='
	, { ENSrcAnlzOpCodeSintax_Assign_BitsAnd,	ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_AssignBitsAnd,	ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_BitsAnd, "AssignBitsAnd" }	// '&='
	, { ENSrcAnlzOpCodeSintax_Assign_BitsOr,	ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_AssignBitsOr,	ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_BitsOr, "AssignBitsOr" }		// '|='
	, { ENSrcAnlzOpCodeSintax_Assign_BitsXor,	ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_AssignBitsXor,	ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_BitsXor, "AssignBitsXor" }	// '^='
	, { ENSrcAnlzOpCodeSintax_Assign_BitsR,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_GreatThan2,	ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_BitsR, "AssignBitsR" }		// '>>='
	, { ENSrcAnlzOpCodeSintax_Assign_BitsL,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Assig, NULL,		ENSrcAnlzSintxCOp_LessThan2,		ENSrcAnlzSintxCOpType_Assign, ENSrcAnlzSintxCOpAssign_BitsL, "AssignBitsL" }		// '<<='
	//
	, { ENSrcAnlzOpCodeSintax_Prog_ValOf,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,		ENSrcAnlzSintxCOp_Asterisk,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_ValOf, "ProgValOf" }				// '*'
	, { ENSrcAnlzOpCodeSintax_Prog_PtrOf,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,		ENSrcAnlzSintxCOp_Ampersand,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_PtrOf, "ProgPtrOf" }				// '&'
	, { ENSrcAnlzOpCodeSintax_Prog_SizeOf,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Unary, NULL,		ENSrcAnlzSintxCOp_Sizeof,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_SizeOf, "ProgSizeOf" }			// 'sizeof'
	, { ENSrcAnlzOpCodeSintax_Prog_MbrOf,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_Dot,			ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_MbrOf, "ProgMbrOf" }				// '.'
	, { ENSrcAnlzOpCodeSintax_Prog_MbrOfPtr,	ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_Arrow,			ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_MbrOfPtr, "ProgMbrOfPtr" }		// '->'
	, { ENSrcAnlzOpCodeSintax_Prog_ParamOpen,	ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Extends, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_GrpOpen,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpOpen, "ProgParamOpen" }		// '(', order is important (paramOpen must be first, than grpOpen)
	//, { ENSrcAnlzOpCodeSintax_Prog_ParamClose,ENSrcAnlzExprOpSideMask_None, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_GrpClose,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpClose, "ProgParamClose" }		// ')'
	, { ENSrcAnlzOpCodeSintax_Prog_GrpOpen,		ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_GrpOpen,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpOpen, "ProgGrpOpen" }			// '(', order is important (paramOpen must be first, than grpOpen)
	//, { ENSrcAnlzOpCodeSintax_Prog_GrpClose,	ENSrcAnlzExprOpSideMask_None, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_GrpClose,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_GrpClose, "ProgGrpClose" }		// ')'
	, { ENSrcAnlzOpCodeSintax_Prog_IdxOpen,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Extends, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_IdxOpen,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_IdxOpen, "ProgIdxOpen" }			// '['
	//, { ENSrcAnlzOpCodeSintax_Prog_IdxClose,	ENSrcAnlzExprOpSideMask_None, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_IdxClose,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_IdxClose, "ProgIdxClose" }		// ']'
	, { ENSrcAnlzOpCodeSintax_Prog_BDefOpen,	ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Extends, ENSrcAnlzSintxCOpPriority_Postf, &__globalCodeBlockSintaxMap,		ENSrcAnlzSintxCOp_BlckOpen,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_BlckOpen, "ProgDefBlckOpen" }		// '{', order is important (defBlckOpen must be first, than BlckOpen)
	//, { ENSrcAnlzOpCodeSintax_Prog_BDefClose,	ENSrcAnlzExprOpSideMask_None, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_BlckClose,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_BlckClose, "ProgDefBlckClose" }	// '}'
	, { ENSrcAnlzOpCodeSintax_Prog_BlckOpen,	ENSrcAnlzExprOpSideMask_Right, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, &__globalCodeBlockSintaxMap,		ENSrcAnlzSintxCOp_BlckOpen,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_BlckOpen, "ProgBlckOpen" }		// '{', order is important (defBlckOpen must be first, than BlckOpen)
	//, { ENSrcAnlzOpCodeSintax_Prog_BlckClose,	ENSrcAnlzExprOpSideMask_None, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Postf, NULL,		ENSrcAnlzSintxCOp_BlckClose,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_BlckClose, "ProgBlckClose" }	// '}'
	, { ENSrcAnlzOpCodeSintax_Prog_Comma,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Comma, NULL,		ENSrcAnlzSintxCOp_Comma,			ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_Comma, "ProgComma" }				// ','
	, { ENSrcAnlzOpCodeSintax_Prog_CondStart,	ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Cond, NULL,		ENSrcAnlzSintxCOp_QuestM,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_CondStart, "ProgCondStart" }		// '?'
	, { ENSrcAnlzOpCodeSintax_Prog_CondMid,		ENSrcAnlzExprOpSideMask_Both, ENSrcAnlzExprOpSeqType_Unique, ENSrcAnlzSintxCOpPriority_Cond, NULL,		ENSrcAnlzSintxCOp_TwoPoints,		ENSrcAnlzSintxCOpType_Prog, ENSrcAnlzSintxCOpProg_CondMid, "ProgCondMid" }			// ':'
};*/

//Extern defined in the header
/*const STSrcAnlzExprOpSintxMap __globalOpCodeSintaxMap = {
	//OpChars
	__globalOpCodeDefs, (sizeof(__globalOpCodeDefs) / sizeof(__globalOpCodeDefs[0])) //ENSrcAnlzOpCodeChars_Count
	//OpSintaxs
	, __globalOpCodeSintax, (sizeof(__globalOpCodeSintax) / sizeof(__globalOpCodeSintax[0])) //ENSrcAnlzOpCodeSintax_Count
	//Index by op lenghts
	, __globalOpCodeSintaxChar1, (sizeof(__globalOpCodeSintaxChar1) / sizeof(__globalOpCodeSintaxChar1[0]))
	, __globalOpCodeSintaxChar2, (sizeof(__globalOpCodeSintaxChar2) / sizeof(__globalOpCodeSintaxChar2[0]))
	, __globalOpCodeSintaxChar3, (sizeof(__globalOpCodeSintaxChar3) / sizeof(__globalOpCodeSintaxChar3[0]))
	, __globalOpCodeSintaxCharN, (sizeof(__globalOpCodeSintaxCharN) / sizeof(__globalOpCodeSintaxCharN[0]))
};*/

//
// Feed
//

/*BOOL SrcAnlzParser_codeFeedExpression(void* pGState){
	BOOL r = FALSE;
	STSrcAnlzParser* state = (STSrcAnlzParser*)pGState;
	STSrcAnlzCodeCParser* parCodeState = &state->codeParse;
 	STNBArray ops; NBArray_init(&ops, sizeof(STSrcAnlzExprOp), NULL);
#	ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
	//Print code to process
	{
 		STNBString strTmp; NBString_init(&strTmp);
		SI32 i; for(i = 0; i < parCodeState->acumTokensCur->defs.use; i++){
			const STSrcAnlzTokn* wDef = NBArray_itmPtrAtIndex(&parCodeState->acumTokensCur->defs, STSrcAnlzTokn, i);
			if(i != 0 && wDef->isSpacePreceded) NBString_concatByte(&strTmp, ' ');
			NBString_concat(&strTmp, &parCodeState->acumTokensCur->strBuff.str[wDef->iAtBuff]);
		}
		//PRINTF_INFO("---------\n");
		PRINTF_INFO("Code-expr: %s\n", strTmp.str);
		//PRINTF_INFO("---------\n");
		NBString_release(&strTmp);
	}
#	endif
	/ *if(!SrcAnlzExprParser_feedTokns(state, ENSrcAnlzExprSrc_ParCodeState, &__globalOpCodeSintaxMap, 0, parCodeState->acumTokensCur->defs.use, ops)){
		PRINTF_ERROR("Could not parse MACRO's value sintax.\n");
		NBASSERT(FALSE)
	} else {
#		ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
		//Print parsed expression
		{
 			STNBString strTmp; NBString_init(&strTmp);
			SI32 iOp = 0; SrcAnlzExprParser_dbgAddOpsToStr(state, ENSrcAnlzExprSrc_ParCodeState, strTmp, &iOp, ops->arreglo(), ops->use, FALSE);
			PRINTF_INFO("Operations parsed: %s.\n", strTmp.str);
			NBASSERT(iOp == ops->use)
			NBString_release(&strTmp);
		}
#		endif
		r = TRUE;
	}* /
	r = TRUE;
	NBArray_release(&ops);
	return r;
}*/

//---------------------------
// Init and release state
//---------------------------
void SrcAnlzCodeC_init(STSrcAnlzCodeC* state){
	//Buffers
	NBString_init(&state->strsBuff); //all strings (members names, vars names, string literals, ...)
	NBString_concatByte(&state->strsBuff, '\0');
	//User definitions
	NBArray_init(&state->defsUsrTypes, sizeof(STSrcAnlzCodeCUsrType), NULL); //user 'struct-or-union' and 'enum' types definitions
	{
		STSrcAnlzCodeCUsrType first;
		NBArray_addValue(&state->defsUsrTypes, first);
	}
	NBArray_init(&state->defsDeclars, sizeof(STSrcAnlzCodeCDeclar), NULL); //declarations (including typedefs and functions)
	{
		STSrcAnlzCodeCDeclar first;
		NBArray_addValue(&state->defsDeclars, first);
	}
	NBArray_init(&state->defsDeclPtrSpecs, sizeof(STSrcAnlzCodeCDeclarPtr), NULL);	//pointer specifications
	{
		STSrcAnlzCodeCDeclarPtr first;
		NBArray_addValue(&state->defsDeclPtrSpecs, first);
	}
	NBArray_init(&state->defsDeclArrSpecs, sizeof(STSrcAnlzCodeCDeclarArr), NULL);	//array sizes specifications
	{
		STSrcAnlzCodeCDeclarArr first;
		NBArray_addValue(&state->defsDeclArrSpecs, first);
	}
	//Blocks
	NBArray_init(&state->defsDeclarsScopes, sizeof(STSrcAnlzCodeCDeclarsScope), NULL); //declarations scopes (blocks of code, enums list, structs/unions defs)
	//First dummy declarScope (index zero is reserved)
	{
		STSrcAnlzCodeCDeclarsScope first;
		NBArray_addValue(&state->defsDeclarsScopes, first);
	}
	//First valid declarScope
	{
		NBASSERT(state->defsDeclarsScopes.use > 0)
		STSrcAnlzCodeCDeclarsScope fBlock;
		fBlock.iParentScope			= 0;
		fBlock.iDeepLvl				= 0;
		fBlock.usrTypesRng.start	= state->defsUsrTypes.use;
		fBlock.usrTypesRng.count	= 0;
		fBlock.declarsRng.start		= state->defsDeclars.use;
		fBlock.declarsRng.count		= 0;
		fBlock.scopesRng.start		= state->defsDeclarsScopes.use + 1; //+1 to exclude itself
		fBlock.scopesRng.count		= 0;
		NBArray_addValue(&state->defsDeclarsScopes, fBlock);
	}
	NBASSERT(state->defsDeclarsScopes.use == 2)
}

void SrcAnlzCodeC_release(STSrcAnlzCodeC* state){
	//Buffers
	NBString_release(&state->strsBuff);
	//User definitions
	NBArray_release(&state->defsUsrTypes);
	NBArray_release(&state->defsDeclars);
	NBArray_release(&state->defsDeclPtrSpecs);
	NBArray_release(&state->defsDeclArrSpecs);
	//Blocks
	NBArray_release(&state->defsDeclarsScopes);
}

//---------------------------
// Find
//---------------------------

BOOL SrcAnlzCodeC_findUsrTypeByName(const STSrcAnlzCodeC* state, const UI32 iTopDeclScope, const BOOL srchParentScope, const char* srchName, const UI32 maxIdxCount, UI32* dstIdx){
	BOOL r = FALSE;
	NBASSERT(iTopDeclScope > 0 && iTopDeclScope < state->defsDeclarsScopes.use)
	if(iTopDeclScope > 0 && iTopDeclScope < state->defsDeclarsScopes.use){
		const STSrcAnlzCodeCDeclarsScope* dScope = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iTopDeclScope);
		NBASSERT(dScope->usrTypesRng.start >= 0 && (dScope->usrTypesRng.start + dScope->usrTypesRng.count) <= state->defsUsrTypes.use)
		r = TRUE;
		UI32 rVal = 0;
		//Search into this scope
		if(dScope->usrTypesRng.count > 0){
			const STSrcAnlzCodeCUsrType* itmLimitR = NBArray_dataPtr(&state->defsUsrTypes, STSrcAnlzCodeCUsrType) + maxIdxCount;
			const STSrcAnlzCodeCUsrType* itmFirst = NBArray_itmPtrAtIndex(&state->defsUsrTypes, STSrcAnlzCodeCUsrType, dScope->usrTypesRng.start);
			if(itmFirst < itmLimitR){ //Ignore any scope that starts at the right-limit (should not exists yet)
				const STSrcAnlzCodeCUsrType* itmAfterLast = itmFirst + dScope->usrTypesRng.count;
				if(itmAfterLast > itmLimitR){
					itmAfterLast = itmLimitR; //Limit scope to the maxIdx (any itm after should not exist yet)
				}
				const STSrcAnlzCodeCUsrType* itm = itmFirst;
				while(itm < itmAfterLast){
					if(itm->iDeepLvl == dScope->iDeepLvl){
						const char* strName = &state->strsBuff.str[itm->iNameFirstChar];
						if(NBString_strIsEqual(strName, srchName)){
							rVal = dScope->usrTypesRng.start + (UI32)(itm - itmFirst);
							break;
						}
					}
					itm++;
				}
			}
		}
		//Search into parent scope
		if(rVal == 0 && srchParentScope && dScope->iParentScope != 0){
			NBASSERT(dScope->iDeepLvl == (NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, dScope->iParentScope)->iDeepLvl + 1))
			if(!SrcAnlzCodeC_findUsrTypeByName(state, dScope->iParentScope, srchParentScope, srchName, maxIdxCount, &rVal)){
				r = FALSE;
			}
		}
		//Set return's value
		if(dstIdx != NULL) *dstIdx = rVal;
	}
	return r;
}

BOOL SrcAnlzCodeC_findDeclarByName(const STSrcAnlzCodeC* state, const UI32 iTopDeclScope, const BOOL srchParentScope, const char* srchName, const UI32 maxIdxCount, UI32* dstIdx){
	BOOL r = FALSE;
	NBASSERT(iTopDeclScope > 0 && iTopDeclScope < state->defsDeclarsScopes.use)
	if(iTopDeclScope > 0 && iTopDeclScope < state->defsDeclarsScopes.use){
		const STSrcAnlzCodeCDeclarsScope* dScope = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iTopDeclScope);
		NBASSERT(dScope->declarsRng.start >= 0 && (dScope->declarsRng.start + dScope->declarsRng.count) <= state->defsDeclars.use)
		r = TRUE;
		UI32 rVal = 0;
		//Search into this scope
		if(dScope->declarsRng.count > 0){
			//PRINTF_INFO("Searching at scope (#%d -> #%d / of %d, level '%d').\n", (SI32)(dScope->declarsRng.start + 1), (SI32)(dScope->declarsRng.start + dScope->declarsRng.count), (SI32)state->defsDeclars.use, (SI32)dScope->iDeepLvl);
			const STSrcAnlzCodeCDeclar* itmLimitR = NBArray_dataPtr(&state->defsDeclars, STSrcAnlzCodeCDeclar) + maxIdxCount;
			const STSrcAnlzCodeCDeclar* itmFirst = NBArray_itmPtrAtIndex(&state->defsDeclars, STSrcAnlzCodeCDeclar, dScope->declarsRng.start);
			if(itmFirst < itmLimitR){ //Ignore any scope that starts at the right-limit (should not exists yet)
				const STSrcAnlzCodeCDeclar* itmAfterLast = itmFirst + dScope->declarsRng.count;
				if(itmAfterLast > itmLimitR){
					itmAfterLast = itmLimitR; //Limit scope to the maxIdx (any itm after should not exist yet)
				}
				const STSrcAnlzCodeCDeclar* itm = itmFirst;
				while(itm < itmAfterLast){
					if(itm->iDeepLvl == dScope->iDeepLvl){
						const char* strName = &state->strsBuff.str[itm->iNameFirstChar];
						//PRINTF_INFO("Compare '%s' vs '%s'.\n", strName, srchName);
						if(NBString_strIsEqual(strName, srchName)){
							rVal = dScope->declarsRng.start + (UI32)(itm - itmFirst);
							break;
						}
					}
					itm++;
				}
			}
		}
		//Search into parent scope
		if(rVal == 0 && srchParentScope && dScope->iParentScope != 0){
			NBASSERT(dScope->iDeepLvl == (NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, dScope->iParentScope)->iDeepLvl + 1))
			if(!SrcAnlzCodeC_findDeclarByName(state, dScope->iParentScope, srchParentScope, srchName, maxIdxCount, &rVal)){
				r = FALSE;
			}
		}
		//Set return's value
		if(dstIdx != NULL) *dstIdx = rVal;
	}
	return r;
}

//---------------------------
// Add
//---------------------------

BOOL SrcAnlzCodeC_getDeclarByIdx(STSrcAnlzCodeC* state, const UI32 iDeclarDef, const UI32 iParentScope, STSrcAnlzCodeCDeclar** dstDef, BOOL* dstIsNewDef){
	BOOL r = FALSE;
	STSrcAnlzCodeCDeclar* def = NULL;
	BOOL isNewDef = FALSE;
	NBASSERT(iParentScope > 0 && iParentScope < state->defsDeclarsScopes.use)
	if(iParentScope > 0 && iParentScope < state->defsDeclarsScopes.use){
		const STSrcAnlzCodeCDeclarsScope* parentScope = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iParentScope);
		NBASSERT(parentScope->declarsRng.start > 0 && (parentScope->declarsRng.start + parentScope->declarsRng.count) <= state->defsDeclars.use)
		if(iDeclarDef < state->defsDeclars.use){
			//Declaration already added
			//PRINTF_INFO("DeclarsScope %d, declar(@%d/%d) ignoring (already added).\n", iParentScope, iDeclarDef, state->defsDeclars.use);
			def = NBArray_itmPtrAtIndex(&state->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
			NBASSERT(def->iParentScope == iParentScope) //must have same parent scope
			NBASSERT(def->iDeepLvl == parentScope->iDeepLvl) //must match deep level
			r = TRUE;
		} else if(iDeclarDef == state->defsDeclars.use){
			//Declaration is new
			//PRINTF_INFO("DeclarsScope %d, declar(@%d/%d) adding.\n", iParentScope, iDeclarDef, state->defsDeclars.use);
			STSrcAnlzCodeCDeclar data;
			data.iParentScope			= iParentScope;
			data.iDeepLvl				= parentScope->iDeepLvl;
			data.szInBits				= 0; //only for struct-or-union members
#			ifdef NB_CONFIG_INCLUDE_ASSERTS
			data.dbgIsTypeDef			= FALSE;
#			endif
			data.type.specifiersMask	= 0;
			data.type.iCustomTypeDef	= 0;
			data.type.ptrSpecsRng.start	= 0;
			data.type.ptrSpecsRng.count	= 0;
			data.type.isArray			= FALSE;
			data.type.arrayTotalSz		= 0;
			data.type.paramsBodyDefIdx	= 0;
			data.type.paramsLenFlexible	= FALSE;
			data.iNameFirstChar			= 0;
			data.iBodyDef				= 0;
			NBArray_addValue(&state->defsDeclars, data);
			//Update scopes's declars range
			{
				UI32 iDeclScope2 = iParentScope;
				STSrcAnlzCodeCDeclarsScope* scope2 = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
				do {
					//PRINTF_INFO("Updating scope %d's range from(%d, +%d) to(%d, +%d).\n", iDeclScope2, scope2->declarsRng.start, scope2->declarsRng.count, scope2->declarsRng.start, (state->defsDeclars.use - scope2->declarsRng.start));
					scope2->declarsRng.count = (state->defsDeclars.use - scope2->declarsRng.start);
					if(scope2->iParentScope == 0){
						break;
					} else {
						iDeclScope2 = scope2->iParentScope;
						scope2 = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
					}
				} while(TRUE);
			}
			def 		= NBArray_itmPtrAtIndex(&state->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
			isNewDef	= TRUE;
			r			= TRUE;
		} else {
			r = FALSE; NBASSERT(FALSE) //Program logic error
		}
	}
	if(dstDef != NULL) *dstDef = def;
	if(dstIsNewDef != NULL) *dstIsNewDef = isNewDef;
	return r;
}

BOOL SrcAnlzCodeC_getDeclarScopeByIdx(STSrcAnlzCodeC* state, const UI32 iDeclScopeDef, const UI32 iParentScope, STSrcAnlzCodeCDeclarsScope** dstDef, BOOL* dstIsNewDef){
	BOOL r = FALSE;
	STSrcAnlzCodeCDeclarsScope* def = NULL;
	BOOL isNewDef = FALSE;
	NBASSERT(iDeclScopeDef > 0 && iParentScope > 0 && iParentScope < state->defsDeclarsScopes.use)
	if(iDeclScopeDef > 0 && iParentScope > 0 && iParentScope < state->defsDeclarsScopes.use){
		const STSrcAnlzCodeCDeclarsScope* parentScope = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iParentScope);
		if(iDeclScopeDef < state->defsDeclarsScopes.use){
			//DeclarScope already added
			//PRINTF_INFO("DeclarsScope %d, declarScope(@%d/%d) ignoring (already added).\n", iParentScope, iDeclScopeDef, state->defsDeclarsScopes.use);
			def	= NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScopeDef);
			NBASSERT(def->iParentScope == iParentScope)
			NBASSERT(def->iDeepLvl == (parentScope->iDeepLvl + 1))
			r = TRUE;
		} else if(iDeclScopeDef == state->defsDeclarsScopes.use){
			//Add new body
			//PRINTF_INFO("DeclarsScope %d, declarScope(@%d/%d) adding.\n", iParentScope, iDeclScopeDef, state->defsDeclarsScopes.use);
			STSrcAnlzCodeCDeclarsScope dScope;
			dScope.iParentScope			= iParentScope;
			dScope.iDeepLvl				= (parentScope->iDeepLvl + 1);
			dScope.usrTypesRng.start	= state->defsUsrTypes.use;
			dScope.usrTypesRng.count	= 0;
			dScope.declarsRng.start		= state->defsDeclars.use;
			dScope.declarsRng.count		= 0;
			dScope.scopesRng.start		= state->defsDeclarsScopes.use + 1; //+1 to exclude itself
			dScope.scopesRng.count		= 0;
			NBArray_addValue(&state->defsDeclarsScopes, dScope);
			//Update scopes's subscopes range
			{
				UI32 iDeclScope2 = iParentScope;
				STSrcAnlzCodeCDeclarsScope* scope2 = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
				do {
					//PRINTF_INFO("Updating scope %d's range from(%d, +%d) to(%d, +%d).\n", iDeclScope2, scope2->scopesRng.start, scope2->scopesRng.count, scope2->scopesRng.start, (state->defsDeclarsScopes.use - scope2->scopesRng.start));
					scope2->scopesRng.count = (state->defsDeclarsScopes.use - scope2->scopesRng.start); NBASSERT(scope2->scopesRng.count > 0)
					if(scope2->iParentScope == 0){
						break;
					} else {
						iDeclScope2 = scope2->iParentScope;
						scope2 = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
					}
				} while(TRUE);
			}
			def 		= NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScopeDef);
			isNewDef	= TRUE;
			r			= TRUE;
		} else {
			r = FALSE; NBASSERT(FALSE)
		}
	}
	if(dstDef != NULL) *dstDef = def;
	if(dstIsNewDef != NULL) *dstIsNewDef = isNewDef;
	return r;
}

//Print

void SrcAnlzCodeC_print(const STSrcAnlzCodeC* state){
	if(state->defsDeclarsScopes.use > 1){
		STNBArraySorted usrTypesPrinted;
		NBArraySorted_init(&usrTypesPrinted, sizeof(UI32), NBCompareUI32);
		STNBString strTmp;
		NBString_init(&strTmp);
		const STSrcAnlzCodeCDeclarsScope* gScope = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, 1); //1 si the global scope
		PRINTF_INFO("GlobalScope iDeepLvl(%d) declarRng(#%d -> #%d, of %d) .\n", (SI32)gScope->iDeepLvl, (SI32)(gScope->declarsRng.start + 1), (SI32)(gScope->declarsRng.start + gScope->declarsRng.count), state->defsDeclars.use);
		SrcAnlzCodeC_printConcatScope(state, 1, 0, &strTmp, &usrTypesPrinted);
		PRINTF_INFO("%s", strTmp.str);
		NBArraySorted_release(&usrTypesPrinted);
		NBString_release(&strTmp);
	}
}

void SrcAnlzCodeC_printConcatScope(const STSrcAnlzCodeC* state, const UI32 iScope, const UI32 printDeepLvl, STNBString* dst, STNBArraySorted* usrTypesPrinted){
	const STSrcAnlzCodeCDeclarsScope* gScope = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iScope); //1 si the global scope
	UI32 i; const UI32 rngEnd = gScope->declarsRng.start + gScope->declarsRng.count;
	for(i = gScope->declarsRng.start; i < rngEnd; i++){
		const STSrcAnlzCodeCDeclar* declar = NBArray_itmPtrAtIndex(&state->defsDeclars, STSrcAnlzCodeCDeclar, i);
		if(declar->iDeepLvl == gScope->iDeepLvl){
			NBASSERT(declar->iParentScope == iScope)
			if(printDeepLvl != 0) NBString_concatRepeatedByte(dst, ' ', printDeepLvl * 4);
			SrcAnlzCodeC_printConcatDeclar(state, i, TRUE, printDeepLvl, dst, usrTypesPrinted);
			NBString_concatByte(dst, ';');
			NBString_concatByte(dst, '\n');
		}
	}
}

void SrcAnlzCodeC_printConcatDeclar(const STSrcAnlzCodeC* state, const UI32 iDeclar, const BOOL followDepthDeclar, const UI32 printDeepLvl, STNBString* dst, STNBArraySorted* usrTypesPrinted){
	UI32 concatdCount = 0;
	NBASSERT(iDeclar >= 0 && iDeclar < state->defsDeclars.use)
	const STSrcAnlzCodeCDeclar* declar = NBArray_itmPtrAtIndex(&state->defsDeclars, STSrcAnlzCodeCDeclar, iDeclar);
	if((declar->type.specifiersMask & ENSrcAnlzDecSpecifiersBit_TS_Declar) != 0 && followDepthDeclar){
		NBASSERT(declar->type.iCustomTypeDef != 0)
		//Follow deeper def
		STNBArray declars;
		NBArray_init(&declars, sizeof(UI32), NBCompareUI32);
		NBArray_add(&declars, &iDeclar, sizeof(iDeclar));
		do {
			NBASSERT(declar->type.iCustomTypeDef >= 0 && declar->type.iCustomTypeDef < state->defsDeclars.use)
			NBArray_add(&declars, &declar->type.iCustomTypeDef, sizeof(declar->type.iCustomTypeDef));
			declar = NBArray_itmPtrAtIndex(&state->defsDeclars, STSrcAnlzCodeCDeclar, declar->type.iCustomTypeDef);
			NBASSERT((declar->type.specifiersMask & ENSrcAnlzDecSpecifiersBit_TS_Declar) == 0 || declar->type.iCustomTypeDef != 0)
		} while((declar->type.specifiersMask & ENSrcAnlzDecSpecifiersBit_TS_Declar) != 0);
		//
		SrcAnlzCodeC_printConcatDeclarWithPathReverse(state, (const UI32*)NBArray_data(&declars), declars.use, declars.use - 1, printDeepLvl, dst, usrTypesPrinted);
		//
		NBArray_release(&declars);
	} else {
		//Specifiers
		NBASSERT(declar->type.specifiersMask != 0)
		if(declar->type.specifiersMask != 0){
			concatdCount += SrcAnlzCodeC_printConcatSpecMask(state, declar, declar->type.specifiersMask, concatdCount, printDeepLvl, dst, usrTypesPrinted);
			NBASSERT(concatdCount != 0)
		}
		//declar's name
		{
			BOOL spaceAdded = FALSE;
			//Concat this name
			//Pointer specs
			if(declar->type.ptrSpecsRng.count != 0){
				const UI32 iLastAfter = declar->type.ptrSpecsRng.start + declar->type.ptrSpecsRng.count;
				UI32 i; for(i = declar->type.ptrSpecsRng.start; i < iLastAfter; i++){
					const STSrcAnlzCodeCDeclarPtr* spec = NBArray_itmPtrAtIndex(&state->defsDeclPtrSpecs, STSrcAnlzCodeCDeclarPtr, i);
					if(spec->iDeepLvl == declar->iDeepLvl){
						if(!spaceAdded){
							if(dst->length != 0) NBString_concatByte(dst, ' ');
							spaceAdded = TRUE;
						}
						NBString_concatByte(dst, '*');
						if(spec->specifiersMask != 0){
							concatdCount += SrcAnlzCodeC_printConcatSpecMask(state, declar, spec->specifiersMask, concatdCount, printDeepLvl, dst, usrTypesPrinted);
						}
					}
				}
			}
			//Plain name
			const char* name = &state->strsBuff.str[declar->iNameFirstChar];
			if(name[0] != '\0'){
				if(!spaceAdded){
					if(dst->length != 0) NBString_concatByte(dst, ' ');
					spaceAdded = TRUE;
				}
				NBString_concat(dst, &state->strsBuff.str[declar->iNameFirstChar]);
			}
		}
		//declar's params
		if(declar->type.paramsBodyDefIdx != 0){
			NBString_concatByte(dst, '(');
			const UI32 pCount = SrcAnlzCodeC_printConcatParams(state, declar->type.paramsBodyDefIdx, dst, usrTypesPrinted);
			if(declar->type.paramsLenFlexible){
				if(pCount != 0) NBString_concat(dst, ", ");
				NBString_concat(dst, "...");
			}
			NBString_concatByte(dst, ')');
		}
		//declar's size in bits
		if(declar->szInBits){
			if(dst->length != 0) NBString_concatByte(dst, ' ');
			NBString_concat(dst, " : ");
			NBString_concatUI32(dst, (UI32)declar->szInBits);
		}
		//declar's body
		if(declar->iBodyDef){
			NBASSERT(declar->iBodyDef >= 0 && declar->iBodyDef < state->defsDeclarsScopes.use)
			if(dst->length != 0) NBString_concatByte(dst, ' ');
			NBString_concat(dst, "{\n");
			SrcAnlzCodeC_printConcatScope(state, declar->iBodyDef, printDeepLvl + 1, dst, usrTypesPrinted);
			if(printDeepLvl != 0) NBString_concatRepeatedByte(dst, ' ', printDeepLvl * 4);
			NBString_concat(dst, "}");
		}
	}
}

void SrcAnlzCodeC_printConcatDeclarWithPathReverse(const STSrcAnlzCodeC* state, const UI32* pathItems, const UI32 pathSz, const UI32 iPathItem, const UI32 printDeepLvl, STNBString* dst, STNBArraySorted* usrTypesPrinted){
	UI32 concatdCount = 0;
	NBASSERT(iPathItem >= 0 && iPathItem < pathSz)
	const UI32 iDeclar = pathItems[iPathItem];
	NBASSERT(iDeclar >= 0 && iDeclar < state->defsDeclars.use)
	const STSrcAnlzCodeCDeclar* declar = NBArray_itmPtrAtIndex(&state->defsDeclars, STSrcAnlzCodeCDeclar, iDeclar);
	//Specifiers
	NBASSERT(declar->type.specifiersMask != 0)
	if(declar->type.specifiersMask != 0){
		concatdCount += SrcAnlzCodeC_printConcatSpecMask(state, declar, declar->type.specifiersMask, concatdCount, printDeepLvl, dst, usrTypesPrinted);
		//NBASSERT(concatdCount != 0)
	}
	//declar's name
	{
		BOOL spaceAdded = FALSE;
		//Concat this name
		//Pointer specs
		if(declar->type.ptrSpecsRng.count != 0){
			const UI32 iLastAfter = declar->type.ptrSpecsRng.start + declar->type.ptrSpecsRng.count;
			UI32 i; for(i = declar->type.ptrSpecsRng.start; i < iLastAfter; i++){
				const STSrcAnlzCodeCDeclarPtr* spec = NBArray_itmPtrAtIndex(&state->defsDeclPtrSpecs, STSrcAnlzCodeCDeclarPtr, i);
				if(spec->iDeepLvl == declar->iDeepLvl){
					if(!spaceAdded){
						if(concatdCount != 0) NBString_concatByte(dst, ' ');
						spaceAdded = TRUE;
					}
					NBString_concatByte(dst, '*');
					if(spec->specifiersMask != 0){
						concatdCount += SrcAnlzCodeC_printConcatSpecMask(state, declar, spec->specifiersMask, concatdCount, printDeepLvl, dst, usrTypesPrinted);
					}
				}
			}
		}
		//Plain name
		const char* name = &state->strsBuff.str[declar->iNameFirstChar];
		if(name[0] != '\0'){
			if(!spaceAdded){
				if(dst->length != 0) NBString_concatByte(dst, ' ');
				spaceAdded = TRUE;
			}
			NBString_concat(dst, &state->strsBuff.str[declar->iNameFirstChar]);
		}
		//Parent declar
		if(iPathItem > 0){
			if(!spaceAdded){
				if(dst->length != 0) NBString_concatByte(dst, ' ');
				spaceAdded = TRUE;
			}
			NBString_concatByte(dst, '(');
			SrcAnlzCodeC_printConcatDeclarWithPathReverse(state, pathItems, pathSz, iPathItem - 1, printDeepLvl, dst, usrTypesPrinted);
			NBString_concatByte(dst, ')');
		}
	}
	//declar's params
	if(declar->type.paramsBodyDefIdx != 0){
		NBString_concatByte(dst, '(');
		const UI32 pCount = SrcAnlzCodeC_printConcatParams(state, declar->type.paramsBodyDefIdx, dst, usrTypesPrinted);
		if(declar->type.paramsLenFlexible){
			if(pCount != 0) NBString_concat(dst, ", ");
			NBString_concat(dst, "...");
		}
		NBString_concatByte(dst, ')');
	}
	//declar's size in bits
	if(declar->szInBits){
		if(dst->length != 0) NBString_concatByte(dst, ' ');
		NBString_concat(dst, " : ");
		NBString_concatUI32(dst, (UI32)declar->szInBits);
	}
	//declar's body
	if(declar->iBodyDef){
		NBASSERT(declar->iBodyDef >= 0 && declar->iBodyDef < state->defsDeclarsScopes.use)
		if(dst->length != 0) NBString_concatByte(dst, ' ');
		NBString_concat(dst, "{\n");
		SrcAnlzCodeC_printConcatScope(state, declar->iBodyDef, printDeepLvl + 1, dst, usrTypesPrinted);
		if(printDeepLvl != 0) NBString_concatRepeatedByte(dst, ' ', printDeepLvl * 4);
		NBString_concat(dst, "}");
	}
}

UI32 SrcAnlzCodeC_printConcatParams(const STSrcAnlzCodeC* state, const UI32 iScope, STNBString* dst, STNBArraySorted* usrTypesPrinted){
	UI32 r = 0;
	const STSrcAnlzCodeCDeclarsScope* pScope = NBArray_itmPtrAtIndex(&state->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iScope);
	UI32 i; const UI32 rngEnd = pScope->declarsRng.start + pScope->declarsRng.count;
	for(i = pScope->declarsRng.start; i < rngEnd; i++){
		const STSrcAnlzCodeCDeclar* declar = NBArray_itmPtrAtIndex(&state->defsDeclars, STSrcAnlzCodeCDeclar, i);
		if(declar->iDeepLvl == pScope->iDeepLvl){
			if(r != 0) NBString_concat(dst, ", ");
			SrcAnlzCodeC_printConcatDeclar(state, i, TRUE, 0, dst, usrTypesPrinted);
			r++;
		}
	}
	return r;
}

UI32 SrcAnlzCodeC_printConcatSpecMask(const STSrcAnlzCodeC* state, const STSrcAnlzCodeCDeclar* declar, const UI32 specMask, const UI32 concatdCount, const UI32 printDeepLvl, STNBString* dst, STNBArraySorted* usrTypesPrinted){
	UI32 r = concatdCount;
	if((specMask & ENSrcAnlzDecSpecifiersBit_TQ_Const) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "const"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TQ_Restrict) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "restrict"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TQ_Volatile) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "volatile"); r++; }
	//
	if((specMask & ENSrcAnlzDecSpecifiersBit_FS_inline) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "inline"); r++; }
	//
	if((specMask & ENSrcAnlzDecSpecifiersBit_SC_Typedef) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "typedef"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_SC_Extern) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "extern"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_SC_Static) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "static"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_SC_Auto) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "auto"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_SC_Register) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "register"); r++; }
	//
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Void) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "void"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Char) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "char"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Short) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "short"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Int) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "int"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Long) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "long"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_LongLong) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "long long"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Float) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "float"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Double) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "double"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Signed) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "signed"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Unsigned) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "unsigned"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS__Bool) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "__Bool"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS__Complex) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "__Complex"); r++; }
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_StructUnion) != 0){
		NBASSERT(declar->type.iCustomTypeDef > 0 && declar->type.iCustomTypeDef < state->defsUsrTypes.use)
		const STSrcAnlzCodeCUsrType* usrType = NBArray_itmPtrAtIndex(&state->defsUsrTypes, STSrcAnlzCodeCUsrType, declar->type.iCustomTypeDef);
		switch (usrType->type) {
			case ENSrcAnlzCodeUsrType_Struct:
				if(r != 0) NBString_concatByte(dst, ' ');
				NBString_concat(dst, "struct");
				r++;
				break;
			case ENSrcAnlzCodeUsrType_Union:
				if(r != 0) NBString_concatByte(dst, ' ');
				NBString_concat(dst, "union");
				r++;
				break;
			default: NBASSERT(FALSE) break;
		}
		//struct-union's body
		if(NBArraySorted_indexOf(usrTypesPrinted, &declar->type.iCustomTypeDef, sizeof(declar->type.iCustomTypeDef), NULL) == -1){
			NBArraySorted_addValue(usrTypesPrinted, declar->type.iCustomTypeDef);
			if(usrType->iBodyDef != 0){
				if(dst->length != 0) NBString_concatByte(dst, ' ');
				NBString_concat(dst, "{\n");
				SrcAnlzCodeC_printConcatScope(state, usrType->iBodyDef, printDeepLvl + 1, dst, usrTypesPrinted);
				if(printDeepLvl != 0) NBString_concatRepeatedByte(dst, ' ', printDeepLvl * 4);
				NBString_concat(dst, "}");
			}
		}
		//struct-union's name
		if(usrType->iNameFirstChar != 0){
			if(r != 0) NBString_concatByte(dst, ' ');
			NBString_concat(dst, &state->strsBuff.str[usrType->iNameFirstChar]);
		}
	}
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Enum) != 0){
		NBASSERT(declar->type.iCustomTypeDef > 0 && declar->type.iCustomTypeDef < state->defsUsrTypes.use)
		const STSrcAnlzCodeCUsrType* usrType = NBArray_itmPtrAtIndex(&state->defsUsrTypes, STSrcAnlzCodeCUsrType, declar->type.iCustomTypeDef);
		switch (usrType->type) {
			case ENSrcAnlzCodeUsrType_Enum:
				if(r != 0) NBString_concatByte(dst, ' ');
				NBString_concat(dst, "enum");
				r++;
				break;
			default: NBASSERT(FALSE) break;
		}
		//enum's body
		if(NBArraySorted_indexOf(usrTypesPrinted, &declar->type.iCustomTypeDef, sizeof(declar->type.iCustomTypeDef), NULL) == -1){
			NBArraySorted_addValue(usrTypesPrinted, declar->type.iCustomTypeDef);
			if(usrType->iBodyDef != 0){
				if(dst->length != 0) NBString_concatByte(dst, ' ');
				NBString_concat(dst, "{\n");
				SrcAnlzCodeC_printConcatScope(state, usrType->iBodyDef, printDeepLvl + 1, dst, usrTypesPrinted);
				if(printDeepLvl != 0) NBString_concatRepeatedByte(dst, ' ', printDeepLvl * 4);
				NBString_concat(dst, "}");
			}
		}
		//enum's name
		if(usrType->iNameFirstChar != 0){
			if(r != 0) NBString_concatByte(dst, ' ');
			NBString_concat(dst, &state->strsBuff.str[usrType->iNameFirstChar]);
		}
	}
	if((specMask & ENSrcAnlzDecSpecifiersBit_TS_TypedefName) != 0){
		NBASSERT(declar->type.iCustomTypeDef > 0 && declar->type.iCustomTypeDef < state->defsDeclars.use)
		const STSrcAnlzCodeCDeclar* typeDeclar = NBArray_itmPtrAtIndex(&state->defsDeclars, STSrcAnlzCodeCDeclar, declar->type.iCustomTypeDef);
		if(r != 0) NBString_concatByte(dst, ' ');
		NBString_concat(dst, &state->strsBuff.str[typeDeclar->iNameFirstChar]); r++;
	}
	//if((specMask & ENSrcAnlzDecSpecifiersBit_TS_Declar) != 0){ if(r != 0) NBString_concatByte(dst, ' '); NBString_concat(dst, "declar"); r++; }
	return (r - concatdCount);
}





