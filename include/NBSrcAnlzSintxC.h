//
//  NBSrcAnlzSintxC.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_SINTX_C_H
#define NB_SRCANLZ_SINTX_C_H

#ifdef __cplusplus
//extern "C" {
#endif

#include "nb/NBFrameworkDefs.h"
#include "NBSrcAnlzSintx.h"	//for 'STSrcAnlzSintxDefs'
#include "NBSrcAnlzExpr.h"	//for 'STSrcAnlzExprOpPriority'

//-------------------------
// Lang operators sintax
//-------------------------
//https://www.tutorialspoint.com/cprogramming/c_operators.htm

// Operators priorities

typedef enum ENSrcAnlzSintxCOpPriority_ {
	//Bigger priority
	ENSrcAnlzSintxCOpPriority_Postf = 0, // () [] -> . ++ -- (Left to right)
	ENSrcAnlzSintxCOpPriority_Unary,	// + - ! ~ ++ -- (type) * & sizeof (Right to left)
	ENSrcAnlzSintxCOpPriority_Mult,		// * / % (Left to right)
	ENSrcAnlzSintxCOpPriority_Add,		// + - (Left to right)
	ENSrcAnlzSintxCOpPriority_Shift,	// << >> (Left to right)
	ENSrcAnlzSintxCOpPriority_Relat,	// < <= > >= (Left to right)
	ENSrcAnlzSintxCOpPriority_Equal,	// == != (Left to right)
	ENSrcAnlzSintxCOpPriority_BAnd,		// & (Left to right)
	ENSrcAnlzSintxCOpPriority_BXor,		// ^ (Left to right)
	ENSrcAnlzSintxCOpPriority_BOr,		// | (Left to right)
	ENSrcAnlzSintxCOpPriority_And,		// && (Left to right)
	ENSrcAnlzSintxCOpPriority_Or,		// || (Left to right)
	ENSrcAnlzSintxCOpPriority_Cond,		// ?: (Right to left)
	ENSrcAnlzSintxCOpPriority_Assig,	// = += -= *= /= %= >>= <<= &= ^= |= (Right to left)
	ENSrcAnlzSintxCOpPriority_Comma,	// , (Left to right)
	//Lower priority
	ENSrcAnlzSintxCOpPriority_Count,
} ENSrcAnlzSintxCOpPriority;

static const STSrcAnlzExprOpPriority __globalSintxCOpPriorities[] = {
	{ ENSrcAnlzSintxCOpPriority_Postf	, "Postfix" }
	, { ENSrcAnlzSintxCOpPriority_Unary	, "Unary" }
	, { ENSrcAnlzSintxCOpPriority_Mult	, "Multiplicative" }
	, { ENSrcAnlzSintxCOpPriority_Add	, "Additive" }
	, { ENSrcAnlzSintxCOpPriority_Shift	, "Shift" }
	, { ENSrcAnlzSintxCOpPriority_Relat	, "Relational" }
	, { ENSrcAnlzSintxCOpPriority_Equal	, "Equality" }
	, { ENSrcAnlzSintxCOpPriority_BAnd	, "BitwiseAND" }
	, { ENSrcAnlzSintxCOpPriority_BXor	, "BitwiseXOR" }
	, { ENSrcAnlzSintxCOpPriority_BOr	, "BitwiseOR" }
	, { ENSrcAnlzSintxCOpPriority_And	, "LogicalAND" }
	, { ENSrcAnlzSintxCOpPriority_Or	, "LogicalOR" }
	, { ENSrcAnlzSintxCOpPriority_Cond	, "Conditional" }
	, { ENSrcAnlzSintxCOpPriority_Assig	, "Assignment" }
	, { ENSrcAnlzSintxCOpPriority_Comma	, "Comma" }
	, { ENSrcAnlzSintxCOpPriority_Count	, "Last" }
};

// Operators types

typedef enum ENSrcAnlzSintxCOpType_ {
	ENSrcAnlzSintxCOpType_Logical = 0, // &&, ||, !
	ENSrcAnlzSintxCOpType_Relatnl,	// ==, !=, <, <=, ...
	ENSrcAnlzSintxCOpType_Arithm,	// +, -, *, /, ...
	ENSrcAnlzSintxCOpType_Bitwise,	// &, |, ~, ...
	ENSrcAnlzSintxCOpType_Assign,	// =, +=, *=, /=, ...
	ENSrcAnlzSintxCOpType_Prog,		// (type), *, &, sizeof, ., ->, ...
	ENSrcAnlzSintxCOpType_Preproc,	// defined, ...
	ENSrcAnlzSintxCOpType_Count,
} ENSrcAnlzSintxCOpType;

// Operators type 'Logic'

typedef enum ENSrcAnlzSintxCOpLogic_ {
	ENSrcAnlzSintxCOpLogic_And = 0,	// '&&'
	ENSrcAnlzSintxCOpLogic_Or,		// '||'
	ENSrcAnlzSintxCOpLogic_Neg,		// '!'
	ENSrcAnlzSintxCOpLogic_Count
} ENSrcAnlzSintxCOpLogic;

// Operators type 'Relation'

typedef enum ENSrcAnlzSintxCOpRelat_ {
	ENSrcAnlzSintxCOpRelat_Equal = 0,// '=='
	ENSrcAnlzSintxCOpRelat_Ineq,	// '!='
	ENSrcAnlzSintxCOpRelat_Great,	// '>'
	ENSrcAnlzSintxCOpRelat_Lower,	// '<'
	ENSrcAnlzSintxCOpRelat_GrtEq,	// '>='
	ENSrcAnlzSintxCOpRelat_LowEq,	// '<='
	ENSrcAnlzSintxCOpRelat_Count
} ENSrcAnlzSintxCOpRelat;

// Operators type 'Arithmetic'

typedef enum ENSrcAnlzSintxCOpArith_ {
	ENSrcAnlzSintxCOpArith_Add = 0,	// '+'
	ENSrcAnlzSintxCOpArith_Sub,		// '-'
	ENSrcAnlzSintxCOpArith_Mul,		// '*'
	ENSrcAnlzSintxCOpArith_Div,		// '/'
	ENSrcAnlzSintxCOpArith_Mod,		// '%'
	ENSrcAnlzSintxCOpArith_Inc,		// '++'
	ENSrcAnlzSintxCOpArith_Dec,		// '--'
	ENSrcAnlzSintxCOpArith_Count
} ENSrcAnlzSintxCOpArith;

// Operators type 'Bitwise'

typedef enum ENSrcAnlzSintxCOpBitw_ {
	ENSrcAnlzSintxCOpBitw_And = 0,	// '&'
	ENSrcAnlzSintxCOpBitw_Or,		// '|'
	ENSrcAnlzSintxCOpBitw_Xor,		// '^' //binary complement (only activate bits activated in both operands)
	ENSrcAnlzSintxCOpBitw_Comp,		// '~' //binary complement (flipping bits values)
	ENSrcAnlzSintxCOpBitw_Right,	// '>>'
	ENSrcAnlzSintxCOpBitw_Left,		// '<<'
	ENSrcAnlzSintxCOpBitw_Count
} ENSrcAnlzSintxCOpBitw;

// Operators type 'Assignment'

typedef enum ENSrcAnlzSintxCOpAssign_ {
	ENSrcAnlzSintxCOpAssign_Set = 0,// '='
	ENSrcAnlzSintxCOpAssign_Add,	// '+='
	ENSrcAnlzSintxCOpAssign_Sub,	// '-='
	ENSrcAnlzSintxCOpAssign_Mult,	// '*='
	ENSrcAnlzSintxCOpAssign_Div,	// '/='
	ENSrcAnlzSintxCOpAssign_Mod,	// '%='
	ENSrcAnlzSintxCOpAssign_BitsAnd,// '&='
	ENSrcAnlzSintxCOpAssign_BitsXor,// '^='
	ENSrcAnlzSintxCOpAssign_BitsOr,	// '|='
	ENSrcAnlzSintxCOpAssign_BitsR,	// '>>='
	ENSrcAnlzSintxCOpAssign_BitsL,	// '<<='
	ENSrcAnlzSintxCOpAssign_Count
} ENSrcAnlzSintxCOpAssign;

// Operators type 'Programming'

typedef enum ENSrcAnlzSintxCOpProg_ {
	ENSrcAnlzSintxCOpProg_ValOf = 0,// '*'
	ENSrcAnlzSintxCOpProg_PtrOf,	// '&'
	ENSrcAnlzSintxCOpProg_SizeOf,	// 'sizeof'
	ENSrcAnlzSintxCOpProg_MbrOf,	// '.'
	ENSrcAnlzSintxCOpProg_MbrOfPtr,	// '->'
	ENSrcAnlzSintxCOpProg_GrpOpen,	// '('
	ENSrcAnlzSintxCOpProg_GrpClose,	// ')'
	ENSrcAnlzSintxCOpProg_IdxOpen,	// '['
	ENSrcAnlzSintxCOpProg_IdxClose,	// ']'
	ENSrcAnlzSintxCOpProg_BlckOpen,	// '{'
	ENSrcAnlzSintxCOpProg_BlckClose,// '}'
	ENSrcAnlzSintxCOpProg_Comma,	// ','
	//type casting					// '(type)'
	ENSrcAnlzSintxCOpProg_CondStart,// '?'
	ENSrcAnlzSintxCOpProg_CondMid,	// ':'
	ENSrcAnlzSintxCOpProg_Count
} ENSrcAnlzSintxCOpProg;

// Operators type 'Preprocessor'

typedef enum ENSrcAnlzSintxCOpPProc_ {
	ENSrcAnlzSintxCOpPProc_def = 0,	// 'defined'
} ENSrcAnlzSintxCOpPProc;

// Operators chars

typedef enum ENSrcAnlzSintxCOp_ {
	//One char
	ENSrcAnlzSintxCOp_GrpOpen = 0,	// '('
	ENSrcAnlzSintxCOp_GrpClose,		// ')'
	ENSrcAnlzSintxCOp_Comma,		// ','
	ENSrcAnlzSintxCOp_Exclamation,	// '!'
	ENSrcAnlzSintxCOp_Ampersand,	// '&'
	ENSrcAnlzSintxCOp_VertBar,		// '|'
	ENSrcAnlzSintxCOp_GreatThan,	// '>'
	ENSrcAnlzSintxCOp_LessThan,		// '<'
	ENSrcAnlzSintxCOp_Plus,			// '+'
	ENSrcAnlzSintxCOp_Minus,		// '-'
	ENSrcAnlzSintxCOp_Asterisk,		// '*'
	ENSrcAnlzSintxCOp_Slash,		// '/'
	ENSrcAnlzSintxCOp_Percent,		// '%'
	ENSrcAnlzSintxCOp_Tilde,		// '~'
	ENSrcAnlzSintxCOp_Caret,		// '^'
	ENSrcAnlzSintxCOp_Equals,		// '='
	ENSrcAnlzSintxCOp_Dot,			// '.'
	ENSrcAnlzSintxCOp_IdxOpen,		// '['
	ENSrcAnlzSintxCOp_IdxClose,		// ']'
	ENSrcAnlzSintxCOp_BlckOpen,		// '{'
	ENSrcAnlzSintxCOp_BlckClose,	// '}'
	ENSrcAnlzSintxCOp_QuestM,		// '?'
	ENSrcAnlzSintxCOp_TwoPoints,	// ':'
	//Two chars
	ENSrcAnlzSintxCOp_Ampersand2,	// '&&'
	ENSrcAnlzSintxCOp_VertBar2,		// '||'
	ENSrcAnlzSintxCOp_Equals2,		// '=='
	ENSrcAnlzSintxCOp_Inequal,		// '!='
	ENSrcAnlzSintxCOp_GreatEqual,	// '>='
	ENSrcAnlzSintxCOp_LessEqual,	// '<='
	ENSrcAnlzSintxCOp_Plus2,		// '++'
	ENSrcAnlzSintxCOp_Minus2,		// '--'
	ENSrcAnlzSintxCOp_AssignAdd,	// '+='
	ENSrcAnlzSintxCOp_AssignSub,	// '-='
	ENSrcAnlzSintxCOp_AssignMult,	// '*='
	ENSrcAnlzSintxCOp_AssignDiv,	// '/='
	ENSrcAnlzSintxCOp_AssignMod,	// '%='
	ENSrcAnlzSintxCOp_AssignBitsAnd,// '&='
	ENSrcAnlzSintxCOp_AssignBitsXor,// '^='
	ENSrcAnlzSintxCOp_AssignBitsOr,	// '|='
	ENSrcAnlzSintxCOp_GreatThan2,	// '>>'
	ENSrcAnlzSintxCOp_LessThan2,	// '<<'
	ENSrcAnlzSintxCOp_Arrow,		// '->'
	//Three chars
	ENSrcAnlzSintxCOp_AssignBitsR,	// '>>='
	ENSrcAnlzSintxCOp_AssignBitsL,	// '<<='
	//Others
	ENSrcAnlzSintxCOp_Sizeof,		// 'sizeof'
	ENSrcAnlzSintxCOp_Defined,		// 'defined'
	//
	ENSrcAnlzSintxCOp_Count
} ENSrcAnlzSintxCOp;

//-------------------------
// Lang general sintax
//-------------------------

typedef enum ENSrzAnlzSintxC_ {
	ENSrzAnlzSintxC_primary_expression = 0
	, ENSrzAnlzSintxC_postfix_expression
	, ENSrzAnlzSintxC_argument_expression_list
	, ENSrzAnlzSintxC_unary_expression
	, ENSrzAnlzSintxC_unary_operator
	, ENSrzAnlzSintxC_cast_expression
	, ENSrzAnlzSintxC_multiplicative_expression
	, ENSrzAnlzSintxC_additive_expression
	, ENSrzAnlzSintxC_shift_expression
	, ENSrzAnlzSintxC_relational_expression
	, ENSrzAnlzSintxC_equality_expression
	, ENSrzAnlzSintxC_AND_expression
	, ENSrzAnlzSintxC_exclusive_OR_expression
	, ENSrzAnlzSintxC_inclusive_OR_expression
	, ENSrzAnlzSintxC_logical_AND_expression
	, ENSrzAnlzSintxC_logical_OR_expression
	, ENSrzAnlzSintxC_conditional_expression
	, ENSrzAnlzSintxC_assignment_expression
	, ENSrzAnlzSintxC_assignment_operator
	, ENSrzAnlzSintxC_expression
	, ENSrzAnlzSintxC_constant_expression
	, ENSrzAnlzSintxC_declaration
	, ENSrzAnlzSintxC_declaration_specifiers
	, ENSrzAnlzSintxC_init_declarator_list
	, ENSrzAnlzSintxC_init_declarator
	, ENSrzAnlzSintxC_storage_class_specifier
	, ENSrzAnlzSintxC_type_specifier
	, ENSrzAnlzSintxC_struct_or_union_specifier
	, ENSrzAnlzSintxC_struct_or_union
	, ENSrzAnlzSintxC_struct_declaration_list
	, ENSrzAnlzSintxC_struct_declaration
	, ENSrzAnlzSintxC_specifier_qualifier_list
	, ENSrzAnlzSintxC_struct_declarator_list
	, ENSrzAnlzSintxC_struct_declarator
	, ENSrzAnlzSintxC_enum_specifier
	, ENSrzAnlzSintxC_enumerator_list
	, ENSrzAnlzSintxC_enumeration_constant
	, ENSrzAnlzSintxC_enumerator
	, ENSrzAnlzSintxC_type_qualifier
	, ENSrzAnlzSintxC_function_specifier
	, ENSrzAnlzSintxC_declarator
	, ENSrzAnlzSintxC_direct_declarator
	, ENSrzAnlzSintxC_pointer
	, ENSrzAnlzSintxC_type_qualifier_list
	, ENSrzAnlzSintxC_parameter_type_list
	, ENSrzAnlzSintxC_parameter_list
	, ENSrzAnlzSintxC_parameter_declaration
	, ENSrzAnlzSintxC_identifier_list
	, ENSrzAnlzSintxC_type_name
	, ENSrzAnlzSintxC_abstract_declarator
	, ENSrzAnlzSintxC_direct_abstract_declarator
	, ENSrzAnlzSintxC_typedef_name
	, ENSrzAnlzSintxC_initializer
	, ENSrzAnlzSintxC_initializer_list
	, ENSrzAnlzSintxC_designation
	, ENSrzAnlzSintxC_designator_list
	, ENSrzAnlzSintxC_designator
	, ENSrzAnlzSintxC_statement
	, ENSrzAnlzSintxC_labeled_statement
	, ENSrzAnlzSintxC_compound_statement
	, ENSrzAnlzSintxC_block_item_list
	, ENSrzAnlzSintxC_block_item
	, ENSrzAnlzSintxC_expression_statement
	, ENSrzAnlzSintxC_selection_statement
	, ENSrzAnlzSintxC_iteration_statement
	, ENSrzAnlzSintxC_jump_statement
	, ENSrzAnlzSintxC_translation_unit
	, ENSrzAnlzSintxC_external_declaration
	, ENSrzAnlzSintxC_function_definition
	, ENSrzAnlzSintxC_declaration_list
	, ENSrzAnlzSintxC_Count
} ENSrzAnlzSintxC;

//Set defs

void SrcAnlzSintxC_initDefs(STSrcAnlzSintxDefs* dst);

#ifdef __cplusplus
//}
#endif

#endif
