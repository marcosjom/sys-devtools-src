//
//  NBSrcAnlzSintxCPlain.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_DEFS_LANG_C_PLAIN_H
#define NB_SRCANLZ_DEFS_LANG_C_PLAIN_H

#ifdef __cplusplus
//extern "C" {
#endif

#include "NBSrcAnlzSintx.h"

// Lang-C sintax definitions from the language reference.

const STSrcAnlzSintxPlainDef __globalLangSintaxDefsPlainC[] = {
	{ "primary-expression", "identifier" }
	, { "primary-expression", "constant" }
	, { "primary-expression", "string-literal" }
	, { "primary-expression", "( expression )" }
	, { "postfix-expression", "primary-expression" }
	, { "postfix-expression", "postfix-expression [ expression ]" }
	, { "postfix-expression", "postfix-expression ( )" }
	, { "postfix-expression", "postfix-expression ( argument-expression-list )" }
	, { "postfix-expression", "postfix-expression . identifier" }
	, { "postfix-expression", "postfix-expression -> identifier" }
	, { "postfix-expression", "postfix-expression ++" }
	, { "postfix-expression", "postfix-expression --" }
	, { "postfix-expression", "( type-name ) { initializer-list }" }
	, { "postfix-expression", "( type-name ) { initializer-list , }" }
	, { "argument-expression-list", "assignment-expression" }
	, { "argument-expression-list", "argument-expression-list , assignment-expression" }
	, { "unary-expression", "postfix-expression" }
	, { "unary-expression", "++ unary-expression" }
	, { "unary-expression", "-- unary-expression" }
	, { "unary-expression", "unary-operator cast-expression" }
	, { "unary-expression", "sizeof unary-expression" }
	, { "unary-expression", "sizeof ( type-name )" }
	, { "unary-operator", "&" }
	, { "unary-operator", "*" }
	, { "unary-operator", "+" }
	, { "unary-operator", "-" }
	, { "unary-operator", "~" }
	, { "unary-operator", "!" }
	, { "cast-expression", "unary-expression" }
	, { "cast-expression", "( type-name ) cast-expression" }
	, { "multiplicative-expression", "cast-expression" }
	, { "multiplicative-expression", "multiplicative-expression * cast-expression" }
	, { "multiplicative-expression", "multiplicative-expression / cast-expression" }
	, { "multiplicative-expression", "multiplicative-expression % cast-expression" }
	, { "additive-expression", "multiplicative-expression" }
	, { "additive-expression", "additive-expression + multiplicative-expression" }
	, { "additive-expression", "additive-expression - multiplicative-expression" }
	, { "shift-expression", "additive-expression" }
	, { "shift-expression", "shift-expression << additive-expression" }
	, { "shift-expression", "shift-expression >> additive-expression" }
	, { "relational-expression", "shift-expression" }
	, { "relational-expression", "relational-expression < shift-expression" }
	, { "relational-expression", "relational-expression > shift-expression" }
	, { "relational-expression", "relational-expression <= shift-expression" }
	, { "relational-expression", "relational-expression >= shift-expression" }
	, { "equality-expression", "relational-expression" }
	, { "equality-expression", "equality-expression == relational-expression" }
	, { "equality-expression", "equality-expression != relational-expression" }
	, { "AND-expression", "equality-expression" }
	, { "AND-expression", "AND-expression & equality-expression" }
	, { "exclusive-OR-expression", "AND-expression" }
	, { "exclusive-OR-expression", "exclusive-OR-expression ^ AND-expression" }
	, { "inclusive-OR-expression", "exclusive-OR-expression" }
	, { "inclusive-OR-expression", "inclusive-OR-expression | exclusive-OR-expression" }
	, { "logical-AND-expression", "inclusive-OR-expression" }
	, { "logical-AND-expression", "logical-AND-expression && inclusive-OR-expression" }
	, { "logical-OR-expression", "logical-AND-expression" }
	, { "logical-OR-expression", "logical-OR-expression || logical-AND-expression" }
	, { "conditional-expression", "logical-OR-expression" }
	, { "conditional-expression", "logical-OR-expression ? expression : conditional-expression" }
	, { "assignment-expression", "conditional-expression" }
	, { "assignment-expression", "unary-expression assignment-operator assignment-expression" }
	, { "assignment-operator", "=" }
	, { "assignment-operator", "*=" }
	, { "assignment-operator", "/=" }
	, { "assignment-operator", "%=" }
	, { "assignment-operator", "+=" }
	, { "assignment-operator", "-=" }
	, { "assignment-operator", "<<=" }
	, { "assignment-operator", ">>=" }
	, { "assignment-operator", "&=" }
	, { "assignment-operator", "^=" }
	, { "assignment-operator", "|=" }
	, { "expression", "assignment-expression" }
	, { "expression", "expression , assignment-expression" }
	, { "constant-expression", "conditional-expression" }
	, { "declaration", "declaration-specifiers ;" }
	, { "declaration", "declaration-specifiers init-declarator-list ;" }
	, { "declaration-specifiers", "storage-class-specifier" }
	, { "declaration-specifiers", "storage-class-specifier declaration-specifiers" }
	, { "declaration-specifiers", "type-specifier" }
	, { "declaration-specifiers", "type-specifier declaration-specifiers" }
	, { "declaration-specifiers", "type-qualifier" }
	, { "declaration-specifiers", "type-qualifier declaration-specifiers" }
	, { "declaration-specifiers", "function-specifier" }
	, { "declaration-specifiers", "function-specifier declaration-specifiers" }
	, { "init-declarator-list", "init-declarator" }
	, { "init-declarator-list", "init-declarator-list , init-declarator" }
	, { "init-declarator", "declarator" }
	, { "init-declarator", "declarator = initializer" }
	, { "storage-class-specifier", "typedef" }
	, { "storage-class-specifier", "extern" }
	, { "storage-class-specifier", "static" }
	, { "storage-class-specifier", "auto" }
	, { "storage-class-specifier", "register" }
	, { "type-specifier", "void" }
	, { "type-specifier", "char" }
	, { "type-specifier", "short" }
	, { "type-specifier", "int" }
	, { "type-specifier", "long" }
	, { "type-specifier", "float" }
	, { "type-specifier", "double" }
	, { "type-specifier", "signed" }
	, { "type-specifier", "unsigned" }
	, { "type-specifier", "_Bool" }
	, { "type-specifier", "_Complex" }
	, { "type-specifier", "struct-or-union-specifier" }
	, { "type-specifier", "enum-specifier" }
	, { "type-specifier", "typedef-name" }
	, { "struct-or-union-specifier", "struct-or-union { struct-declaration-list }" }
	, { "struct-or-union-specifier", "struct-or-union identifier { struct-declaration-list }" }
	, { "struct-or-union-specifier", "struct-or-union identifier" }
	, { "struct-or-union", "struct" }
	, { "struct-or-union", "union" }
	, { "struct-declaration-list", "struct-declaration" }
	, { "struct-declaration-list", "struct-declaration-list struct-declaration" }
	, { "struct-declaration", "specifier-qualifier-list struct-declarator-list ;" }
	, { "specifier-qualifier-list", "type-specifier" }
	, { "specifier-qualifier-list", "type-specifier specifier-qualifier-list" }
	, { "specifier-qualifier-list", "type-qualifier" }
	, { "specifier-qualifier-list", "type-qualifier specifier-qualifier-list" }
	, { "struct-declarator-list", "struct-declarator" }
	, { "struct-declarator-list", "struct-declarator-list , struct-declarator" }
	, { "struct-declarator", "declarator" }
	, { "struct-declarator", ": constant-expression" }
	, { "struct-declarator", "declarator : constant-expression" }
	, { "enum-specifier", "enum { enumerator-list }" }
	, { "enum-specifier", "enum identifier { enumerator-list }" }
	, { "enum-specifier", "enum { enumerator-list , }" }
	, { "enum-specifier", "enum identifier { enumerator-list , }" }
	, { "enum-specifier", "enum identifier" }
	, { "enumerator-list", "enumerator" }
	, { "enumerator-list", "enumerator-list , enumerator" }
	, { "enumeration-constant", "identifier" }
	, { "enumerator", "enumeration-constant" }
	, { "enumerator", "enumeration-constant = constant-expression" }
	, { "type-qualifier", "const" }
	, { "type-qualifier", "restrict" }
	, { "type-qualifier", "volatile" }
	, { "function-specifier", "inline" }
	, { "declarator", "direct-declarator" }
	, { "declarator", "pointer direct-declarator" }
	, { "direct-declarator", "identifier" }
	, { "direct-declarator", "( declarator )" }
	, { "direct-declarator", "direct-declarator [ ]" }
	, { "direct-declarator", "direct-declarator [ assignment-expression ]" }
	, { "direct-declarator", "direct-declarator [ type-qualifier-list ]" }
	, { "direct-declarator", "direct-declarator [ type-qualifier-list assignment-expression ]" }
	, { "direct-declarator", "direct-declarator [ static assignment-expression ]" }
	, { "direct-declarator", "direct-declarator [ static type-qualifier-list assignment-expression ]" }
	, { "direct-declarator", "direct-declarator [ type-qualifier-list static assignment-expression ]" }
	, { "direct-declarator", "direct-declarator [ * ]" }
	, { "direct-declarator", "direct-declarator [ type-qualifier-list * ]" }
	, { "direct-declarator", "direct-declarator ( parameter-type-list )" }
	, { "direct-declarator", "direct-declarator ( )" }
	, { "direct-declarator", "direct-declarator ( identifier-list )" }
	, { "pointer", "*" }
	, { "pointer", "* type-qualifier-list" }
	, { "pointer", "* pointer" }
	, { "pointer", "* type-qualifier-list pointer" }
	, { "type-qualifier-list", "type-qualifier" }
	, { "type-qualifier-list", "type-qualifier-list type-qualifier" }
	, { "parameter-type-list", "parameter-list" }
	, { "parameter-type-list", "parameter-list , ..." }
	, { "parameter-list", "parameter-declaration" }
	, { "parameter-list", "parameter-list , parameter-declaration" }
	, { "parameter-declaration", "declaration-specifiers declarator" }
	, { "parameter-declaration", "declaration-specifiers" }
	, { "parameter-declaration", "declaration-specifiers abstract-declarator" }
	, { "identifier-list", "identifier" }
	, { "identifier-list", "identifier-list , identifier" }
	, { "type-name", "specifier-qualifier-list" }
	, { "type-name", "specifier-qualifier-list abstract-declarator" }
	, { "abstract-declarator", "pointer" }
	, { "abstract-declarator", "direct-abstract-declarator" }
	, { "abstract-declarator", "pointer direct-abstract-declarator" }
	, { "direct-abstract-declarator", "( abstract-declarator )" }
	, { "direct-abstract-declarator", "[ ]" }
	, { "direct-abstract-declarator", "direct-abstract-declarator [ ]" }
	, { "direct-abstract-declarator", "[ type-qualifier-list ]" }
	, { "direct-abstract-declarator", "[ assignment-expression ]" }
	, { "direct-abstract-declarator", "direct-abstract-declarator [ type-qualifier-list ]" }
	, { "direct-abstract-declarator", "direct-abstract-declarator [ assignment-expression ]" }
	, { "direct-abstract-declarator", "[ type-qualifier-list assignment-expression ]" }
	, { "direct-abstract-declarator", "direct-abstract-declarator [ type-qualifier-list assignment-expression ]" }
	, { "direct-abstract-declarator", "[ static assignment-expression ]" }
	, { "direct-abstract-declarator", "[ static type-qualifier-list assignment-expression ]" }
	, { "direct-abstract-declarator", "direct-abstract-declarator [ assignment-expression ]" }
	, { "direct-abstract-declarator", "direct-abstract-declarator [ static type-qualifier-list assignment-expression ]" }
	, { "direct-abstract-declarator", "[ type-qualifier-list static assignment-expression ]" }
	, { "direct-abstract-declarator", "direct-abstract-declarator [ type-qualifier-list static assignment-expression ]" }
	, { "direct-abstract-declarator", "[ * ]" }
	, { "direct-abstract-declarator", "direct-abstract-declarator [ * ]" }
	, { "direct-abstract-declarator", "( )" }
	, { "direct-abstract-declarator", "( parameter-type-list )" }
	, { "direct-abstract-declarator", "direct-abstract-declarator ( )" }
	, { "direct-abstract-declarator", "direct-abstract-declarator ( parameter-type-list )" }
	, { "typedef-name", "identifier" }
	, { "initializer", "assignment-expression" }
	, { "initializer", "{ initializer-list }" }
	, { "initializer", "{ initializer-list , }" }
	, { "initializer-list", "initializer" }
	, { "initializer-list", "designation initializer" }
	, { "initializer-list", "initializer-list , initializer" }
	, { "initializer-list", "initializer-list , designation initializer" }
	, { "designation", "designator-list =" }
	, { "designator-list", "designator" }
	, { "designator-list", "designator-list designator" }
	, { "designator", "[ constant-expression ]" }
	, { "designator", ". identifier" }
	, { "statement", "labeled-statement" }
	, { "statement", "compound-statement" }
	, { "statement", "expression-statement" }
	, { "statement", "selection-statement" }
	, { "statement", "iteration-statement" }
	, { "statement", "jump-statement" }
	, { "labeled-statement", "identifier : statement" }
	, { "labeled-statement", "case constant-expression : statement" }
	, { "labeled-statement", "default : statement" }
	, { "compound-statement", "{ }" }
	, { "compound-statement", "{ block-item-list }" }
	, { "block-item-list", "block-item" }
	, { "block-item-list", "block-item-list block-item" }
	, { "block-item", "declaration" }
	, { "block-item", "statement" }
	, { "expression-statement", ";" }
	, { "expression-statement", "expression ;" }
	, { "selection-statement", "if ( expression ) statement" }
	, { "selection-statement", "if ( expression ) statement else statement" }
	, { "selection-statement", "switch ( expression ) statement" }
	, { "iteration-statement", "while ( expression ) statement" }
	, { "iteration-statement", "do statement while ( expression ) ;" }
	, { "iteration-statement", "for ( ; ; ) statement" }
	, { "iteration-statement", "for ( expression ; ; ) statement" }
	, { "iteration-statement", "for ( ; expression ; ) statement" }
	, { "iteration-statement", "for ( ; ; expression ) statement" }
	, { "iteration-statement", "for ( expression ; expression ; ) statement" }
	, { "iteration-statement", "for ( ; expression ; expression ) statement" }
	, { "iteration-statement", "for ( expression ; ; expression ) statement" }
	, { "iteration-statement", "for ( expression ; expression ; expression ) statement" }
	, { "iteration-statement", "for ( declaration ; ) statement" }
	, { "iteration-statement", "for ( declaration expression ; ) statement" }
	, { "iteration-statement", "for ( declaration ; expression ) statement" }
	, { "iteration-statement", "for ( declaration expression ; expression ) statement" }
	, { "jump-statement", "goto identifier ;" }
	, { "jump-statement", "continue ;" }
	, { "jump-statement", "break ;" }
	, { "jump-statement", "return ;" }
	, { "jump-statement", "return expression ;" }
	, { "translation-unit", "external-declaration" }
	, { "translation-unit", "translation-unit external-declaration" }
	, { "external-declaration", "function-definition" }
	, { "external-declaration", "declaration" }
	, { "function-definition", "declaration-specifiers declarator compound-statement" }
	//, { "function-definition", "declaration-specifiers declarator declaration-list compound-statement" } //Todo: implement this sintax definition
	, { "declaration-list", "declaration" }
	, { "declaration-list", "declaration-list declaration" }
};


#ifdef __cplusplus
//}
#endif

#endif
