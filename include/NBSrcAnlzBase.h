//
//  NBSrcAnlzBase.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_BASE_H
#define NB_SRCANLZ_BASE_H

#include "nb/NBFrameworkDefs.h"
#include "nb/core/NBString.h"
#include "nb/core/NBArraySorted.h"

#ifdef __cplusplus
//extern "C" {
#endif

#define NB_SRCANLZ_PARSE_BUFFER_SIZE		4096	//File parse buffer size

//Range of indexes
typedef struct STSrcAnlzRange_ {
	UI32	start;	//first index
	UI32	count;	//count
} STSrcAnlzRange;

//String index (also enabled for searches)
typedef struct STSrcAnlzStrIdx_ {
	UI32				nameStart;	//name's first char index
	//
	UI32				itmIndex;	//item index
	const STNBString*	strings;	//strings content
} STSrcAnlzStrIdx;

BOOL SrcAnlzStrIdx_compare(const ENCompareMode mode, const void* itmBase, const void* itmCmp, const UI32 bytesPerItem);

#ifdef __cplusplus
//}
#endif

#endif
