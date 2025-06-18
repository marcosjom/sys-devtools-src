//
//  NBSrcAnlzSintxParseTree.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "nb/core/NBArraySorted.h"
#include "NBSrcAnlzSintxParseTree.h"
#include "NBSrcAnlzTokn.h"
#include "NBSrcAnlzMacro.h"

void SrcAnlzSintxParseTree_init(STSrcAnlzSintxParseTree* tree){
	NBArray_init(&tree->nodes, sizeof(STSrcAnlzSintxParseTreeNode), NULL);
	NBArray_init(&tree->freeNodesIdxs, sizeof(UI16), NBCompareUI16);
	//Add first root node
	STSrcAnlzSintxParseTreeNode firstNode;
	firstNode.refsCount		= 1;
	firstNode.iSintaxDefSeq	= 0xFFFF;
	firstNode.iValue		= 0;
	firstNode.iNextPart		= 0;
	firstNode.iChildSeq		= 0;
	NBArray_addValue(&tree->nodes, firstNode);
}

void SrcAnlzSintxParseTree_release(STSrcAnlzSintxParseTree* tree){
	NBArray_release(&tree->nodes);
	NBArray_release(&tree->freeNodesIdxs);
}

void SrcAnlzSintxParseTree_reset(STSrcAnlzSintxParseTree* tree){
	NBArray_empty(&tree->nodes);
	NBArray_empty(&tree->freeNodesIdxs);
	//Add first root node
	STSrcAnlzSintxParseTreeNode firstNode;
	firstNode.refsCount		= 1;
	firstNode.iSintaxDefSeq	= 0xFFFF;
	firstNode.iValue		= 0;
	firstNode.iNextPart		= 0;
	firstNode.iChildSeq		= 0;
	NBArray_addValue(&tree->nodes, firstNode);
}

//--------------------------
// Retain-release nodes
//--------------------------

BOOL SrcAnlzSintxParseTree_retainChildsNodes(STSrcAnlzSintxParseTree* tree, const UI16 iFirstChildSeq){
	BOOL r = FALSE;
	STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
	NBASSERT(iFirstChildSeq < nodes->use)
	if(iFirstChildSeq < nodes->use){
		r = TRUE;
		UI16 iChild = iFirstChildSeq;
		STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
		do {
			if(child->refsCount == 0){
				//Cannot relase a already released node
				r = FALSE; NBASSERT(FALSE)
				break;
			} else {
				//Increment references
				child->refsCount++;
				//Retain childs
				if(child->iChildSeq != 0){
					if(!SrcAnlzSintxParseTree_retainChildsNodes(tree, child->iChildSeq)){
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
				}
			}
		} while(TRUE);
	}
	return r;
}

BOOL SrcAnlzSintxParseTree_releaseChildsNodes(STSrcAnlzSintxParseTree* tree, const UI16 iFirstChildSeq){
	BOOL r = FALSE;
	STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
	STNBArray* freeNodesIdxs = &tree->freeNodesIdxs; //UI16
	NBASSERT(iFirstChildSeq < nodes->use)
	if(iFirstChildSeq < nodes->use){
		r = TRUE;
		UI16 iChild = iFirstChildSeq;
		STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
		do {
			if(child->refsCount == 0){
				//Cannot relase a already released node
				r = FALSE; NBASSERT(FALSE)
				break;
			} else {
				//Decrease references
				child->refsCount--;
				if(child->refsCount == 0){
					//Release node
					NBArray_addValue(freeNodesIdxs, iChild);
				}
				//Release childs
				if(child->iChildSeq != 0){
					if(!SrcAnlzSintxParseTree_releaseChildsNodes(tree, child->iChildSeq)){
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
				}
			}
		} while(TRUE);
	}
	return r;
}

//--------------------------
// Find
//--------------------------

BOOL SrcAnlzSintxParseTree_defEnumCanStartWith(const STSrcAnlzSintxDefs* sintxDef, const SI32 iDefEnum, const STSrcAnlzSintxPart startPart){
	BOOL r = FALSE;
	NBASSERT(startPart.type >= 0 && startPart.type < ENSrzAnlzSintxPartType_Count)
	const STSrcAnlzSintx* sintaxDef = &sintxDef->defs[iDefEnum];
	if(sintaxDef->deepHints != NULL && sintaxDef->deepHintsSz > 0){
		//Binary Search (should be ordered)
		SI32 rIdx			= -1;
		SI32 posStart		= 0;
		SI32 posEnd			= (sintaxDef->deepHintsSz - 1);
		SI32 posMid;
		while(posStart <= posEnd){
			posMid			= posStart + ((posEnd - posStart)/2);
			if(STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &sintaxDef->deepHints[posMid], &startPart, sizeof(startPart))){
				rIdx		= posMid; break;
			} else {
				if(STSrcAnlzSintxPart_compare(ENCompareMode_Lower, &startPart, &sintaxDef->deepHints[posMid], sizeof(startPart))){
					posEnd	= posMid - 1;
				} else {
					posStart = posMid + 1;
				}
			}
		}
		//
		r = (rIdx != -1);
	}
	return r;
}

BOOL SrcAnlzSintxParseTree_defSeqCanStartWith(const STSrcAnlzSintxDefs* sintxDef, const SI32 iDefSeq, const STSrcAnlzSintxPart startPart){
	BOOL r = FALSE;
	NBASSERT(startPart.type >= 0 && startPart.type < ENSrzAnlzSintxPartType_Count)
	const STSrcAnlzSintxSeq* sintaxDefSeq = &sintxDef->defsSeqs[iDefSeq];
	if(sintaxDefSeq->deepHints != NULL && sintaxDefSeq->deepHintsSz > 0){
		//Binary Search (should be ordered)
		SI32 rIdx			= -1;
		SI32 posStart		= 0;
		SI32 posEnd			= (sintaxDefSeq->deepHintsSz - 1);
		SI32 posMid;
		while(posStart <= posEnd){
			posMid			= posStart + ((posEnd - posStart)/2);
			const STSrcAnlzSintxPart* midPart = &sintaxDefSeq->deepHints[posMid];
			if(STSrcAnlzSintxPart_compare(ENCompareMode_Equal, midPart, &startPart, sizeof(startPart))){
				rIdx		= posMid; break;
			} else {
				if(STSrcAnlzSintxPart_compare(ENCompareMode_Lower, &startPart, midPart, sizeof(startPart))){
					posEnd	= posMid - 1;
				} else {
					posStart = posMid + 1;
				}
			}
		}
		//
		r = (rIdx != -1);
	}
	return r;
}

BOOL SrcAnlzSintxParseTree_nextRequiredPosibsPartsChilds(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree, const UI16 iFirstChild, STSrcAnlzSintxPart* dstPartsReqs, const UI16 dstPartsReqsSz, UI16* dstPartsReqsFoundCount, BOOL* dstSeqWasCompleted){
	BOOL r = FALSE;
	const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
	if(iFirstChild < nodes->use){
		r = TRUE;
		const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iFirstChild);
		NBASSERT(child->iSintaxDefSeq >= 0 && child->iSintaxDefSeq < sintxDef->defsSeqsSz)
		const STSrcAnlzSintxSeq* sintaxDefSeq = &sintxDef->defsSeqs[child->iSintaxDefSeq];
		BOOL seqWasCompleted = FALSE;
		UI16 partsReqsFoundCount = 0; if(dstPartsReqsFoundCount != NULL) partsReqsFoundCount = *dstPartsReqsFoundCount;
		UI16 iPart = 0;
		do {
			//Next
			if(child->iNextPart != 0){
				//Ignore, move to last child in this seq
				child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, child->iNextPart);
				iPart++;
			} else {
				//Analyze, is the last added part
				NBASSERT(iPart < sintaxDefSeq->partsSz)
				const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
				BOOL isComplete = FALSE;
				if(child->iChildSeq == 0){
					isComplete =  TRUE;
				} else {
					if(!SrcAnlzSintxParseTree_nextRequiredPosibsPartsChilds(sintxDef, tree, child->iChildSeq, dstPartsReqs, dstPartsReqsSz, &partsReqsFoundCount, &isComplete)){
						r = FALSE; NBASSERT(FALSE)
						break;
					}
				}
				//
				if(isComplete){
					//Analyze recursive definitions
					if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
						NBASSERT(partDef->idx >= 0 && partDef->idx < sintxDef->defsSeqsRngsSz)
						const STSrcAnlzRange* rng = &sintxDef->defsSeqsRngs[partDef->idx];
						NBASSERT(rng->start >= 0 && (rng->start + rng->count) <= sintxDef->defsSeqsSz)
						SI32 i; for(i = 0; i < rng->count; i++){
							const SI32 iSintaxDefSeq = (rng->start + i);
							const STSrcAnlzSintxSeq* sintaxDefSeq = &sintxDef->defsSeqs[iSintaxDefSeq];
							NBASSERT(partDef->idx == sintaxDefSeq->iDefEnum)
							NBASSERT(sintaxDefSeq->parts != NULL && sintaxDefSeq->partsSz > 0)
							if(sintaxDefSeq->parts != NULL && sintaxDefSeq->partsSz > 1){
								const STSrcAnlzSintxPart* firstPartDef = &sintaxDefSeq->parts[0];
								if(STSrcAnlzSintxPart_compare(ENCompareMode_Equal, firstPartDef, partDef, sizeof(*firstPartDef))){
									//Recursive definition
									const STSrcAnlzSintxPart* secondPartDef = &sintaxDefSeq->parts[1];
									//Add next part definition
									if(partsReqsFoundCount < dstPartsReqsSz){
										UI16 i; for(i = 0; i < partsReqsFoundCount; i++){
											const STSrcAnlzSintxPart* dstPart = &dstPartsReqs[i];
											if(STSrcAnlzSintxPart_compare(ENCompareMode_Equal, secondPartDef, dstPart, sizeof(*secondPartDef))){
												break;
											}
										}
										if(i == partsReqsFoundCount){
											//Add this part definition (not duplicated)
											dstPartsReqs[i] = *secondPartDef;
											partsReqsFoundCount++;
										}
									}
								}
							}
						}
					}
					//Analyze next part
					NBASSERT(iPart < sintaxDefSeq->partsSz)
					if((iPart + 1) == sintaxDefSeq->partsSz){
						//This sequence is completed
						seqWasCompleted = TRUE;
					} else {
						//Add next part definition
						if(partsReqsFoundCount < dstPartsReqsSz){
							const STSrcAnlzSintxPart* nextPartDef = &sintaxDefSeq->parts[iPart + 1];
							UI16 i; for(i = 0; i < partsReqsFoundCount; i++){
								const STSrcAnlzSintxPart* dstPart = &dstPartsReqs[i];
								if(STSrcAnlzSintxPart_compare(ENCompareMode_Equal, nextPartDef, dstPart, sizeof(*nextPartDef))){
									break;
								}
							}
							if(i == partsReqsFoundCount){
								//Add this part definition (not duplicated)
								dstPartsReqs[i] = *nextPartDef;
								partsReqsFoundCount++;
							}
						}
					}
				}
				break;
			}
		} while(TRUE);
		if(dstSeqWasCompleted != NULL) *dstSeqWasCompleted = seqWasCompleted;
		if(dstPartsReqsFoundCount != NULL) *dstPartsReqsFoundCount = partsReqsFoundCount;
	}
	return r;
}

BOOL SrcAnlzSintxParseTree_nextRequiredPosibsParts(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree, STSrcAnlzSintxPart* dstPartsReqs, const UI16 dstPartsReqsSz, UI16* dstPartsReqsFoundCount){
	BOOL r = FALSE;
	if(tree != NULL){
		const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
		if(nodes->use > 0){
			r = TRUE;
			UI16 partsReqsFoundCount = 0;
			//Run all posibilities
			const STSrcAnlzSintxParseTreeNode* rootNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, 0);
			if(rootNode->iNextPart != 0){
				do {
					//Next
					rootNode	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, rootNode->iNextPart);
					//Process
					if(rootNode->iChildSeq != 0){
						BOOL seqWasCompleted = FALSE;
						if(!SrcAnlzSintxParseTree_nextRequiredPosibsPartsChilds(sintxDef, tree, rootNode->iChildSeq, dstPartsReqs, dstPartsReqsSz, &partsReqsFoundCount, &seqWasCompleted)){
							r = FALSE; NBASSERT(FALSE)
							break;
						}
						//TODO: analyze here if this part is recursive
					}
				} while(rootNode->iNextPart != 0);
			}
			if(dstPartsReqsFoundCount != NULL) *dstPartsReqsFoundCount = partsReqsFoundCount;
		}
	}
	return r;
}

//-------------------------
// Modify tree
//-------------------------

// Clone posibility

BOOL SrcAnlz_sintxParseCloneChilds(STSrcAnlzSintxParseTree* dstTree, const UI16 iBaseParent, const UI16 iNewParent, const UI16 iNodeStop, UI16* dstNewNodeStop, BOOL* dstStopWasFound, const UI16 iNodeToTrack, UI16* dstNewNodeTracked){
	BOOL r = FALSE;
	if(dstTree != NULL){
		STNBArray* nodes = &dstTree->nodes; //STSrcAnlzSintxParseTreeNode
		STNBArray* freeNodesIdxs = &dstTree->freeNodesIdxs; //UI16
		if(nodes->use > 0){
			NBASSERT(iBaseParent != 0 && iNewParent != 0)
			if(iBaseParent != 0 && iNewParent != 0){
				r = TRUE;
				const STSrcAnlzSintxParseTreeNode baseParent = NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iBaseParent);
				NBASSERT(baseParent.refsCount != 0)
				if(baseParent.iChildSeq != 0){
					UI16 iPrevNewNode = 0;
					UI16 iBaseChild = baseParent.iChildSeq;
					STSrcAnlzSintxParseTreeNode baseChild = NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iBaseChild);
					do {
						NBASSERT(baseChild.refsCount != 0)
						//Add clone
						STSrcAnlzSintxParseTreeNode node;
						node.refsCount		= 1; //Referenced once below
						node.iSintaxDefSeq	= baseChild.iSintaxDefSeq;
						node.iValue			= baseChild.iValue;
						node.iNextPart		= 0;
						node.iChildSeq		= 0;
						UI16 iNewNode;
						if(freeNodesIdxs->use > 0){
							NBASSERT(NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1) < nodes->use)
							NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1))->refsCount == 0)
							iNewNode		= NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1);
							NBArray_removeItemAtIndex(freeNodesIdxs, freeNodesIdxs->use - 1);
							NBArray_setItemAt(nodes, iNewNode, &node, sizeof(node));
						} else {
							iNewNode	= nodes->use;
							NBArray_addValue(nodes, node); NBASSERT(nodes->use < 0xFFFF)
						}
						//Link new node
						if(iPrevNewNode == 0){
							NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNewParent)->iChildSeq = iNewNode; //Link to parent start of child
						} else {
							NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iPrevNewNode)->iNextPart = iNewNode; //Link to previous part
						}
						//Track cloned node
						if(iNodeToTrack != 0 && iNodeToTrack == iBaseChild){
							if(dstNewNodeTracked != NULL) *dstNewNodeTracked = iNewNode;
						}
						//Stop copy
						if(iNodeStop != 0 && iNodeStop == iBaseChild){
							NBASSERT(iNewNode != 0)
							if(dstNewNodeStop != NULL) *dstNewNodeStop = iNewNode;
							if(dstStopWasFound != NULL) *dstStopWasFound = TRUE;
							break;
						}
						//Clone childs (if necesary)
						if(baseChild.iNextPart != 0){
							NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, baseChild.iChildSeq)->refsCount > 0)
							//When is not last-part, reuse the original childs-seq (do not clone)
							NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNewNode)->iChildSeq = baseChild.iChildSeq;
							if(baseChild.iChildSeq != 0){
								if(!SrcAnlzSintxParseTree_retainChildsNodes(dstTree, baseChild.iChildSeq)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						} else if(baseChild.iChildSeq != 0){
							//Is last-part: clone childs-seq (cand be incompleted)
							BOOL stopWasFound = FALSE;
							if(!SrcAnlz_sintxParseCloneChilds(dstTree, iBaseChild, iNewNode, iNodeStop, dstNewNodeStop, &stopWasFound, iNodeToTrack, dstNewNodeTracked)){
								r = FALSE; NBASSERT(FALSE)
								break;
							} else if(stopWasFound){
								if(dstStopWasFound != NULL) *dstStopWasFound = TRUE;
								break;
							}
						}
						//Next part
						if(baseChild.iNextPart == 0){
							break;
						} else {
							//Next part
							iPrevNewNode	= iNewNode;
							iBaseChild		= baseChild.iNextPart;
							baseChild		= NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iBaseChild);
						}
					} while(TRUE);
				}
			}
		}
	}
	return r;
}
	
BOOL SrcAnlz_sintxParseClonePosibility(STSrcAnlzSintxParseTree* dstTree, const UI16 iBaseRootNode, UI16* iPrevRootNode, UI16* dstNewRootNode, const UI16 iNodeStop, UI16* dstNewNodeStop, const UI16 iNodeToTrack, UI16* dstNewNodeTracked){
	BOOL r = FALSE;
	if(dstTree != NULL){
		STNBArray* nodes = &dstTree->nodes; //STSrcAnlzSintxParseTreeNode
		STNBArray* freeNodesIdxs = &dstTree->freeNodesIdxs; //UI16
		if(nodes->use > 0){
			if(iBaseRootNode != 0){
				const STSrcAnlzSintxParseTreeNode baseRootNode = NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iBaseRootNode);
				NBASSERT(baseRootNode.iSintaxDefSeq == 0xFFFF)
				//Create root node
				UI16 iNewRootNode;
				{
					STSrcAnlzSintxParseTreeNode node;
					node.refsCount		= 1;	//Referenced once below
					node.iSintaxDefSeq	= 0xFFFF;
					node.iValue			= 0;
					node.iNextPart		= iBaseRootNode;
					node.iChildSeq		= 0;
					if(freeNodesIdxs->use > 0){
						NBASSERT(NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1) < nodes->use)
						NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1))->refsCount == 0)
						iNewRootNode	= NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1);
						NBArray_removeItemAtIndex(freeNodesIdxs, freeNodesIdxs->use - 1);
						NBArray_setItemAt(nodes, iNewRootNode, &node, sizeof(node));
					} else {
						iNewRootNode	= nodes->use;
						NBArray_addValue(nodes, node); NBASSERT(nodes->use < 0xFFFF)
					}
				}
				if(iNodeToTrack != 0 && iNodeToTrack == iBaseRootNode){
					if(dstNewNodeTracked != NULL) *dstNewNodeTracked = iNewRootNode;
				}
				if(iNodeStop != 0 && iNodeStop == iBaseRootNode){
					//Only cloning root node
					NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, (iPrevRootNode == NULL ? 0 : *iPrevRootNode))->iNextPart == iBaseRootNode)
					NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, (iPrevRootNode == NULL ? 0 : *iPrevRootNode))->iNextPart = iNewRootNode;
					if(dstNewNodeStop != NULL) *dstNewNodeStop = iNewRootNode;
					r = TRUE;
				} else {
					//Clone childs
					if(SrcAnlz_sintxParseCloneChilds(dstTree, iBaseRootNode, iNewRootNode, iNodeStop, dstNewNodeStop, NULL, iNodeToTrack, dstNewNodeTracked)){
						//Update linking (insert new rootNode between prevRoot and baseRoot nodes)
						NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, (iPrevRootNode == NULL ? 0 : *iPrevRootNode))->iNextPart == iBaseRootNode)
						NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, (iPrevRootNode == NULL ? 0 : *iPrevRootNode))->iNextPart = iNewRootNode;
						r = TRUE;
					} else {
						NBASSERT(FALSE)
					}
				}
				if(dstNewRootNode != NULL) *dstNewRootNode = iNewRootNode;
				if(iPrevRootNode != NULL) *iPrevRootNode = iNewRootNode;
			}
		}
	}
	return r;
}

// Add posibilities

BOOL SrcAnlz_sintxParseAddPosibilitiesColumnPriv(const STSrcAnlzSintxDefs* sintxDef, STSrcAnlzSintxParseTree* dstTree, UI16* pRootNode, UI16* pPrevRootNode, const UI16 pParentNode, const SI32 iDefEnum, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue, const BOOL addRecursiveDefs, BOOL* dstPartFoundWasConsumed){
	BOOL r = FALSE;
	NBASSERT(dstTree != NULL)
	STNBArray* nodes = &dstTree->nodes; //STSrcAnlzSintxParseTreeNode
	STNBArray* freeNodesIdxs = &dstTree->freeNodesIdxs; //UI16
	NBASSERT(nodes->use > 0)
	if(dstTree != NULL){
		if(nodes->use > 0){
			r = TRUE;
			UI16 iRootNode		= 0; if(pRootNode != NULL) iRootNode = *pRootNode;
			UI16 iPrevRootNode	= 0; if(pPrevRootNode != NULL) iPrevRootNode = *pPrevRootNode;
			UI16 iParentNode	= pParentNode;
			BOOL partFoundWasConsumed = FALSE;
			NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iRootNode)->iSintaxDefSeq == 0xFFFF)
			NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iPrevRootNode)->iSintaxDefSeq == 0xFFFF)
			NBASSERT(iDefEnum >= 0 && iDefEnum < sintxDef->defsSeqsRngsSz)
			//Analyze range of posibilities
			const STSrcAnlzRange* rng = &sintxDef->defsSeqsRngs[iDefEnum];
			NBASSERT(rng->start >= 0 && (rng->start + rng->count) <= sintxDef->defsSeqsSz)
			SI32 i; for(i = 0; i < rng->count; i++){
				const SI32 iSintaxDefSeq = (rng->start + i);
				const STSrcAnlzSintxSeq* sintaxDefSeq = &sintxDef->defsSeqs[iSintaxDefSeq];
				NBASSERT(iDefEnum == sintaxDefSeq->iDefEnum)
				NBASSERT(sintaxDefSeq->parts != NULL && sintaxDefSeq->partsSz > 0)
				if(sintaxDefSeq->parts != NULL && sintaxDefSeq->partsSz > 0){
					const STSrcAnlzSintxPart* firstPart = &sintaxDefSeq->parts[0];
					//Determine if this part can be a start
					BOOL partStartsSeq = FALSE;
					if(STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, firstPart, sizeof(partFound))){
						partStartsSeq	= TRUE;
					} else {
						//Must be a non-recursive-def-seq
						if(addRecursiveDefs || (firstPart->type != ENSrzAnlzSintxPartType_SintaxDef || firstPart->idx != iDefEnum)){
							if(SrcAnlzSintxParseTree_defSeqCanStartWith(sintxDef, iSintaxDefSeq, partFound)){
								partStartsSeq = TRUE;
							}
						}
					}
					//Add sequence
					if(partStartsSeq){
						//Start root node (if necesary)
						if(iRootNode == 0){
							NBASSERT(iParentNode == 0)
							STSrcAnlzSintxParseTreeNode node;
							node.refsCount		= 1; //Referenced once below
							node.iSintaxDefSeq	= 0xFFFF;
							node.iValue			= 0;
							node.iNextPart		= 0;
							node.iChildSeq		= 0;
							if(freeNodesIdxs->use > 0){
								NBASSERT(NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1) < nodes->use)
								NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1))->refsCount == 0)
								iRootNode		= NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1);
								NBArray_removeItemAtIndex(freeNodesIdxs, freeNodesIdxs->use - 1);
								NBArray_setItemAt(nodes, iRootNode, &node, sizeof(node));
							} else {
								iRootNode		= nodes->use;
								NBArray_addValue(nodes, node); NBASSERT(nodes->use < 0xFFFF)
							}
							iParentNode			= iRootNode;
							//Link to prev root node
							NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iPrevRootNode)->iNextPart == 0) //The prev node, must be the last of the chain of posiblities
							NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iPrevRootNode)->iNextPart = iRootNode;
							if(pRootNode != NULL) *pRootNode = iRootNode;
						}
						NBASSERT(iRootNode != 0)
						NBASSERT(iParentNode != 0)
						if(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iParentNode)->iChildSeq != 0){
							//Clone posibility and continue sequence
							UI16 oldPrevNode = iPrevRootNode, iRootClonedNode = 0, iParentClonedNode = 0;
							if(!SrcAnlz_sintxParseClonePosibility(dstTree, iRootNode, &iPrevRootNode, &iRootClonedNode, iParentNode, &iParentClonedNode, 0, NULL)){
								r = FALSE; NBASSERT(FALSE)
								break;
							} else {
								NBASSERT(iPrevRootNode == iRootClonedNode)
								NBASSERT(iRootClonedNode != 0)
								NBASSERT(iParentClonedNode != 0)
								//Add part to cloned posibility
								STSrcAnlzSintxParseTreeNode node;
								node.refsCount		= 1; //referenced once below
								node.iSintaxDefSeq	= iSintaxDefSeq;
								node.iValue			= (!STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, firstPart, sizeof(partFound)) ? 0 : partIdxValue);
								node.iNextPart		= 0;
								node.iChildSeq		= 0;
								UI16 iNewNode;
								if(freeNodesIdxs->use > 0){
									NBASSERT(NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1) < nodes->use)
									NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1))->refsCount == 0)
									iNewNode		= NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1);
									NBArray_removeItemAtIndex(freeNodesIdxs, freeNodesIdxs->use - 1);
									NBArray_setItemAt(nodes, iNewNode, &node, sizeof(node));
								} else {
									iNewNode		= nodes->use;
									NBArray_addValue(nodes, node); NBASSERT(nodes->use < 0xFFFF)
								}
								//Link to parent
								NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iParentClonedNode)->iChildSeq == 0)
								NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iParentClonedNode)->iChildSeq = iNewNode;
								//Add children (recursively)
								if(!STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, firstPart, sizeof(partFound)) && firstPart->type == ENSrzAnlzSintxPartType_SintaxDef){
									BOOL partFoundWasConsumedAtClone = FALSE;
									if(!SrcAnlz_sintxParseAddPosibilitiesColumnPriv(sintxDef, dstTree, &iRootClonedNode, &oldPrevNode, iNewNode, firstPart->idx, partFound, partIdxValue, FALSE, &partFoundWasConsumedAtClone)){
										r = FALSE; NBASSERT(FALSE)
										break;
									}
									NBASSERT(partFoundWasConsumedAtClone)
								}
								NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNewNode)->iChildSeq != 0 || STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, firstPart, sizeof(partFound)) || firstPart->type != ENSrzAnlzSintxPartType_SintaxDef)
							}
						} else {
							//Add part to this posibility
							STSrcAnlzSintxParseTreeNode node;
							node.refsCount		= 1; //Referenced once below
							node.iSintaxDefSeq	= iSintaxDefSeq;
							node.iValue			= (!STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, firstPart, sizeof(partFound)) ? 0 : partIdxValue);
							node.iNextPart		= 0;
							node.iChildSeq		= 0;
							UI16 iNewNode;
							if(freeNodesIdxs->use > 0){
								NBASSERT(NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1) < nodes->use)
								NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1))->refsCount == 0)
								iNewNode		= NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1);
								NBArray_removeItemAtIndex(freeNodesIdxs, freeNodesIdxs->use - 1);
								NBArray_setItemAt(nodes, iNewNode, &node, sizeof(node));
							} else {
								iNewNode		= nodes->use;
								NBArray_addValue(nodes, node); NBASSERT(nodes->use < 0xFFFF)
							}
							partFoundWasConsumed = TRUE;
							//Link to parent
							NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iParentNode)->iChildSeq == 0)
							NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iParentNode)->iChildSeq = iNewNode;
							//Add children (recursively)
							if(!STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, firstPart, sizeof(partFound)) && firstPart->type == ENSrzAnlzSintxPartType_SintaxDef){
								BOOL partWasConsumedAtDef = FALSE;
								if(!SrcAnlz_sintxParseAddPosibilitiesColumnPriv(sintxDef, dstTree, &iRootNode, &iPrevRootNode, iNewNode, firstPart->idx, partFound, partIdxValue, FALSE, &partWasConsumedAtDef)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
								NBASSERT(partWasConsumedAtDef)
							}
							NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNewNode)->iChildSeq != 0 || STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, firstPart, sizeof(partFound)) || firstPart->type != ENSrzAnlzSintxPartType_SintaxDef)
						}
					}
				}
			}
			if(dstPartFoundWasConsumed != NULL) *dstPartFoundWasConsumed = partFoundWasConsumed;
			if(pPrevRootNode != NULL) *pPrevRootNode = iPrevRootNode;
		}
	}
	return r;
}

BOOL SrcAnlzSintxParseTree_addRootPosibilities(const STSrcAnlzSintxDefs* sintxDef, STSrcAnlzSintxParseTree* dstTree, const SI32 iDefEnum, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue){
	BOOL r = FALSE;
	NBASSERT(dstTree != NULL)
	NBASSERT(partFound.type >= 0 && partFound.type < ENSrzAnlzSintxPartType_Count)
	if(dstTree != NULL){
		STNBArray* nodes = &dstTree->nodes; //STSrcAnlzSintxParseTreeNode
		NBASSERT(nodes->use > 0)
		if(nodes->use > 0){
			//Search for last posib node
			UI16 iPrevRoot = 0;
			const STSrcAnlzSintxParseTreeNode* rootNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, 0);
			if(rootNode->iNextPart != 0){
				do {
					iPrevRoot = rootNode->iNextPart;
					rootNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, rootNode->iNextPart);
				} while(rootNode->iNextPart != 0);
			}
			//Add posibilities (include recursive posibilities)
			UI16 iNewRootNode = 0;
			if(SrcAnlz_sintxParseAddPosibilitiesColumnPriv(sintxDef, dstTree, &iNewRootNode, &iPrevRoot, 0, iDefEnum, partFound, partIdxValue, TRUE, NULL)){
				NBASSERT(SrcAnlzSintxParseTree_dbgValidate(sintxDef, dstTree));
				r = TRUE;
			} else {
				NBASSERT(FALSE)
			}
		}
	}
	return r;
}

//Continue posibilities
	
BOOL SrcAnlzSintxParseTree_continuePosibilitiesChilds(const STSrcAnlzSintxDefs* sintxDef, STSrcAnlzSintxParseTree* dstTree, const UI16 iRootNode, UI16* pPrevRootNode, const UI16 iParentNode, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue, BOOL* dstPartFoundWasConsumed, BOOL* dstChildsSeqWasAlreadyCompleted){
	BOOL r = FALSE;
	NBASSERT(iRootNode != 0)
	NBASSERT(dstTree != NULL)
	STNBArray* nodes = &dstTree->nodes; //STSrcAnlzSintxParseTreeNode
	STNBArray* freeNodesIdxs = &dstTree->freeNodesIdxs; //UI16
	NBASSERT(nodes->use > 0)
	if(dstTree != NULL){
		if(nodes->use > 0){
			r = TRUE;
			BOOL partFoundWasConsumed = FALSE;
			BOOL childsSeqWasAlreadyCompleted = FALSE;
			UI16 childsCount = 0;
			UI16 iPrevRootNode	= 0; if(pPrevRootNode != NULL) iPrevRootNode = *pPrevRootNode;
			NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iRootNode)->iSintaxDefSeq == 0xFFFF)
			NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iPrevRootNode)->iSintaxDefSeq == 0xFFFF)
			//Run childs seq
			const STSrcAnlzSintxParseTreeNode parentNode = NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iParentNode);
			NBASSERT(parentNode.refsCount != 0)
			if(parentNode.iChildSeq != 0){
				UI16 iChildNode = parentNode.iChildSeq;
				STSrcAnlzSintxParseTreeNode childNode = NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChildNode);
				NBASSERT(childNode.iSintaxDefSeq >= 0 && childNode.iSintaxDefSeq < sintxDef->defsSeqsSz)
				const STSrcAnlzSintxSeq* sintaxDefSeq = &sintxDef->defsSeqs[childNode.iSintaxDefSeq];
				do {
					NBASSERT(childNode.refsCount != 0)
					childsCount++;
					if(childNode.iNextPart != 0){
						//Move to the next part in the sequence
						NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChildNode)->iSintaxDefSeq == NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, childNode.iNextPart)->iSintaxDefSeq) //Must be same sequence
						iChildNode	= childNode.iNextPart;
						childNode	= NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChildNode);
					} else {
						//This is the current last-part of the sequence
						NBASSERT(childsCount <= sintaxDefSeq->partsSz)
						const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[childsCount - 1];
						BOOL partWasAlreadyCompleted = FALSE;
						if(partDef->type != ENSrzAnlzSintxPartType_SintaxDef){
							partWasAlreadyCompleted = TRUE;
						} else {
							if(childNode.iChildSeq == 0){
								partWasAlreadyCompleted = TRUE;	//Assuming the childs are defined inside a subscope.
							} else {
								if(!SrcAnlzSintxParseTree_continuePosibilitiesChilds(sintxDef, dstTree, iRootNode, &iPrevRootNode, iChildNode, partFound, partIdxValue, &partFoundWasConsumed, &partWasAlreadyCompleted)){
									r = FALSE; NBASSERT(FALSE)
									break;
								}
							}
						}
						//Try to add next part of the sequence
						if(partWasAlreadyCompleted){
							//Try to continue the part with its recursive definitions (with a clone)
							if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
								NBASSERT(partDef->idx >= 0 && partDef->idx < sintxDef->defsSeqsRngsSz)
								const STSrcAnlzRange* rng = &sintxDef->defsSeqsRngs[partDef->idx];
								NBASSERT(rng->start >= 0 && (rng->start + rng->count) <= sintxDef->defsSeqsSz)
								SI32 i; for(i = 0; i < rng->count; i++){
									const SI32 iSintaxDefSeq = (rng->start + i);
									const STSrcAnlzSintxSeq* sintaxDefSeq = &sintxDef->defsSeqs[iSintaxDefSeq];
									NBASSERT(partDef->idx == sintaxDefSeq->iDefEnum)
									NBASSERT(sintaxDefSeq->parts != NULL && sintaxDefSeq->partsSz > 0)
									if(sintaxDefSeq->parts != NULL && sintaxDefSeq->partsSz > 1){
										const STSrcAnlzSintxPart* firstPartDef = &sintaxDefSeq->parts[0];
										if(STSrcAnlzSintxPart_compare(ENCompareMode_Equal, partDef, firstPartDef, sizeof(*partDef))){
											//Recursive definition
											const STSrcAnlzSintxPart* secondPartDef = &sintaxDefSeq->parts[1];
											BOOL partContinuesSeq	= FALSE;
											if(STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, secondPartDef, sizeof(partFound))){
												partContinuesSeq	= TRUE;
											} else if(secondPartDef->type == ENSrzAnlzSintxPartType_SintaxDef){
												if(SrcAnlzSintxParseTree_defEnumCanStartWith(sintxDef, secondPartDef->idx, partFound)){
													partContinuesSeq = TRUE;
												}
											}
											//Add
											if(partContinuesSeq){
												//Clone and add the new part to the clone's seq
												UI16 oldPrevRoot = iPrevRootNode, iNewRootNode = 0, iClonedChild = 0;
												if(!SrcAnlz_sintxParseClonePosibility(dstTree, iRootNode, &iPrevRootNode, &iNewRootNode, 0, NULL, iChildNode, &iClonedChild)){
													r = FALSE; NBASSERT(FALSE)
													break;
												} else {
													NBASSERT(iNewRootNode != 0)
													NBASSERT(iClonedChild != 0)
													NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNewRootNode)->iSintaxDefSeq == 0xFFFF)
													NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChildNode)->iSintaxDefSeq == NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iClonedChild)->iSintaxDefSeq)
													NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChildNode)->iNextPart == 0 && NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iClonedChild)->iNextPart == 0)
													NBASSERT((NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChildNode)->iChildSeq != 0) == (NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iClonedChild)->iChildSeq != 0))
													NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iClonedChild)->refsCount == 1)
													//Inject recursive start part
													STSrcAnlzSintxParseTreeNode node;
													node.refsCount		= 1; //Referenced once below
													node.iSintaxDefSeq	= iSintaxDefSeq;
													node.iValue			= 0;
													node.iNextPart		= 0;
													node.iChildSeq		= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iClonedChild)->iChildSeq;
													UI16 iNewNode;
													if(freeNodesIdxs->use > 0){
														NBASSERT(NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1) < nodes->use)
														NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1))->refsCount == 0)
														iNewNode		= NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1);
														NBArray_removeItemAtIndex(freeNodesIdxs, freeNodesIdxs->use - 1);
														NBArray_setItemAt(nodes, iNewNode, &node, sizeof(node));
													} else {
														iNewNode		= nodes->use;
														NBArray_addValue(nodes, node); NBASSERT(nodes->use < 0xFFFF)
													}
													NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iClonedChild)->iChildSeq = iNewNode;
													//Add second part
													STSrcAnlzSintxParseTreeNode node2;
													node2.refsCount		= 1; //Referenced once below
													node2.iSintaxDefSeq	= iSintaxDefSeq;
													node2.iValue		= (!STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, secondPartDef, sizeof(partFound)) ? 0 : partIdxValue);
													node2.iNextPart		= 0;
													node2.iChildSeq		= 0;
													UI16 iNewNode2;
													if(freeNodesIdxs->use > 0){
														NBASSERT(NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1) < nodes->use)
														NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1))->refsCount == 0)
														iNewNode2		= NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1);
														NBArray_removeItemAtIndex(freeNodesIdxs, freeNodesIdxs->use - 1);
														NBArray_setItemAt(nodes, iNewNode2, &node2, sizeof(node2));
													} else {
														iNewNode2		= nodes->use;
														NBArray_addValue(nodes, node2); NBASSERT(nodes->use < 0xFFFF)
													}
													NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNewNode)->iNextPart == 0)
													NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNewNode)->iNextPart = iNewNode2;
													//Add posibilities (this creates other clones if necesary)
													if(!STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, secondPartDef, sizeof(partFound)) && secondPartDef->type == ENSrzAnlzSintxPartType_SintaxDef){
														BOOL partWasConsumedAtClone = FALSE;
														UI16 iRootNode2 = iNewRootNode;
														if(!SrcAnlz_sintxParseAddPosibilitiesColumnPriv(sintxDef, dstTree, &iRootNode2, &oldPrevRoot, iNewNode2, secondPartDef->idx, partFound, partIdxValue, FALSE, &partWasConsumedAtClone)){
															r = FALSE; NBASSERT(FALSE)
															break;
														}
														NBASSERT(partWasConsumedAtClone)
													}
													NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNewNode2)->iChildSeq != 0 || STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, secondPartDef, sizeof(partFound)) || secondPartDef->type != ENSrzAnlzSintxPartType_SintaxDef)
												}
											}
										}
									}
								}
							}
							//
							NBASSERT(childsCount <= sintaxDefSeq->partsSz)
							if(childsCount == sintaxDefSeq->partsSz){
								//This childs sequence was already completed
								childsSeqWasAlreadyCompleted = TRUE;
							} else if(!partFoundWasConsumed){
								//Try to continue the seq adding the next part
								NBASSERT(childsCount < sintaxDefSeq->partsSz)
								const STSrcAnlzSintxPart* nextPartDef = &sintaxDefSeq->parts[childsCount];
								//Determine if next part is posible
								BOOL partContinuesSeq	= FALSE;
								if((partFound.idx == nextPartDef->idx && partFound.type == nextPartDef->type)){
									partContinuesSeq	= TRUE;
								} else if(nextPartDef->type == ENSrzAnlzSintxPartType_SintaxDef){
									if(SrcAnlzSintxParseTree_defEnumCanStartWith(sintxDef, nextPartDef->idx, partFound)){
										partContinuesSeq = TRUE;
									}
								}
								//Add part
								if(partContinuesSeq){
									//Add next part
									STSrcAnlzSintxParseTreeNode node;
									node.refsCount		= 1; //Referenced once below
									node.iSintaxDefSeq	= childNode.iSintaxDefSeq;
									node.iValue			= (!STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, nextPartDef, sizeof(partFound)) ? 0 : partIdxValue);
									node.iNextPart		= 0;
									node.iChildSeq		= 0;
									UI16 iNewNode;
									if(freeNodesIdxs->use > 0){
										NBASSERT(NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1) < nodes->use)
										NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1))->refsCount == 0)
										iNewNode		= NBArray_itmValueAtIndex(freeNodesIdxs, UI16, freeNodesIdxs->use - 1);
										NBArray_removeItemAtIndex(freeNodesIdxs, freeNodesIdxs->use - 1);
										NBArray_setItemAt(nodes, iNewNode, &node, sizeof(node));
									} else {
										iNewNode		= nodes->use;
										NBArray_addValue(nodes, node); NBASSERT(nodes->use < 0xFFFF)
									}
									partFoundWasConsumed = TRUE;
									NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChildNode)->iNextPart == 0)
									NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChildNode)->iNextPart = iNewNode;
									//Add posibilities (this creates clones if necesary)
									if(!STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, nextPartDef, sizeof(partFound)) && nextPartDef->type == ENSrzAnlzSintxPartType_SintaxDef){
										BOOL partWasConsumedAtDef = FALSE;
										UI16 iRootNode2 = iRootNode;
										if(!SrcAnlz_sintxParseAddPosibilitiesColumnPriv(sintxDef, dstTree, &iRootNode2, &iPrevRootNode, iNewNode, nextPartDef->idx, partFound, partIdxValue, FALSE, &partWasConsumedAtDef)){
											r = FALSE; NBASSERT(FALSE)
											break;
										}
										NBASSERT(partWasConsumedAtDef)
									}
									NBASSERT(NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNewNode)->iChildSeq != 0 || STSrcAnlzSintxPart_compare(ENCompareMode_Equal, &partFound, nextPartDef, sizeof(partFound)) || nextPartDef->type != ENSrzAnlzSintxPartType_SintaxDef)
/*#								ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
								} else {
 									STNBString tmpStr; NBString_init(&tmpStr);
									NBString_concat(&tmpStr, "FOUND ");
									switch(partFound.type){
										case ENSrzAnlzSintxPartType_Keyword: NBString_concat(&tmpStr, "'"); NBString_concat(&tmpStr, sintxDef->keyws[partFound.idx]); NBString_concat(&tmpStr, "'"); break;
										case ENSrzAnlzSintxPartType_Punctuator: NBString_concat(&tmpStr, "'"); NBString_concat(&tmpStr, sintxDef->puncts[partFound.idx]); NBString_concat(&tmpStr, "'"); break;
										case ENSrzAnlzSintxPartType_Identifier: NBString_concat(&tmpStr, "identifier"); NBASSERT(partFound.idx == -1) break;
										case ENSrzAnlzSintxPartType_Constant: NBString_concat(&tmpStr, "constant"); NBASSERT(partFound.idx == -1) break;
										case ENSrzAnlzSintxPartType_StrLiteral: NBString_concat(&tmpStr, "strLiteral"); NBASSERT(partFound.idx == -1) break;
										default: NBString_concat(&tmpStr, "unknownPartType"); NBASSERT(FALSE) break;
									}
									NBString_concat(&tmpStr, " BUT EXPECTED ");
									switch(nextPartDef->type){
										case ENSrzAnlzSintxPartType_Keyword: NBString_concat(&tmpStr, "'"); NBString_concat(&tmpStr, sintxDef->keyws[nextPartDef->idx]); NBString_concat(&tmpStr, "'"); break;
										case ENSrzAnlzSintxPartType_Punctuator: NBString_concat(&tmpStr, "'"); NBString_concat(&tmpStr, sintxDef->puncts[nextPartDef->idx]); NBString_concat(&tmpStr, "'"); break;
										case ENSrzAnlzSintxPartType_Identifier: NBString_concat(&tmpStr, "identifier"); NBASSERT(nextPartDef->idx == -1) break;
										case ENSrzAnlzSintxPartType_Constant: NBString_concat(&tmpStr, "constant"); NBASSERT(nextPartDef->idx == -1) break;
										case ENSrzAnlzSintxPartType_StrLiteral: NBString_concat(&tmpStr, "strLiteral"); NBASSERT(nextPartDef->idx == -1) break;
										case ENSrzAnlzSintxPartType_SintaxDef:
											{
												const STSrcAnlzSintx* sintaxDef = &sintxDef->defs[nextPartDef->idx];
												NBString_concat(&tmpStr, sintaxDef->name);
												NBString_concat(&tmpStr, "(");
												if(sintaxDef->deepHints != NULL && sintaxDef->deepHintsSz > 0){
													SI32 i; for(i = 0; i < sintaxDef->deepHintsSz; i++){
														const STSrcAnlzSintxPart partHint = sintaxDef->deepHints[i];
														NBASSERT(partHint.type != ENSrzAnlzSintxPartType_SintaxDef)
														if(i != 0) NBString_concat(&tmpStr, (i + 1) < sintaxDefSeq->deepHintsSz ? ", " : " or ");
														switch(partHint.type){
															case ENSrzAnlzSintxPartType_Keyword: NBString_concat(&tmpStr, "'"); NBString_concat(&tmpStr, sintxDef->keyws[partHint.idx]); NBString_concat(&tmpStr, "'"); break;
															case ENSrzAnlzSintxPartType_Punctuator: NBString_concat(&tmpStr, "'"); NBString_concat(&tmpStr, sintxDef->puncts[partHint.idx]); NBString_concat(&tmpStr, "'"); break;
															case ENSrzAnlzSintxPartType_Identifier: NBString_concat(&tmpStr, "identifier"); break;
															case ENSrzAnlzSintxPartType_Constant: NBString_concat(&tmpStr, "constant"); break;
															case ENSrzAnlzSintxPartType_StrLiteral: NBString_concat(&tmpStr, "strLiteral"); break;
															default: NBString_concat(&tmpStr, "unknownPartType"); NBASSERT(FALSE) break;
														}
													}
												}
												NBString_concat(&tmpStr, ")");
											}
											break;
										default: NBString_concat(&tmpStr, "unknownPartType"); NBASSERT(FALSE) break;
									}
									NBString_concat(&tmpStr, ".");
									PRINTF_INFO("%s\n", tmpStr.str);
									NBString_release(&tmpStr);
#								endif*/
								}
							}
						}
						break;
					}
				} while(TRUE);
			}
			if(dstPartFoundWasConsumed != NULL) *dstPartFoundWasConsumed = partFoundWasConsumed;
			if(dstChildsSeqWasAlreadyCompleted != NULL) *dstChildsSeqWasAlreadyCompleted = childsSeqWasAlreadyCompleted;
			if(pPrevRootNode != NULL) *pPrevRootNode = iPrevRootNode;
		}
	}
	return r;
}

BOOL SrcAnlzSintxParseTree_continuePosibilities(const STSrcAnlzSintxDefs* sintxDef, STSrcAnlzSintxParseTree* dstTree, const STSrcAnlzSintxPart partFound, const UI32 partIdxValue, UI16* dstAnyWasCompletePosib){
	BOOL r = FALSE;
	NBASSERT(partFound.type >= 0 && partFound.type < ENSrzAnlzSintxPartType_Count)
	if(dstTree != NULL){
		STNBArray* nodes = &dstTree->nodes; //STSrcAnlzSintxParseTreeNode
		STNBArray* freeNodesIdxs = &dstTree->freeNodesIdxs; //UI16
		if(nodes->use > 0){
			r = TRUE;
			//Run all posibilities
			UI16 iRootNode = 0, iPrevRoot = 0;
			STSrcAnlzSintxParseTreeNode rootNode = NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, 0);
			NBASSERT(rootNode.refsCount != 0)
			if(rootNode.iNextPart != 0){
				do {
					//Next
					iRootNode	= rootNode.iNextPart;
					rootNode	= NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iRootNode);
					NBASSERT(rootNode.refsCount != 0)
					//Process
					BOOL partWasConsumed = FALSE, childsSeqWasAlreadyCompleted = FALSE;
					if(!SrcAnlzSintxParseTree_continuePosibilitiesChilds(sintxDef, dstTree, iRootNode, &iPrevRoot, iRootNode, partFound, partIdxValue, &partWasConsumed, &childsSeqWasAlreadyCompleted)){
						r = FALSE; NBASSERT(FALSE)
						break;
					} else {
						if(partWasConsumed){
							//Posibility was extended (continue)
							iPrevRoot			= iRootNode;
						} else {
							//Was a completed but this newPart invalidates it.
							if(childsSeqWasAlreadyCompleted){
								if(dstAnyWasCompletePosib != NULL) *dstAnyWasCompletePosib = iRootNode;
							}
							//Decrease refs
							{
								//Release rootNode
								STSrcAnlzSintxParseTreeNode* rootNodePtr = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iRootNode);
								NBASSERT(rootNodePtr->refsCount == 1)
								if(rootNodePtr->refsCount == 0){
									r = FALSE; NBASSERT(FALSE)
									break;
								} else {
									rootNodePtr->refsCount--;
									if(rootNodePtr->refsCount == 0){
										//Release childs
										if(rootNode.iChildSeq != 0){
											if(!SrcAnlzSintxParseTree_releaseChildsNodes(dstTree, rootNode.iChildSeq)){
												r = FALSE; NBASSERT(FALSE)
												break;
											}
										}
										//Release node
										NBArray_addValue(freeNodesIdxs, iRootNode);
									}
								}
							}
							//Posibility did not match (remove)
							//Remove by (1) linking prev to next and (2) not updating iPrevRoot's value
							NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iPrevRoot)->iNextPart = rootNode.iNextPart;
						}
					}
				} while(rootNode.iNextPart != 0);
				NBASSERT(SrcAnlzSintxParseTree_dbgValidate(sintxDef, dstTree));
			}
		}
	}
	return r;
}


BOOL SrcAnlz_sintxParseGetLastDefEnum(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree, const UI16 iFirstChild, SI32* dstLastDefEnum){
	BOOL r = FALSE;
	const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
	NBASSERT(iFirstChild < nodes->use)
	if(iFirstChild < nodes->use){
		r = TRUE;
		UI32 iPart = 0;
		UI16 iChild = iFirstChild;
		const STSrcAnlzSintxParseTreeNode* child = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iChild);
		const STSrcAnlzSintxSeq* sintaxDefSeq = &sintxDef->defsSeqs[child->iSintaxDefSeq];
		do {
			NBASSERT(iPart < sintaxDefSeq->partsSz)
			if(iPart >= sintaxDefSeq->partsSz){
				r = FALSE; NBASSERT(FALSE)
				break;
			} else {
				const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
				//Process
				if(partDef->type == ENSrzAnlzSintxPartType_SintaxDef){
					NBASSERT(partDef->idx >= 0)
					if(dstLastDefEnum != NULL) *dstLastDefEnum = partDef->idx;
					if(child->iChildSeq != 0){
						if(!SrcAnlz_sintxParseGetLastDefEnum(sintxDef, tree, child->iChildSeq, dstLastDefEnum)){
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
			}
			NBASSERT(r) //Program logic error, allways 'break' when setting 'r' to 'FALSE'
		} while(TRUE);
	}
	return r;
}
	
BOOL SrcAnlzSintxParseTree_removePosibilitiesEndedByDefEnum(const STSrcAnlzSintxDefs* sintxDef, STSrcAnlzSintxParseTree* dstTree, const SI32 iDefEnum, UI32* dstRemovedCount){
	BOOL r = FALSE;
	SI32 removedCount = 0;
	if(dstTree != NULL){
		STNBArray* nodes = &dstTree->nodes; //STSrcAnlzSintxParseTreeNode
		STNBArray* freeNodesIdxs = &dstTree->freeNodesIdxs; //UI16
		if(nodes->use > 0){
			r = TRUE;
			//Run all posibilities
			UI16 iRootNode = 0, iPrevRoot = 0;
			STSrcAnlzSintxParseTreeNode rootNode = NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, 0);
			NBASSERT(rootNode.refsCount != 0)
			if(rootNode.iNextPart != 0 && iDefEnum >= 0){
				do {
					//Next
					iRootNode	= rootNode.iNextPart;
					rootNode	= NBArray_itmValueAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iRootNode);
					NBASSERT(rootNode.refsCount != 0)
					//Process
					if(rootNode.iChildSeq == 0){
						//Mantain posibility (continue)
						iPrevRoot = iRootNode;
					} else {
						SI32 iLastDefEnum = -1;
						if(!SrcAnlz_sintxParseGetLastDefEnum(sintxDef, dstTree, rootNode.iChildSeq, &iLastDefEnum)){
							r = FALSE; NBASSERT(FALSE)
							break;
						} else {
							if(iLastDefEnum != iDefEnum){
								//Mantain posibility (continue)
								iPrevRoot = iRootNode;
							} else {
								//Remove posibility
								//PRINTF_INFO("Removing posibility root %d ended in forbiden part.\n", iRootNode);
								//Decrease refs
								{
									//Release rootNode
									STSrcAnlzSintxParseTreeNode* rootNodePtr = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iRootNode);
									NBASSERT(rootNodePtr->refsCount == 1)
									if(rootNodePtr->refsCount == 0){
										r = FALSE; NBASSERT(FALSE)
										break;
									} else {
										rootNodePtr->refsCount--;
										if(rootNodePtr->refsCount == 0){
											//Release childs
											if(rootNode.iChildSeq != 0){
												if(!SrcAnlzSintxParseTree_releaseChildsNodes(dstTree, rootNode.iChildSeq)){
													r = FALSE; NBASSERT(FALSE)
													break;
												}
											}
											//Release node
											NBArray_addValue(freeNodesIdxs, iRootNode);
										}
									}
								}
								//Posibility did not match (remove)
								//Remove by (1) linking prev to next and (2) not updating iPrevRoot's value
								NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iPrevRoot)->iNextPart = rootNode.iNextPart;
								removedCount++;
							}
						}
					}
				} while(rootNode.iNextPart != 0);
				NBASSERT(SrcAnlzSintxParseTree_dbgValidate(sintxDef, dstTree));
			}
		}
	}
	if(dstRemovedCount != NULL) *dstRemovedCount = removedCount;
	return r;
}

//
// Debug, validate tree
//

#ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
BOOL SrcAnlzSintxParseTree_dbgValidateNodeChilds(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree, const UI16 iFirstChild, STNBArray* dstIdxsRefs /*UI16*/){
	BOOL r = FALSE;
	const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
	if(iFirstChild != 0){
		UI16 iPrevNode = iFirstChild, iNode = iFirstChild;
		const STSrcAnlzSintxParseTreeNode* thisNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNode);
		const STSrcAnlzSintxParseTreeNode* prevNode = thisNode;
		const STSrcAnlzSintxSeq* sintaxDefSeq = &sintxDef->defsSeqs[thisNode->iSintaxDefSeq];
		r = TRUE;
		UI16 iPart = 0;
		do {
			if(dstIdxsRefs != NULL) NBArray_addValue(dstIdxsRefs, iNode);
			NBASSERT(thisNode->refsCount != 0)
			NBASSERT(prevNode->iSintaxDefSeq == thisNode->iSintaxDefSeq)
			const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iPart];
			switch(partDef->type){
				case ENSrzAnlzSintxPartType_Keyword:
					NBASSERT(partDef->idx >= 0 && partDef->idx < sintxDef->keywsSz)
					NBASSERT(thisNode->iChildSeq == 0)
					break;
				case ENSrzAnlzSintxPartType_Punctuator:
					NBASSERT(partDef->idx >= 0 && partDef->idx < sintxDef->punctsSz)
					NBASSERT(thisNode->iChildSeq == 0)
					break;
				case ENSrzAnlzSintxPartType_Identifier:
					NBASSERT(thisNode->iChildSeq == 0)
					break;
				case ENSrzAnlzSintxPartType_Constant:
					NBASSERT(thisNode->iChildSeq == 0)
					break;
				case ENSrzAnlzSintxPartType_StrLiteral:
					NBASSERT(thisNode->iChildSeq == 0)
					break;
				case ENSrzAnlzSintxPartType_SintaxDef:
					if(thisNode->iChildSeq != 0){
						if(!SrcAnlzSintxParseTree_dbgValidateNodeChilds(sintxDef, tree, thisNode->iChildSeq, dstIdxsRefs)){
							NBASSERT(FALSE)
							r = FALSE;
						}
					}
					break;
				default:
					NBASSERT(FALSE)
					break;
			}
			//Next part
			if(thisNode->iNextPart == 0){
				break;
			} else {
				iPrevNode	= iNode;
				prevNode	= thisNode;
				iNode		= thisNode->iNextPart;
				thisNode	= NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iNode);
				iPart++;
			}
		} while(r);
	}
	return r;
}
#endif


#ifdef CONFIG_NB_INCLUIR_VALIDACIONES_ASSERT
BOOL SrcAnlzSintxParseTree_dbgValidate(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree){
	BOOL r = FALSE;
	const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
	const STNBArray* freeNodesIdxs = &tree->freeNodesIdxs; //UI16
	NBASSERT(nodes->use > 0) //At least the root node must exist
	if(nodes->use > 0){
		r = TRUE;
		STNBArray* idxsRefs = NULL; //idxsRefs = (STNBArray*)NBMemory_alloc(sizeof(STNBArray)); //uncomment to validate idxsRefs
		if(idxsRefs != NULL) NBArray_init(idxsRefs, sizeof(UI16), NBCompareUI16);
		UI16 iRootNode = 0;
		const STSrcAnlzSintxParseTreeNode* rootNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iRootNode);
		if(idxsRefs != NULL) NBArray_add(idxsRefs, &iRootNode, sizeof(iRootNode));
		NBASSERT(rootNode->refsCount != 0)
		if(rootNode->iNextPart != 0){
			do {
				//Load next root node
				if(idxsRefs != NULL) NBArray_add(idxsRefs, &rootNode->iNextPart, sizeof(rootNode->iNextPart));
				iRootNode = rootNode->iNextPart;
				rootNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iRootNode);
				NBASSERT(rootNode->refsCount != 0)
				NBASSERT(rootNode->iSintaxDefSeq == 0xFFFF)
				NBASSERT(rootNode->iChildSeq != 0)
				//Build string
				if(rootNode->iChildSeq != 0){
					if(!SrcAnlzSintxParseTree_dbgValidateNodeChilds(sintxDef, tree, rootNode->iChildSeq, idxsRefs)){
						r = FALSE; NBASSERT(FALSE)
						break;
					}
				}
			} while(rootNode->iNextPart != 0);
		}
		//Validate refsCount
		if(idxsRefs != NULL){
			//Run all nodes
			UI16 errsFound = 0, activeCount = 0, inactiveCount = 0;
			UI16 i; for(i = 0; i < nodes->use; i++){
				const STSrcAnlzSintxParseTreeNode* node = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, i);
				UI16 refsFound = 0;
				SI32 i2; for(i2 = 0; i2 < idxsRefs->use; i2++){
					if(NBArray_itmValueAtIndex(idxsRefs, UI16, i2) == i){
						refsFound++;
					}
				}
				if(node->refsCount != refsFound){
					PRINTF_ERROR("NODE id%d indicates %d references, but found %d.\n", i, node->refsCount, refsFound);
					errsFound++;
				}
				//Validate freeNodes index
				if(refsFound == 0){
					NBASSERT(NBArray_indexOf(freeNodesIdxs, &i, sizeof(i)) != -1)
					inactiveCount++;
				} else {
					NBASSERT(NBArray_indexOf(freeNodesIdxs, &i, sizeof(i)) == -1)
					activeCount++;
				}
			}
			NBASSERT(errsFound == 0)
			NBASSERT(nodes->use == (activeCount + inactiveCount))
			NBASSERT(freeNodesIdxs->use == inactiveCount)
		}
		//Release
		if(idxsRefs != NULL){
			NBArray_release(idxsRefs);
			NBMemory_free(idxsRefs);
			idxsRefs = NULL;
		}
	}
	return r;
}
#endif

//
// Debug, print tree
//

void SrcAnlz_sintxParseAddNodeChildsToStr(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree, const UI16 iFirstChild, STNBString* tmpStr){
	const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
	NBASSERT(iFirstChild != 0)
	NBASSERT(iFirstChild < nodes->use)
	if(iFirstChild != 0){
		const STSrcAnlzSintxParseTreeNode* thisNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iFirstChild);
		const STSrcAnlzSintxParseTreeNode* prevNode = thisNode;
		const STSrcAnlzSintxSeq* sintaxDefSeq = &sintxDef->defsSeqs[thisNode->iSintaxDefSeq];
		UI16 iChild = 0;
		do {
			//Add part to tmpStr
			NBASSERT(prevNode->iSintaxDefSeq == thisNode->iSintaxDefSeq)
			const STSrcAnlzSintxPart* partDef = &sintaxDefSeq->parts[iChild];
			switch(partDef->type){
				case ENSrzAnlzSintxPartType_Keyword:
					NBString_concat(tmpStr, " '");
					NBASSERT(partDef->idx >= 0 && partDef->idx < sintxDef->keywsSz)
					NBString_concat(tmpStr, sintxDef->keyws[partDef->idx]);
					NBString_concat(tmpStr, "'");
					NBASSERT(thisNode->iChildSeq == 0)
					break;
				case ENSrzAnlzSintxPartType_Punctuator:
					NBString_concat(tmpStr, " '");
					NBASSERT(partDef->idx >= 0 && partDef->idx < sintxDef->punctsSz)
					NBString_concat(tmpStr, sintxDef->puncts[partDef->idx]);
					NBString_concat(tmpStr, "'");
					NBASSERT(thisNode->iChildSeq == 0)
					break;
				case ENSrzAnlzSintxPartType_Identifier:
					NBString_concat(tmpStr, " Identifier");
					NBASSERT(thisNode->iChildSeq == 0)
					break;
				case ENSrzAnlzSintxPartType_Constant:
					NBString_concat(tmpStr, " Constant");
					NBASSERT(thisNode->iChildSeq == 0)
					break;
				case ENSrzAnlzSintxPartType_StrLiteral:
					NBString_concat(tmpStr, " StrLiteral");
					NBASSERT(thisNode->iChildSeq == 0)
					break;
				case ENSrzAnlzSintxPartType_SintaxDef:
					{
						NBString_concat(tmpStr, " ");
						NBString_concat(tmpStr, sintxDef->defs[partDef->idx].name);
						//NBString_concat(&tmpStr, "_");
						//NBString_concatSI32(&tmpStr, (SI32)thisNode->iSintaxDefSeq);
						if(thisNode->iChildSeq != 0){
							NBString_concat(tmpStr, "{");
							SrcAnlz_sintxParseAddNodeChildsToStr(sintxDef, tree, thisNode->iChildSeq, tmpStr);
							NBString_concat(tmpStr, " }");
						}
					}
					break;
				default:
					NBString_concat(tmpStr, " UnknownPartType");
					NBASSERT(FALSE)
					break;
			}
			//Next
			if(thisNode->iNextPart == 0){
				NBASSERT(iChild < sintaxDefSeq->partsSz)
				if((iChild + 1) < sintaxDefSeq->partsSz){
					NBString_concat(tmpStr, " + ");
					NBString_concatSI32(tmpStr, (SI32)sintaxDefSeq->partsSz - iChild - 1);
				}
				break;
			} else {
				prevNode = thisNode;
				thisNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, thisNode->iNextPart);
				iChild++;
			}
		} while(TRUE);
	}
}

void SrcAnlzSintxParseTree_printChilds(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree, const UI16 iParentNode){
	const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
	if(iParentNode < nodes->use){
		STNBString tmpStr; NBString_init(&tmpStr);
		const STSrcAnlzSintxParseTreeNode* parentNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, iParentNode);
		if(parentNode->iChildSeq != 0){
			SrcAnlz_sintxParseAddNodeChildsToStr(sintxDef, tree, parentNode->iChildSeq, &tmpStr);
		}
		PRINTF_INFO("%s id%d: %s.\n", (parentNode->iSintaxDefSeq == 0xFFFF ? "Posibility" : "Childs"), iParentNode, tmpStr.str);
		NBString_release(&tmpStr);
	}
}

void SrcAnlzSintxParseTree_print(const STSrcAnlzSintxDefs* sintxDef, const STSrcAnlzSintxParseTree* tree){
	const STNBArray* nodes = &tree->nodes; //STSrcAnlzSintxParseTreeNode
	if(nodes->use == 0){
		PRINTF_INFO("Posibility tree is empty.\n");
	} else {
		const STSrcAnlzSintxParseTreeNode* rootNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, 0);
		if(rootNode->iNextPart == 0){
			PRINTF_INFO("Posibility tree is empty.\n");
		} else {
			UI16 count = 0;
			STNBString tmpStr; NBString_init(&tmpStr);
			do {
				//Load next root node
				const UI16 iRootNode = rootNode->iNextPart;
				rootNode = NBArray_itmPtrAtIndex(nodes, STSrcAnlzSintxParseTreeNode, rootNode->iNextPart);
				NBASSERT(rootNode->iSintaxDefSeq == 0xFFFF)
				//Build string
				if(rootNode->iChildSeq != 0){
					SrcAnlz_sintxParseAddNodeChildsToStr(sintxDef, tree, rootNode->iChildSeq, &tmpStr);
				}
				count++;
				//Print string
				PRINTF_INFO("Posibility #%d-id%d: %s.\n", count, iRootNode, tmpStr.str);
				NBString_empty(&tmpStr);
			} while(rootNode->iNextPart != 0);
			NBString_release(&tmpStr);
		}
	}
}


