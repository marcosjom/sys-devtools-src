//
//  NBSrcAnlzSintxParseTree.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_SINTX_PARSE_H
#define NB_SRCANLZ_SINTX_PARSE_H

#include "nb/NBFrameworkDefs.h"
//
#include "NBSrcAnlzSintx.h"		//for 'STSrcAnlzSintxPart'
#include "NBSrcAnlzParserI.h"	//for 'STSrcAnlzParserI'
//

#ifdef __cplusplus
//extern "C" {
#endif

// Sintax parsing node (sintax-seq posibilities tree)

typedef struct STSrcAnlzSintxParseTreeNode_ {
	UI16	refsCount;			//This node references count (when 0 the node can be reused)
	UI16	iSintaxDefSeq;		//Idx of item at "STSrcAnlzSintxSeq __globalSintxCDefSeqs[]" arrays
	UI32	iValue;				//Value of this node (could be a enumeration-def, struct-def, block-of-code, etc...)
	//Branches of the tree
	UI16	iNextPart;			//Next part in this definitionnextPartDef
	UI16	iChildSeq;			//Child that starts the depper level (definition of this part)
} STSrcAnlzSintxParseTreeNode;

// Sintax parsing tree (sintax-seq posibilities)

typedef struct STSrcAnlzSintxParseTree_ {
	STNBArray	nodes;			//STSrcAnlzSintxParseTreeNode
	STNBArray	freeNodesIdxs;	//UI16
} STSrcAnlzSintxParseTree;

void SrcAnlzSintxParseTree_init(STSrcAnlzSintxParseTree* tree);
void SrcAnlzSintxParseTree_release(STSrcAnlzSintxParseTree* tree);

void SrcAnlzSintxParseTree_reset(STSrcAnlzSintxParseTree* tree);

//Retain-release nodes
BOOL SrcAnlzSintxParseTree_retainChildsNodes(STSrcAnlzSintxParseTree* tree, const UI16 iFirstChildSeq);
BOOL SrcAnlzSintxParseTree_releaseChildsNodes(STSrcAnlzSintxParseTree* tree, const UI16 iFirstChildSeq);

//Find
BOOL SrcAnlzSintxParseTree_defEnumCanStartWith(const STSrcAnlzSintxDefs* sintxDef, const SI32 iDefEnum, const STSrcAnlzSintxPart startPart);
BOOL SrcAnlzSintxParseTree_defSeqCanStartWith(const STSrcAnlzSintxDefs* sintxDef, const SI32 iDefSeq, const STSrcAnlzSintxPart startPart);
BOOL SrcAnlzSintxParseTree_nextRequiredPosibsParts(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree, STSrcAnlzSintxPart* dstPartsReqs, const UI16 dstPartsReqsSz, UI16* dstPartsReqsFoundCount);

//Modify tree
BOOL SrcAnlzSintxParseTree_addRootPosibilities(const STSrcAnlzSintxDefs* sintxDef, STSrcAnlzSintxParseTree* tree, const SI32 iDefEnum, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue);
BOOL SrcAnlzSintxParseTree_continuePosibilities(const STSrcAnlzSintxDefs* sintxDef, STSrcAnlzSintxParseTree* tree, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue, UI16* dstAnyWasCompletePosib);
BOOL SrcAnlzSintxParseTree_removePosibilitiesEndedByDefEnum(const STSrcAnlzSintxDefs* sintxDef, STSrcAnlzSintxParseTree* tree, const SI32 iDefEnum, UI32* dstRemovedCount);

//Validate tree
#ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
BOOL SrcAnlzSintxParseTree_dbgValidate(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree);
#endif

//Print tree
void SrcAnlzSintxParseTree_print(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree);
void SrcAnlzSintxParseTree_printChilds(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree, const UI16 iParentNode);



#ifdef __cplusplus
//}
#endif

#endif
