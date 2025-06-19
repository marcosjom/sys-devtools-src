//
//  NBSrcAnlzCodeCParserConsume.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzCodeCParserConsume.h"
#include "NBSrcAnlzSintxC.h"	//for 'ENSrzAnlzSintxC'

//---------------------------
// Find
//---------------------------

//, { "declarator", "direct-declarator" }
//, { "declarator", "pointer direct-declarator" }
//, { "direct-declarator", "identifier" }
//, { "direct-declarator", "( declarator )" }
//, { "direct-declarator", "direct-declarator [ ]" }
//, { "direct-declarator", "direct-declarator [ assignment-expression ]" }
//, { "direct-declarator", "direct-declarator [ type-qualifier-list ]" }
//, { "direct-declarator", "direct-declarator [ type-qualifier-list assignment-expression ]" }
//, { "direct-declarator", "direct-declarator [ static assignment-expression ]" }
//, { "direct-declarator", "direct-declarator [ static type-qualifier-list assignment-expression ]" }
//, { "direct-declarator", "direct-declarator [ type-qualifier-list static assignment-expression ]" }
//, { "direct-declarator", "direct-declarator [ * ]" }
//, { "direct-declarator", "direct-declarator [ type-qualifier-list * ]" }
//, { "direct-declarator", "direct-declarator ( parameter-type-list )" }
//, { "direct-declarator", "direct-declarator ( )" }
//, { "direct-declarator", "direct-declarator ( identifier-list )" }
BOOL SrcAnlzCodeCParser_findDeclaratorIdentifierNode(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, const UI16 iFirstChild, const char** dstName){
	BOOL r = FALSE;
	const char* rName = NULL;
	if(state != NULL && srcScope != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			do {
				NBASSERT(rName == NULL)
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					if(partDef->type == ENSrzAnlzSintxPartType_Identifier){
						//Found the declarator's name
						NBASSERT(sintaxDefSeq->partsSz == 1) //only the identifier (expecting definition = { "direct-declarator", "identifier" } )
						NBASSERT(child->iValue < state->acumTokensCur->defs.use)
						const STSrcAnlzTokn* tokenDef = NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, child->iValue);
						const char* tokenStr = &state->acumTokensCur->strBuff.str[tokenDef->iAtBuff];
						rName = tokenStr;
						break;
					} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						if(partDef->idx == ENSrzAnlzSintxC_pointer){
							//Ignore
						} else if(partDef->idx == ENSrzAnlzSintxC_declarator || partDef->idx == ENSrzAnlzSintxC_direct_declarator){
							//Recursive call
							if(child->iChildSeq != 0){
								if(!SrcAnlzCodeCParser_findDeclaratorIdentifierNode(state, srcScope, child->iChildSeq, &rName)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else if(rName != NULL){
									break;
								}
							}
						} else {
							//No identifier found
							r = FALSE; NBASSERT(FALSE)
							break;
						}
					}
				}
				//Next
				if(child->iNextPart == 0){
					break;
				} else {
					iChild	= child->iNextPart;
					child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
					iPart++;
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
		}
	}
	if(dstName != NULL) *dstName = rName;
	return r;
}

//---------------------------
// Process
//---------------------------

BOOL SrcAnlzCodeCParser_consume(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STNBArray* nodes = &srcScope->parseTree.nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			//
			const UI32 iDeclScope = srcScope->iDeclScope;
			srcScope->stats.iLastActiveDecScope = iDeclScope;
			NBASSERT(iDeclScope > 0 && iDeclScope < dstCode->defsDeclarsScopes.use)
			switch(sintaxDefSeq->iDefEnum){
				case ENSrzAnlzSintxC_struct_declaration:
					//Struct declaration
					if(!SrcAnlzCodeCParser_consumeStructDeclaration(state, srcScope, dstCode, iFirstChild, iDeclScope)){
						r = FALSE; NBASSERT(FALSE)
					}
					break;
				case ENSrzAnlzSintxC_declaration:
					//Declaration
					if(!SrcAnlzCodeCParser_consumeDeclaration(state, srcScope, dstCode, iFirstChild, iDeclScope)){
						r = FALSE; NBASSERT(FALSE)
					}
					break;
				case ENSrzAnlzSintxC_function_definition:
					//Function definition
					if(!SrcAnlzCodeCParser_consumeFunctionDefinition(state, srcScope, dstCode, iFirstChild, iDeclScope)){
						r = FALSE; NBASSERT(FALSE)
					}
					break;
				case ENSrzAnlzSintxC_block_item:
					//Block item (declaration or statement)
					if(child->iChildSeq != 0){
						if(!SrcAnlzCodeCParser_consumeBlockItem(state, srcScope, dstCode, iFirstChild, iDeclScope)){
							r = FALSE; NBASSERT(FALSE)
						}
					}
					break;
				default:
					PRINTF_INFO("Posibility not processed: '%s'.\n", state->sintxDefs.defs[sintaxDefSeq->iDefEnum].name);
					break;
			}
		}
	}
	return r;
}

//, { "block-item", "declaration" }
//, { "block-item", "statement" }
BOOL SrcAnlzCodeCParser_consumeBlockItem(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			do {
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					//Process
					if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						if(partDef->idx == ENSrzAnlzSintxC_declaration){
							if(child->iChildSeq != 0){
								if(!SrcAnlzCodeCParser_consumeDeclaration(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						} else if(partDef->idx == ENSrzAnlzSintxC_statement){
							if(child->iChildSeq != 0){
								if(!SrcAnlzCodeCParser_consumeStatement(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						} else {
							r = FALSE; NBASSERT(FALSE)
							break;
						}
					}
					//Next
					if(child->iNextPart == 0){
						break;
					} else {
						iChild	= child->iNextPart;
						child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
						iPart++;
					}
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
		}
	}
	return r;
}

//---------------------------
// Process common
//---------------------------

//, { "specifier-qualifier-list", "type-specifier" }
//, { "specifier-qualifier-list", "type-specifier specifier-qualifier-list" }
//, { "specifier-qualifier-list", "type-qualifier" }
//, { "specifier-qualifier-list", "type-qualifier specifier-qualifier-list" }
BOOL SrcAnlzCodeCParser_consumeSpecifierQualifierList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, STSrcAnlzCodeCDeclarType* dstType, UI32* dstSubmask){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI32 iPart = 0, parentMask = 0, thisMask = 0; if(dstType != NULL) parentMask = dstType->specifiersMask;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			do {
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					//Process
					if(partDef->type == ENSrzAnlzSintxPartType_Keyword){
						NBASSERT(child->iChildSeq == 0)
						NBASSERT(partDef->idx < state->sintxDefs.keywsSz)
						const char* keyword = state->sintxDefs.keyws[partDef->idx];
						//Validate token = keyword
						NBASSERT(child->iValue < state->acumTokensCur->defs.use) //token's value
						NBASSERT(NBString_strIsEqual(keyword, &state->acumTokensCur->strBuff.str[NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, child->iValue)->iAtBuff])) //token value is keyword?
						//TypeSpecifier
						if(NBString_strIsEqual(keyword, "void"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Void;
						else if(NBString_strIsEqual(keyword, "char"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Char;
						else if(NBString_strIsEqual(keyword, "short"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Short;
						else if(NBString_strIsEqual(keyword, "int"))		thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Int;
						else if(NBString_strIsEqual(keyword, "long"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Long;
						else if(NBString_strIsEqual(keyword, "float"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Float;
						else if(NBString_strIsEqual(keyword, "double"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Double;
						else if(NBString_strIsEqual(keyword, "signed"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Signed;
						else if(NBString_strIsEqual(keyword, "unsigned")) thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Unsigned;
						else if(NBString_strIsEqual(keyword, "_Bool"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS__Bool;
						else if(NBString_strIsEqual(keyword, "_Complex")) thisMask |= ENSrcAnlzDecSpecifiersBit_TS__Complex;
						//"struct-or-union-specifier" (processed below as a 'SintaxDef')
						//"enum-specifier" (processed below as a 'SintaxDef')
						//"typedef-name" (processed below as a 'SintaxDef')
						//TypeQualifier
						else if(NBString_strIsEqual(keyword, "const"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TQ_Const;
						else if(NBString_strIsEqual(keyword, "restrict")) thisMask |= ENSrcAnlzDecSpecifiersBit_TQ_Restrict;
						else if(NBString_strIsEqual(keyword, "volatile")) thisMask |= ENSrcAnlzDecSpecifiersBit_TQ_Volatile;
						//
						else {
							PRINTF_ERROR("Unexpected keyword '%s' in specifierQualifierList's mask\n", keyword);
							r = FALSE; NBASSERT(FALSE)
							break;
						}
					} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						if(partDef->idx == ENSrzAnlzSintxC_struct_or_union_specifier){
							if(((parentMask | thisMask) & ENSrcAnlzDecSpecifiersBits_TS_ALL) != 0){
								//Type was already specified
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								//Add or search structUnion definition
								UI32 iStructUnionDef = 0;
								if(!SrcAnlzCodeCParser_consumeStructOrUnionSpecifier(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &iStructUnionDef, FALSE)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									thisMask |= ENSrcAnlzDecSpecifiersBit_TS_StructUnion;
									if(dstType != NULL){
										if(dstType->iCustomTypeDef != 0){
											//A custom type def was already set (struct, enum, typedef, ...)
											r = FALSE; NBASSERT(FALSE)
											break;
										} else {
											dstType->iCustomTypeDef = iStructUnionDef;
										}
									}
								}
							}
						} else if(partDef->idx == ENSrzAnlzSintxC_enum_specifier){
							if(((parentMask | thisMask) & ENSrcAnlzDecSpecifiersBits_TS_ALL) != 0){
								//Type was already specified
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								//Add or search enum definition
								UI32 iEnumDef = 0;
								if(!SrcAnlzCodeCParser_consumeEnumSpecifier(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &iEnumDef, FALSE)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									NBASSERT(iEnumDef > 0)
									thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Enum;
									if(dstType != NULL){
										if(dstType->iCustomTypeDef != 0){
											//A custom type def was already set (struct, enum, typedef, ...)
											r = FALSE; NBASSERT(FALSE)
											break;
										} else {
											dstType->iCustomTypeDef = iEnumDef;
										}
									}
								}
							}
						} else if(partDef->idx == ENSrzAnlzSintxC_typedef_name){
							if(((parentMask | thisMask) & ENSrcAnlzDecSpecifiersBits_TS_ALL) != 0){
								//Type was already specified
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								//Search typedef definition
								thisMask |= ENSrcAnlzDecSpecifiersBit_TS_TypedefName;
								if(dstType != NULL){
									if(dstType->iCustomTypeDef != 0){
										//A custom type def was already set (struct, enum, typedef, ...)
										r = FALSE; NBASSERT(FALSE)
										break;
									} else if(child->iChildSeq == 0){
										//No child nodes
										r = FALSE; NBASSERT(FALSE)
										break;
									} else {
										//Search typedef definition
										UI32 iDeclrDef = 0;
										if(!SrcAnlzCodeCParser_consumeTypedefName(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &iDeclrDef)){
											r = FALSE; NBASSERT(FALSE)
											break;
										} else if(iDeclrDef == 0){
											r = FALSE; NBASSERT(FALSE)
											break;
										} else {
											dstType->iCustomTypeDef = iDeclrDef;
										}
									}
								}
							}
						} else {
							NBASSERT(partDef->idx == ENSrzAnlzSintxC_specifier_qualifier_list || partDef->idx == ENSrzAnlzSintxC_type_specifier || partDef->idx == ENSrzAnlzSintxC_type_qualifier)
							//Process childs
							if(child->iChildSeq != 0){
								NBASSERT(partDef->type == ENSrzAnlzSintxPartType_SintaxDef)
								UI32 submask = 0;
								if(!SrcAnlzCodeCParser_consumeSpecifierQualifierList(state, srcScope, dstCode, child->iChildSeq, iDeclScope, dstType, &submask)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									//Merge masks
									thisMask |= submask;
									//LongLong
									if((thisMask & ENSrcAnlzDecSpecifiersBit_TS_Long) != 0 && (submask & ENSrcAnlzDecSpecifiersBit_TS_Long)){
										thisMask |= ENSrcAnlzDecSpecifiersBit_TS_LongLong;
									}
								}
							}
						}
					} else {
						PRINTF_INFO("Unexpected partType in 'SpecifierQualifierList'.\n");
						r = FALSE; NBASSERT(FALSE)
						break;
					}
					//Next
					if(child->iNextPart == 0){
						break;
					} else {
						iChild	= child->iNextPart;
						child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
						iPart++;
					}
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
			if(dstSubmask != NULL) *dstSubmask = thisMask;
			if(dstType != NULL) dstType->specifiersMask |= thisMask;
		}
	}
	return r;
}

//, { "declaration-specifiers", "storage-class-specifier" }
//, { "declaration-specifiers", "storage-class-specifier declaration-specifiers" }
//, { "declaration-specifiers", "type-specifier" }
//, { "declaration-specifiers", "type-specifier declaration-specifiers" }
//, { "declaration-specifiers", "type-qualifier" }
//, { "declaration-specifiers", "type-qualifier declaration-specifiers" }
//, { "declaration-specifiers", "function-specifier" }
//, { "declaration-specifiers", "function-specifier declaration-specifiers" }
//, { "storage-class-specifier", "typedef" }
//, { "storage-class-specifier", "extern" }
//, { "storage-class-specifier", "static" }
//, { "storage-class-specifier", "auto" }
//, { "storage-class-specifier", "register" }
//, { "type-specifier", "void" }
//, { "type-specifier", "char" }
//, { "type-specifier", "short" }
//, { "type-specifier", "int" }
//, { "type-specifier", "long" }
//, { "type-specifier", "float" }
//, { "type-specifier", "double" }
//, { "type-specifier", "signed" }
//, { "type-specifier", "unsigned" }
//, { "type-specifier", "_Bool" }
//, { "type-specifier", "_Complex" }
//, { "type-specifier", "struct-or-union-specifier" }
//, { "type-specifier", "enum-specifier" }
//, { "type-specifier", "typedef-name" }
//, { "type-qualifier", "const" }
//, { "type-qualifier", "restrict" }
//, { "type-qualifier", "volatile" }
//, { "function-specifier", "inline" }
BOOL SrcAnlzCodeCParser_consumeDeclarationSpecifiers(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, STSrcAnlzCodeCDeclarType* dstType, UI32* dstSubmask, const BOOL forceNewUsrTypes){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI32 iPart = 0, parentMask = 0, thisMask = 0; if(dstType != NULL) parentMask = dstType->specifiersMask;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			do {
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					//Process
					if(partDef->type == ENSrzAnlzSintxPartType_Keyword){
						NBASSERT(child->iChildSeq == 0)
						NBASSERT(partDef->idx < state->sintxDefs.keywsSz)
						const char* keyword = state->sintxDefs.keyws[partDef->idx];
						//PRINTF_INFO("DeclarationSpecifiers: '%s'.\n", keyword);
						//Validate token = keyword
						NBASSERT(child->iValue < state->acumTokensCur->defs.use) //token's value
						NBASSERT(NBString_strIsEqual(keyword, &state->acumTokensCur->strBuff.str[NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, child->iValue)->iAtBuff])) //token value is keyword?
						//StorageClassSpecifier
						if(NBString_strIsEqual(keyword, "typedef"))		thisMask |= ENSrcAnlzDecSpecifiersBit_SC_Typedef;
						else if(NBString_strIsEqual(keyword, "extern"))	thisMask |= ENSrcAnlzDecSpecifiersBit_SC_Extern;
						else if(NBString_strIsEqual(keyword, "static"))	thisMask |= ENSrcAnlzDecSpecifiersBit_SC_Static;
						else if(NBString_strIsEqual(keyword, "auto"))	thisMask |= ENSrcAnlzDecSpecifiersBit_SC_Auto;
						else if(NBString_strIsEqual(keyword, "register")) thisMask |= ENSrcAnlzDecSpecifiersBit_SC_Register;
						//TypeSpecifier
						else if(NBString_strIsEqual(keyword, "void"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Void;
						else if(NBString_strIsEqual(keyword, "char"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Char;
						else if(NBString_strIsEqual(keyword, "short"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Short;
						else if(NBString_strIsEqual(keyword, "int"))		thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Int;
						else if(NBString_strIsEqual(keyword, "long"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Long;
						else if(NBString_strIsEqual(keyword, "float"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Float;
						else if(NBString_strIsEqual(keyword, "double"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Double;
						else if(NBString_strIsEqual(keyword, "signed"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Signed;
						else if(NBString_strIsEqual(keyword, "unsigned")) thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Unsigned;
						else if(NBString_strIsEqual(keyword, "_Bool"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TS__Bool;
						else if(NBString_strIsEqual(keyword, "_Complex")) thisMask |= ENSrcAnlzDecSpecifiersBit_TS__Complex;
						//"struct-or-union-specifier" (processed below as a 'SintaxDef')
						//"enum-specifier" (processed below as a 'SintaxDef')
						//"typedef-name" (processed below as a 'SintaxDef')
						//TypeQualifier
						else if(NBString_strIsEqual(keyword, "const"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TQ_Const;
						else if(NBString_strIsEqual(keyword, "restrict")) thisMask |= ENSrcAnlzDecSpecifiersBit_TQ_Restrict;
						else if(NBString_strIsEqual(keyword, "volatile")) thisMask |= ENSrcAnlzDecSpecifiersBit_TQ_Volatile;
						//FunctionSpecifier
						else if(NBString_strIsEqual(keyword, "inline"))	thisMask |= ENSrcAnlzDecSpecifiersBit_FS_inline;
						//
						else {
							PRINTF_ERROR("Unexpected keyword '%s' in specifiers's mask\n", keyword);
							r = FALSE; NBASSERT(FALSE)
							break;
						}
					} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						//PRINTF_INFO("DeclarationSpecifiers: '%s'.\n", state->sintxDefs.defs[partDef->idx].name);
						if(partDef->idx == ENSrzAnlzSintxC_struct_or_union_specifier){
							if(((parentMask | thisMask) & ENSrcAnlzDecSpecifiersBits_TS_ALL) != 0){
								//Type was already specified
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								//Add or search structUnion definition
								UI32 iStructUnionDef = 0;
								if(!SrcAnlzCodeCParser_consumeStructOrUnionSpecifier(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &iStructUnionDef, forceNewUsrTypes)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									thisMask |= ENSrcAnlzDecSpecifiersBit_TS_StructUnion;
									if(dstType != NULL){
										if(dstType->iCustomTypeDef != 0){
											//A custom type def was already set (struct, enum, typedef, ...)
											r = FALSE; NBASSERT(FALSE)
											break;
										} else {
											dstType->iCustomTypeDef = iStructUnionDef;
										}
									}
								}
							}
						} else if(partDef->idx == ENSrzAnlzSintxC_enum_specifier){
							if(((parentMask | thisMask) & ENSrcAnlzDecSpecifiersBits_TS_ALL) != 0){
								//Type was already specified
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								//Add or search enum definition
								UI32 iEnumDef = 0;
								if(!SrcAnlzCodeCParser_consumeEnumSpecifier(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &iEnumDef, forceNewUsrTypes)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									NBASSERT(iEnumDef > 0)
									thisMask |= ENSrcAnlzDecSpecifiersBit_TS_Enum;
									if(dstType != NULL){
										if(dstType->iCustomTypeDef != 0){
											//A custom type def was already set (struct, enum, typedef, ...)
											r = FALSE; NBASSERT(FALSE)
											break;
										} else {
											dstType->iCustomTypeDef = iEnumDef;
										}
									}
								}
							}
						} else if(partDef->idx == ENSrzAnlzSintxC_typedef_name){
							if(((parentMask | thisMask) & ENSrcAnlzDecSpecifiersBits_TS_ALL) != 0){
								//Type was already specified
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								//Search typedef definition
								thisMask |= ENSrcAnlzDecSpecifiersBit_TS_TypedefName;
								if(dstType != NULL){
									if(dstType->iCustomTypeDef != 0){
										//A custom type def was already set (struct, enum, typedef, ...)
										r = FALSE; NBASSERT(FALSE)
										break;
									} else if(child->iChildSeq == 0){
										//No child nodes
										r = FALSE; NBASSERT(FALSE)
										break;
									} else {
										//Search typedef definition
										UI32 iDeclrDef = 0;
										if(!SrcAnlzCodeCParser_consumeTypedefName(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &iDeclrDef)){
											r = FALSE; NBASSERT(FALSE)
											break;
										} else if(iDeclrDef == 0){
											r = FALSE; NBASSERT(FALSE)
											break;
										} else {
											dstType->iCustomTypeDef = iDeclrDef;
										}
									}
								}
							}
						} else {
							NBASSERT(partDef->idx == ENSrzAnlzSintxC_declaration_specifiers || partDef->idx == ENSrzAnlzSintxC_storage_class_specifier || partDef->idx == ENSrzAnlzSintxC_type_specifier || partDef->idx == ENSrzAnlzSintxC_type_qualifier || partDef->idx == ENSrzAnlzSintxC_function_specifier)
							//Process childs
							if(child->iChildSeq != 0){
								NBASSERT(partDef->type == ENSrzAnlzSintxPartType_SintaxDef)
								UI32 submask = 0;
								if(!SrcAnlzCodeCParser_consumeDeclarationSpecifiers(state, srcScope, dstCode, child->iChildSeq, iDeclScope, dstType, &submask, forceNewUsrTypes)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									//Merge masks
									thisMask |= submask;
									//LongLong
									if((thisMask & ENSrcAnlzDecSpecifiersBit_TS_Long) != 0 && (submask & ENSrcAnlzDecSpecifiersBit_TS_Long)){
										thisMask |= ENSrcAnlzDecSpecifiersBit_TS_LongLong;
									}
								}
							}
						}
					} else {
						PRINTF_INFO("Unexpected partType in 'DeclarationSpecifiers'.\n");
						r = FALSE; NBASSERT(FALSE)
						break;
					}
					//Next
					if(child->iNextPart == 0){
						break;
					} else {
						iChild	= child->iNextPart;
						child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
						iPart++;
					}
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
			if(dstSubmask != NULL) *dstSubmask = thisMask;
			if(dstType != NULL) dstType->specifiersMask |= thisMask;
		}
	}
	return r;
}

//, { "type-qualifier-list", "type-qualifier" }
//, { "type-qualifier-list", "type-qualifier-list type-qualifier" }
//, { "type-qualifier", "const" }
//, { "type-qualifier", "restrict" }
//, { "type-qualifier", "volatile" }
BOOL SrcAnlzCodeCParser_consumeTypeQualifierList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, UI32* dstSubmask){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI32 iPart = 0, thisMask = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			do {
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					//Process
					if(partDef->type == ENSrzAnlzSintxPartType_Keyword){
						NBASSERT(child->iChildSeq == 0)
						NBASSERT(partDef->idx < state->sintxDefs.keywsSz)
						const char* keyword = state->sintxDefs.keyws[partDef->idx];
						//Validate token = keyword
						NBASSERT(child->iValue < state->acumTokensCur->defs.use) //token's value
						NBASSERT(NBString_strIsEqual(keyword, &state->acumTokensCur->strBuff.str[NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, child->iValue)->iAtBuff])) //token value is keyword?
						//TypeQualifier
						if(NBString_strIsEqual(keyword, "const"))	thisMask |= ENSrcAnlzDecSpecifiersBit_TQ_Const;
						else if(NBString_strIsEqual(keyword, "restrict")) thisMask |= ENSrcAnlzDecSpecifiersBit_TQ_Restrict;
						else if(NBString_strIsEqual(keyword, "volatile")) thisMask |= ENSrcAnlzDecSpecifiersBit_TQ_Volatile;
						//
						else {
							PRINTF_ERROR("Unexpected keyword '%s' in type-quelifiers-list mask\n", keyword);
							r = FALSE; NBASSERT(FALSE)
							break;
						}
					} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						NBASSERT(partDef->idx == ENSrzAnlzSintxC_type_qualifier_list || partDef->idx == ENSrzAnlzSintxC_type_qualifier)
						if(partDef->idx == ENSrzAnlzSintxC_type_qualifier_list || partDef->idx == ENSrzAnlzSintxC_type_qualifier){
							if(child->iChildSeq != 0){
								UI32 submask = 0;
								if(!SrcAnlzCodeCParser_consumeTypeQualifierList(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &submask)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									//Merge masks
									thisMask |= submask;
								}
							}
						}
					}
					//Next
					if(child->iNextPart == 0){
						break;
					} else {
						iChild	= child->iNextPart;
						child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
						iPart++;
					}
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
			if(dstSubmask != NULL) *dstSubmask = thisMask;
		}
	}
	return r;
}

//, { "function-definition", "declaration-specifiers declarator compound-statement" }
//, { "function-definition", "declaration-specifiers declarator declaration-list compound-statement" } //Todo: implement this sintax definition
BOOL SrcAnlzCodeCParser_consumeFunctionDefinition(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_function_definition)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_function_definition){
				r = TRUE;
				//
				const STSrcAnlzSintxParseTreeNode* decSpecsNode = NULL;
				const STSrcAnlzSintxParseTreeNode* declrtorNode = NULL;
				const STSrcAnlzSintxParseTreeNode* decListNode = NULL;
				const STSrcAnlzSintxParseTreeNode* bodyNode = NULL;
				//
				UI32 iPart = 0;
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if((partDef->type == ENSrzAnlzSintxPartType_SintaxDef)){
							if(partDef->idx == ENSrzAnlzSintxC_declaration_specifiers){
								//Declaration base-type
								NBASSERT(decSpecsNode == NULL)
								decSpecsNode = child;
							} else if(partDef->idx == ENSrzAnlzSintxC_declarator){
								//Declarator
								NBASSERT(declrtorNode == NULL)
								declrtorNode = child;
							} else if(partDef->idx == ENSrzAnlzSintxC_declaration_list){
								//Declaration list
								NBASSERT(decListNode == NULL)
								decListNode = child;
							} else if(partDef->idx == ENSrzAnlzSintxC_compound_statement){
								//Compound statement
								NBASSERT(bodyNode == NULL)
								bodyNode = child;
							} else {
								//Unexpected sintax def in declaration
								r = FALSE; NBASSERT(FALSE)
								break;
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
				//
				//Process declaration with no "declarator-list"
				//
				if(r){
					if(decSpecsNode == NULL || declrtorNode == NULL || bodyNode == NULL){
						r = FALSE; NBASSERT(FALSE)
					} else {
						if(decListNode != NULL){
							//, { "function-definition", "declaration-specifiers declarator declaration-list compound-statement" } //Todo: implement this sintax definition
							r = FALSE; NBASSERT(FALSE)
						} else {
							//, { "function-definition", "declaration-specifiers declarator compound-statement" }
							if(decSpecsNode->iChildSeq == 0 || declrtorNode->iChildSeq == 0 || (bodyNode->iChildSeq == 0 && bodyNode->iValue <= 0)){
								r = FALSE; NBASSERT(FALSE)
							} else {
								//Search for declarator identifier (name)
								const char* funcName = NULL;
								if(!SrcAnlzCodeCParser_findDeclaratorIdentifierNode(state, srcScope, declrtorNode->iChildSeq, &funcName)){
									r = FALSE; NBASSERT(FALSE)
								} else if(funcName == NULL){
									r = FALSE; NBASSERT(FALSE) //No identifier
								} else if(funcName[0] == '\0'){
									r = FALSE; NBASSERT(FALSE) //Empty identifier
								} else {
									STSrcAnlzCodeCDeclarType typeBase;
									typeBase.specifiersMask		= 0;
									typeBase.iCustomTypeDef		= 0;
									typeBase.ptrSpecsRng.start	= 0;
									typeBase.ptrSpecsRng.count	= 0;
									typeBase.isArray			= FALSE;
									typeBase.arrayTotalSz		= 0;
									typeBase.paramsBodyDefIdx	= 0;
									typeBase.paramsLenFlexible	= FALSE;
									if(!SrcAnlzCodeCParser_consumeDeclarationSpecifiers(state, srcScope, dstCode, decSpecsNode->iChildSeq, iDeclScope, &typeBase, NULL, FALSE)){
										r = FALSE; NBASSERT(FALSE)
									} else {
										UI32 iDeclarDef = 0;
										NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
										if(!SrcAnlzCodeC_findDeclarByName(dstCode, iDeclScope, FALSE, funcName, (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count), &iDeclarDef)){
											r = FALSE; NBASSERT(FALSE)
										} else {
											const STSrcAnlzCodeCDeclarsScope* scope = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope);
											//Compare with existing declaration.
											if(iDeclarDef != 0){
												const STSrcAnlzCodeCDeclar* declar = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
												NBASSERT(declar->iDeepLvl == scope->iDeepLvl) //Program logic error ('SrcAnlzCodeC_findDeclarByName' should filter only same level declars)
												//ToDo: compare previous declaration and this declaration signatures
												//ToDo: set previous declaration params name as this declaration.
												//PRINTF_INFO("Function was previous declared @%d.\n", iDeclarDef);
											}
											//Add new declaration
											if(r && iDeclarDef == 0){
												//Determine if this declaration was already added
												iDeclarDef = (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count);
												STSrcAnlzCodeCDeclar* declarDef = NULL; BOOL isNewDef = FALSE;
												if(!SrcAnlzCodeC_getDeclarByIdx(dstCode, iDeclarDef, iDeclScope, &declarDef, &isNewDef)){
													r = FALSE; NBASSERT(FALSE)
												} else {
													if(isNewDef){
														//Declar was added at this call
														declarDef->type = typeBase;
													} else {
														//Declar was added at prev call
														NBASSERT((declarDef->type.specifiersMask & typeBase.specifiersMask) == typeBase.specifiersMask) //Must match the base specs
													}
													srcScope->stats.declarsRng.count++;
													NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
													//PRINTF_INFO("Function was not previous declared @%d.\n", iDeclarDef);
													//Process declarator
													if(!SrcAnlzCodeCParser_consumeDeclarator(state, srcScope, dstCode, declrtorNode->iChildSeq, iDeclScope, iDeclarDef)){
														r = FALSE; NBASSERT(FALSE)
													}
												}
											}
											//Link with function's body
											if(r){
												NBASSERT(iDeclarDef != 0)
												//, { "compound-statement", "{ }" }
												//, { "compound-statement", "{ block-item-list }" }
												if(bodyNode->iValue > 0){
													//Body already defined
													STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
													NBASSERT(bodyNode->iValue > 0 && bodyNode->iValue < dstCode->defsDeclarsScopes.use)
													NBASSERT(bodyNode->iValue == (srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count))
													NBASSERT(data->iBodyDef == 0 || data->iBodyDef == bodyNode->iValue) //New or same value
													data->iBodyDef			 	= bodyNode->iValue;
													const STSrcAnlzCodeCDeclarsScope* funcBodyDef = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, data->iBodyDef);
													//PRINTF_INFO("function's members already stored in definition %d, rangeUsrTypes(%d, +%d), rangeDeclars(%d, +%d), rangeSubscopes(%d, +%d).\n", data->iBodyDef, funcBodyDef->usrTypesRng.start, funcBodyDef->usrTypesRng.count, funcBodyDef->declarsRng.start, funcBodyDef->declarsRng.count, funcBodyDef->scopesRng.start, funcBodyDef->scopesRng.count);
													NBASSERT(funcBodyDef->iParentScope == iDeclScope)
													NBASSERT(funcBodyDef->iDeepLvl == (NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope)->iDeepLvl + 1))
													srcScope->stats.usrTypesRng.count	+= funcBodyDef->usrTypesRng.count;
													srcScope->stats.declarsRng.count	+= funcBodyDef->declarsRng.count;
													srcScope->stats.scopesRng.count		+= funcBodyDef->scopesRng.count + 1; //+1, the subscope itself
													NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
													NBASSERT((srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count) <= dstCode->defsUsrTypes.use)
													NBASSERT((srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count) <= dstCode->defsDeclarsScopes.use)
												} else {
													//Analyze body node
													BOOL bodyIsComplete = FALSE;
													const STSrcAnlzSintxParseTreeNode* blockItmLstNode = NULL;
													{
														const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, bodyNode->iChildSeq);
														const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
														NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_compound_statement)
														if(sintaxDefSeq->iDefEnum != ENSrzAnlzSintxC_compound_statement){
															r = FALSE; NBASSERT(FALSE)
														} else {
															if(sintaxDefSeq->partsSz == 2){
																NBASSERT(sintaxDefSeq->parts[0].type == ENSrzAnlzSintxPartType_Punctuator) //'{'
																NBASSERT(sintaxDefSeq->parts[1].type == ENSrzAnlzSintxPartType_Punctuator) //'}'
																if(child->iNextPart != 0){
																	bodyIsComplete = TRUE;
																}
															} else if(sintaxDefSeq->partsSz == 3){
																NBASSERT(sintaxDefSeq->parts[0].type == ENSrzAnlzSintxPartType_Punctuator) //'{'
																NBASSERT(sintaxDefSeq->parts[1].type == ENSrzAnlzSintxPartType_SintaxDef) //'block-item-list'
																NBASSERT(sintaxDefSeq->parts[2].type == ENSrzAnlzSintxPartType_Punctuator) //'}'
																if(child->iNextPart != 0){
																	child = blockItmLstNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, child->iNextPart);
																	if(child->iNextPart != 0){
																		bodyIsComplete = TRUE;
																	}
																}
															} else {
																r = FALSE; NBASSERT(FALSE)
															}
														}
													}
													if(r && bodyIsComplete){
														STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
														if(blockItmLstNode == NULL){
															//The function's body is empty
															const UI32 iBodyDef = srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count;
															STSrcAnlzCodeCDeclarsScope* scopeDef = NULL; BOOL isNewDef = FALSE;
															if(!SrcAnlzCodeC_getDeclarScopeByIdx(dstCode, iBodyDef, iDeclScope, &scopeDef, &isNewDef)){
																r = FALSE; NBASSERT(FALSE)
															} else {
																if(isNewDef){
																	//Declar was added at this call
																	NBASSERT(data->iBodyDef == 0)
																	data->iBodyDef = iBodyDef;
																} else {
																	//Declar was added at prev call
																	NBASSERT(data->iBodyDef == iBodyDef)
																}
																srcScope->stats.scopesRng.count++;
																NBASSERT((srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count) <= dstCode->defsDeclarsScopes.use)
															}
														} else {
															//The function's body is not-empty
															if(blockItmLstNode->iValue == 0){
																r = FALSE; NBASSERT(FALSE)
															} else {
																NBASSERT(blockItmLstNode->iValue > 0 && blockItmLstNode->iValue < dstCode->defsDeclarsScopes.use)
																NBASSERT(blockItmLstNode->iValue == (srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count))
																NBASSERT(data->iBodyDef == 0 || data->iBodyDef == blockItmLstNode->iValue) //New or same value
																data->iBodyDef			 	= blockItmLstNode->iValue;
																const STSrcAnlzCodeCDeclarsScope* funcBodyDef = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, data->iBodyDef);
																//PRINTF_INFO("function's members already stored in definition %d, rangeUsrTypes(%d, +%d), rangeDeclars(%d, +%d), rangeSubscopes(%d, +%d).\n", data->iBodyDef, funcBodyDef->usrTypesRng.start, funcBodyDef->usrTypesRng.count, funcBodyDef->declarsRng.start, funcBodyDef->declarsRng.count, funcBodyDef->scopesRng.start, funcBodyDef->scopesRng.count);
																NBASSERT(funcBodyDef->iParentScope == iDeclScope)
																NBASSERT(funcBodyDef->iDeepLvl == (NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope)->iDeepLvl + 1))
																srcScope->stats.usrTypesRng.count	+= funcBodyDef->usrTypesRng.count;
																srcScope->stats.declarsRng.count	+= funcBodyDef->declarsRng.count;
																srcScope->stats.scopesRng.count		+= funcBodyDef->scopesRng.count + 1; //+1, the subscope itself
																NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
																NBASSERT((srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count) <= dstCode->defsUsrTypes.use)
																NBASSERT((srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count) <= dstCode->defsDeclarsScopes.use)
															}
														}
														srcScope->stats.iLastActiveDecScope = iDeclScope;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return r;
}

//, { "declaration", "declaration-specifiers ;" }
//, { "declaration", "declaration-specifiers init-declarator-list ;" }
BOOL SrcAnlzCodeCParser_consumeDeclaration(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_declaration)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_declaration){
				r = TRUE;
				//
				BOOL hasDeclaratorList		= (sintaxDefSeq->partsSz > 2);
				BOOL typeFound				= FALSE;
				BOOL listFound				= FALSE;
				BOOL endFound				= FALSE;
				STSrcAnlzCodeCDeclarType typeBase;
				typeBase.specifiersMask		= 0;
				typeBase.iCustomTypeDef		= 0;
				typeBase.ptrSpecsRng.start	= 0;
				typeBase.ptrSpecsRng.count	= 0;
				typeBase.isArray			= FALSE;
				typeBase.arrayTotalSz		= 0;
				typeBase.paramsBodyDefIdx	= 0;
				typeBase.paramsLenFlexible	= FALSE;
				//
				UI32 iPart = 0;
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_Punctuator){
							NBASSERT(child->iChildSeq == 0)
							NBASSERT(partDef->idx < state->sintxDefs.punctsSz)
							const char* punctuator = state->sintxDefs.puncts[partDef->idx];
							//PRINTF_INFO("Punctuator '%s'\n", punctuator);
							if(NBString_strIsEqual(punctuator, ";")){
								endFound = TRUE;
							}
						} else if((partDef->type == ENSrzAnlzSintxPartType_SintaxDef)){
							if(partDef->idx == ENSrzAnlzSintxC_declaration_specifiers){
								//Declaration base-type
								NBASSERT(typeBase.specifiersMask == 0)
								NBASSERT(child->iChildSeq != 0)
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeDeclarationSpecifiers(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &typeBase, NULL, (!hasDeclaratorList))){
										r = FALSE; NBASSERT(FALSE)
										break;
									} else {
										NBASSERT(typeBase.specifiersMask != 0)
										typeFound = TRUE;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_init_declarator_list){
								//Declaration name's list (with derived types such as pointer, type qualifiers and params)
								NBASSERT(child->iChildSeq != 0)
								if(child->iChildSeq != 0){
									NBASSERT(typeFound) //Type must be found before
									if(!SrcAnlzCodeCParser_consumeInitDeclaratorList(state, srcScope, dstCode, child->iChildSeq, iDeclScope, typeBase)){
										r = FALSE; NBASSERT(FALSE)
										break;
									} else {
										listFound = TRUE;
									}
								}
							} else {
								//Unexpected sintax def in declaration
								r = FALSE; NBASSERT(FALSE)
								break;
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
				//
				//Process declaration with no "declarator-list"
				//
				if(r && typeFound && !listFound && endFound){
					//Determine if this declaration was already added
					const UI32 iDeclarDef = (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count);
					STSrcAnlzCodeCDeclar* declarDef = NULL; BOOL isNewDef = FALSE;
					if(!SrcAnlzCodeC_getDeclarByIdx(dstCode, iDeclarDef, iDeclScope, &declarDef, &isNewDef)){
						r = FALSE; NBASSERT(FALSE)
					} else {
						if(isNewDef){
							//Declar was added at this call
							declarDef->type = typeBase;
						} else {
							//Declar was added at prev call
							NBASSERT((declarDef->type.specifiersMask & typeBase.specifiersMask) == typeBase.specifiersMask) //Must match the base specs
						}
						srcScope->stats.declarsRng.count++;
						NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
					}
				}
			}
		}
	}
	return r;
}

//, { "init-declarator-list", "init-declarator" }
//, { "init-declarator-list", "init-declarator-list , init-declarator" }
//, { "init-declarator", "declarator" }
//, { "init-declarator", "declarator = initializer" }
BOOL SrcAnlzCodeCParser_consumeInitDeclaratorList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const STSrcAnlzCodeCDeclarType typeBase){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			do {
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					//Process
					if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						NBASSERT(partDef->idx == ENSrzAnlzSintxC_init_declarator_list || partDef->idx == ENSrzAnlzSintxC_init_declarator || partDef->idx == ENSrzAnlzSintxC_declarator || partDef->idx == ENSrzAnlzSintxC_initializer)
						if(partDef->idx == ENSrzAnlzSintxC_declarator){
							if(child->iChildSeq != 0){
								//Determine if this declaration was already added
								const UI32 iDeclarDef = (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count);
								STSrcAnlzCodeCDeclar* declarDef = NULL; BOOL isNewDef = FALSE;
								if(!SrcAnlzCodeC_getDeclarByIdx(dstCode, iDeclarDef, iDeclScope, &declarDef, &isNewDef)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									if(isNewDef){
										//Declar was added at this call
										declarDef->type = typeBase;
									} else {
										//Declar was added at prev call
										NBASSERT((declarDef->type.specifiersMask & typeBase.specifiersMask) == typeBase.specifiersMask) //Must match the base specs
									}
									srcScope->stats.declarsRng.count++;
									NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
									//Process declarator
									if(!SrcAnlzCodeCParser_consumeDeclarator(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iDeclarDef)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							}
						} else if(partDef->idx == ENSrzAnlzSintxC_initializer){
							//Todo: consume references
						} else {
							//Process childs
							if(child->iChildSeq != 0){
								if(!SrcAnlzCodeCParser_consumeInitDeclaratorList(state, srcScope, dstCode, child->iChildSeq, iDeclScope, typeBase)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						}
					}
				}
				//Next
				if(child->iNextPart == 0){
					break;
				} else {
					iChild	= child->iNextPart;
					child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
					iPart++;
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
		}
	}
	return r;
}

//, { "declarator", "direct-declarator" }
//, { "declarator", "pointer direct-declarator" }
//, { "direct-declarator", "identifier" }
//, { "direct-declarator", "( declarator )" }
//, { "direct-declarator", "direct-declarator [ ]" }
//, { "direct-declarator", "direct-declarator [ assignment-expression ]" }
//, { "direct-declarator", "direct-declarator [ type-qualifier-list ]" }
//, { "direct-declarator", "direct-declarator [ type-qualifier-list assignment-expression ]" }
//, { "direct-declarator", "direct-declarator [ static assignment-expression ]" }
//, { "direct-declarator", "direct-declarator [ static type-qualifier-list assignment-expression ]" }
//, { "direct-declarator", "direct-declarator [ type-qualifier-list static assignment-expression ]" }
//, { "direct-declarator", "direct-declarator [ * ]" }
//, { "direct-declarator", "direct-declarator [ type-qualifier-list * ]" }
//, { "direct-declarator", "direct-declarator ( parameter-type-list )" }
//, { "direct-declarator", "direct-declarator ( )" }
//, { "direct-declarator", "direct-declarator ( identifier-list )" }
BOOL SrcAnlzCodeCParser_consumeDeclarator(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const UI32 iDeclarDef){
	BOOL r = FALSE;
	NBASSERT(iDeclarDef != 0)
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_declarator || sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_direct_declarator)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_declarator || sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_direct_declarator){
				r = TRUE;
				//
				BOOL arrayOpened = FALSE, paramsOpened = FALSE;
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_Identifier){
							//Set declarator's name
							NBASSERT(iDeclarDef > 0)
							NBASSERT(sintaxDefSeq->partsSz == 1) //only the identifier (expecting definition = { "direct-declarator", "identifier" } )
							STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
							NBASSERT(data->iDeepLvl == NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope)->iDeepLvl)
							if(data->iNameFirstChar == 0){ //Declaration first name
								NBASSERT(child->iValue < state->acumTokensCur->defs.use)
								const STSrcAnlzTokn* tokenDef = NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, child->iValue);
								const char* tokenStr	= &state->acumTokensCur->strBuff.str[tokenDef->iAtBuff];
								data->iNameFirstChar	= dstCode->strsBuff.length;
								NBString_concat(&dstCode->strsBuff, tokenStr);
								NBString_concatByte(&dstCode->strsBuff, '\0');
								PRINTF_INFO("DeclarsScope %d, declar(@%d/%d) setting name '%s'%s.\n", iDeclScope, iDeclarDef, srcScope->stats.declarsRng.count, tokenStr, (data->type.specifiersMask & ENSrcAnlzDecSpecifiersBit_SC_Typedef) != 0 ? " (typedef)" : "");
							}
						} else if(partDef->type == ENSrzAnlzSintxPartType_Punctuator){
							NBASSERT(child->iChildSeq == 0)
							NBASSERT(partDef->idx < state->sintxDefs.punctsSz)
							const char* punctuator = state->sintxDefs.puncts[partDef->idx];
							//PRINTF_INFO("Punctuator '%s'\n", punctuator);
							if(NBString_strIsEqual(punctuator, "[")){
								//Start array definition
								NBASSERT(iDeclarDef > 0)
								NBASSERT(!arrayOpened)
								STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
								arrayOpened = TRUE;
								data->type.isArray	= TRUE;
							} else if(NBString_strIsEqual(punctuator, "(")){
								if((iPart + 1) < sintaxDefSeq->partsSz){
									const STSrcAnlzSintxPart* nextPartDef = &sintaxDefSeq->parts[iPart + 1];
									if(nextPartDef->type != ENSrzAnlzSintxPartType_SintaxDef || nextPartDef->idx != ENSrzAnlzSintxC_declarator){ //Not "(" + "declarator"
										//Start params definitions
										NBASSERT(iDeclarDef > 0)
										NBASSERT(!paramsOpened)
										const STSrcAnlzCodeCDeclarsScope* scope = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope);
										STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
										NBASSERT(data->iDeepLvl == scope->iDeepLvl || (data->dbgIsTypeDef && data->iDeepLvl > scope->iDeepLvl))
										//Add params declarScope
										const UI32 iParamsScope = srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count;
										STSrcAnlzCodeCDeclarsScope* scopeDef = NULL; BOOL isNewDef = FALSE;
										if(!SrcAnlzCodeC_getDeclarScopeByIdx(dstCode, iParamsScope, iDeclScope, &scopeDef, &isNewDef)){
											r = FALSE; NBASSERT(FALSE)
											break;
										} else {
											if(isNewDef){
												//Declar was added at this call
												NBASSERT(data->type.paramsBodyDefIdx == 0)
												data->type.paramsBodyDefIdx = iParamsScope;
											} else {
												//Declar was added at prev call
												NBASSERT(data->type.paramsBodyDefIdx == iParamsScope)
											}
											srcScope->stats.scopesRng.count++;
											NBASSERT((srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count) <= dstCode->defsDeclarsScopes.use)
											paramsOpened = TRUE;
											srcScope->stats.iLastActiveDecScope = data->type.paramsBodyDefIdx;
										}
									}
								}
							} else if(NBString_strIsEqual(punctuator, ")")){
								if(paramsOpened){
									//End params definitions
									NBASSERT(iDeclarDef > 0)
									NBASSERT(NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef)->type.paramsBodyDefIdx != 0) //Params declaration must started before this point
									paramsOpened = FALSE;
									srcScope->stats.iLastActiveDecScope = iDeclScope;
									//PRINTF_INFO("Declarator %d closed with paramsRange(%d, +%d) level(%d).\n", iDeclarDef, data->type.paramsRng.start, data->type.paramsRng.count, (iDclrDeepLvl + 1));
								}
							} else if(NBString_strIsEqual(punctuator, "]")){
								if(arrayOpened){
									//End array definition
									NBASSERT(iDeclarDef > 0)
									arrayOpened = FALSE;
								}
							}
						} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							if(partDef->idx == ENSrzAnlzSintxC_parameter_type_list){
								//Add parameters type-list
								NBASSERT(iDeclarDef != 0)
								STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
								NBASSERT(paramsOpened && data->type.paramsBodyDefIdx != 0) //Params declaration must started before this point
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeDeclaratorParamTypeList(state, srcScope, dstCode, child->iChildSeq, data->type.paramsBodyDefIdx)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_identifier_list){
								//Add parameters identifier-list
								NBASSERT(paramsOpened)
								NBASSERT(iDeclarDef != 0)
								STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
								NBASSERT(paramsOpened && data->type.paramsBodyDefIdx != 0) //Params declaration must started before this point
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeDeclaratorParamIdentifierList(state, srcScope, dstCode, child->iChildSeq, data->type.paramsBodyDefIdx)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_pointer){
								NBASSERT(iDeclarDef != 0)
								STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
								//Pointer spec must be processed only once (to avoid unecesary array fragmentation)
								if(data->type.ptrSpecsRng.count == 0){
									if(child->iChildSeq != 0){
										if(!SrcAnlzCodeCParser_consumePointer(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iDeclarDef)){
											r = FALSE; NBASSERT(FALSE)
											break;
										}
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_assignment_expression || partDef->idx == ENSrzAnlzSintxC_type_qualifier_list){
								//Todo: add support to this
								NBASSERT(arrayOpened)
								//PRINTF_ERROR("Todo: 'declarator', add support to 'assignment_expression' and 'type_qualifier_list'.\n");
							} else if(partDef->idx == ENSrzAnlzSintxC_declarator){
								//-------------------
								//Sub declarator, a declar pointing to another declar
								//Examples:
								//declares 3 functions (normal, pointer returner, and a pointer to a function).
								//int f(void), *fip(), (*pfi)();
								//declares an array apfi of three pointers to functions returning int and having two parameters each.
								//int (*apfi[3])(int *x, int *y);
								//declares a function fpfi that returns a pointer to a function returning an int. The function fpfi has two parameters: a pointer to a function returning an int (with one parameter of type long int), and an int. The pointer returned by fpfi points to a function that has one int parameter and accepts zero or more additional arguments of any type.
								//int (*fpfi(int (*)(long), int))(int, ...);
								//-------------------
								//Process childs
								if(child->iChildSeq != 0){
									//Determine if this declaration was already added
									const UI32 iSubDeclarDef = (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count);
									STSrcAnlzCodeCDeclar* declarDef = NULL; BOOL isNewDef = FALSE;
									if(!SrcAnlzCodeC_getDeclarByIdx(dstCode, iSubDeclarDef, iDeclScope, &declarDef, &isNewDef)){
										r = FALSE; NBASSERT(FALSE)
										break;
									} else {
										NBASSERT(iDeclarDef > 0)
										STSrcAnlzCodeCDeclar* parentDeclar = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
										if(isNewDef){
											//Declar was added at this call
											parentDeclar->iDeepLvl++;		//Push parent declar one level deeper (is not part of the scope's declaration)
#											ifdef NB_CONFIG_INCLUDE_ASSERTS
											parentDeclar->dbgIsTypeDef		= TRUE;	//is a type definition, not a declaration
#											endif
											declarDef->type.specifiersMask	= ((parentDeclar->type.specifiersMask & ~ENSrcAnlzDecSpecifiersBits_TS_ALL) | ENSrcAnlzDecSpecifiersBit_TS_Declar);
											declarDef->type.iCustomTypeDef	= iDeclarDef;
										} else {
											//Declar was added at prev call
											NBASSERT(declarDef->type.specifiersMask == ((parentDeclar->type.specifiersMask & ~ENSrcAnlzDecSpecifiersBits_TS_ALL) | ENSrcAnlzDecSpecifiersBit_TS_Declar)) //Must match the base specs
											NBASSERT(declarDef->type.iCustomTypeDef	== iDeclarDef) //Must match the base declar
										}
										srcScope->stats.declarsRng.count++;
										NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
										//Process declarator
										if(!SrcAnlzCodeCParser_consumeDeclarator(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iSubDeclarDef)){
											r = FALSE; NBASSERT(FALSE)
											break;
										}
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_direct_declarator){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeDeclarator(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iDeclarDef)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else {
								r = FALSE; NBASSERT(FALSE)
								break;
							}
						}
					}
					//Next
					if(child->iNextPart == 0){
						break;
					} else {
						iChild	= child->iNextPart;
						child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
						iPart++;
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
				//
				NBASSERT(!arrayOpened)
				NBASSERT(!paramsOpened)
			}
		}
	}
	return r;
}

//, { "abstract-declarator", "pointer" }
//, { "abstract-declarator", "direct-abstract-declarator" }
//, { "abstract-declarator", "pointer direct-abstract-declarator" }
//, { "direct-abstract-declarator", "( abstract-declarator )" }
//, { "direct-abstract-declarator", "[ ]" }
//, { "direct-abstract-declarator", "direct-abstract-declarator [ ]" }
//, { "direct-abstract-declarator", "[ type-qualifier-list ]" }
//, { "direct-abstract-declarator", "[ assignment-expression ]" }
//, { "direct-abstract-declarator", "direct-abstract-declarator [ type-qualifier-list ]" }
//, { "direct-abstract-declarator", "direct-abstract-declarator [ assignment-expression ]" }
//, { "direct-abstract-declarator", "[ type-qualifier-list assignment-expression ]" }
//, { "direct-abstract-declarator", "direct-abstract-declarator [ type-qualifier-list assignment-expression ]" }
//, { "direct-abstract-declarator", "[ static assignment-expression ]" }
//, { "direct-abstract-declarator", "[ static type-qualifier-list assignment-expression ]" }
//, { "direct-abstract-declarator", "direct-abstract-declarator [ assignment-expression ]" }
//, { "direct-abstract-declarator", "direct-abstract-declarator [ static type-qualifier-list assignment-expression ]" }
//, { "direct-abstract-declarator", "[ type-qualifier-list static assignment-expression ]" }
//, { "direct-abstract-declarator", "direct-abstract-declarator [ type-qualifier-list static assignment-expression ]" }
//, { "direct-abstract-declarator", "[ * ]" }
//, { "direct-abstract-declarator", "direct-abstract-declarator [ * ]" }
//, { "direct-abstract-declarator", "( )" }
//, { "direct-abstract-declarator", "( parameter-type-list )" }
//, { "direct-abstract-declarator", "direct-abstract-declarator ( )" }
//, { "direct-abstract-declarator", "direct-abstract-declarator ( parameter-type-list )" }
BOOL SrcAnlzCodeCParser_consumeAbstractDeclarator(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const UI32 iDeclarDef){
	BOOL r = FALSE;
	NBASSERT(iDeclarDef != 0)
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_abstract_declarator || sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_direct_abstract_declarator)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_abstract_declarator || sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_direct_abstract_declarator){
				r = TRUE;
				//
				BOOL arrayOpened = FALSE, paramsOpened = FALSE;
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_Punctuator){
							NBASSERT(child->iChildSeq == 0)
							NBASSERT(partDef->idx < state->sintxDefs.punctsSz)
							const char* punctuator = state->sintxDefs.puncts[partDef->idx];
							//PRINTF_INFO("Punctuator '%s'\n", punctuator);
							if(NBString_strIsEqual(punctuator, "[")){
								//Start array definition
								NBASSERT(iDeclarDef > 0)
								NBASSERT(!arrayOpened)
								STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
								arrayOpened = TRUE;
								data->type.isArray	= TRUE;
							} else if(NBString_strIsEqual(punctuator, "(")){
								if((iPart + 1) < sintaxDefSeq->partsSz){
									const STSrcAnlzSintxPart* nextPartDef = &sintaxDefSeq->parts[iPart + 1];
									if(nextPartDef->type != ENSrzAnlzSintxPartType_SintaxDef || nextPartDef->idx != ENSrzAnlzSintxC_abstract_declarator){ //Not "(" + "abstract-declarator"
										//Start params definitions
										NBASSERT(iDeclarDef > 0)
										NBASSERT(!paramsOpened)
										const STSrcAnlzCodeCDeclarsScope* scope = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope);
										STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
										NBASSERT(data->iDeepLvl == scope->iDeepLvl || (data->dbgIsTypeDef && data->iDeepLvl > scope->iDeepLvl))
										//Add params declarScope
										const UI32 iParamsScope = srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count;
										STSrcAnlzCodeCDeclarsScope* scopeDef = NULL; BOOL isNewDef = FALSE;
										if(!SrcAnlzCodeC_getDeclarScopeByIdx(dstCode, iParamsScope, iDeclScope, &scopeDef, &isNewDef)){
											r = FALSE; NBASSERT(FALSE)
											break;
										} else {
											if(isNewDef){
												//Declar was added at this call
												NBASSERT(data->type.paramsBodyDefIdx == 0)
												data->type.paramsBodyDefIdx = iParamsScope;
											} else {
												//Declar was added at prev call
												NBASSERT(data->type.paramsBodyDefIdx == iParamsScope)
											}
											srcScope->stats.scopesRng.count++;
											NBASSERT((srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count) <= dstCode->defsDeclarsScopes.use)
											paramsOpened = TRUE;
											srcScope->stats.iLastActiveDecScope = data->type.paramsBodyDefIdx;
										}
									}
								}
							} else if(NBString_strIsEqual(punctuator, ")")){
								if(paramsOpened){
									//End params definitions
									NBASSERT(iDeclarDef > 0)
									STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
									NBASSERT(data->type.paramsBodyDefIdx != 0) //Params declaration must started before this point
									paramsOpened = FALSE;
									srcScope->stats.iLastActiveDecScope = iDeclScope;
									//PRINTF_INFO("AbstractDeclarator %d closed with paramsRange(%d, +%d) level(%d).\n", iDeclarDef, data->type.paramsRng.start, data->type.paramsRng.count, iDeclScope);
								}
							} else if(NBString_strIsEqual(punctuator, "]")){
								if(arrayOpened){
									//End array definition
									NBASSERT(iDeclarDef > 0)
									arrayOpened = FALSE;
								}
							}
						} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							if(partDef->idx == ENSrzAnlzSintxC_parameter_type_list){
								//Add parameters type-list
								NBASSERT(iDeclarDef != 0)
								STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
								NBASSERT(paramsOpened && data->type.paramsBodyDefIdx != 0) //Params declaration must started before this point
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeDeclaratorParamTypeList(state, srcScope, dstCode, child->iChildSeq, data->type.paramsBodyDefIdx)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_pointer){
								NBASSERT(iDeclarDef != 0)
								STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
								//Pointer spec must be processed only once (to avoid unecesary array fragmentation)
								if(data->type.ptrSpecsRng.count == 0){
									if(child->iChildSeq != 0){
										if(!SrcAnlzCodeCParser_consumePointer(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iDeclarDef)){
											r = FALSE; NBASSERT(FALSE)
											break;
										}
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_assignment_expression || partDef->idx == ENSrzAnlzSintxC_type_qualifier_list){
								//Todo: add support to this
								//PRINTF_WARNING("ToDo: 'abstract-declarator', add support to 'assignment_expression' and 'type_qualifier_list'.\n");
							} else if(partDef->idx == ENSrzAnlzSintxC_abstract_declarator){
								//-------------------
								//Sub abstract declarator, a declar pointing to another declar
								//Examples:
								//declares 3 functions (normal, pointer returner, and a pointer to a function).
								//int f(void), *fip(), (*pfi)();
								//declares an array apfi of three pointers to functions returning int and having two parameters each.
								//int (*apfi[3])(int *x, int *y);
								//declares a function fpfi that returns a pointer to a function returning an int. The function fpfi has two parameters: a pointer to a function returning an int (with one parameter of type long int), and an int. The pointer returned by fpfi points to a function that has one int parameter and accepts zero or more additional arguments of any type.
								//int (*fpfi(int (*)(long), int))(int, ...);
								//-------------------
								//Process childs
								if(child->iChildSeq != 0){
									//Determine if this declaration was already added
									const UI32 iSubDeclarDef = (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count);
									STSrcAnlzCodeCDeclar* declarDef = NULL; BOOL isNewDef = FALSE;
									if(!SrcAnlzCodeC_getDeclarByIdx(dstCode, iSubDeclarDef, iDeclScope, &declarDef, &isNewDef)){
										r = FALSE; NBASSERT(FALSE)
										break;
									} else {
										NBASSERT(iDeclarDef > 0)
										STSrcAnlzCodeCDeclar* parentDeclar = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
										if(isNewDef){
											//Declar was added at this call
											parentDeclar->iDeepLvl++;		//Push parent declar one level deeper (is not part of the scope's declaration)
#											ifdef NB_CONFIG_INCLUDE_ASSERTS
											parentDeclar->dbgIsTypeDef		= TRUE;	//is a type definition, not a declaration
#											endif
											declarDef->type.specifiersMask	= ((parentDeclar->type.specifiersMask & ~ENSrcAnlzDecSpecifiersBits_TS_ALL) | ENSrcAnlzDecSpecifiersBit_TS_Declar);
											declarDef->type.iCustomTypeDef	= iDeclarDef;
										} else {
											//Declar was added at prev call
											NBASSERT(declarDef->type.specifiersMask == ((parentDeclar->type.specifiersMask & ~ENSrcAnlzDecSpecifiersBits_TS_ALL) | ENSrcAnlzDecSpecifiersBit_TS_Declar)) //Must match the base specs
											NBASSERT(declarDef->type.iCustomTypeDef	== iDeclarDef) //Must match the base declar
										}
										srcScope->stats.declarsRng.count++;
										NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
										//Process declarator
										if(!SrcAnlzCodeCParser_consumeAbstractDeclarator(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iSubDeclarDef)){
											r = FALSE; NBASSERT(FALSE)
											break;
										}
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_direct_abstract_declarator){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeAbstractDeclarator(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iDeclarDef)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else {
								r = FALSE; NBASSERT(FALSE)
								break;
							}
						}
					}
					//Next
					if(child->iNextPart == 0){
						break;
					} else {
						iChild	= child->iNextPart;
						child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
						iPart++;
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
				//
				NBASSERT(!arrayOpened)
				NBASSERT(!paramsOpened)
			}
		}
	}
	return r;
}

//, { "pointer", "*" }
//, { "pointer", "* type-qualifier-list" }
//, { "pointer", "* pointer" }
//, { "pointer", "* type-qualifier-list pointer" }
BOOL SrcAnlzCodeCParser_consumePointer(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const UI32 iDeclarDef){
	BOOL r = FALSE;
	NBASSERT(iDeclarDef != 0)
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			do {
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					//Process
					if(partDef->type == ENSrzAnlzSintxPartType_Punctuator){
						NBASSERT(child->iChildSeq == 0)
						NBASSERT(partDef->idx < state->sintxDefs.punctsSz)
						const char* punctuator = state->sintxDefs.puncts[partDef->idx];
						//PRINTF_INFO("Punctuator '%s'\n", punctuator);
						if(NBString_strIsEqual(punctuator, "*")){
							//Start array definition
							NBASSERT(iDeclarDef > 0)
							STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
							if(data->type.ptrSpecsRng.start == 0){
								data->type.ptrSpecsRng.start = dstCode->defsDeclPtrSpecs.use;
								NBASSERT(data->type.ptrSpecsRng.start > 0)
								NBASSERT(data->type.ptrSpecsRng.count == 0)
							}
							STSrcAnlzCodeCDeclarPtr ptrSpec;
							ptrSpec.specifiersMask	= 0;
							ptrSpec.iDeepLvl		= data->iDeepLvl;
							NBArray_addValue(&dstCode->defsDeclPtrSpecs, ptrSpec);
							data->type.ptrSpecsRng.count++;
						} else {
							r = FALSE; NBASSERT(FALSE)
							break;
						}
					} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						if(partDef->idx == ENSrzAnlzSintxC_pointer){
							if(child->iChildSeq != 0){
								if(!SrcAnlzCodeCParser_consumePointer(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iDeclarDef)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						} else if(partDef->idx == ENSrzAnlzSintxC_type_qualifier_list){
							UI32 submask = 0;
							if(!SrcAnlzCodeCParser_consumeTypeQualifierList(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &submask)){
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								//Apply mask to last pointer def
								NBASSERT(iDeclarDef > 0)
								STSrcAnlzCodeCDeclar* data = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclarDef);
								NBASSERT(data->type.ptrSpecsRng.start != 0 && data->type.ptrSpecsRng.count != 0)
								NBASSERT((data->type.ptrSpecsRng.start + data->type.ptrSpecsRng.count) <= dstCode->defsDeclPtrSpecs.use)
								STSrcAnlzCodeCDeclarPtr* ptrSpec = NBArray_itmPtrAtIndex(&dstCode->defsDeclPtrSpecs, STSrcAnlzCodeCDeclarPtr, data->type.ptrSpecsRng.start + data->type.ptrSpecsRng.count - 1);
								NBASSERT(ptrSpec->specifiersMask == 0)
								ptrSpec->specifiersMask |= submask;
							}
						} else {
							r = FALSE; NBASSERT(FALSE)
							break;
						}
					}
				}
				//Next
				if(child->iNextPart == 0){
					break;
				} else {
					iChild	= child->iNextPart;
					child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
					iPart++;
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
		}
	}
	return r;
}

//, { "identifier-list", "identifier" }
//, { "identifier-list", "identifier-list , identifier" }
BOOL SrcAnlzCodeCParser_consumeDeclaratorParamIdentifierList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			do {
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					//Process
					if(partDef->type == ENSrzAnlzSintxPartType_Identifier){
						NBASSERT(child->iValue < state->acumTokensCur->defs.use)
						const STSrcAnlzTokn* tokenDef = NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, child->iValue);
						const char* tokenStr = &state->acumTokensCur->strBuff.str[tokenDef->iAtBuff];
						//Determine if this declaration was already added
						const UI32 iDeclarDef = (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count);
						STSrcAnlzCodeCDeclar* declarDef = NULL; BOOL isNewDef = FALSE;
						if(!SrcAnlzCodeC_getDeclarByIdx(dstCode, iDeclarDef, iDeclScope, &declarDef, &isNewDef)){
							r = FALSE; NBASSERT(FALSE)
							break;
						} else {
							if(isNewDef){
								//Declar was added at this call
								declarDef->iNameFirstChar = dstCode->strsBuff.length;
								NBString_concat(&dstCode->strsBuff, tokenStr);
								NBString_concatByte(&dstCode->strsBuff, '\0');
							} else {
								//Declar was added at prev call
								NBASSERT(declarDef->iNameFirstChar != 0) //Must have a name
								NBASSERT(NBString_strIsEqual(tokenStr, &dstCode->strsBuff.str[declarDef->iNameFirstChar])) //must have same name
							}
							srcScope->stats.declarsRng.count++;
							NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
						}
					} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						NBASSERT(partDef->idx == ENSrzAnlzSintxC_identifier_list)
						if(child->iChildSeq != 0){
							if(!SrcAnlzCodeCParser_consumeDeclaratorParamIdentifierList(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
								r = FALSE; NBASSERT(FALSE)
								break;
							}
						}
					}
				}
				//Next
				if(child->iNextPart == 0){
					break;
				} else {
					iChild	= child->iNextPart;
					child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
					iPart++;
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
		}
	}
	return r;
}

//, { "parameter-type-list", "parameter-list" }
//, { "parameter-type-list", "parameter-list , ..." }
//, { "parameter-list", "parameter-declaration" }
//, { "parameter-list", "parameter-list , parameter-declaration" }
//, { "parameter-declaration", "declaration-specifiers declarator" }
//, { "parameter-declaration", "declaration-specifiers" }
//, { "parameter-declaration", "declaration-specifiers abstract-declarator" }
BOOL SrcAnlzCodeCParser_consumeDeclaratorParamTypeList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			UI32 iDeclarDef = 0;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			do {
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					//Process
					if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						//PRINTF_INFO("ParamTypeList: '%s'.\n", state->sintxDefs.defs[partDef->idx].name);
						if(partDef->idx == ENSrzAnlzSintxC_declaration_specifiers){
							//Start new parameter
							NBASSERT(child->iChildSeq != 0)
							if(child->iChildSeq != 0){
								STSrcAnlzCodeCDeclarType type;
								type.specifiersMask		= 0;
								type.iCustomTypeDef		= 0;
								type.ptrSpecsRng.start	= 0;
								type.ptrSpecsRng.count	= 0;
								type.isArray			= FALSE;
								type.arrayTotalSz		= 0;
								type.paramsBodyDefIdx	= 0;
								type.paramsLenFlexible	= FALSE;
								if(!SrcAnlzCodeCParser_consumeDeclarationSpecifiers(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &type, NULL, FALSE)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									NBASSERT(type.specifiersMask != 0)
									//Determine if this declaration was already added
									iDeclarDef = (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count);
									STSrcAnlzCodeCDeclar* declarDef = NULL; BOOL isNewDef = FALSE;
									if(!SrcAnlzCodeC_getDeclarByIdx(dstCode, iDeclarDef, iDeclScope, &declarDef, &isNewDef)){
										r = FALSE; NBASSERT(FALSE)
										break;
									} else {
										if(isNewDef){
											//Declar was added at this call
											declarDef->type = type;
										} else {
											//Declar was added at prev call
											NBASSERT((declarDef->type.specifiersMask & type.specifiersMask) == type.specifiersMask) //Must match the base specs
										}
										srcScope->stats.declarsRng.count++;
										NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
									}
								}
							}
						} else if(partDef->idx == ENSrzAnlzSintxC_declarator){
							//Continue parameter
							NBASSERT(iDeclarDef != 0)
							NBASSERT(child->iChildSeq != 0)
							if(child->iChildSeq != 0){
								if(!SrcAnlzCodeCParser_consumeDeclarator(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iDeclarDef)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						} else if(partDef->idx == ENSrzAnlzSintxC_abstract_declarator){
							//Continue parameter
							NBASSERT(iDeclarDef != 0)
							NBASSERT(child->iChildSeq != 0)
							if(child->iChildSeq != 0){
								if(!SrcAnlzCodeCParser_consumeAbstractDeclarator(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iDeclarDef)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						} else {
							//Process childs
							NBASSERT(partDef->idx == ENSrzAnlzSintxC_parameter_list || partDef->idx == ENSrzAnlzSintxC_parameter_declaration)
							if(child->iChildSeq != 0){
								if(!SrcAnlzCodeCParser_consumeDeclaratorParamTypeList(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						}
					}
				}
				//Next
				if(child->iNextPart == 0){
					break;
				} else {
					iChild	= child->iNextPart;
					child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
					iPart++;
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
		}
	}
	return r;
}

//, { "typedef-name", "identifier" }
BOOL SrcAnlzCodeCParser_consumeTypedefName(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, UI32* dstIdx){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			//Search nodes
			{
				UI32 iPart = 0;
				UI16 iChild = iFirstChild;
				const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
				const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
				NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_typedef_name)
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						NBASSERT(partDef->type == ENSrzAnlzSintxPartType_Identifier)
						if(partDef->type == ENSrzAnlzSintxPartType_Identifier){
							NBASSERT(child->iValue < state->acumTokensCur->defs.use)
							const STSrcAnlzTokn* tokenDef = NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, child->iValue);
							const char* tokenStr	= &state->acumTokensCur->strBuff.str[tokenDef->iAtBuff];
							UI32 iDeclar = 0;
							NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
							if(!SrcAnlzCodeC_findDeclarByName(dstCode, iDeclScope, TRUE, tokenStr, (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count), &iDeclar)){
								r = FALSE; NBASSERT(FALSE)
								break;
							} else if(iDeclar == 0){
								//typedef not found
								PRINTF_ERROR("typedef with name '%s' not found.\n", tokenStr);
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								NBASSERT(iDeclar < dstCode->defsDeclars.use)
								const STSrcAnlzCodeCDeclar* declar = NBArray_itmPtrAtIndex(&dstCode->defsDeclars, STSrcAnlzCodeCDeclar, iDeclar);
								if((declar->type.specifiersMask & ENSrcAnlzDecSpecifiersBit_SC_Typedef) == 0){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									if(dstIdx != NULL) *dstIdx = iDeclar;
								}
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
							NBASSERT(FALSE) //This should be a single node seq
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
		}
	}
	return r;
}

//, { "enum-specifier", "enum { enumerator-list }" }
//, { "enum-specifier", "enum identifier { enumerator-list }" }
//, { "enum-specifier", "enum { enumerator-list , }" }
//, { "enum-specifier", "enum identifier { enumerator-list , }" }
//, { "enum-specifier", "enum identifier" }
BOOL SrcAnlzCodeCParser_consumeEnumSpecifier(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, UI32* dstIdx, const BOOL forceNewUsrType){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			BOOL isNewTypeDeclar = FALSE, isCompleted = FALSE;
			const STSrcAnlzSintxParseTreeNode* nameNode = NULL;
			const STSrcAnlzSintxParseTreeNode* enumListNode = NULL;
			//Search nodes
			{
				UI32 iPart = 0;
				UI16 iChild = iFirstChild;
				const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
				const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
				NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_enum_specifier)
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_Punctuator){
							NBASSERT(child->iChildSeq == 0)
							NBASSERT(partDef->idx < state->sintxDefs.punctsSz)
							const char* punctuator = state->sintxDefs.puncts[partDef->idx];
							//PRINTF_INFO("Punctuator '%s'\n", punctuator);
							if(NBString_strIsEqual(punctuator, "{")){
								NBASSERT(!isNewTypeDeclar) //Only one '{' allowed
								isNewTypeDeclar = TRUE;
							} else if(NBString_strIsEqual(punctuator, "}")){
								NBASSERT(isNewTypeDeclar) //'{' must be found
								NBASSERT(!isCompleted) //Only one '}' allowed
								isCompleted = TRUE;
							} else if(NBString_strIsEqual(punctuator, ",")){
								//Nothing
							} else {
								NBASSERT(FALSE) //Unexpected punctuator
							}
						} else if(partDef->type == ENSrzAnlzSintxPartType_Identifier){
							if(nameNode != NULL){
								PRINTF_ERROR("enum-specifier, more than one identifier (enum's name) found.\n");
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								nameNode = child;
							}
						} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							NBASSERT(partDef->idx == ENSrzAnlzSintxC_enumerator_list) //The only posible, in this lang sintax
							if(partDef->idx == ENSrzAnlzSintxC_enumerator_list){
								if(enumListNode != NULL){
									PRINTF_ERROR("enum-specifier, more than one enumerator-list (enum's members) found.\n");
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									enumListNode = child;
								}
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
			//Analyze nodes
			if(r){
				//Specs from "C99_lang_specs_n1256.pdf":
				//The presence of a struct-declaration-list in a struct-or-union-specifier declares a new type, within a translation unit.
				UI32 iEnumDef = 0;
				const char* usrTypeName = NULL;
				if(nameNode != NULL){
					NBASSERT(nameNode->iValue < state->acumTokensCur->defs.use)
					const STSrcAnlzTokn* tokenDef = NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, nameNode->iValue);
					usrTypeName = &state->acumTokensCur->strBuff.str[tokenDef->iAtBuff];
					//Look for already defined usrTypeDef (by name)
					UI32 iEnumDef2 = 0;
					NBASSERT((srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count) <= dstCode->defsUsrTypes.use)
					if(!SrcAnlzCodeC_findUsrTypeByName(dstCode, iDeclScope, TRUE, usrTypeName, (srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count + 1), &iEnumDef2)){
						r = FALSE; NBASSERT(FALSE)
					} else if(iEnumDef2 != 0){
						const STSrcAnlzCodeCUsrType* data = NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iEnumDef2);
						if(data->type == ENSrcAnlzCodeUsrType_Enum){
							iEnumDef = iEnumDef2;
						}
					}
				}
				//
				if(isNewTypeDeclar || forceNewUsrType || usrTypeName == NULL){
					if(iEnumDef != 0){
						const STSrcAnlzCodeCUsrType* data = NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iEnumDef);
						if(iEnumDef == (srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count)){
							NBASSERT(data->iParentScope == iDeclScope)
							srcScope->stats.usrTypesRng.count++;
						} else {
							//Force creation if is not at current scope
							if(data->iParentScope != iDeclScope){
								iEnumDef = 0;
							}
						}
					} else {
						//Forcing the creation of a new usrType
						const UI32 iEnumDef2 = (srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count);
						NBASSERT(iEnumDef2 <= dstCode->defsUsrTypes.use)
						if(iEnumDef2 < dstCode->defsUsrTypes.use){
							//usrTypeDef already added
							NBASSERT(NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iEnumDef2)->iParentScope == iDeclScope) //must have same parent scope
							NBASSERT(NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iEnumDef2)->type == ENSrcAnlzCodeUsrType_Enum) //must be same type
							iEnumDef = iEnumDef2;
							srcScope->stats.usrTypesRng.count++;
							NBASSERT((srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count) <= dstCode->defsUsrTypes.use)
						} else if(iEnumDef2 == dstCode->defsUsrTypes.use){
							//usrTypeDef is new (will be created)
						} else {
							r = FALSE; NBASSERT(FALSE) //Program logic error
						}
					}
				}
				//Add new usrTypeDef
				if(r && iEnumDef == 0){
					iEnumDef = dstCode->defsUsrTypes.use; NBASSERT(iEnumDef > 0)
					const STSrcAnlzCodeCDeclarsScope* scope = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope);
					STSrcAnlzCodeCUsrType data;
					data.iParentScope	= iDeclScope;
					data.iDeepLvl		= scope->iDeepLvl;
					data.type			= ENSrcAnlzCodeUsrType_Enum;
					data.isComplete		= isCompleted;
					data.iNameFirstChar	= 0;
					data.iBodyDef		= 0;
					if(usrTypeName != NULL){
						data.iNameFirstChar	= dstCode->strsBuff.length;
						NBString_concat(&dstCode->strsBuff, usrTypeName);
						NBString_concatByte(&dstCode->strsBuff, '\0');
						PRINTF_INFO("DeclarsScope %d, userType %d(@%d) added with name '%s' (enum).\n", iDeclScope, srcScope->stats.usrTypesRng.count, dstCode->defsUsrTypes.use, usrTypeName);
					} else {
						PRINTF_INFO("DeclarsScope %d, userType %d(@%d) added with no-name (enum).\n", iDeclScope, srcScope->stats.usrTypesRng.count, dstCode->defsUsrTypes.use);
					}
					NBArray_addValue(&dstCode->defsUsrTypes, data);
					srcScope->stats.usrTypesRng.count++;
					//Update scopes's usrTypes range
					{
						UI32 iDeclScope2 = iDeclScope;
						STSrcAnlzCodeCDeclarsScope* scope2 = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
						do {
							//PRINTF_INFO("Updating scope %d's usersTypes range from(%d, +%d) to(%d, +%d).\n", iDeclScope2, scope2->usrTypesRng.start, scope2->usrTypesRng.count, scope2->usrTypesRng.start, (dstCode->defsUsrTypes.use - scope2->usrTypesRng.start));
							scope2->usrTypesRng.count = (dstCode->defsUsrTypes.use - scope2->usrTypesRng.start);
							if(scope2->iParentScope == 0){
								break;
							} else {
								iDeclScope2 = scope2->iParentScope;
								scope2 = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
							}
						} while(TRUE);
					}
				} else {
					//PRINTF_INFO("DeclarsScope %d, userType %d(@%d) ignoring with name '%s' (enum) (already added).\n", iDeclScope, srcScope->stats.usrTypesRng.count, iEnumDef, usrTypeName);
				}
				//Process enum's body
				if(r && enumListNode != NULL){
					NBASSERT(iEnumDef > 0)
					NBASSERT(isNewTypeDeclar && isCompleted)
					NBASSERT(NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iEnumDef)->iParentScope == iDeclScope) //must have same parent scope
					NBASSERT(enumListNode->iChildSeq > 0)
					if(enumListNode->iChildSeq > 0){
						if(!SrcAnlzCodeCParser_consumeEnumeratorList(state, srcScope, dstCode, enumListNode->iChildSeq, iDeclScope, iEnumDef)){
							r = FALSE; NBASSERT(FALSE)
						}
					}
				}
				//Return result
				if(r){
					NBASSERT(iEnumDef > 0)
					if(dstIdx != NULL) *dstIdx = iEnumDef;
				}
			}
		}
	}
	return r;
}

//, { "enumerator-list", "enumerator" }
//, { "enumerator-list", "enumerator-list , enumerator" }
//, { "enumerator", "enumeration-constant" }
//, { "enumerator", "enumeration-constant = constant-expression" }
//, { "enumeration-constant", "identifier" }
BOOL SrcAnlzCodeCParser_consumeEnumeratorList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const UI32 iEnumDef){
	BOOL r = FALSE;
	NBASSERT(iEnumDef > 0)
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			r = TRUE;
			do {
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					//Process
					if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_enumerator_list || sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_enumerator || sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_enumeration_constant || sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_constant_expression)
						if(child->iChildSeq != 0){
							if(sintaxDefSeq->iDefEnum != ENSrzAnlzSintxC_constant_expression){
								if(!SrcAnlzCodeCParser_consumeEnumeratorList(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iEnumDef)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						}
					} else if(partDef->type == ENSrzAnlzSintxPartType_Identifier){
						//Specs from "C99_lang_specs_n1256.pdf":
						//The identifiers in an enumerator list are declared as constants that have type int and may appear wherever such are permitted.
						//Thus, the identifiers of enumeration constants declared in the same scope shall all be distinct from each other and from other identifiers declared in ordinary declarators.
						//Each enumerated type shall be compatible with char, a signed integer type, or an unsigned integer type.
						NBASSERT(child->iValue < state->acumTokensCur->defs.use)
						const STSrcAnlzTokn* tokenDef = NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, child->iValue);
						const char* tokenStr = &state->acumTokensCur->strBuff.str[tokenDef->iAtBuff];
						//Determine if this declaration was already added
						const UI32 iDeclarDef = (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count);
						STSrcAnlzCodeCDeclar* declarDef = NULL; BOOL isNewDef = FALSE;
						if(!SrcAnlzCodeC_getDeclarByIdx(dstCode, iDeclarDef, iDeclScope, &declarDef, &isNewDef)){
							r = FALSE; NBASSERT(FALSE)
							break;
						} else {
							if(isNewDef){
								//Declar was added at this call
								declarDef->type.specifiersMask	= (ENSrcAnlzDecSpecifiersBit_TQ_Const | ENSrcAnlzDecSpecifiersBit_TS_Enum);
								declarDef->type.iCustomTypeDef	= iEnumDef;
								declarDef->iNameFirstChar = dstCode->strsBuff.length;
								NBString_concat(&dstCode->strsBuff, tokenStr);
								NBString_concatByte(&dstCode->strsBuff, '\0');
								PRINTF_INFO("Added enumerator '%s' declar(@%d) for enum '%s'.\n", tokenStr, iDeclarDef, &dstCode->strsBuff.str[NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iEnumDef)->iNameFirstChar]);
							} else {
								//Declar was added at prev call
								NBASSERT(declarDef->type.specifiersMask == (ENSrcAnlzDecSpecifiersBit_TQ_Const | ENSrcAnlzDecSpecifiersBit_TS_Enum)) //Must match the base specs
								NBASSERT(declarDef->type.iCustomTypeDef	== iEnumDef)
								NBASSERT(declarDef->iNameFirstChar != 0) //Must have a name
								NBASSERT(NBString_strIsEqual(tokenStr, &dstCode->strsBuff.str[declarDef->iNameFirstChar])) //Must match name
							}
							srcScope->stats.declarsRng.count++;
							NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
						}
					}
					//Next
					if(child->iNextPart == 0){
						break;
					} else {
						iChild	= child->iNextPart;
						child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
						iPart++;
					}
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
		}
	}
	return r;
}

//, { "struct-or-union-specifier", "struct-or-union { struct-declaration-list }" }
//, { "struct-or-union-specifier", "struct-or-union identifier { struct-declaration-list }" }
//, { "struct-or-union-specifier", "struct-or-union identifier" }
//, { "struct-or-union", "struct" }
//, { "struct-or-union", "union" }
BOOL SrcAnlzCodeCParser_consumeStructOrUnionSpecifier(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, UI32* dstIdx, const BOOL forceNewUsrType){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			BOOL isNewTypeDeclar = FALSE, isCompleted = FALSE;
			const STSrcAnlzSintxParseTreeNode* typeNode = NULL;
			const STSrcAnlzSintxParseTreeNode* nameNode = NULL;
			const STSrcAnlzSintxParseTreeNode* decListNode = NULL;
			//Search nodes
			{
				UI32 iPart = 0;
				UI16 iChild = iFirstChild;
				const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
				const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
				NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_struct_or_union_specifier)
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_Punctuator){
							NBASSERT(child->iChildSeq == 0)
							NBASSERT(partDef->idx < state->sintxDefs.punctsSz)
							const char* punctuator = state->sintxDefs.puncts[partDef->idx];
							//PRINTF_INFO("Punctuator '%s'\n", punctuator);
							if(NBString_strIsEqual(punctuator, "{")){
								NBASSERT(!isNewTypeDeclar) //Only one '{' allowed
								isNewTypeDeclar = TRUE;
							} else if(NBString_strIsEqual(punctuator, "}")){
								NBASSERT(isNewTypeDeclar) //'{' must be found
								NBASSERT(!isCompleted) //Only one '}' allowed
								isCompleted = TRUE;
							} else {
								NBASSERT(FALSE) //Unexpected punctuator
							}
						} else if(partDef->type == ENSrzAnlzSintxPartType_Identifier){
							NBASSERT(child->iChildSeq == 0)
							if(nameNode != NULL){
								PRINTF_ERROR("struct-or-union-specifier, more than one identifier (struct-or-union's name) found.\n");
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								nameNode = child;
							}
						} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							NBASSERT(partDef->idx == ENSrzAnlzSintxC_struct_or_union || partDef->idx == ENSrzAnlzSintxC_struct_declaration_list)
							if(partDef->idx == ENSrzAnlzSintxC_struct_or_union){
								if(typeNode != NULL){
									PRINTF_ERROR("struct-or-union-specifier, more than one struct-or-union node (struct-or-union's type) found.\n");
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									typeNode = child;
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_struct_declaration_list){
								if(decListNode != NULL){
									PRINTF_ERROR("struct-or-union-specifier, more than one struct-declaration-list node (struct-or-union's members) found.\n");
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									decListNode = child;
								}
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
			//Analyze nodes
			if(r){
				if(typeNode == NULL){
					r = FALSE; NBASSERT(FALSE) //A name or members-list is requiered
				} else {
					if(typeNode->iChildSeq == 0){
						r = FALSE; NBASSERT(FALSE) //Could not determine type
					} else {
						const STSrcAnlzSintxParseTreeNode* keywChild		= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, typeNode->iChildSeq);
						const STSrcAnlzSintxSeq* sintaxDefSeq	= &state->sintxDefs.defsSeqs[keywChild->iSintaxDefSeq];
						const STSrcAnlzSintxPart* partDef		= &sintaxDefSeq->parts[0];
						if(partDef->type != ENSrzAnlzSintxPartType_Keyword){
							r = FALSE; NBASSERT(FALSE) //Could not determine type
						} else {
							NBASSERT(keywChild->iChildSeq == 0)
							NBASSERT(partDef->idx < state->sintxDefs.keywsSz)
							const char* keyword = state->sintxDefs.keyws[partDef->idx];
							const ENSrcAnlzCodeUsrType type = (NBString_strIsEqual(keyword, "struct") ? ENSrcAnlzCodeUsrType_Struct : NBString_strIsEqual(keyword, "union") ? ENSrcAnlzCodeUsrType_Union : ENSrcAnlzCodeUsrType_Count);
							if(type == ENSrcAnlzCodeUsrType_Count){
								r = FALSE; NBASSERT(FALSE) //Could not determine type
							} else {
								//Specs from "C99_lang_specs_n1256.pdf":
								//The presence of a struct-declaration-list in a struct-or-union-specifier declares a new type, within a translation unit.
								UI32 iStructDef = 0;
								const char* usrTypeName = NULL;
								//Search name
								if(nameNode != NULL){
									NBASSERT(nameNode->iValue < state->acumTokensCur->defs.use)
									const STSrcAnlzTokn* tokenDef = NBArray_itmPtrAtIndex(&state->acumTokensCur->defs, STSrcAnlzTokn, nameNode->iValue);
									usrTypeName = &state->acumTokensCur->strBuff.str[tokenDef->iAtBuff];
									//Look for already defined usrTypeDef (by name)
									UI32 iStructDef2 = 0;
									NBASSERT((srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count) <= dstCode->defsUsrTypes.use)
									if(!SrcAnlzCodeC_findUsrTypeByName(dstCode, iDeclScope, TRUE, usrTypeName, (srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count + 1), &iStructDef2)){
										r = FALSE; NBASSERT(FALSE)
									} else if(iStructDef2 != 0){
										const STSrcAnlzCodeCUsrType* data = NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iStructDef2);
										if(data->type == type){
											iStructDef = iStructDef2;
										}
									}
								}
								//
								if(isNewTypeDeclar || forceNewUsrType || usrTypeName == NULL){
									if(iStructDef != 0){
										const STSrcAnlzCodeCUsrType* data = NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iStructDef);
										if(iStructDef == (srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count)){
											NBASSERT(data->iParentScope == iDeclScope)
											srcScope->stats.usrTypesRng.count++;
										} else {
											//Force creation if is not at current scope
											if(data->iParentScope != iDeclScope){
												iStructDef = 0;
											}
										}
									} else {
										//Forcing the creation of a new usrType
										const UI32 iStructDef2 = (srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count);
										NBASSERT(iStructDef2 <= dstCode->defsUsrTypes.use)
										if(iStructDef2 < dstCode->defsUsrTypes.use){
											//usrTypeDef already added
											NBASSERT(NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iStructDef2)->iParentScope == iDeclScope) //must have same parent scope
											NBASSERT(NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iStructDef2)->type == type) //must be same type
											iStructDef = iStructDef2;
											srcScope->stats.usrTypesRng.count++;
											NBASSERT((srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count) <= dstCode->defsUsrTypes.use)
										} else if(iStructDef2 == dstCode->defsUsrTypes.use){
											//usrTypeDef is new (will be created)
										} else {
											r = FALSE; NBASSERT(FALSE) //Program logic error
										}
									}
								}
								//Add new usrTypeDef
								if(r && iStructDef == 0){
									iStructDef = dstCode->defsUsrTypes.use; NBASSERT(iStructDef > 0)
									const STSrcAnlzCodeCDeclarsScope* scope = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope);
									STSrcAnlzCodeCUsrType data;
									data.iParentScope	= iDeclScope;
									data.iDeepLvl		= scope->iDeepLvl;
									data.type			= type;
									data.isComplete		= isCompleted;
									data.iNameFirstChar	= 0;
									data.iBodyDef		= 0;
									if(usrTypeName != NULL){
										data.iNameFirstChar	= dstCode->strsBuff.length;
										NBString_concat(&dstCode->strsBuff, usrTypeName);
										NBString_concatByte(&dstCode->strsBuff, '\0');
										PRINTF_INFO("DeclarsScope %d, userType %d(@%d) added with name '%s' (%s).\n", iDeclScope, srcScope->stats.usrTypesRng.count, dstCode->defsUsrTypes.use, usrTypeName, (type == ENSrcAnlzCodeUsrType_Struct ? "struct" : "union"));
									} else {
										PRINTF_INFO("DeclarsScope %d, userType %d(@%d) added with no-name (%s).\n", iDeclScope, srcScope->stats.usrTypesRng.count, dstCode->defsUsrTypes.use, (type == ENSrcAnlzCodeUsrType_Struct ? "struct" : "union"));
									}
									NBArray_addValue(&dstCode->defsUsrTypes, data);
									srcScope->stats.usrTypesRng.count++;
									//Update scopes's usrTypes range
									{
										UI32 iDeclScope2 = iDeclScope;
										STSrcAnlzCodeCDeclarsScope* scope2 = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
										do {
											//PRINTF_INFO("Updating scope %d's usersTypes range from(%d, +%d) to(%d, +%d).\n", iDeclScope2, scope2->usrTypesRng.start, scope2->usrTypesRng.count, scope2->usrTypesRng.start, (dstCode->defsUsrTypes.use - scope2->usrTypesRng.start));
											scope2->usrTypesRng.count = (dstCode->defsUsrTypes.use - scope2->usrTypesRng.start);
											if(scope2->iParentScope == 0){
												break;
											} else {
												iDeclScope2 = scope2->iParentScope;
												scope2 = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope2);
											}
										} while(TRUE);
									}
								} else {
									//PRINTF_INFO("DeclarsScope %d, userType %d(@%d) ignoring with name '%s' (%s) (already added).\n", iDeclScope, srcScope->stats.usrTypesRng.count, iStructDef, usrTypeName, (type == ENSrcAnlzCodeUsrType_Struct ? "struct" : "union"));
								}
								//Process struct-or-union body
								if(r && decListNode != NULL){
									NBASSERT(iStructDef != 0)
									NBASSERT(isNewTypeDeclar && isCompleted)
									if(decListNode->iValue == 0){
										r = FALSE; NBASSERT(FALSE)
									} else {
										NBASSERT(decListNode->iValue > 0 && decListNode->iValue < dstCode->defsDeclarsScopes.use)
										//ToDo: remove this next assert; other inner scopes could be created before this inner-scope.
										//NBASSERT(decListNode->iValue == (srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count))
										STSrcAnlzCodeCUsrType* data	= NBArray_itmPtrAtIndex(&dstCode->defsUsrTypes, STSrcAnlzCodeCUsrType, iStructDef);
										NBASSERT(!data->isComplete || isCompleted)
										NBASSERT(data->iBodyDef == 0 || data->iBodyDef == decListNode->iValue) //First or same value
										data->isComplete			= isCompleted;
										data->iBodyDef			 	= decListNode->iValue;
										const STSrcAnlzCodeCDeclarsScope* structBodyDef = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, data->iBodyDef);
										//PRINTF_INFO("%s's members already stored in definition %d, rangeUsrTypes(%d, +%d), rangeDeclars(%d, +%d), rangeSubscopes(%d, +%d).\n", (type == ENSrcAnlzCodeUsrType_Struct ? "struct" : "union"), data->iBodyDef, structBodyDef->usrTypesRng.start, structBodyDef->usrTypesRng.count, structBodyDef->declarsRng.start, structBodyDef->declarsRng.count, structBodyDef->scopesRng.start, structBodyDef->scopesRng.count);
										NBASSERT(structBodyDef->iParentScope == iDeclScope)
										NBASSERT(structBodyDef->iDeepLvl == (NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope)->iDeepLvl + 1))
										srcScope->stats.usrTypesRng.count	+= structBodyDef->usrTypesRng.count;
										srcScope->stats.declarsRng.count	+= structBodyDef->declarsRng.count;
										srcScope->stats.scopesRng.count		+= structBodyDef->scopesRng.count + 1; //+1, the subscope itself
										NBASSERT((srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count) <= dstCode->defsUsrTypes.use)
										NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
										NBASSERT((srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count) <= dstCode->defsDeclarsScopes.use)
									}
								}
								//Return result
								if(r){
									NBASSERT(iStructDef > 0)
									if(dstIdx != NULL) *dstIdx = iStructDef;
								}
							}
						}
					}
				}
			}
		}
	}
	return r;
}

//, { "struct-declaration-list", "struct-declaration" }
//, { "struct-declaration-list", "struct-declaration-list struct-declaration" }
BOOL SrcAnlzCodeCParser_consumeStructDeclarationList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_struct_declaration_list)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_struct_declaration_list){
				r = TRUE;
				UI32 iPart = 0;
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if((partDef->type == ENSrzAnlzSintxPartType_SintaxDef)){
							NBASSERT(partDef->idx == ENSrzAnlzSintxC_struct_declaration_list || partDef->idx == ENSrzAnlzSintxC_struct_declaration)
							if(partDef->idx == ENSrzAnlzSintxC_struct_declaration){
								if(!SrcAnlzCodeCParser_consumeStructDeclaration(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							} else {
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeStructDeclarationList(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
		}
	}
	return r;
}

//, { "struct-declaration", "specifier-qualifier-list struct-declarator-list ;" }
BOOL SrcAnlzCodeCParser_consumeStructDeclaration(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_struct_declaration)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_struct_declaration){
				r = TRUE;
				//
				BOOL typeFound				= FALSE;
				BOOL listFound				= FALSE;
				STSrcAnlzCodeCDeclarType typeBase;
				typeBase.specifiersMask		= 0;
				typeBase.iCustomTypeDef		= 0;
				typeBase.ptrSpecsRng.start	= 0;
				typeBase.ptrSpecsRng.count	= 0;
				typeBase.isArray			= FALSE;
				typeBase.arrayTotalSz		= 0;
				typeBase.paramsBodyDefIdx	= 0;
				typeBase.paramsLenFlexible	= FALSE;
				//
				UI32 iPart = 0;
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if((partDef->type == ENSrzAnlzSintxPartType_SintaxDef)){
							if(partDef->idx == ENSrzAnlzSintxC_specifier_qualifier_list){
								//Declaration base-type
								NBASSERT(typeBase.specifiersMask == 0)
								NBASSERT(child->iChildSeq != 0)
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeSpecifierQualifierList(state, srcScope, dstCode, child->iChildSeq, iDeclScope, &typeBase, NULL)){
										r = FALSE; NBASSERT(FALSE)
										break;
									} else {
										NBASSERT(typeBase.specifiersMask != 0)
										typeFound = TRUE;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_struct_declarator_list){
								//Declaration name's list (with derived types such as pointer, type qualifiers and params)
								NBASSERT(child->iChildSeq != 0)
								if(child->iChildSeq != 0){
									NBASSERT(typeFound)
									if(!SrcAnlzCodeCParser_consumeStructDeclaratorList(state, srcScope, dstCode, child->iChildSeq, iDeclScope, typeBase)){
										r = FALSE; NBASSERT(FALSE)
										break;
									} else {
										listFound = TRUE;
									}
								}
							} else {
								//Unexpected sintax def in declaration
								r = FALSE; NBASSERT(FALSE)
								break;
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
				NBASSERT(typeFound)
			}
		}
	}
	return r;
}

//, { "struct-declarator-list", "struct-declarator" }
//, { "struct-declarator-list", "struct-declarator-list , struct-declarator" }
BOOL SrcAnlzCodeCParser_consumeStructDeclaratorList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const STSrcAnlzCodeCDeclarType typeBase){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			r = TRUE;
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			do {
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				if(iPart >= sintaxDefSeq->partsSz){
					r = FALSE; NBASSERT(FALSE)
					break;
				} else {
					const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
					//Process
					if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						NBASSERT(partDef->idx == ENSrzAnlzSintxC_struct_declarator_list || partDef->idx == ENSrzAnlzSintxC_struct_declarator)
						if(partDef->idx == ENSrzAnlzSintxC_struct_declarator){
							NBASSERT(child->iChildSeq != 0)
							if(child->iChildSeq != 0){
								//Determine if this declaration was already added
								const UI32 iDeclarDef = (srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count);
								STSrcAnlzCodeCDeclar* declarDef = NULL; BOOL isNewDef = FALSE;
								if(!SrcAnlzCodeC_getDeclarByIdx(dstCode, iDeclarDef, iDeclScope, &declarDef, &isNewDef)){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									if(isNewDef){
										//Declar was added at this call
										declarDef->type	= typeBase;
									} else {
										//Declar was added at prev call
										NBASSERT((declarDef->type.specifiersMask & typeBase.specifiersMask) == typeBase.specifiersMask) //Must match the base specs
									}
									srcScope->stats.declarsRng.count++;
									NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
									//Process
									if(!SrcAnlzCodeCParser_consumeStructDeclarator(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iDeclarDef)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							}
						} else {
							//Process childs
							if(child->iChildSeq != 0){
								if(!SrcAnlzCodeCParser_consumeStructDeclaratorList(state, srcScope, dstCode, child->iChildSeq, iDeclScope, typeBase)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						}
					}
				}
				//Next
				if(child->iNextPart == 0){
					break;
				} else {
					iChild	= child->iNextPart;
					child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
					iPart++;
				}
				NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
			} while(TRUE);
		}
	}
	return r;
}

//, { "struct-declarator", "declarator" }
//, { "struct-declarator", ": constant-expression" }
//, { "struct-declarator", "declarator : constant-expression" }
BOOL SrcAnlzCodeCParser_consumeStructDeclarator(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const UI16 iDeclarDef){
	BOOL r = FALSE;
	NBASSERT(iDeclarDef != 0)
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_struct_declarator)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_struct_declarator){
				r = TRUE;
				//
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							if(partDef->idx == ENSrzAnlzSintxC_declarator){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeDeclarator(state, srcScope, dstCode, child->iChildSeq, iDeclScope, iDeclarDef)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_constant_expression){
								//Sz in bits
								//ToDo: process.
							} else {
								r = FALSE; NBASSERT(FALSE)
								break;
							}
						}
					}
					//Next
					if(child->iNextPart == 0){
						break;
					} else {
						iChild	= child->iNextPart;
						child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
						iPart++;
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
		}
	}
	return r;
}

//---------------------------
// Process statements
//---------------------------

//, { "statement", "labeled-statement" }
//, { "statement", "compound-statement" }
//, { "statement", "expression-statement" }
//, { "statement", "selection-statement" }
//, { "statement", "iteration-statement" }
//, { "statement", "jump-statement" }
BOOL SrcAnlzCodeCParser_consumeStatement(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_statement)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_statement){
				r = TRUE;
				//
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							if(partDef->idx == ENSrzAnlzSintxC_labeled_statement){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeStatementLabeled(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_compound_statement){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeStatementCompound(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_expression_statement){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeStatementExpression(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_selection_statement){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeStatementSelection(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_iteration_statement){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeStatementIteration(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_jump_statement){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeStatementJump(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else {
								NBASSERT(FALSE)
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
		}
	}
	return r;
}

//, { "labeled-statement", "identifier : statement" }
//, { "labeled-statement", "case constant-expression : statement" }
//, { "labeled-statement", "default : statement" }
BOOL SrcAnlzCodeCParser_consumeStatementLabeled(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_labeled_statement)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_labeled_statement){
				r = TRUE;
				//
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							if(partDef->idx == ENSrzAnlzSintxC_statement){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeStatement(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_constant_expression){
								//ToDo
							} else {
								r = FALSE; NBASSERT(FALSE) //Unexpected sintaxDef type
								break;
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
		}
	}
	return r;
}

//, { "compound-statement", "{ }" }
//, { "compound-statement", "{ block-item-list }" }
BOOL SrcAnlzCodeCParser_consumeStatementCompound(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			BOOL opened = FALSE, blockItemFound = FALSE, closed = FALSE;
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_compound_statement)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_compound_statement){
				r = TRUE;
				//
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_Punctuator){
							NBASSERT(child->iChildSeq == 0)
							NBASSERT(partDef->idx < state->sintxDefs.punctsSz)
							const char* punctuator = state->sintxDefs.puncts[partDef->idx];
							//PRINTF_INFO("Punctuator '%s'\n", punctuator);
							if(NBString_strIsEqual(punctuator, "{")){
								NBASSERT(!opened)
								opened = TRUE;
							} else if(NBString_strIsEqual(punctuator, "}")){
								NBASSERT(opened)
								NBASSERT(!closed)
								closed = TRUE;
							}
						} else if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							if(partDef->idx == ENSrzAnlzSintxC_block_item_list){
								NBASSERT(opened)
								NBASSERT(!closed)
								blockItemFound = TRUE;
								//Link subScope to the current subscope
								if(child->iValue == 0){
									r = FALSE; NBASSERT(FALSE) //All 'block-item-list' should be parsed in its own parsingScope
								} else {
									NBASSERT(child->iValue > 0 && child->iValue < dstCode->defsDeclarsScopes.use)
									NBASSERT(child->iValue == (srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count))
									const STSrcAnlzCodeCDeclarsScope* subBodyDef = NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, child->iValue);
									//PRINTF_INFO("subscope's members already stored in definition %d, rangeUsrTypes(%d, +%d), rangeDeclars(%d, +%d), rangeSubscopes(%d, +%d).\n", child->iValue, subBodyDef->usrTypesRng.start, subBodyDef->usrTypesRng.count, subBodyDef->declarsRng.start, subBodyDef->declarsRng.count, subBodyDef->scopesRng.start, subBodyDef->scopesRng.count);
									NBASSERT(subBodyDef->iParentScope == iDeclScope)
									NBASSERT(subBodyDef->iDeepLvl == (NBArray_itmPtrAtIndex(&dstCode->defsDeclarsScopes, STSrcAnlzCodeCDeclarsScope, iDeclScope)->iDeepLvl + 1))
									srcScope->stats.usrTypesRng.count	+= subBodyDef->usrTypesRng.count;
									srcScope->stats.declarsRng.count	+= subBodyDef->declarsRng.count;
									srcScope->stats.scopesRng.count		+= subBodyDef->scopesRng.count + 1; //+1, the subscope itself
									NBASSERT((srcScope->stats.declarsRng.start + srcScope->stats.declarsRng.count) <= dstCode->defsDeclars.use)
									NBASSERT((srcScope->stats.usrTypesRng.start + srcScope->stats.usrTypesRng.count) <= dstCode->defsUsrTypes.use)
									NBASSERT((srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count) <= dstCode->defsDeclarsScopes.use)
								}
							} else {
								r = FALSE; NBASSERT(FALSE) //Unexpected sintaxDef type
								break;
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
				//
				//Empty block-item-list
				if(r && !blockItemFound && closed){
					NBASSERT(opened)
					const UI32 iBodyDef = srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count;
					STSrcAnlzCodeCDeclarsScope* scopeDef = NULL; BOOL isNewDef = FALSE;
					if(!SrcAnlzCodeC_getDeclarScopeByIdx(dstCode, iBodyDef, iDeclScope, &scopeDef, &isNewDef)){
						r = FALSE; NBASSERT(FALSE)
					} else {
						if(isNewDef){
							//Declar was added at this call
						} else {
							//Declar was added at prev call
							NBASSERT(scopeDef->usrTypesRng.count == 0) //Must be a empty scope
							NBASSERT(scopeDef->declarsRng.count == 0) //Must be a empty scope
							NBASSERT(scopeDef->scopesRng.count == 0) //Must be a empty scope
						}
						srcScope->stats.scopesRng.count++;
						NBASSERT((srcScope->stats.scopesRng.start + srcScope->stats.scopesRng.count) <= dstCode->defsDeclarsScopes.use)
					}
				}
			}
		}
	}
	return r;
}

//, { "expression-statement", ";" }
//, { "expression-statement", "expression ;" }
BOOL SrcAnlzCodeCParser_consumeStatementExpression(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_expression_statement)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_expression_statement){
				r = TRUE;
				//
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							if(partDef->idx == ENSrzAnlzSintxC_expression){
								//ToDo
							} else {
								r = FALSE; NBASSERT(FALSE) //Unexpected sintaxDef type
								break;
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
		}
	}
	return r;
}

//, { "selection-statement", "if ( expression ) statement" }
//, { "selection-statement", "if ( expression ) statement else statement" }
//, { "selection-statement", "switch ( expression ) statement" }
BOOL SrcAnlzCodeCParser_consumeStatementSelection(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_selection_statement)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_selection_statement){
				r = TRUE;
				//
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							if(partDef->idx == ENSrzAnlzSintxC_statement){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeStatement(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_expression){
								//ToDo
							} else {
								r = FALSE; NBASSERT(FALSE) //Unexpected sintaxDef type
								break;
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
		}
	}
	return r;
}

//, { "iteration-statement", "while ( expression ) statement" }
//, { "iteration-statement", "do statement while ( expression ) ;" }
//, { "iteration-statement", "for ( ; ; ) statement" }
//, { "iteration-statement", "for ( expression ; ; ) statement" }
//, { "iteration-statement", "for ( ; expression ; ) statement" }
//, { "iteration-statement", "for ( ; ; expression ) statement" }
//, { "iteration-statement", "for ( expression ; expression ; ) statement" }
//, { "iteration-statement", "for ( ; expression ; expression ) statement" }
//, { "iteration-statement", "for ( expression ; ; expression ) statement" }
//, { "iteration-statement", "for ( expression ; expression ; expression ) statement" }
//, { "iteration-statement", "for ( declaration ; ) statement" }
//, { "iteration-statement", "for ( declaration expression ; ) statement" }
//, { "iteration-statement", "for ( declaration ; expression ) statement" }
//, { "iteration-statement", "for ( declaration expression ; expression ) statement" }
BOOL SrcAnlzCodeCParser_consumeStatementIteration(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_iteration_statement)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_iteration_statement){
				r = TRUE;
				//
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							if(partDef->idx == ENSrzAnlzSintxC_declaration){
								if(child->iChildSeq != 0){
									//ToDo: create a subDeclarsScope if a declaration is found, to ensure this declaration is only visible to the statement
									if(!SrcAnlzCodeCParser_consumeDeclaration(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_statement){
								if(child->iChildSeq != 0){
									if(!SrcAnlzCodeCParser_consumeStatement(state, srcScope, dstCode, child->iChildSeq, iDeclScope)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
								}
							} else if(partDef->idx == ENSrzAnlzSintxC_expression){
								//ToDo
							} else {
								r = FALSE; NBASSERT(FALSE) //Unexpected sintaxDef type
								break;
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
		}
	}
	return r;
}

//, { "jump-statement", "goto identifier ;" }
//, { "jump-statement", "continue ;" }
//, { "jump-statement", "break ;" }
//, { "jump-statement", "return ;" }
//, { "jump-statement", "return expression ;" }
BOOL SrcAnlzCodeCParser_consumeStatementJump(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope){
	BOOL r = FALSE;
	if(state != NULL && srcScope != NULL && dstCode != NULL){
		const STSrcAnlzSintxParseTree* tree = &srcScope->parseTree;
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(iFirstChild < nodes->use)
		if(iFirstChild < nodes->use){
			UI32 iPart = 0;
			UI16 iChild = iFirstChild;
			const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
			const STSrcAnlzSintxSeq* sintaxDefSeq = &state->sintxDefs.defsSeqs[child->iSintaxDefSeq];
			NBASSERT(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_jump_statement)
			if(sintaxDefSeq->iDefEnum == ENSrzAnlzSintxC_jump_statement){
				r = TRUE;
				//
				do {
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if(iPart >= sintaxDefSeq->partsSz){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
						//Process
						if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
							if(partDef->idx == ENSrzAnlzSintxC_expression){
								//ToDo
							} else {
								r = FALSE; NBASSERT(FALSE) //Unexpected sintaxDef type
								break;
							}
						}
						//Next
						if(child->iNextPart == 0){
							break;
						} else {
							iChild	= child->iNextPart;
							child	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
							iPart++;
						}
					}
					NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
				} while(TRUE);
			}
		}
	}
	return r;
}
