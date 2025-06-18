//
//  NBSrcAnlzCodeCParserConsume.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_CODE_C_PARSER_CONSUME_H
#define NB_SRCANLZ_CODE_C_PARSER_CONSUME_H

#include "nb/NBFrameworkDefs.h"
//
#include "NBSrcAnlzCodeCParser.h" //for 'STSrcAnlzCodeCParser'

#ifdef __cplusplus
//extern "C" {
#endif

//Find
BOOL SrcAnlzCodeCParser_findDeclaratorIdentifierNode(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, const UI16 iFirstChild, const char** dstName);

//Process
BOOL SrcAnlzCodeCParser_consume(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild);
BOOL SrcAnlzCodeCParser_consumeBlockItem(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);

//Process common
BOOL SrcAnlzCodeCParser_consumeSpecifierQualifierList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, STSrcAnlzCodeCDeclarType* dstType, UI32* dstSubmask);

//Process declarations
BOOL SrcAnlzCodeCParser_consumeDeclarationSpecifiers(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, STSrcAnlzCodeCDeclarType* dstType, UI32* dstSubmask, const BOOL forceNewUsrTypes);
BOOL SrcAnlzCodeCParser_consumeTypeQualifierList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, UI32* dstSubmask);
BOOL SrcAnlzCodeCParser_consumeFunctionDefinition(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeDeclaration(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeInitDeclaratorList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const STSrcAnlzCodeCDeclarType typeBase);
BOOL SrcAnlzCodeCParser_consumeDeclarator(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const UI32 iDeclarDef);
BOOL SrcAnlzCodeCParser_consumeAbstractDeclarator(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const UI32 iDeclarDef);
BOOL SrcAnlzCodeCParser_consumePointer(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const UI32 iDeclarDef);
BOOL SrcAnlzCodeCParser_consumeDeclaratorParamIdentifierList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeDeclaratorParamTypeList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeTypedefName(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, UI32* dstIdx);
BOOL SrcAnlzCodeCParser_consumeEnumSpecifier(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, UI32* dstIdx, const BOOL forceNewUsrTypes);
BOOL SrcAnlzCodeCParser_consumeEnumeratorList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const UI32 iEnumDef);
BOOL SrcAnlzCodeCParser_consumeStructOrUnionSpecifier(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, UI32* dstIdx, const BOOL forceNewUsrTypes);
BOOL SrcAnlzCodeCParser_consumeStructDeclarationList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeStructDeclaration(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeStructDeclaratorList(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const STSrcAnlzCodeCDeclarType typeBase);
BOOL SrcAnlzCodeCParser_consumeStructDeclarator(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope, const UI16 iDeclarDef);

//Process statements
BOOL SrcAnlzCodeCParser_consumeStatement(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeStatementLabeled(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeStatementCompound(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeStatementExpression(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeStatementSelection(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeStatementIteration(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);
BOOL SrcAnlzCodeCParser_consumeStatementJump(const STSrcAnlzCodeCParser* state, STSrcAnlzCodeCParseScope* srcScope, STSrcAnlzCodeC* dstCode, const UI16 iFirstChild, const UI32 iDeclScope);

#ifdef __cplusplus
//}
#endif

#endif
