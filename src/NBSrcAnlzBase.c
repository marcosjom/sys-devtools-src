//
//  NBSrcAnlzBase.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "NBSrcAnlzBase.h"

BOOL SrcAnlzStrIdx_compare(const ENCompareMode mode, const void* itmBase, const void* itmCmp, const UI32 bytesPerItem){
	NBASSERT(bytesPerItem == sizeof(STSrcAnlzStrIdx))
	if(bytesPerItem == sizeof(STSrcAnlzStrIdx)){
		const STSrcAnlzStrIdx* obj = itmBase;
		const STSrcAnlzStrIdx* other = itmCmp;
		switch (mode) {
			case ENCompareMode_Equal:
				return NBString_strIsEqual(&obj->strings->str[obj->nameStart], &other->strings->str[other->nameStart]);
			case ENCompareMode_Lower:
				return NBString_strIsLower(&obj->strings->str[obj->nameStart], &other->strings->str[other->nameStart]);
			case ENCompareMode_LowerOrEqual:
				return NBString_strIsLowerOrEqual(&obj->strings->str[obj->nameStart], &other->strings->str[other->nameStart]);
			case ENCompareMode_Greater:
				return NBString_strIsGreater(&obj->strings->str[obj->nameStart], &other->strings->str[other->nameStart]);
			case ENCompareMode_GreaterOrEqual:
				return NBString_strIsGreaterOrEqual(&obj->strings->str[obj->nameStart], &other->strings->str[other->nameStart]);
			default:
				NBASSERT(FALSE) //Program logic error
				break;
		}
	}
	NBASSERT(FALSE) //Program logic error
	return FALSE;
}

