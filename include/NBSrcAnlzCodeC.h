//
//  NBSrcAnlzCodeC.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_CODE_C_H
#define NB_SRCANLZ_CODE_C_H

#include "nb/NBFrameworkDefs.h"
//
#include "nb/core/NBString.h"
#include "nb/core/NBArray.h"
#include "NBSrcAnlzBase.h" //for 'STSrcAnlzRange'

#ifdef __cplusplus
//extern "C" {
#endif

//Declarator specifier mask bits

typedef enum ENSrcAnlzDecSpecifiersBit_ {
	//StorageClass
	ENSrcAnlzDecSpecifiersBit_SC_Typedef		= (1 << 0),
	ENSrcAnlzDecSpecifiersBit_SC_Extern			= (1 << 1),
	ENSrcAnlzDecSpecifiersBit_SC_Static			= (1 << 2),
	ENSrcAnlzDecSpecifiersBit_SC_Auto			= (1 << 3),
	ENSrcAnlzDecSpecifiersBit_SC_Register		= (1 << 4),
	ENSrcAnlzDecSpecifiersBits_SC_ALL			= (ENSrcAnlzDecSpecifiersBit_SC_Typedef | ENSrcAnlzDecSpecifiersBit_SC_Extern
												   | ENSrcAnlzDecSpecifiersBit_SC_Static | ENSrcAnlzDecSpecifiersBit_SC_Auto | ENSrcAnlzDecSpecifiersBit_SC_Register),
	//TypeSpecifier
	ENSrcAnlzDecSpecifiersBit_TS_Void			= (1 << 5),
	ENSrcAnlzDecSpecifiersBit_TS_Char			= (1 << 6),
	ENSrcAnlzDecSpecifiersBit_TS_Short			= (1 << 7),
	ENSrcAnlzDecSpecifiersBit_TS_Int			= (1 << 8),
	ENSrcAnlzDecSpecifiersBit_TS_Long			= (1 << 9),
	ENSrcAnlzDecSpecifiersBit_TS_LongLong		= (1 << 10),
	ENSrcAnlzDecSpecifiersBit_TS_Float			= (1 << 11),
	ENSrcAnlzDecSpecifiersBit_TS_Double			= (1 << 12),
	ENSrcAnlzDecSpecifiersBit_TS_Signed			= (1 << 13),
	ENSrcAnlzDecSpecifiersBit_TS_Unsigned		= (1 << 14),
	ENSrcAnlzDecSpecifiersBit_TS__Bool			= (1 << 15),
	ENSrcAnlzDecSpecifiersBit_TS__Complex		= (1 << 16),
	ENSrcAnlzDecSpecifiersBit_TS_StructUnion	= (1 << 17),	//Declared struct-or-union (usrType at iCustomTypeDef)
	ENSrcAnlzDecSpecifiersBit_TS_Enum			= (1 << 18),	//Declared enum (usrType at iCustomTypeDef)
	ENSrcAnlzDecSpecifiersBit_TS_TypedefName	= (1 << 19),	//Declared typedef (declaration by name at iCustomTypeDef)
	ENSrcAnlzDecSpecifiersBit_TS_Declar			= (1 << 20),	//Declaration's type (declarator or abstract-declarator between parenthesis stored at iCustomTypeDef)
	ENSrcAnlzDecSpecifiersBits_TS_ALL			= (ENSrcAnlzDecSpecifiersBit_TS_Void | ENSrcAnlzDecSpecifiersBit_TS_Char | ENSrcAnlzDecSpecifiersBit_TS_Short
												   | ENSrcAnlzDecSpecifiersBit_TS_Int | ENSrcAnlzDecSpecifiersBit_TS_Long | ENSrcAnlzDecSpecifiersBit_TS_LongLong
												   | ENSrcAnlzDecSpecifiersBit_TS_Float | ENSrcAnlzDecSpecifiersBit_TS_Double | ENSrcAnlzDecSpecifiersBit_TS_Signed
												   | ENSrcAnlzDecSpecifiersBit_TS_Unsigned | ENSrcAnlzDecSpecifiersBit_TS__Bool | ENSrcAnlzDecSpecifiersBit_TS__Complex
												   | ENSrcAnlzDecSpecifiersBit_TS_StructUnion | ENSrcAnlzDecSpecifiersBit_TS_Enum | ENSrcAnlzDecSpecifiersBit_TS_TypedefName
												   | ENSrcAnlzDecSpecifiersBit_TS_Declar),
	//TypeQualifier
	ENSrcAnlzDecSpecifiersBit_TQ_Const			= (1 << 21),
	ENSrcAnlzDecSpecifiersBit_TQ_Restrict		= (1 << 22),
	ENSrcAnlzDecSpecifiersBit_TQ_Volatile		= (1 << 23),
	ENSrcAnlzDecSpecifiersBits_TQ_ALL			= (ENSrcAnlzDecSpecifiersBit_TQ_Const | ENSrcAnlzDecSpecifiersBit_TQ_Restrict | ENSrcAnlzDecSpecifiersBit_TQ_Volatile),
	//FunctionSpecifier
	ENSrcAnlzDecSpecifiersBit_FS_inline			= (1 << 24),
	ENSrcAnlzDecSpecifiersBits_FS_ALL			= (ENSrcAnlzDecSpecifiersBit_FS_inline),
} ENSrcAnlzDecSpecifiersMask;

//Types
typedef struct STSrcAnlzCodeCDeclarType_ {
	UI32			specifiersMask;	//storage-class, type-specifier, type-qualifier and function-qualifier
	UI32			iCustomTypeDef;	//If not a native type, index to the corresponding definition (struct, union, enumeration, typedef ...)
	//Pointer
	STSrcAnlzRange	ptrSpecsRng;	//pointers specs.
	//Array
	BOOL			isArray;		//'FALSE' if one instance, 'TRUE' if is an array.
	UI32			arrayTotalSz;	//array total size (if definition is isComplete), this results from multiplying the dimensions.
	//Function
	UI32			paramsBodyDefIdx;	//params's definition 'STSrcAnlzCodeCDeclarsScope' (0 for not-defined)
	BOOL			paramsLenFlexible;	//the parms definition is flexible (last param is "...")
} STSrcAnlzCodeCDeclarType;

//Types pointer props
typedef struct STSrcAnlzCodeCDeclarPtr_ {
	UI32			specifiersMask;	//type-qualifier
	UI16			iDeepLvl;		//deep level (to identify the params of each block)
} STSrcAnlzCodeCDeclarPtr;

//Types array size's state
typedef enum ENSrcAnlzCodeArrSzType_ {
	ENSrcAnlzCodeArrSzType_Incomplete = 0,	//Size not defined
	ENSrcAnlzCodeArrSzType_Variable,		//Size is '*'
	ENSrcAnlzCodeArrSzType_Complete			//Size is an expression
} ENSrcAnlzCodeArrSzType;

//Types array size (one of its dimensions)
typedef struct STSrcAnlzCodeCDeclarArr_ {
	UI8			szType;			//ENSrcAnlzCodeArrSzType
	UI16		iDeepLvl;		//deep level (to identify the params of each block)
} STSrcAnlzCodeCDeclarArr;

//User's types  (struct, union and enum)
typedef enum ENSrcAnlzCodeUsrType_ {
	ENSrcAnlzCodeUsrType_Enum = 0,
	ENSrcAnlzCodeUsrType_Struct,
	ENSrcAnlzCodeUsrType_Union,
	ENSrcAnlzCodeUsrType_Count
} ENSrcAnlzCodeUsrType;

typedef struct STSrcAnlzCodeCUsrType_ {
	UI32			iParentScope;	//parent code block (0 for root scope)
	UI16			iDeepLvl;		//deep level (to identify the parent's block)
	UI8				type;			//ENSrcAnlzCodeUsrType ('struct', 'union', 'enum')
	BOOL			isComplete;		//the body's '}' was found.
	UI32			iNameFirstChar;	//name's first char index
	UI32			iBodyDef;		//body's definition 'STSrcAnlzCodeCDeclarsScope' (0 for not-defined)
} STSrcAnlzCodeCUsrType;

//Declarations (including typedefs and functions)
typedef struct STSrcAnlzCodeCDeclar_ {
	UI32			iParentScope;	//parent code block (0 for root scope)
	UI16			iDeepLvl;		//deep level (to identify the params of each block)
	UI8				szInBits;		//default:0, Only allowed in struct/union members declaration.
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	BOOL			dbgIsTypeDef;	//is a type definition, not a declaration
#	endif
	STSrcAnlzCodeCDeclarType type;	//type
	UI32			iNameFirstChar;	//name's first char index
	UI32			iBodyDef;		//function body's 'STSrcAnlzCodeCDeclarsScope'
} STSrcAnlzCodeCDeclar;

//Declare scope (block of code, enum declaration, struct/union declaration)
typedef struct STSrcAnlzCodeCDeclarsScope_ {
	UI32				iParentScope;	//parent declarations scope
	UI16				iDeepLvl;		//deep level (to identify the declars, structs, enums of this block)
	//Declarations
	STSrcAnlzRange		usrTypesRng;	//struct, union and enum declarations range in global array
	STSrcAnlzRange		declarsRng;		//declarations range in global array
	STSrcAnlzRange		scopesRng;		//Subscopes (block-item-list)
} STSrcAnlzCodeCDeclarsScope;

//c-code state
typedef struct STSrcAnlzCodeC_ {
	//Buffers
	STNBString	strsBuff;			//all strings (members names, vars names, string literals, ...)
	//User definitions
	STNBArray	defsUsrTypes;		//STSrcAnlzCodeCUsrType, user 'struct-or-union' or 'enum' types definitions
	STNBArray	defsDeclars;		//STSrcAnlzCodeCDeclar, declarations definitions (specifiers + name)
	STNBArray	defsDeclPtrSpecs;	//STSrcAnlzCodeCDeclarPtr, pointer specifications
	STNBArray	defsDeclArrSpecs;	//STSrcAnlzCodeCDeclarArr, array sizes specifications
	STNBArray	defsDeclarsScopes;	//STSrcAnlzCodeCDeclarsScope, declarations scopes (blocks of code, enums list, structs/unions defs)
} STSrcAnlzCodeC;

//Init and release state
void SrcAnlzCodeC_init(STSrcAnlzCodeC* state);
void SrcAnlzCodeC_release(STSrcAnlzCodeC* state);

//Find
BOOL SrcAnlzCodeC_findUsrTypeByName(const STSrcAnlzCodeC* state, const UI32 iTopDeclScope, const BOOL srchParentScope, const char* srchName, const UI32 maxIdxCount, UI32* dstIdx);
BOOL SrcAnlzCodeC_findDeclarByName(const STSrcAnlzCodeC* state, const UI32 iTopDeclScope, const BOOL srchParentScope, const char* srchName, const UI32 maxIdxCount, UI32* dstIdx);

//Add
BOOL SrcAnlzCodeC_getDeclarByIdx(STSrcAnlzCodeC* state, const UI32 iDeclarDef, const UI32 iParentScope, STSrcAnlzCodeCDeclar** dstDef, BOOL* dstIsNewDef);
BOOL SrcAnlzCodeC_getDeclarScopeByIdx(STSrcAnlzCodeC* state, const UI32 iDeclScopeDef, const UI32 iParentScope, STSrcAnlzCodeCDeclarsScope** dstDef, BOOL* dstIsNewDef);

//Print
void SrcAnlzCodeC_print(const STSrcAnlzCodeC* state);
void SrcAnlzCodeC_printConcatScope(const STSrcAnlzCodeC* state, const UI32 iScope, const UI32 printDeepLvl, STNBString* dst, STNBArraySorted* usrTypesPrinted);
void SrcAnlzCodeC_printConcatDeclar(const STSrcAnlzCodeC* state, const UI32 iDeclar, const BOOL followDepthDeclar, const UI32 printDeepLvl, STNBString* dst, STNBArraySorted* usrTypesPrinted);
void SrcAnlzCodeC_printConcatDeclarWithPathReverse(const STSrcAnlzCodeC* state, const UI32* pathItems, const UI32 pathSz, const UI32 iPathItem, const UI32 printDeepLvl, STNBString* dst, STNBArraySorted* usrTypesPrinted);
UI32 SrcAnlzCodeC_printConcatParams(const STSrcAnlzCodeC* state, const UI32 iScope, STNBString* dst, STNBArraySorted* usrTypesPrinted);
UI32 SrcAnlzCodeC_printConcatSpecMask(const STSrcAnlzCodeC* state, const STSrcAnlzCodeCDeclar* declar, const UI32 specMask, const UI32 tokensAdded, const UI32 printDeepLvl, STNBString* dst, STNBArraySorted* usrTypesPrinted);


#ifdef __cplusplus
//}
#endif

#endif
