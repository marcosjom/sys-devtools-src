//
//  NBToolLaunchscreensGenerator.h
//  DevTools
//
//  Created by Nicaragua Binary on 20/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef NBToolLaunchscreensGenerator_H_INCLUIDO
#define NBToolLaunchscreensGenerator_H_INCLUIDO

#include "AUFrameworkBase.h"

//Launchscreen scales config

typedef struct NBToolGenLaunchSize_ {
	UI32 w;
	UI32 h;
	const char* pre;	//name prefix
	const char* post;	//name postfix
} NBToolGenLaunchSize;

const STNBStructMap* NBToolGenLaunchSize_getSharedStructMap(void);

typedef struct NBToolGenLaunchJob_ {
	const char* 		name;
	BOOL				enabled;
	const char*			dstFolder;
	const char*			namesPrefix;
	STNBColor8			bgColor;
	STNBPoint			imgRelPos;
	STNBSize			imgRelSz;
	const char**		imgVers;
	UI32				imgVersSz;
	NBToolGenLaunchSize* sizes;
	UI32				sizesSz;
} NBToolGenLaunchJob;

const STNBStructMap* NBToolGenLaunchJob_getSharedStructMap(void);

typedef struct NBToolGenLaunchJobs_ {
	NBToolGenLaunchJob*	jobs;
	UI32				jobsSz;
} NBToolGenLaunchJobs;

const STNBStructMap* NBToolGenLaunchJobs_getSharedStructMap(void);

// Class

class NBToolLaunchscreensGenerator {
	public:
		static BOOL generateLaunchscreens(const NBToolGenLaunchJob* job);
};


#endif
