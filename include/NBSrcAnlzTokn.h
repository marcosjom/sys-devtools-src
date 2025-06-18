//
//  NBSrcAnlzTokn.h
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#ifndef NB_SRCANLZ_TOKN_H
#define NB_SRCANLZ_TOKN_H

#include "nb/NBFrameworkDefs.h"
//
#include "nb/core/NBString.h"
#include "nb/core/NBArray.h"
#include "NBSrcAnlzToknI.h"
#include "NBSrcAnlzSintx.h"	//for 'STSrcAnlzSintxPart'

// Token

typedef struct STSrcAnlzTokn_ {
	UI32				iAtBuff;			//Start of this token in tokens buffer
	STSrcAnlzSintxPart	sintxPart;			//Sintax part (type: keyword, punctuator, identifier, etc...) and index.
	BOOL				isSpacePreceded;	//is separated by a space
} STSrcAnlzTokn;

// Tokens buffer

typedef struct STSrcAnlzTokns_ {
	STNBArray		defs;		//STSrcAnlzTokn, First char index of each string in 'tokensAcum'
	STNBString		strBuff;	//token's content buffer
} STSrcAnlzTokns;

#ifdef __cplusplus
//}
#endif

#endif
