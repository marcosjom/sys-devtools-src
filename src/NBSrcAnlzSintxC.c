//
//  NBSrcAnlzSintxC.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzSintxC.h"

static const char* __globalKeywords_c[] = {
	//1-char
	//2-chars
	"do", "if"
	//3-chars
	, "for", "int"
	//4-chars
	, "auto", "case", "char", "else", "enum", "goto", "long", "void"
	//5-chars
	, "break", "const", "float", "short", "union", "while", "_Bool"
	//6-chars
	, "double", "extern", "inline", "return", "signed", "sizeof", "static", "struct", "switch"
	//7-chars
	, "default", "typedef"
	//8-chars
	, "continue", "register", "restrict", "unsigned", "volatile", "_Complex"
	//9-chars
	//10-chars
	, "_Imaginary"
	, NULL
};

static const STSrcAnlzRange __globalKeywords_c_lenRngs[] = {
	{0, 0} //1-char
	, {0, 2} //2-chars
	, {2, 2} //3-chars
	, {4, 8} //4-chars
	, {12, 7} //5-chars
	, {19, 9} //6-chars
	, {28, 2} //7-chars
	, {30, 6} //8-chars
	, {36, 0} //9-chars
	, {36, 1} //10-chars
};

static const char* __globalPunctuators_c[] = {
	//1-char
	"[", "]", "(", ")", "{", "}", ".", "&", "*", "+", "-", "~", "!", "/", "%", "<", ">", "^", "|", "?", ":", ";", "=", ",", "#"
	//2-chars
	, "->", "++", "--", "<<", ">>", "<=", ">=", "==", "!=", "&&", "||", "*=", "/=", "%=", "+=", "-=", "&=", "^=", "|=", "##", "<:", ":>", "<%", "%>", "%:"
	//3-chars
	, "...", "<<=", ">>="
	//4-chars
	, "%:%:"
	, NULL
};

static const STSrcAnlzRange __globalPunctuators_c_lenRngs[] = {
	{0, 25} //1-char
	, {25, 25} //2-chars
	, {50, 3} //3-chars
	, {53, 1} //4-chars
};

//-------------------------
// Lang general sintax
//-------------------------

//
// Optimization: array of def posibilities
// (pregenerated deep search arrays, no-definitions references)
//

const STSrcAnlzSintxPart __globalSintxCSeqHints_primary_expression[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_argument_expression_list[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_operator[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_cast_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_multiplicative_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_additive_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_shift_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_relational_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_equality_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_AND_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_exclusive_OR_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_inclusive_OR_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_logical_AND_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_logical_OR_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_conditional_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 22 }
	, { ENSrzAnlzSintxPartType_Punctuator, 36 }
	, { ENSrzAnlzSintxPartType_Punctuator, 37 }
	, { ENSrzAnlzSintxPartType_Punctuator, 38 }
	, { ENSrzAnlzSintxPartType_Punctuator, 39 }
	, { ENSrzAnlzSintxPartType_Punctuator, 40 }
	, { ENSrzAnlzSintxPartType_Punctuator, 41 }
	, { ENSrzAnlzSintxPartType_Punctuator, 42 }
	, { ENSrzAnlzSintxPartType_Punctuator, 43 }
	, { ENSrzAnlzSintxPartType_Punctuator, 51 }
	, { ENSrzAnlzSintxPartType_Punctuator, 52 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_constant_expression[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_specifiers[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_init_declarator_list[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 23 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 24 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_init_declarator[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_storage_class_specifier[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_or_union_specifier[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_or_union[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declaration_list[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 29 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 30 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declaration[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_specifier_qualifier_list[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declarator_list[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 20 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 32 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 33 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declarator[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 20 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enum_specifier[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enumerator_list[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 35 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 36 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 37 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enumeration_constant[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enumerator[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 36 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_qualifier[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_function_specifier[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declarator[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_pointer[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_qualifier_list[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 43 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_parameter_type_list[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 45 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 46 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_parameter_list[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 45 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 46 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_parameter_declaration[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_identifier_list[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 47 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_name[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_abstract_declarator[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_typedef_name[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_initializer[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_initializer_list[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 52 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 53 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 54 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 55 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 56 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_designation[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 55 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 56 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_designator_list[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 55 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 56 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_designator[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_statement[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 1 }
	, { ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Keyword, 5 }
	, { ENSrzAnlzSintxPartType_Keyword, 9 }
	, { ENSrzAnlzSintxPartType_Keyword, 12 }
	, { ENSrzAnlzSintxPartType_Keyword, 17 }
	, { ENSrzAnlzSintxPartType_Keyword, 22 }
	, { ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Keyword, 27 }
	, { ENSrzAnlzSintxPartType_Keyword, 28 }
	, { ENSrzAnlzSintxPartType_Keyword, 30 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 58 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 59 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 62 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 63 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 64 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 65 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_labeled_statement[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 5 }
	, { ENSrzAnlzSintxPartType_Keyword, 28 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_compound_statement[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_block_item_list[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 1 }
	, { ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 5 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 9 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 12 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 17 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 22 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 27 }
	, { ENSrzAnlzSintxPartType_Keyword, 28 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 30 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 57 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 58 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 59 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 60 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 61 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 62 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 63 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 64 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 65 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_block_item[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 1 }
	, { ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 5 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 9 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 12 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 17 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 22 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 27 }
	, { ENSrzAnlzSintxPartType_Keyword, 28 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 30 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 57 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 58 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 59 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 62 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 63 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 64 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 65 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_expression_statement[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_selection_statement[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 1 }
	, { ENSrzAnlzSintxPartType_Keyword, 27 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Keyword, 17 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_jump_statement[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 9 }
	, { ENSrzAnlzSintxPartType_Keyword, 12 }
	, { ENSrzAnlzSintxPartType_Keyword, 22 }
	, { ENSrzAnlzSintxPartType_Keyword, 30 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_translation_unit[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 66 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 67 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 68 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_external_declaration[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 68 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_function_definition[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_list[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 69 }
};

//
// Language sintax definitions (parts and deep-search arrays)
//

const STSrcAnlzSintx __globalSintxCDefs[] = {
	{"primary_expression", __globalSintxCSeqHints_primary_expression, 4}
	, {"postfix_expression", __globalSintxCSeqHints_postfix_expression, 6}
	, {"argument_expression_list", __globalSintxCSeqHints_argument_expression_list, 31}
	, {"unary_expression", __globalSintxCSeqHints_unary_expression, 16}
	, {"unary_operator", __globalSintxCSeqHints_unary_operator, 6}
	, {"cast_expression", __globalSintxCSeqHints_cast_expression, 17}
	, {"multiplicative_expression", __globalSintxCSeqHints_multiplicative_expression, 19}
	, {"additive_expression", __globalSintxCSeqHints_additive_expression, 20}
	, {"shift_expression", __globalSintxCSeqHints_shift_expression, 21}
	, {"relational_expression", __globalSintxCSeqHints_relational_expression, 22}
	, {"equality_expression", __globalSintxCSeqHints_equality_expression, 23}
	, {"AND_expression", __globalSintxCSeqHints_AND_expression, 24}
	, {"exclusive_OR_expression", __globalSintxCSeqHints_exclusive_OR_expression, 25}
	, {"inclusive_OR_expression", __globalSintxCSeqHints_inclusive_OR_expression, 26}
	, {"logical_AND_expression", __globalSintxCSeqHints_logical_AND_expression, 27}
	, {"logical_OR_expression", __globalSintxCSeqHints_logical_OR_expression, 28}
	, {"conditional_expression", __globalSintxCSeqHints_conditional_expression, 28}
	, {"assignment_expression", __globalSintxCSeqHints_assignment_expression, 29}
	, {"assignment_operator", __globalSintxCSeqHints_assignment_operator, 11}
	, {"expression", __globalSintxCSeqHints_expression, 31}
	, {"constant_expression", __globalSintxCSeqHints_constant_expression, 29}
	, {"declaration", __globalSintxCSeqHints_declaration, 33}
	, {"declaration_specifiers", __globalSintxCSeqHints_declaration_specifiers, 32}
	, {"init_declarator_list", __globalSintxCSeqHints_init_declarator_list, 8}
	, {"init_declarator", __globalSintxCSeqHints_init_declarator, 6}
	, {"storage_class_specifier", __globalSintxCSeqHints_storage_class_specifier, 5}
	, {"type_specifier", __globalSintxCSeqHints_type_specifier, 19}
	, {"struct_or_union_specifier", __globalSintxCSeqHints_struct_or_union_specifier, 3}
	, {"struct_or_union", __globalSintxCSeqHints_struct_or_union, 2}
	, {"struct_declaration_list", __globalSintxCSeqHints_struct_declaration_list, 27}
	, {"struct_declaration", __globalSintxCSeqHints_struct_declaration, 25}
	, {"specifier_qualifier_list", __globalSintxCSeqHints_specifier_qualifier_list, 24}
	, {"struct_declarator_list", __globalSintxCSeqHints_struct_declarator_list, 9}
	, {"struct_declarator", __globalSintxCSeqHints_struct_declarator, 7}
	, {"enum_specifier", __globalSintxCSeqHints_enum_specifier, 1}
	, {"enumerator_list", __globalSintxCSeqHints_enumerator_list, 4}
	, {"enumeration_constant", __globalSintxCSeqHints_enumeration_constant, 1}
	, {"enumerator", __globalSintxCSeqHints_enumerator, 2}
	, {"type_qualifier", __globalSintxCSeqHints_type_qualifier, 3}
	, {"function_specifier", __globalSintxCSeqHints_function_specifier, 1}
	, {"declarator", __globalSintxCSeqHints_declarator, 5}
	, {"direct_declarator", __globalSintxCSeqHints_direct_declarator, 3}
	, {"pointer", __globalSintxCSeqHints_pointer, 1}
	, {"type_qualifier_list", __globalSintxCSeqHints_type_qualifier_list, 5}
	, {"parameter_type_list", __globalSintxCSeqHints_parameter_type_list, 35}
	, {"parameter_list", __globalSintxCSeqHints_parameter_list, 35}
	, {"parameter_declaration", __globalSintxCSeqHints_parameter_declaration, 33}
	, {"identifier_list", __globalSintxCSeqHints_identifier_list, 2}
	, {"type_name", __globalSintxCSeqHints_type_name, 25}
	, {"abstract_declarator", __globalSintxCSeqHints_abstract_declarator, 5}
	, {"direct_abstract_declarator", __globalSintxCSeqHints_direct_abstract_declarator, 3}
	, {"typedef_name", __globalSintxCSeqHints_typedef_name, 1}
	, {"initializer", __globalSintxCSeqHints_initializer, 31}
	, {"initializer_list", __globalSintxCSeqHints_initializer_list, 38}
	, {"designation", __globalSintxCSeqHints_designation, 4}
	, {"designator_list", __globalSintxCSeqHints_designator_list, 4}
	, {"designator", __globalSintxCSeqHints_designator, 2}
	, {"statement", __globalSintxCSeqHints_statement, 50}
	, {"labeled_statement", __globalSintxCSeqHints_labeled_statement, 3}
	, {"compound_statement", __globalSintxCSeqHints_compound_statement, 1}
	, {"block_item_list", __globalSintxCSeqHints_block_item_list, 86}
	, {"block_item", __globalSintxCSeqHints_block_item, 84}
	, {"expression_statement", __globalSintxCSeqHints_expression_statement, 32}
	, {"selection_statement", __globalSintxCSeqHints_selection_statement, 2}
	, {"iteration_statement", __globalSintxCSeqHints_iteration_statement, 3}
	, {"jump_statement", __globalSintxCSeqHints_jump_statement, 4}
	, {"translation_unit", __globalSintxCSeqHints_translation_unit, 37}
	, {"external_declaration", __globalSintxCSeqHints_external_declaration, 35}
	, {"function_definition", __globalSintxCSeqHints_function_definition, 33}
	, {"declaration_list", __globalSintxCSeqHints_declaration_list, 35}
};

const STSrcAnlzRange __globalSintxCDefSeqsRngs[] = {
	{ 0, 4 }
	,  { 4, 10 }
	,  { 14, 2 }
	,  { 16, 6 }
	,  { 22, 6 }
	,  { 28, 2 }
	,  { 30, 4 }
	,  { 34, 3 }
	,  { 37, 3 }
	,  { 40, 5 }
	,  { 45, 3 }
	,  { 48, 2 }
	,  { 50, 2 }
	,  { 52, 2 }
	,  { 54, 2 }
	,  { 56, 2 }
	,  { 58, 2 }
	,  { 60, 2 }
	,  { 62, 11 }
	,  { 73, 2 }
	,  { 75, 1 }
	,  { 76, 2 }
	,  { 78, 8 }
	,  { 86, 2 }
	,  { 88, 2 }
	,  { 90, 5 }
	,  { 95, 14 }
	,  { 109, 3 }
	,  { 112, 2 }
	,  { 114, 2 }
	,  { 116, 1 }
	,  { 117, 4 }
	,  { 121, 2 }
	,  { 123, 3 }
	,  { 126, 5 }
	,  { 131, 2 }
	,  { 133, 1 }
	,  { 134, 2 }
	,  { 136, 3 }
	,  { 139, 1 }
	,  { 140, 2 }
	,  { 142, 14 }
	,  { 156, 4 }
	,  { 160, 2 }
	,  { 162, 2 }
	,  { 164, 2 }
	,  { 166, 3 }
	,  { 169, 2 }
	,  { 171, 2 }
	,  { 173, 3 }
	,  { 176, 21 }
	,  { 197, 1 }
	,  { 198, 3 }
	,  { 201, 4 }
	,  { 205, 1 }
	,  { 206, 2 }
	,  { 208, 2 }
	,  { 210, 6 }
	,  { 216, 3 }
	,  { 219, 2 }
	,  { 221, 2 }
	,  { 223, 2 }
	,  { 225, 2 }
	,  { 227, 3 }
	,  { 230, 14 }
	,  { 244, 5 }
	,  { 249, 2 }
	,  { 251, 2 }
	,  { 253, 1 }
	,  { 254, 2 }
};

const STSrcAnlzSintxPart __globalSintxCSeq_primary_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_primary_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Constant, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_primary_expression_2[] = {
	{ ENSrzAnlzSintxPartType_StrLiteral, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_primary_expression_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_postfix_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_primary_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_postfix_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_postfix_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_postfix_expression_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_postfix_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_postfix_expression_3[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_postfix_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_argument_expression_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_postfix_expression_4[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_postfix_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_postfix_expression_5[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_postfix_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 25 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_postfix_expression_6[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_postfix_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_postfix_expression_7[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_postfix_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_postfix_expression_8[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_name }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_postfix_expression_9[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_name }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_argument_expression_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_argument_expression_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_argument_expression_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_postfix_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_unary_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_expression_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_unary_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_expression_3[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_unary_operator }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_cast_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_expression_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_unary_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_expression_5[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_name }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_operator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 7 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_operator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_operator_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 9 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_operator_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 10 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_operator_4[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 11 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_unary_operator_5[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 12 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_cast_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_unary_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_cast_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_name }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_cast_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_multiplicative_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_cast_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_multiplicative_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_multiplicative_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_cast_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_multiplicative_expression_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_multiplicative_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_cast_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_multiplicative_expression_3[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_multiplicative_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_cast_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_additive_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_multiplicative_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_additive_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_additive_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_multiplicative_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_additive_expression_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_additive_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_multiplicative_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_shift_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_additive_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_shift_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_shift_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_additive_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_shift_expression_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_shift_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 29 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_additive_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_relational_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_shift_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_relational_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_relational_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_shift_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_relational_expression_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_relational_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_shift_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_relational_expression_3[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_relational_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 30 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_shift_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_relational_expression_4[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_relational_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_shift_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_equality_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_relational_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_equality_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_equality_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 32 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_relational_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_equality_expression_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_equality_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 33 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_relational_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_AND_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_equality_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_AND_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_AND_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_equality_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_exclusive_OR_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_AND_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_exclusive_OR_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_exclusive_OR_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_AND_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_inclusive_OR_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_exclusive_OR_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_inclusive_OR_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_inclusive_OR_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 18 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_exclusive_OR_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_logical_AND_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_inclusive_OR_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_logical_AND_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_logical_AND_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_inclusive_OR_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_logical_OR_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_logical_AND_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_logical_OR_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_logical_OR_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 35 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_logical_AND_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_conditional_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_logical_OR_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_conditional_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_logical_OR_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 19 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 20 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_conditional_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_conditional_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_unary_expression }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_operator }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 22 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 36 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 37 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 38 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_4[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 39 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_5[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 40 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_6[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_7[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 52 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_8[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_9[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_assignment_operator_10[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 43 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_expression_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_constant_expression_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_conditional_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_specifiers }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_specifiers }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_init_declarator_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_specifiers_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_storage_class_specifier }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_specifiers_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_storage_class_specifier }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_specifiers }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_specifiers_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_specifier }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_specifiers_3[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_specifier }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_specifiers }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_specifiers_4[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_specifiers_5[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_specifiers }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_specifiers_6[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_function_specifier }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_specifiers_7[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_function_specifier }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_specifiers }
};
const STSrcAnlzSintxPart __globalSintxCSeq_init_declarator_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_init_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_init_declarator_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_init_declarator_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_init_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_init_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_init_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer }
};
const STSrcAnlzSintxPart __globalSintxCSeq_storage_class_specifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 29 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_storage_class_specifier_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 20 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_storage_class_specifier_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 25 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_storage_class_specifier_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_storage_class_specifier_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 31 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 11 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 6 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 15 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 10 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_5[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 14 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_6[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 19 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_7[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 23 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_8[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 33 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_9[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 18 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_10[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 35 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_11[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_or_union_specifier }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_12[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_enum_specifier }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_specifier_13[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_typedef_name }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_or_union_specifier_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_or_union }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_declaration_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_or_union_specifier_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_or_union }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_declaration_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_or_union_specifier_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_or_union }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_or_union_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 26 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_or_union_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 16 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_declaration_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_declaration }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_declaration_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_declaration_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_declaration }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_declaration_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_specifier_qualifier_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_declarator_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_specifier_qualifier_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_specifier }
};
const STSrcAnlzSintxPart __globalSintxCSeq_specifier_qualifier_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_specifier }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_specifier_qualifier_list }
};
const STSrcAnlzSintxPart __globalSintxCSeq_specifier_qualifier_list_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier }
};
const STSrcAnlzSintxPart __globalSintxCSeq_specifier_qualifier_list_3[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_specifier_qualifier_list }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_declarator_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_declarator_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_declarator_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_struct_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 20 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_constant_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_struct_declarator_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 20 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_constant_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_enum_specifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_enumerator_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_enum_specifier_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_enumerator_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_enum_specifier_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_enumerator_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_enum_specifier_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_enumerator_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_enum_specifier_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_enumerator_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_enumerator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_enumerator_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_enumerator_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_enumerator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_enumeration_constant_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_enumerator_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_enumeration_constant }
};
const STSrcAnlzSintxPart __globalSintxCSeq_enumerator_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_enumeration_constant }
	, { ENSrzAnlzSintxPartType_Punctuator, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_constant_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_qualifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 13 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_qualifier_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 32 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_qualifier_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 34 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_function_specifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_pointer }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_3[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_4[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_5[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_6[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_7[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_8[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_9[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_10[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_11[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_parameter_type_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_12[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_declarator_13[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_identifier_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_pointer_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_pointer_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
};
const STSrcAnlzSintxPart __globalSintxCSeq_pointer_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_pointer }
};
const STSrcAnlzSintxPart __globalSintxCSeq_pointer_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_pointer }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_qualifier_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_qualifier_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier }
};
const STSrcAnlzSintxPart __globalSintxCSeq_parameter_type_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_parameter_list }
};
const STSrcAnlzSintxPart __globalSintxCSeq_parameter_type_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_parameter_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_Punctuator, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_parameter_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_parameter_declaration }
};
const STSrcAnlzSintxPart __globalSintxCSeq_parameter_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_parameter_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_parameter_declaration }
};
const STSrcAnlzSintxPart __globalSintxCSeq_parameter_declaration_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_specifiers }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_parameter_declaration_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_specifiers }
};
const STSrcAnlzSintxPart __globalSintxCSeq_parameter_declaration_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_specifiers }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_abstract_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_identifier_list_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_identifier_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_identifier_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_name_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_specifier_qualifier_list }
};
const STSrcAnlzSintxPart __globalSintxCSeq_type_name_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_specifier_qualifier_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_abstract_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_abstract_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_pointer }
};
const STSrcAnlzSintxPart __globalSintxCSeq_abstract_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_abstract_declarator_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_pointer }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_4[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_5[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_6[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_7[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_8[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_9[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_10[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_11[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_12[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_13[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_14[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_type_qualifier_list }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_15[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_16[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_17[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_18[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_parameter_type_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_19[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_direct_abstract_declarator_20[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_direct_abstract_declarator }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_parameter_type_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_typedef_name_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_initializer_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_assignment_expression }
};
const STSrcAnlzSintxPart __globalSintxCSeq_initializer_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_initializer_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_initializer_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer }
};
const STSrcAnlzSintxPart __globalSintxCSeq_initializer_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_designation }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer }
};
const STSrcAnlzSintxPart __globalSintxCSeq_initializer_list_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer }
};
const STSrcAnlzSintxPart __globalSintxCSeq_initializer_list_3[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 23 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_designation }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_initializer }
};
const STSrcAnlzSintxPart __globalSintxCSeq_designation_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_designator_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 22 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_designator_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_designator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_designator_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_designator_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_designator }
};
const STSrcAnlzSintxPart __globalSintxCSeq_designator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_constant_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_designator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_statement_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_labeled_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_statement_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_compound_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_statement_2[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_statement_3[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_selection_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_statement_4[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_iteration_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_statement_5[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_jump_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_labeled_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Punctuator, 20 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_labeled_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_constant_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 20 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_labeled_statement_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 28 }
	, { ENSrzAnlzSintxPartType_Punctuator, 20 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_compound_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_compound_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_block_item_list }
	, { ENSrzAnlzSintxPartType_Punctuator, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_block_item_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_block_item }
};
const STSrcAnlzSintxPart __globalSintxCSeq_block_item_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_block_item_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_block_item }
};
const STSrcAnlzSintxPart __globalSintxCSeq_block_item_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration }
};
const STSrcAnlzSintxPart __globalSintxCSeq_block_item_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_expression_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_expression_statement_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_selection_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 1 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_selection_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 1 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
	, { ENSrzAnlzSintxPartType_Keyword, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_selection_statement_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 27 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 17 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
	, { ENSrzAnlzSintxPartType_Keyword, 17 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_5[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_6[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_7[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_8[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_9[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_10[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_11[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_12[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_iteration_statement_13[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_jump_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 9 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_jump_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 30 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_jump_statement_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_jump_statement_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 22 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_jump_statement_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_expression }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeq_translation_unit_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_external_declaration }
};
const STSrcAnlzSintxPart __globalSintxCSeq_translation_unit_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_translation_unit }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_external_declaration }
};
const STSrcAnlzSintxPart __globalSintxCSeq_external_declaration_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_function_definition }
};
const STSrcAnlzSintxPart __globalSintxCSeq_external_declaration_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration }
};
const STSrcAnlzSintxPart __globalSintxCSeq_function_definition_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_specifiers }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declarator }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_compound_statement }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_list_0[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration }
};
const STSrcAnlzSintxPart __globalSintxCSeq_declaration_list_1[] = {
	{ ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration_list }
	, { ENSrzAnlzSintxPartType_SintaxDef, ENSrzAnlzSintxC_declaration }
};

//
// Optimization: array of def posibilities
// (pregenerated deep search arrays, no-definitions references)
//

const STSrcAnlzSintxPart __globalSintxCSeqHints_primary_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_primary_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Constant, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_primary_expression_2[] = {
	{ ENSrzAnlzSintxPartType_StrLiteral, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_primary_expression_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression_4[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression_5[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression_6[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression_7[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression_8[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_postfix_expression_9[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_argument_expression_list_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_argument_expression_list_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 26 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_expression_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 27 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_expression_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_expression_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_expression_5[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_operator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 7 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_operator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_operator_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 9 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_operator_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 10 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_operator_4[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 11 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_unary_operator_5[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 12 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_cast_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_cast_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_multiplicative_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_multiplicative_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_multiplicative_expression_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_multiplicative_expression_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_additive_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_additive_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_additive_expression_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_shift_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_shift_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_shift_expression_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_relational_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_relational_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_relational_expression_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_relational_expression_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_relational_expression_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_equality_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_equality_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_equality_expression_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_AND_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_AND_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_exclusive_OR_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_exclusive_OR_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_inclusive_OR_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_inclusive_OR_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_logical_AND_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_logical_AND_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_logical_OR_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_logical_OR_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_conditional_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_conditional_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 22 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 36 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 37 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 38 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_4[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 39 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_5[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 40 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_6[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_7[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 52 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_8[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_9[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_assignment_operator_10[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 43 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_expression_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_constant_expression_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_specifiers_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_specifiers_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_specifiers_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_specifiers_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_specifiers_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_specifiers_5[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_specifiers_6[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_specifiers_7[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_init_declarator_list_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 24 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_init_declarator_list_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 23 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 24 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_init_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_init_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_storage_class_specifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 29 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_storage_class_specifier_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 20 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_storage_class_specifier_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 25 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_storage_class_specifier_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_storage_class_specifier_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 31 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 11 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 6 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 15 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 10 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_5[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 14 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_6[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 19 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_7[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 23 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_8[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 33 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_9[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 18 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_10[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 35 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_11[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_12[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_specifier_13[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_or_union_specifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_or_union_specifier_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_or_union_specifier_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_or_union_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 26 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_or_union_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 16 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declaration_list_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 30 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declaration_list_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 29 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 30 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declaration_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_specifier_qualifier_list_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_specifier_qualifier_list_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_specifier_qualifier_list_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_specifier_qualifier_list_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declarator_list_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 20 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 33 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declarator_list_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 20 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 32 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 33 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 20 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_struct_declarator_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 40 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enum_specifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enum_specifier_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enum_specifier_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enum_specifier_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enum_specifier_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enumerator_list_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 36 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 37 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enumerator_list_1[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 35 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 36 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 37 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enumeration_constant_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enumerator_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 36 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_enumerator_1[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 36 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_qualifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 13 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_qualifier_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 32 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_qualifier_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 34 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_function_specifier_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_4[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_5[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_6[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_7[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_8[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_9[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_10[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_11[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_12[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_declarator_13[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 41 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_pointer_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_pointer_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_pointer_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_pointer_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_qualifier_list_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_qualifier_list_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 43 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_parameter_type_list_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 45 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 46 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_parameter_type_list_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 45 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 46 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_parameter_list_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 46 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_parameter_list_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 45 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 46 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_parameter_declaration_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_parameter_declaration_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_parameter_declaration_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_identifier_list_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_identifier_list_1[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 47 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_name_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_type_name_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 31 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_abstract_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_abstract_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_abstract_declarator_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 42 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_3[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_4[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_5[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_6[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_7[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_8[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_9[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_10[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_11[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_12[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_13[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_14[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_15[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_16[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_17[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_18[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_19[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_direct_abstract_declarator_20[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 50 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_typedef_name_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_initializer_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_initializer_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_initializer_2[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_initializer_list_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 52 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_initializer_list_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 54 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 55 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 56 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_initializer_list_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 52 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 53 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 54 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 55 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 56 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_initializer_list_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 52 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 53 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 54 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 55 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 56 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_designation_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 55 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 56 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_designator_list_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 56 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_designator_list_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
	, { ENSrzAnlzSintxPartType_Punctuator, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 55 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 56 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_designator_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_designator_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 6 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 5 }
	, { ENSrzAnlzSintxPartType_Keyword, 28 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 58 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 59 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_statement_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 62 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_statement_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 1 }
	, { ENSrzAnlzSintxPartType_Keyword, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 63 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_statement_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Keyword, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 64 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_statement_5[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 9 }
	, { ENSrzAnlzSintxPartType_Keyword, 12 }
	, { ENSrzAnlzSintxPartType_Keyword, 22 }
	, { ENSrzAnlzSintxPartType_Keyword, 30 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 65 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_labeled_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Identifier, -1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_labeled_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 5 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_labeled_statement_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 28 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_compound_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_compound_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 4 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_block_item_list_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 1 }
	, { ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 5 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 9 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 12 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 17 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 22 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 27 }
	, { ENSrzAnlzSintxPartType_Keyword, 28 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 30 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 57 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 58 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 59 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 61 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 62 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 63 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 64 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 65 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_block_item_list_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 1 }
	, { ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 5 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 9 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 12 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 17 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 22 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 27 }
	, { ENSrzAnlzSintxPartType_Keyword, 28 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 30 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 57 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 58 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 59 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 60 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 61 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 62 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 63 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 64 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 65 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_block_item_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_block_item_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 0 }
	, { ENSrzAnlzSintxPartType_Keyword, 1 }
	, { ENSrzAnlzSintxPartType_Keyword, 2 }
	, { ENSrzAnlzSintxPartType_Keyword, 5 }
	, { ENSrzAnlzSintxPartType_Keyword, 9 }
	, { ENSrzAnlzSintxPartType_Keyword, 12 }
	, { ENSrzAnlzSintxPartType_Keyword, 17 }
	, { ENSrzAnlzSintxPartType_Keyword, 22 }
	, { ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Keyword, 27 }
	, { ENSrzAnlzSintxPartType_Keyword, 28 }
	, { ENSrzAnlzSintxPartType_Keyword, 30 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 4 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 21 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 57 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 58 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 59 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 62 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 63 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 64 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 65 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_expression_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Punctuator, 21 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_expression_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 24 }
	, { ENSrzAnlzSintxPartType_Punctuator, 2 }
	, { ENSrzAnlzSintxPartType_Punctuator, 7 }
	, { ENSrzAnlzSintxPartType_Punctuator, 8 }
	, { ENSrzAnlzSintxPartType_Punctuator, 9 }
	, { ENSrzAnlzSintxPartType_Punctuator, 10 }
	, { ENSrzAnlzSintxPartType_Punctuator, 11 }
	, { ENSrzAnlzSintxPartType_Punctuator, 12 }
	, { ENSrzAnlzSintxPartType_Punctuator, 26 }
	, { ENSrzAnlzSintxPartType_Punctuator, 27 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_Constant, -1 }
	, { ENSrzAnlzSintxPartType_StrLiteral, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 0 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 3 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 4 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 5 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 6 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 7 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 8 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 9 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 10 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 11 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 12 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 13 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 14 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 15 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 16 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 17 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 19 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_selection_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_selection_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 1 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_selection_statement_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 27 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 17 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 0 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_5[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_6[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_7[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_8[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_9[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_10[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_11[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_12[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_iteration_statement_13[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 2 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_jump_statement_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 9 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_jump_statement_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 30 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_jump_statement_2[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 12 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_jump_statement_3[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 22 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_jump_statement_4[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 22 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_translation_unit_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 67 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 68 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_translation_unit_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 66 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 67 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 68 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_external_declaration_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 68 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_external_declaration_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_function_definition_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_list_0[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
};
const STSrcAnlzSintxPart __globalSintxCSeqHints_declaration_list_1[] = {
	{ ENSrzAnlzSintxPartType_Keyword, 3 }
	, { ENSrzAnlzSintxPartType_Keyword, 4 }
	, { ENSrzAnlzSintxPartType_Keyword, 6 }
	, { ENSrzAnlzSintxPartType_Keyword, 8 }
	, { ENSrzAnlzSintxPartType_Keyword, 10 }
	, { ENSrzAnlzSintxPartType_Keyword, 11 }
	, { ENSrzAnlzSintxPartType_Keyword, 13 }
	, { ENSrzAnlzSintxPartType_Keyword, 14 }
	, { ENSrzAnlzSintxPartType_Keyword, 15 }
	, { ENSrzAnlzSintxPartType_Keyword, 16 }
	, { ENSrzAnlzSintxPartType_Keyword, 18 }
	, { ENSrzAnlzSintxPartType_Keyword, 19 }
	, { ENSrzAnlzSintxPartType_Keyword, 20 }
	, { ENSrzAnlzSintxPartType_Keyword, 21 }
	, { ENSrzAnlzSintxPartType_Keyword, 23 }
	, { ENSrzAnlzSintxPartType_Keyword, 25 }
	, { ENSrzAnlzSintxPartType_Keyword, 26 }
	, { ENSrzAnlzSintxPartType_Keyword, 29 }
	, { ENSrzAnlzSintxPartType_Keyword, 31 }
	, { ENSrzAnlzSintxPartType_Keyword, 32 }
	, { ENSrzAnlzSintxPartType_Keyword, 33 }
	, { ENSrzAnlzSintxPartType_Keyword, 34 }
	, { ENSrzAnlzSintxPartType_Keyword, 35 }
	, { ENSrzAnlzSintxPartType_Identifier, -1 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 21 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 22 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 25 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 26 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 27 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 28 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 34 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 38 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 39 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 51 }
	, { ENSrzAnlzSintxPartType_SintaxDef, 69 }
};

//
// Language sintax definitions (parts and deep-search arrays)
//

const STSrcAnlzSintxSeq __globalSintxCDefSeqs[] = {
	{ ENSrzAnlzSintxC_primary_expression, __globalSintxCSeq_primary_expression_0, 1, __globalSintxCSeqHints_primary_expression_0, 1 }
	, { ENSrzAnlzSintxC_primary_expression, __globalSintxCSeq_primary_expression_1, 1, __globalSintxCSeqHints_primary_expression_1, 1 }
	, { ENSrzAnlzSintxC_primary_expression, __globalSintxCSeq_primary_expression_2, 1, __globalSintxCSeqHints_primary_expression_2, 1 }
	, { ENSrzAnlzSintxC_primary_expression, __globalSintxCSeq_primary_expression_3, 3, __globalSintxCSeqHints_primary_expression_3, 1 }
	, { ENSrzAnlzSintxC_postfix_expression, __globalSintxCSeq_postfix_expression_0, 1, __globalSintxCSeqHints_postfix_expression_0, 5 }
	, { ENSrzAnlzSintxC_postfix_expression, __globalSintxCSeq_postfix_expression_1, 4, __globalSintxCSeqHints_postfix_expression_1, 6 }
	, { ENSrzAnlzSintxC_postfix_expression, __globalSintxCSeq_postfix_expression_2, 3, __globalSintxCSeqHints_postfix_expression_2, 6 }
	, { ENSrzAnlzSintxC_postfix_expression, __globalSintxCSeq_postfix_expression_3, 4, __globalSintxCSeqHints_postfix_expression_3, 6 }
	, { ENSrzAnlzSintxC_postfix_expression, __globalSintxCSeq_postfix_expression_4, 3, __globalSintxCSeqHints_postfix_expression_4, 6 }
	, { ENSrzAnlzSintxC_postfix_expression, __globalSintxCSeq_postfix_expression_5, 3, __globalSintxCSeqHints_postfix_expression_5, 6 }
	, { ENSrzAnlzSintxC_postfix_expression, __globalSintxCSeq_postfix_expression_6, 2, __globalSintxCSeqHints_postfix_expression_6, 6 }
	, { ENSrzAnlzSintxC_postfix_expression, __globalSintxCSeq_postfix_expression_7, 2, __globalSintxCSeqHints_postfix_expression_7, 6 }
	, { ENSrzAnlzSintxC_postfix_expression, __globalSintxCSeq_postfix_expression_8, 6, __globalSintxCSeqHints_postfix_expression_8, 1 }
	, { ENSrzAnlzSintxC_postfix_expression, __globalSintxCSeq_postfix_expression_9, 7, __globalSintxCSeqHints_postfix_expression_9, 1 }
	, { ENSrzAnlzSintxC_argument_expression_list, __globalSintxCSeq_argument_expression_list_0, 1, __globalSintxCSeqHints_argument_expression_list_0, 30 }
	, { ENSrzAnlzSintxC_argument_expression_list, __globalSintxCSeq_argument_expression_list_1, 3, __globalSintxCSeqHints_argument_expression_list_1, 31 }
	, { ENSrzAnlzSintxC_unary_expression, __globalSintxCSeq_unary_expression_0, 1, __globalSintxCSeqHints_unary_expression_0, 6 }
	, { ENSrzAnlzSintxC_unary_expression, __globalSintxCSeq_unary_expression_1, 2, __globalSintxCSeqHints_unary_expression_1, 1 }
	, { ENSrzAnlzSintxC_unary_expression, __globalSintxCSeq_unary_expression_2, 2, __globalSintxCSeqHints_unary_expression_2, 1 }
	, { ENSrzAnlzSintxC_unary_expression, __globalSintxCSeq_unary_expression_3, 2, __globalSintxCSeqHints_unary_expression_3, 7 }
	, { ENSrzAnlzSintxC_unary_expression, __globalSintxCSeq_unary_expression_4, 2, __globalSintxCSeqHints_unary_expression_4, 1 }
	, { ENSrzAnlzSintxC_unary_expression, __globalSintxCSeq_unary_expression_5, 4, __globalSintxCSeqHints_unary_expression_5, 1 }
	, { ENSrzAnlzSintxC_unary_operator, __globalSintxCSeq_unary_operator_0, 1, __globalSintxCSeqHints_unary_operator_0, 1 }
	, { ENSrzAnlzSintxC_unary_operator, __globalSintxCSeq_unary_operator_1, 1, __globalSintxCSeqHints_unary_operator_1, 1 }
	, { ENSrzAnlzSintxC_unary_operator, __globalSintxCSeq_unary_operator_2, 1, __globalSintxCSeqHints_unary_operator_2, 1 }
	, { ENSrzAnlzSintxC_unary_operator, __globalSintxCSeq_unary_operator_3, 1, __globalSintxCSeqHints_unary_operator_3, 1 }
	, { ENSrzAnlzSintxC_unary_operator, __globalSintxCSeq_unary_operator_4, 1, __globalSintxCSeqHints_unary_operator_4, 1 }
	, { ENSrzAnlzSintxC_unary_operator, __globalSintxCSeq_unary_operator_5, 1, __globalSintxCSeqHints_unary_operator_5, 1 }
	, { ENSrzAnlzSintxC_cast_expression, __globalSintxCSeq_cast_expression_0, 1, __globalSintxCSeqHints_cast_expression_0, 17 }
	, { ENSrzAnlzSintxC_cast_expression, __globalSintxCSeq_cast_expression_1, 4, __globalSintxCSeqHints_cast_expression_1, 1 }
	, { ENSrzAnlzSintxC_multiplicative_expression, __globalSintxCSeq_multiplicative_expression_0, 1, __globalSintxCSeqHints_multiplicative_expression_0, 18 }
	, { ENSrzAnlzSintxC_multiplicative_expression, __globalSintxCSeq_multiplicative_expression_1, 3, __globalSintxCSeqHints_multiplicative_expression_1, 19 }
	, { ENSrzAnlzSintxC_multiplicative_expression, __globalSintxCSeq_multiplicative_expression_2, 3, __globalSintxCSeqHints_multiplicative_expression_2, 19 }
	, { ENSrzAnlzSintxC_multiplicative_expression, __globalSintxCSeq_multiplicative_expression_3, 3, __globalSintxCSeqHints_multiplicative_expression_3, 19 }
	, { ENSrzAnlzSintxC_additive_expression, __globalSintxCSeq_additive_expression_0, 1, __globalSintxCSeqHints_additive_expression_0, 19 }
	, { ENSrzAnlzSintxC_additive_expression, __globalSintxCSeq_additive_expression_1, 3, __globalSintxCSeqHints_additive_expression_1, 20 }
	, { ENSrzAnlzSintxC_additive_expression, __globalSintxCSeq_additive_expression_2, 3, __globalSintxCSeqHints_additive_expression_2, 20 }
	, { ENSrzAnlzSintxC_shift_expression, __globalSintxCSeq_shift_expression_0, 1, __globalSintxCSeqHints_shift_expression_0, 20 }
	, { ENSrzAnlzSintxC_shift_expression, __globalSintxCSeq_shift_expression_1, 3, __globalSintxCSeqHints_shift_expression_1, 21 }
	, { ENSrzAnlzSintxC_shift_expression, __globalSintxCSeq_shift_expression_2, 3, __globalSintxCSeqHints_shift_expression_2, 21 }
	, { ENSrzAnlzSintxC_relational_expression, __globalSintxCSeq_relational_expression_0, 1, __globalSintxCSeqHints_relational_expression_0, 21 }
	, { ENSrzAnlzSintxC_relational_expression, __globalSintxCSeq_relational_expression_1, 3, __globalSintxCSeqHints_relational_expression_1, 22 }
	, { ENSrzAnlzSintxC_relational_expression, __globalSintxCSeq_relational_expression_2, 3, __globalSintxCSeqHints_relational_expression_2, 22 }
	, { ENSrzAnlzSintxC_relational_expression, __globalSintxCSeq_relational_expression_3, 3, __globalSintxCSeqHints_relational_expression_3, 22 }
	, { ENSrzAnlzSintxC_relational_expression, __globalSintxCSeq_relational_expression_4, 3, __globalSintxCSeqHints_relational_expression_4, 22 }
	, { ENSrzAnlzSintxC_equality_expression, __globalSintxCSeq_equality_expression_0, 1, __globalSintxCSeqHints_equality_expression_0, 22 }
	, { ENSrzAnlzSintxC_equality_expression, __globalSintxCSeq_equality_expression_1, 3, __globalSintxCSeqHints_equality_expression_1, 23 }
	, { ENSrzAnlzSintxC_equality_expression, __globalSintxCSeq_equality_expression_2, 3, __globalSintxCSeqHints_equality_expression_2, 23 }
	, { ENSrzAnlzSintxC_AND_expression, __globalSintxCSeq_AND_expression_0, 1, __globalSintxCSeqHints_AND_expression_0, 23 }
	, { ENSrzAnlzSintxC_AND_expression, __globalSintxCSeq_AND_expression_1, 3, __globalSintxCSeqHints_AND_expression_1, 24 }
	, { ENSrzAnlzSintxC_exclusive_OR_expression, __globalSintxCSeq_exclusive_OR_expression_0, 1, __globalSintxCSeqHints_exclusive_OR_expression_0, 24 }
	, { ENSrzAnlzSintxC_exclusive_OR_expression, __globalSintxCSeq_exclusive_OR_expression_1, 3, __globalSintxCSeqHints_exclusive_OR_expression_1, 25 }
	, { ENSrzAnlzSintxC_inclusive_OR_expression, __globalSintxCSeq_inclusive_OR_expression_0, 1, __globalSintxCSeqHints_inclusive_OR_expression_0, 25 }
	, { ENSrzAnlzSintxC_inclusive_OR_expression, __globalSintxCSeq_inclusive_OR_expression_1, 3, __globalSintxCSeqHints_inclusive_OR_expression_1, 26 }
	, { ENSrzAnlzSintxC_logical_AND_expression, __globalSintxCSeq_logical_AND_expression_0, 1, __globalSintxCSeqHints_logical_AND_expression_0, 26 }
	, { ENSrzAnlzSintxC_logical_AND_expression, __globalSintxCSeq_logical_AND_expression_1, 3, __globalSintxCSeqHints_logical_AND_expression_1, 27 }
	, { ENSrzAnlzSintxC_logical_OR_expression, __globalSintxCSeq_logical_OR_expression_0, 1, __globalSintxCSeqHints_logical_OR_expression_0, 27 }
	, { ENSrzAnlzSintxC_logical_OR_expression, __globalSintxCSeq_logical_OR_expression_1, 3, __globalSintxCSeqHints_logical_OR_expression_1, 28 }
	, { ENSrzAnlzSintxC_conditional_expression, __globalSintxCSeq_conditional_expression_0, 1, __globalSintxCSeqHints_conditional_expression_0, 28 }
	, { ENSrzAnlzSintxC_conditional_expression, __globalSintxCSeq_conditional_expression_1, 5, __globalSintxCSeqHints_conditional_expression_1, 28 }
	, { ENSrzAnlzSintxC_assignment_expression, __globalSintxCSeq_assignment_expression_0, 1, __globalSintxCSeqHints_assignment_expression_0, 29 }
	, { ENSrzAnlzSintxC_assignment_expression, __globalSintxCSeq_assignment_expression_1, 3, __globalSintxCSeqHints_assignment_expression_1, 17 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_0, 1, __globalSintxCSeqHints_assignment_operator_0, 1 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_1, 1, __globalSintxCSeqHints_assignment_operator_1, 1 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_2, 1, __globalSintxCSeqHints_assignment_operator_2, 1 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_3, 1, __globalSintxCSeqHints_assignment_operator_3, 1 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_4, 1, __globalSintxCSeqHints_assignment_operator_4, 1 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_5, 1, __globalSintxCSeqHints_assignment_operator_5, 1 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_6, 1, __globalSintxCSeqHints_assignment_operator_6, 1 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_7, 1, __globalSintxCSeqHints_assignment_operator_7, 1 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_8, 1, __globalSintxCSeqHints_assignment_operator_8, 1 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_9, 1, __globalSintxCSeqHints_assignment_operator_9, 1 }
	, { ENSrzAnlzSintxC_assignment_operator, __globalSintxCSeq_assignment_operator_10, 1, __globalSintxCSeqHints_assignment_operator_10, 1 }
	, { ENSrzAnlzSintxC_expression, __globalSintxCSeq_expression_0, 1, __globalSintxCSeqHints_expression_0, 30 }
	, { ENSrzAnlzSintxC_expression, __globalSintxCSeq_expression_1, 3, __globalSintxCSeqHints_expression_1, 31 }
	, { ENSrzAnlzSintxC_constant_expression, __globalSintxCSeq_constant_expression_0, 1, __globalSintxCSeqHints_constant_expression_0, 29 }
	, { ENSrzAnlzSintxC_declaration, __globalSintxCSeq_declaration_0, 2, __globalSintxCSeqHints_declaration_0, 33 }
	, { ENSrzAnlzSintxC_declaration, __globalSintxCSeq_declaration_1, 3, __globalSintxCSeqHints_declaration_1, 33 }
	, { ENSrzAnlzSintxC_declaration_specifiers, __globalSintxCSeq_declaration_specifiers_0, 1, __globalSintxCSeqHints_declaration_specifiers_0, 6 }
	, { ENSrzAnlzSintxC_declaration_specifiers, __globalSintxCSeq_declaration_specifiers_1, 2, __globalSintxCSeqHints_declaration_specifiers_1, 6 }
	, { ENSrzAnlzSintxC_declaration_specifiers, __globalSintxCSeq_declaration_specifiers_2, 1, __globalSintxCSeqHints_declaration_specifiers_2, 20 }
	, { ENSrzAnlzSintxC_declaration_specifiers, __globalSintxCSeq_declaration_specifiers_3, 2, __globalSintxCSeqHints_declaration_specifiers_3, 20 }
	, { ENSrzAnlzSintxC_declaration_specifiers, __globalSintxCSeq_declaration_specifiers_4, 1, __globalSintxCSeqHints_declaration_specifiers_4, 4 }
	, { ENSrzAnlzSintxC_declaration_specifiers, __globalSintxCSeq_declaration_specifiers_5, 2, __globalSintxCSeqHints_declaration_specifiers_5, 4 }
	, { ENSrzAnlzSintxC_declaration_specifiers, __globalSintxCSeq_declaration_specifiers_6, 1, __globalSintxCSeqHints_declaration_specifiers_6, 2 }
	, { ENSrzAnlzSintxC_declaration_specifiers, __globalSintxCSeq_declaration_specifiers_7, 2, __globalSintxCSeqHints_declaration_specifiers_7, 2 }
	, { ENSrzAnlzSintxC_init_declarator_list, __globalSintxCSeq_init_declarator_list_0, 1, __globalSintxCSeqHints_init_declarator_list_0, 7 }
	, { ENSrzAnlzSintxC_init_declarator_list, __globalSintxCSeq_init_declarator_list_1, 3, __globalSintxCSeqHints_init_declarator_list_1, 8 }
	, { ENSrzAnlzSintxC_init_declarator, __globalSintxCSeq_init_declarator_0, 1, __globalSintxCSeqHints_init_declarator_0, 6 }
	, { ENSrzAnlzSintxC_init_declarator, __globalSintxCSeq_init_declarator_1, 3, __globalSintxCSeqHints_init_declarator_1, 6 }
	, { ENSrzAnlzSintxC_storage_class_specifier, __globalSintxCSeq_storage_class_specifier_0, 1, __globalSintxCSeqHints_storage_class_specifier_0, 1 }
	, { ENSrzAnlzSintxC_storage_class_specifier, __globalSintxCSeq_storage_class_specifier_1, 1, __globalSintxCSeqHints_storage_class_specifier_1, 1 }
	, { ENSrzAnlzSintxC_storage_class_specifier, __globalSintxCSeq_storage_class_specifier_2, 1, __globalSintxCSeqHints_storage_class_specifier_2, 1 }
	, { ENSrzAnlzSintxC_storage_class_specifier, __globalSintxCSeq_storage_class_specifier_3, 1, __globalSintxCSeqHints_storage_class_specifier_3, 1 }
	, { ENSrzAnlzSintxC_storage_class_specifier, __globalSintxCSeq_storage_class_specifier_4, 1, __globalSintxCSeqHints_storage_class_specifier_4, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_0, 1, __globalSintxCSeqHints_type_specifier_0, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_1, 1, __globalSintxCSeqHints_type_specifier_1, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_2, 1, __globalSintxCSeqHints_type_specifier_2, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_3, 1, __globalSintxCSeqHints_type_specifier_3, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_4, 1, __globalSintxCSeqHints_type_specifier_4, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_5, 1, __globalSintxCSeqHints_type_specifier_5, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_6, 1, __globalSintxCSeqHints_type_specifier_6, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_7, 1, __globalSintxCSeqHints_type_specifier_7, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_8, 1, __globalSintxCSeqHints_type_specifier_8, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_9, 1, __globalSintxCSeqHints_type_specifier_9, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_10, 1, __globalSintxCSeqHints_type_specifier_10, 1 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_11, 1, __globalSintxCSeqHints_type_specifier_11, 4 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_12, 1, __globalSintxCSeqHints_type_specifier_12, 2 }
	, { ENSrzAnlzSintxC_type_specifier, __globalSintxCSeq_type_specifier_13, 1, __globalSintxCSeqHints_type_specifier_13, 2 }
	, { ENSrzAnlzSintxC_struct_or_union_specifier, __globalSintxCSeq_struct_or_union_specifier_0, 4, __globalSintxCSeqHints_struct_or_union_specifier_0, 3 }
	, { ENSrzAnlzSintxC_struct_or_union_specifier, __globalSintxCSeq_struct_or_union_specifier_1, 5, __globalSintxCSeqHints_struct_or_union_specifier_1, 3 }
	, { ENSrzAnlzSintxC_struct_or_union_specifier, __globalSintxCSeq_struct_or_union_specifier_2, 2, __globalSintxCSeqHints_struct_or_union_specifier_2, 3 }
	, { ENSrzAnlzSintxC_struct_or_union, __globalSintxCSeq_struct_or_union_0, 1, __globalSintxCSeqHints_struct_or_union_0, 1 }
	, { ENSrzAnlzSintxC_struct_or_union, __globalSintxCSeq_struct_or_union_1, 1, __globalSintxCSeqHints_struct_or_union_1, 1 }
	, { ENSrzAnlzSintxC_struct_declaration_list, __globalSintxCSeq_struct_declaration_list_0, 1, __globalSintxCSeqHints_struct_declaration_list_0, 26 }
	, { ENSrzAnlzSintxC_struct_declaration_list, __globalSintxCSeq_struct_declaration_list_1, 2, __globalSintxCSeqHints_struct_declaration_list_1, 27 }
	, { ENSrzAnlzSintxC_struct_declaration, __globalSintxCSeq_struct_declaration_0, 3, __globalSintxCSeqHints_struct_declaration_0, 25 }
	, { ENSrzAnlzSintxC_specifier_qualifier_list, __globalSintxCSeq_specifier_qualifier_list_0, 1, __globalSintxCSeqHints_specifier_qualifier_list_0, 20 }
	, { ENSrzAnlzSintxC_specifier_qualifier_list, __globalSintxCSeq_specifier_qualifier_list_1, 2, __globalSintxCSeqHints_specifier_qualifier_list_1, 20 }
	, { ENSrzAnlzSintxC_specifier_qualifier_list, __globalSintxCSeq_specifier_qualifier_list_2, 1, __globalSintxCSeqHints_specifier_qualifier_list_2, 4 }
	, { ENSrzAnlzSintxC_specifier_qualifier_list, __globalSintxCSeq_specifier_qualifier_list_3, 2, __globalSintxCSeqHints_specifier_qualifier_list_3, 4 }
	, { ENSrzAnlzSintxC_struct_declarator_list, __globalSintxCSeq_struct_declarator_list_0, 1, __globalSintxCSeqHints_struct_declarator_list_0, 8 }
	, { ENSrzAnlzSintxC_struct_declarator_list, __globalSintxCSeq_struct_declarator_list_1, 3, __globalSintxCSeqHints_struct_declarator_list_1, 9 }
	, { ENSrzAnlzSintxC_struct_declarator, __globalSintxCSeq_struct_declarator_0, 1, __globalSintxCSeqHints_struct_declarator_0, 6 }
	, { ENSrzAnlzSintxC_struct_declarator, __globalSintxCSeq_struct_declarator_1, 2, __globalSintxCSeqHints_struct_declarator_1, 1 }
	, { ENSrzAnlzSintxC_struct_declarator, __globalSintxCSeq_struct_declarator_2, 3, __globalSintxCSeqHints_struct_declarator_2, 6 }
	, { ENSrzAnlzSintxC_enum_specifier, __globalSintxCSeq_enum_specifier_0, 4, __globalSintxCSeqHints_enum_specifier_0, 1 }
	, { ENSrzAnlzSintxC_enum_specifier, __globalSintxCSeq_enum_specifier_1, 5, __globalSintxCSeqHints_enum_specifier_1, 1 }
	, { ENSrzAnlzSintxC_enum_specifier, __globalSintxCSeq_enum_specifier_2, 5, __globalSintxCSeqHints_enum_specifier_2, 1 }
	, { ENSrzAnlzSintxC_enum_specifier, __globalSintxCSeq_enum_specifier_3, 6, __globalSintxCSeqHints_enum_specifier_3, 1 }
	, { ENSrzAnlzSintxC_enum_specifier, __globalSintxCSeq_enum_specifier_4, 2, __globalSintxCSeqHints_enum_specifier_4, 1 }
	, { ENSrzAnlzSintxC_enumerator_list, __globalSintxCSeq_enumerator_list_0, 1, __globalSintxCSeqHints_enumerator_list_0, 3 }
	, { ENSrzAnlzSintxC_enumerator_list, __globalSintxCSeq_enumerator_list_1, 3, __globalSintxCSeqHints_enumerator_list_1, 4 }
	, { ENSrzAnlzSintxC_enumeration_constant, __globalSintxCSeq_enumeration_constant_0, 1, __globalSintxCSeqHints_enumeration_constant_0, 1 }
	, { ENSrzAnlzSintxC_enumerator, __globalSintxCSeq_enumerator_0, 1, __globalSintxCSeqHints_enumerator_0, 2 }
	, { ENSrzAnlzSintxC_enumerator, __globalSintxCSeq_enumerator_1, 3, __globalSintxCSeqHints_enumerator_1, 2 }
	, { ENSrzAnlzSintxC_type_qualifier, __globalSintxCSeq_type_qualifier_0, 1, __globalSintxCSeqHints_type_qualifier_0, 1 }
	, { ENSrzAnlzSintxC_type_qualifier, __globalSintxCSeq_type_qualifier_1, 1, __globalSintxCSeqHints_type_qualifier_1, 1 }
	, { ENSrzAnlzSintxC_type_qualifier, __globalSintxCSeq_type_qualifier_2, 1, __globalSintxCSeqHints_type_qualifier_2, 1 }
	, { ENSrzAnlzSintxC_function_specifier, __globalSintxCSeq_function_specifier_0, 1, __globalSintxCSeqHints_function_specifier_0, 1 }
	, { ENSrzAnlzSintxC_declarator, __globalSintxCSeq_declarator_0, 1, __globalSintxCSeqHints_declarator_0, 3 }
	, { ENSrzAnlzSintxC_declarator, __globalSintxCSeq_declarator_1, 2, __globalSintxCSeqHints_declarator_1, 2 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_0, 1, __globalSintxCSeqHints_direct_declarator_0, 1 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_1, 3, __globalSintxCSeqHints_direct_declarator_1, 1 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_2, 3, __globalSintxCSeqHints_direct_declarator_2, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_3, 4, __globalSintxCSeqHints_direct_declarator_3, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_4, 4, __globalSintxCSeqHints_direct_declarator_4, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_5, 5, __globalSintxCSeqHints_direct_declarator_5, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_6, 5, __globalSintxCSeqHints_direct_declarator_6, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_7, 6, __globalSintxCSeqHints_direct_declarator_7, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_8, 6, __globalSintxCSeqHints_direct_declarator_8, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_9, 4, __globalSintxCSeqHints_direct_declarator_9, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_10, 5, __globalSintxCSeqHints_direct_declarator_10, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_11, 4, __globalSintxCSeqHints_direct_declarator_11, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_12, 3, __globalSintxCSeqHints_direct_declarator_12, 3 }
	, { ENSrzAnlzSintxC_direct_declarator, __globalSintxCSeq_direct_declarator_13, 4, __globalSintxCSeqHints_direct_declarator_13, 3 }
	, { ENSrzAnlzSintxC_pointer, __globalSintxCSeq_pointer_0, 1, __globalSintxCSeqHints_pointer_0, 1 }
	, { ENSrzAnlzSintxC_pointer, __globalSintxCSeq_pointer_1, 2, __globalSintxCSeqHints_pointer_1, 1 }
	, { ENSrzAnlzSintxC_pointer, __globalSintxCSeq_pointer_2, 2, __globalSintxCSeqHints_pointer_2, 1 }
	, { ENSrzAnlzSintxC_pointer, __globalSintxCSeq_pointer_3, 3, __globalSintxCSeqHints_pointer_3, 1 }
	, { ENSrzAnlzSintxC_type_qualifier_list, __globalSintxCSeq_type_qualifier_list_0, 1, __globalSintxCSeqHints_type_qualifier_list_0, 4 }
	, { ENSrzAnlzSintxC_type_qualifier_list, __globalSintxCSeq_type_qualifier_list_1, 2, __globalSintxCSeqHints_type_qualifier_list_1, 5 }
	, { ENSrzAnlzSintxC_parameter_type_list, __globalSintxCSeq_parameter_type_list_0, 1, __globalSintxCSeqHints_parameter_type_list_0, 35 }
	, { ENSrzAnlzSintxC_parameter_type_list, __globalSintxCSeq_parameter_type_list_1, 3, __globalSintxCSeqHints_parameter_type_list_1, 35 }
	, { ENSrzAnlzSintxC_parameter_list, __globalSintxCSeq_parameter_list_0, 1, __globalSintxCSeqHints_parameter_list_0, 34 }
	, { ENSrzAnlzSintxC_parameter_list, __globalSintxCSeq_parameter_list_1, 3, __globalSintxCSeqHints_parameter_list_1, 35 }
	, { ENSrzAnlzSintxC_parameter_declaration, __globalSintxCSeq_parameter_declaration_0, 2, __globalSintxCSeqHints_parameter_declaration_0, 33 }
	, { ENSrzAnlzSintxC_parameter_declaration, __globalSintxCSeq_parameter_declaration_1, 1, __globalSintxCSeqHints_parameter_declaration_1, 33 }
	, { ENSrzAnlzSintxC_parameter_declaration, __globalSintxCSeq_parameter_declaration_2, 2, __globalSintxCSeqHints_parameter_declaration_2, 33 }
	, { ENSrzAnlzSintxC_identifier_list, __globalSintxCSeq_identifier_list_0, 1, __globalSintxCSeqHints_identifier_list_0, 1 }
	, { ENSrzAnlzSintxC_identifier_list, __globalSintxCSeq_identifier_list_1, 3, __globalSintxCSeqHints_identifier_list_1, 2 }
	, { ENSrzAnlzSintxC_type_name, __globalSintxCSeq_type_name_0, 1, __globalSintxCSeqHints_type_name_0, 25 }
	, { ENSrzAnlzSintxC_type_name, __globalSintxCSeq_type_name_1, 2, __globalSintxCSeqHints_type_name_1, 25 }
	, { ENSrzAnlzSintxC_abstract_declarator, __globalSintxCSeq_abstract_declarator_0, 1, __globalSintxCSeqHints_abstract_declarator_0, 2 }
	, { ENSrzAnlzSintxC_abstract_declarator, __globalSintxCSeq_abstract_declarator_1, 1, __globalSintxCSeqHints_abstract_declarator_1, 3 }
	, { ENSrzAnlzSintxC_abstract_declarator, __globalSintxCSeq_abstract_declarator_2, 2, __globalSintxCSeqHints_abstract_declarator_2, 2 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_0, 3, __globalSintxCSeqHints_direct_abstract_declarator_0, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_1, 2, __globalSintxCSeqHints_direct_abstract_declarator_1, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_2, 3, __globalSintxCSeqHints_direct_abstract_declarator_2, 3 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_3, 3, __globalSintxCSeqHints_direct_abstract_declarator_3, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_4, 3, __globalSintxCSeqHints_direct_abstract_declarator_4, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_5, 4, __globalSintxCSeqHints_direct_abstract_declarator_5, 3 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_6, 4, __globalSintxCSeqHints_direct_abstract_declarator_6, 3 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_7, 4, __globalSintxCSeqHints_direct_abstract_declarator_7, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_8, 5, __globalSintxCSeqHints_direct_abstract_declarator_8, 3 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_9, 4, __globalSintxCSeqHints_direct_abstract_declarator_9, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_10, 5, __globalSintxCSeqHints_direct_abstract_declarator_10, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_11, 4, __globalSintxCSeqHints_direct_abstract_declarator_11, 3 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_12, 6, __globalSintxCSeqHints_direct_abstract_declarator_12, 3 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_13, 5, __globalSintxCSeqHints_direct_abstract_declarator_13, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_14, 6, __globalSintxCSeqHints_direct_abstract_declarator_14, 3 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_15, 3, __globalSintxCSeqHints_direct_abstract_declarator_15, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_16, 4, __globalSintxCSeqHints_direct_abstract_declarator_16, 3 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_17, 2, __globalSintxCSeqHints_direct_abstract_declarator_17, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_18, 3, __globalSintxCSeqHints_direct_abstract_declarator_18, 1 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_19, 3, __globalSintxCSeqHints_direct_abstract_declarator_19, 3 }
	, { ENSrzAnlzSintxC_direct_abstract_declarator, __globalSintxCSeq_direct_abstract_declarator_20, 4, __globalSintxCSeqHints_direct_abstract_declarator_20, 3 }
	, { ENSrzAnlzSintxC_typedef_name, __globalSintxCSeq_typedef_name_0, 1, __globalSintxCSeqHints_typedef_name_0, 1 }
	, { ENSrzAnlzSintxC_initializer, __globalSintxCSeq_initializer_0, 1, __globalSintxCSeqHints_initializer_0, 30 }
	, { ENSrzAnlzSintxC_initializer, __globalSintxCSeq_initializer_1, 3, __globalSintxCSeqHints_initializer_1, 1 }
	, { ENSrzAnlzSintxC_initializer, __globalSintxCSeq_initializer_2, 4, __globalSintxCSeqHints_initializer_2, 1 }
	, { ENSrzAnlzSintxC_initializer_list, __globalSintxCSeq_initializer_list_0, 1, __globalSintxCSeqHints_initializer_list_0, 32 }
	, { ENSrzAnlzSintxC_initializer_list, __globalSintxCSeq_initializer_list_1, 2, __globalSintxCSeqHints_initializer_list_1, 5 }
	, { ENSrzAnlzSintxC_initializer_list, __globalSintxCSeq_initializer_list_2, 3, __globalSintxCSeqHints_initializer_list_2, 38 }
	, { ENSrzAnlzSintxC_initializer_list, __globalSintxCSeq_initializer_list_3, 4, __globalSintxCSeqHints_initializer_list_3, 38 }
	, { ENSrzAnlzSintxC_designation, __globalSintxCSeq_designation_0, 2, __globalSintxCSeqHints_designation_0, 4 }
	, { ENSrzAnlzSintxC_designator_list, __globalSintxCSeq_designator_list_0, 1, __globalSintxCSeqHints_designator_list_0, 3 }
	, { ENSrzAnlzSintxC_designator_list, __globalSintxCSeq_designator_list_1, 2, __globalSintxCSeqHints_designator_list_1, 4 }
	, { ENSrzAnlzSintxC_designator, __globalSintxCSeq_designator_0, 3, __globalSintxCSeqHints_designator_0, 1 }
	, { ENSrzAnlzSintxC_designator, __globalSintxCSeq_designator_1, 2, __globalSintxCSeqHints_designator_1, 1 }
	, { ENSrzAnlzSintxC_statement, __globalSintxCSeq_statement_0, 1, __globalSintxCSeqHints_statement_0, 4 }
	, { ENSrzAnlzSintxC_statement, __globalSintxCSeq_statement_1, 1, __globalSintxCSeqHints_statement_1, 2 }
	, { ENSrzAnlzSintxC_statement, __globalSintxCSeq_statement_2, 1, __globalSintxCSeqHints_statement_2, 33 }
	, { ENSrzAnlzSintxC_statement, __globalSintxCSeq_statement_3, 1, __globalSintxCSeqHints_statement_3, 3 }
	, { ENSrzAnlzSintxC_statement, __globalSintxCSeq_statement_4, 1, __globalSintxCSeqHints_statement_4, 4 }
	, { ENSrzAnlzSintxC_statement, __globalSintxCSeq_statement_5, 1, __globalSintxCSeqHints_statement_5, 5 }
	, { ENSrzAnlzSintxC_labeled_statement, __globalSintxCSeq_labeled_statement_0, 3, __globalSintxCSeqHints_labeled_statement_0, 1 }
	, { ENSrzAnlzSintxC_labeled_statement, __globalSintxCSeq_labeled_statement_1, 4, __globalSintxCSeqHints_labeled_statement_1, 1 }
	, { ENSrzAnlzSintxC_labeled_statement, __globalSintxCSeq_labeled_statement_2, 3, __globalSintxCSeqHints_labeled_statement_2, 1 }
	, { ENSrzAnlzSintxC_compound_statement, __globalSintxCSeq_compound_statement_0, 2, __globalSintxCSeqHints_compound_statement_0, 1 }
	, { ENSrzAnlzSintxC_compound_statement, __globalSintxCSeq_compound_statement_1, 3, __globalSintxCSeqHints_compound_statement_1, 1 }
	, { ENSrzAnlzSintxC_block_item_list, __globalSintxCSeq_block_item_list_0, 1, __globalSintxCSeqHints_block_item_list_0, 85 }
	, { ENSrzAnlzSintxC_block_item_list, __globalSintxCSeq_block_item_list_1, 2, __globalSintxCSeqHints_block_item_list_1, 86 }
	, { ENSrzAnlzSintxC_block_item, __globalSintxCSeq_block_item_0, 1, __globalSintxCSeqHints_block_item_0, 34 }
	, { ENSrzAnlzSintxC_block_item, __globalSintxCSeq_block_item_1, 1, __globalSintxCSeqHints_block_item_1, 51 }
	, { ENSrzAnlzSintxC_expression_statement, __globalSintxCSeq_expression_statement_0, 1, __globalSintxCSeqHints_expression_statement_0, 1 }
	, { ENSrzAnlzSintxC_expression_statement, __globalSintxCSeq_expression_statement_1, 2, __globalSintxCSeqHints_expression_statement_1, 31 }
	, { ENSrzAnlzSintxC_selection_statement, __globalSintxCSeq_selection_statement_0, 5, __globalSintxCSeqHints_selection_statement_0, 1 }
	, { ENSrzAnlzSintxC_selection_statement, __globalSintxCSeq_selection_statement_1, 7, __globalSintxCSeqHints_selection_statement_1, 1 }
	, { ENSrzAnlzSintxC_selection_statement, __globalSintxCSeq_selection_statement_2, 5, __globalSintxCSeqHints_selection_statement_2, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_0, 5, __globalSintxCSeqHints_iteration_statement_0, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_1, 7, __globalSintxCSeqHints_iteration_statement_1, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_2, 6, __globalSintxCSeqHints_iteration_statement_2, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_3, 7, __globalSintxCSeqHints_iteration_statement_3, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_4, 7, __globalSintxCSeqHints_iteration_statement_4, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_5, 7, __globalSintxCSeqHints_iteration_statement_5, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_6, 8, __globalSintxCSeqHints_iteration_statement_6, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_7, 8, __globalSintxCSeqHints_iteration_statement_7, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_8, 8, __globalSintxCSeqHints_iteration_statement_8, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_9, 9, __globalSintxCSeqHints_iteration_statement_9, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_10, 6, __globalSintxCSeqHints_iteration_statement_10, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_11, 7, __globalSintxCSeqHints_iteration_statement_11, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_12, 7, __globalSintxCSeqHints_iteration_statement_12, 1 }
	, { ENSrzAnlzSintxC_iteration_statement, __globalSintxCSeq_iteration_statement_13, 8, __globalSintxCSeqHints_iteration_statement_13, 1 }
	, { ENSrzAnlzSintxC_jump_statement, __globalSintxCSeq_jump_statement_0, 3, __globalSintxCSeqHints_jump_statement_0, 1 }
	, { ENSrzAnlzSintxC_jump_statement, __globalSintxCSeq_jump_statement_1, 2, __globalSintxCSeqHints_jump_statement_1, 1 }
	, { ENSrzAnlzSintxC_jump_statement, __globalSintxCSeq_jump_statement_2, 2, __globalSintxCSeqHints_jump_statement_2, 1 }
	, { ENSrzAnlzSintxC_jump_statement, __globalSintxCSeq_jump_statement_3, 2, __globalSintxCSeqHints_jump_statement_3, 1 }
	, { ENSrzAnlzSintxC_jump_statement, __globalSintxCSeq_jump_statement_4, 3, __globalSintxCSeqHints_jump_statement_4, 1 }
	, { ENSrzAnlzSintxC_translation_unit, __globalSintxCSeq_translation_unit_0, 1, __globalSintxCSeqHints_translation_unit_0, 36 }
	, { ENSrzAnlzSintxC_translation_unit, __globalSintxCSeq_translation_unit_1, 2, __globalSintxCSeqHints_translation_unit_1, 37 }
	, { ENSrzAnlzSintxC_external_declaration, __globalSintxCSeq_external_declaration_0, 1, __globalSintxCSeqHints_external_declaration_0, 34 }
	, { ENSrzAnlzSintxC_external_declaration, __globalSintxCSeq_external_declaration_1, 1, __globalSintxCSeqHints_external_declaration_1, 34 }
	, { ENSrzAnlzSintxC_function_definition, __globalSintxCSeq_function_definition_0, 3, __globalSintxCSeqHints_function_definition_0, 33 }
	, { ENSrzAnlzSintxC_declaration_list, __globalSintxCSeq_declaration_list_0, 1, __globalSintxCSeqHints_declaration_list_0, 34 }
	, { ENSrzAnlzSintxC_declaration_list, __globalSintxCSeq_declaration_list_1, 2, __globalSintxCSeqHints_declaration_list_1, 35 }
};

//---------------------------
// Set defs
//---------------------------
void SrcAnlzSintxC_initDefs(STSrcAnlzSintxDefs* dst){
	//Keywords
	dst->keyws			= __globalKeywords_c;
	dst->keywsSz		= (sizeof(__globalKeywords_c) / sizeof(__globalKeywords_c[0]));
	dst->keywsRngs		= __globalKeywords_c_lenRngs;
	dst->keywsRngsSz	= (sizeof(__globalKeywords_c_lenRngs) / sizeof(__globalKeywords_c_lenRngs[0]));
	//Punctuators
	dst->puncts			= __globalPunctuators_c;
	dst->punctsSz		= (sizeof(__globalPunctuators_c) / sizeof(__globalPunctuators_c[0]));
	dst->punctsRngs		= __globalPunctuators_c_lenRngs;
	dst->punctsRngsSz	= (sizeof(__globalPunctuators_c_lenRngs) / sizeof(__globalPunctuators_c_lenRngs[0]));
	//Sintax defs
	dst->defs			= __globalSintxCDefs;
	dst->defsSz			= (sizeof(__globalSintxCDefs) / sizeof(__globalSintxCDefs[0]));
	//Sintax defs sequences
	dst->defsSeqs		= __globalSintxCDefSeqs;
	dst->defsSeqsSz		= (sizeof(__globalSintxCDefSeqs) / sizeof(__globalSintxCDefSeqs[0]));
	dst->defsSeqsRngs	= __globalSintxCDefSeqsRngs;
	dst->defsSeqsRngsSz	= (sizeof(__globalSintxCDefSeqsRngs) / sizeof(__globalSintxCDefSeqsRngs[0]));
#	ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
	//Validate keywords definitions
	{
		SI32 prevSz = 0, count = 0;
		const char** strPtr = __globalKeywords_c;
		while(*strPtr != NULL){ NBASSERT(prevSz <= NBString_strLenBytes(*strPtr)); prevSz = NBString_strLenBytes(*strPtr); strPtr++; count++; }
		//
		const SI32 rngsSz = (sizeof(__globalKeywords_c_lenRngs) / sizeof(__globalKeywords_c_lenRngs[0]));
		SI32 i, sum = 0; STSrcAnlzRange lastRng = {0, 0};
		for(i = 0; i < rngsSz; i++){
			const STSrcAnlzRange* rng = &__globalKeywords_c_lenRngs[i];
			NBASSERT(rng->start >= 0 && rng->count >= 0)
			NBASSERT(rng->start == (lastRng.start + lastRng.count))
			NBASSERT(rng->start >= 0 && rng->start < count)
			NBASSERT((rng->start + rng->count) <= count)
			SI32 i2; for(i2 = 0; i2 < rng->count; i2++){ NBASSERT(NBString_strLenBytes(__globalKeywords_c[rng->start + i2]) == (i + 1)) }
			lastRng = *rng;
			sum += rng->count;
		}
		NBASSERT(count == sum)
	}
	//Validate punctuators definitions
	{
		SI32 prevSz = 0, count = 0;
		const char** strPtr = __globalPunctuators_c;
		while(*strPtr != NULL){ NBASSERT(prevSz <= NBString_strLenBytes(*strPtr)); prevSz = NBString_strLenBytes(*strPtr); strPtr++; count++; }
		//
		const SI32 rngsSz = (sizeof(__globalPunctuators_c_lenRngs) / sizeof(__globalPunctuators_c_lenRngs[0]));
		SI32 i, sum = 0; STSrcAnlzRange lastRng = {0, 0};
		for(i = 0; i < rngsSz; i++){
			const STSrcAnlzRange* rng = &__globalPunctuators_c_lenRngs[i];
			NBASSERT(rng->start >= 0 && rng->count >= 0)
			NBASSERT(rng->start == (lastRng.start + lastRng.count))
			NBASSERT(rng->start >= 0 && rng->start < count)
			NBASSERT((rng->start + rng->count) <= count)
			SI32 i2; for(i2 = 0; i2 < rng->count; i2++){ NBASSERT(NBString_strLenBytes(__globalPunctuators_c[rng->start + i2]) == (i + 1)) }
			lastRng = *rng;
			sum += rng->count;
		}
		NBASSERT(count == sum)
	}
	//Validate sintaxs
	{
		const SI32 count = (sizeof(__globalSintxCDefSeqs) / sizeof(__globalSintxCDefSeqs[0]));
		SI32 i, prevEnumVal = -1;
		for(i = 0; i < count; i++){
			const STSrcAnlzSintxSeq* def = &__globalSintxCDefSeqs[i];
			NBASSERT(prevEnumVal <= def->iDefEnum)
			prevEnumVal = def->iDefEnum;
		}
	}
	//Validate hints order (must be arranged for binary search)
	{
		SI32 i; const SI32 count = (sizeof(__globalSintxCDefs) / sizeof(__globalSintxCDefs[0]));
		for(i = 0; i < count; i++){
			const STSrcAnlzSintx* def = &__globalSintxCDefs[i];
			NBASSERT(def->deepHints != NULL && def->deepHintsSz > 0)
			if(def->deepHints != NULL && def->deepHintsSz > 0){
				STSrcAnlzSintxPart prevPart = def->deepHints[0];
				SI32 i2; for(i2 = 1; i2 < def->deepHintsSz; i2++){
					STSrcAnlzSintxPart part = def->deepHints[i2];
					NBASSERT(STSrcAnlzSintxPart_compare(ENCompareMode_Lower, &prevPart, &part, sizeof(part)))
					prevPart = part;
				}
			}
		}
	}
	//Validate seqs-hints order (must be arranged for binary search)
	{
		SI32 i; const SI32 count = (sizeof(__globalSintxCDefSeqs) / sizeof(__globalSintxCDefSeqs[0]));
		for(i = 0; i < count; i++){
			const STSrcAnlzSintxSeq* def = &__globalSintxCDefSeqs[i];
			NBASSERT(def->deepHints != NULL && def->deepHintsSz > 0)
			if(def->deepHints != NULL && def->deepHintsSz > 0){
				STSrcAnlzSintxPart prevPart = def->deepHints[0];
				SI32 i2; for(i2 = 1; i2 < def->deepHintsSz; i2++){
					STSrcAnlzSintxPart part = def->deepHints[i2];
					NBASSERT(STSrcAnlzSintxPart_compare(ENCompareMode_Lower, &prevPart, &part, sizeof(part)))
					prevPart = part;
				}
			}
		}
	}
#	endif
}
