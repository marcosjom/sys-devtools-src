//
//  NBToolAppIconsGenerator.h
//  DevTools
//
//  Created by Nicaragua Binary on 20/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef NBToolAppIconsGenerator_H_INCLUIDO
#define NBToolAppIconsGenerator_H_INCLUIDO

#include "AUFrameworkBase.h"

//Icons scales config

typedef struct NBToolGenIconSize_ {
	float	szPoints;	//opyional-1
	float	scale;		//optional-1
	UI16	width;		//optional-2
	UI16	height;		//optional-2
	const char* post;
} NBToolGenIconSize;

const STNBStructMap* NBToolGenIconSize_getSharedStructMap(void);

typedef struct NBToolGenIconsJob_ {
	const char* 		name;
	BOOL				enabled;
	const char*			baseIcon;
	const char*			dstFolder;
	const char*			namesPrefix;
	NBToolGenIconSize*	sizes;
	UI32				sizesSz;
} NBToolGenIconsJob;

const STNBStructMap* NBToolGenIconsJob_getSharedStructMap(void);

typedef struct NBToolGenIconsJobs_ {
	NBToolGenIconsJob*	jobs;
	UI32				jobsSz;
} NBToolGenIconsJobs;

const STNBStructMap* NBToolGenIconsJobs_getSharedStructMap(void);

// Class

class NBToolAppIconsGenerator {
	public:
		static BOOL generateAppIcons(const char* basePng, const char* dstFolder, const char* iconsBaseName, const NBToolGenIconSize* sizes, const SI32 sizesLen);
};


#endif
