//
//  NBToolImagesScaler.cpp
//  DevTools
//
//  Created by Nicaragua Binary on 20/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "AUFrameworkBaseStdAfx.h"
#include "NBToolImagesScaler.h"
#include "nb/2d/NBBitmap.h"
#include "nb/2d/NBPng.h"

typedef struct STGenResProfileDef_ {
	ENGenResProfile	idEnum;
	const char*		nodeName;
} STGenResProfileDef;

static STGenResProfileDef ___globalGenResProfilesDef[] = {
	{ENGenResProfile_Dev, "dev"}
	, {ENGenResProfile_Pkg, "pkg"}
};

// Found files

void NBToolImagesScaler::fileInit(NBGenResFile* data, const char* filePath){
	data->filePath	= new AUCadena8(filePath);
	data->scaleBase2 = 0;
	data->tasks		= NULL;
}

void NBToolImagesScaler::fileFinalize(NBGenResFile* data){
	if(data->filePath != NULL) data->filePath->liberar(); data->filePath = NULL;
	data->scaleBase2 = 0;
	if(data->tasks != NULL){
		SI32 i; const SI32 count = data->tasks->conteo;
		for(i = 0 ; i < count; i++){
			NBGenResFileTask* task = data->tasks->elemPtr(i);
			NBToolImagesScaler::fileTaskFinalize(task);
		}
		data->tasks->vaciar();
		data->tasks->liberar();
		data->tasks = NULL;
	}
}

SI32 NBToolImagesScaler::filesExecuteTasks(NBGenResState* state, const ENGenResProfile profile){
	SI32 r = 0;
	if(state->filesFound != NULL){
		//All files
		SI32 i; const SI32 count = state->filesFound->conteo;
		for(i = 0 ; i < count; i++){
			NBGenResFile* file = state->filesFound->elemPtr(i);
			if(file->tasks != NULL){
				if(file->tasks->conteo > 0){
					//All tasks
					r += NBToolImagesScaler::generateScaledImage(state, profile, file->scaleBase2, file->filePath->str(), file->tasks->elemPtr(0), file->tasks->conteo);
				}
			}
		}
	}
	return r;
}

void NBToolImagesScaler::filesFinalize(AUArregloNativoOrdenadoMutableP<NBGenResFile>* filesFound){
	if(filesFound != NULL){
		SI32 i; const SI32 count = filesFound->conteo;
		for(i = 0 ; i < count; i++){
			NBGenResFile* file = filesFound->elemPtr(i);
			NBToolImagesScaler::fileFinalize(file);
		}
		filesFound->vaciar();
	}
}

// Found file tasks

void NBToolImagesScaler::fileTaskInit(NBGenResFileTask* data, const char* filePath){
	data->dstPath	= new AUCadena8(filePath);
	NBToolImagesScaler::configScaleRuleInit(&data->rule);
	data->dstStats	= NULL;
}

void NBToolImagesScaler::fileTaskFinalize(NBGenResFileTask* data){
	if(data->dstPath != NULL) data->dstPath->liberar(); data->dstPath = NULL;
	NBToolImagesScaler::configScaleRuleFinalize(&data->rule);
	data->dstStats	= NULL;
}

// Stats

void NBToolImagesScaler::statsInit(NBGenResStats* data){
	data->qFormatPng			= 0;
	data->qFormatJpeg			= 0;
	//
	data->conteoRGBA			= 0;
	data->pixelsRGBA			= 0;
	data->conteoRGBA_Paleta		= 0;
	data->pixelsRGBA_Paleta		= 0;
	data->conteoRGB				= 0;
	data->pixelsRGB				= 0;
	data->conteoRGB_Paleta		= 0;
	data->pixelsRGB_Paleta		= 0;
	data->conteoGris			= 0;
	data->pixelsGris			= 0;
	data->conteoGrisAlpha		= 0;
	data->pixelsGrisAlpha		= 0;
	//
	data->qProcessedImgs		= 0;
	data->pixelsTotal			= 0;
	//
	data->qNotImgs				= 0;
	data->qIgnoredImgs			= 0;
	data->qSkippedImgs			= 0;
	data->qErrorImgs			= 0;
}

void NBToolImagesScaler::statsPrint(const NBGenResStats* data){
	PRINTF_INFO("Resultados imgs:\n");
	if(data->qProcessedImgs !=0 && data->pixelsTotal != 0){
		PRINTF_INFO("RGBA:              cantidad (%4d) (%2d%%) pixeles (%10d) (%2d%%)\n", data->conteoRGBA, (SI32)(100*data->conteoRGBA/data->qProcessedImgs), (SI32)data->pixelsRGBA, (SI32)(100*data->pixelsRGBA/data->pixelsTotal));
		PRINTF_INFO("RGBA (con paleta): cantidad (%4d) (%2d%%) pixeles (%10d) (%2d%%)\n", data->conteoRGBA_Paleta, (SI32)(100*data->conteoRGBA_Paleta/data->qProcessedImgs), (SI32)data->pixelsRGBA_Paleta, (SI32)(100*data->pixelsRGBA_Paleta/data->pixelsTotal));
		PRINTF_INFO("RGB:               cantidad (%4d) (%2d%%) pixeles (%10d) (%2d%%)\n", data->conteoRGB, (SI32)(100*data->conteoRGB/data->qProcessedImgs), (SI32)data->pixelsRGB, (SI32)(100*data->pixelsRGB/data->pixelsTotal));
		PRINTF_INFO("RGB (con paleta):  cantidad (%4d) (%2d%%) pixeles (%10d) (%2d%%)\n", data->conteoRGB_Paleta, (SI32)(100*data->conteoRGB_Paleta/data->qProcessedImgs), (SI32)data->pixelsRGB_Paleta, (SI32)(100*data->pixelsRGB_Paleta/data->pixelsTotal));
		PRINTF_INFO("GRIS:              cantidad (%4d) (%2d%%) pixeles (%10d) (%2d%%)\n", data->conteoGris, (SI32)(100*data->conteoGris/data->qProcessedImgs), (SI32)data->pixelsGris, (SI32)(100*data->pixelsGris/data->pixelsTotal));
		PRINTF_INFO("GRIS ALPHA:        cantidad (%4d) (%2d%%) pixeles (%10d) (%2d%%)\n", data->conteoGrisAlpha, (SI32)(100*data->conteoGrisAlpha/data->qProcessedImgs), (SI32)data->pixelsGrisAlpha, (SI32)(100*data->pixelsGrisAlpha/data->pixelsTotal));
	}
	PRINTF_INFO("TOTAL:             CANTIDAD (%4d)       PIXELES (%10d)\n", data->qProcessedImgs, (SI32)data->pixelsTotal);
	PRINTF_INFO("                   (%4d PNGs, %4d JPEGs)\n", (SI32)data->qFormatPng, (SI32)data->qFormatJpeg);
	PRINTF_INFO("Others             notImages(%4d)\n", data->qNotImgs);
	PRINTF_INFO("Others             imgIgnore(%4d)\n", data->qIgnoredImgs);
	PRINTF_INFO("Others             imgSkippd(%4d)\n", data->qSkippedImgs);
	PRINTF_INFO("Others             imgErrors(%4d)\n", data->qErrorImgs);
}


//Buffers

void NBToolImagesScaler::stateInit(NBGenResState* data){
	//Files found
	data->filesFound	= new AUArregloNativoOrdenadoMutableP<NBGenResFile>();
	//JPEG reusable save-state
	data->jpegSaveState = NULL;
	//PNG reusable save-state
	data->pngSaveState	= NULL;
}

void NBToolImagesScaler::statePrepareJPEG(NBGenResState* data){
	//JPEG reusable save-state
	if(data->jpegSaveState == NULL){
		data->jpegSaveState = AUMapaBits::jpegWriteStateCreate();
	}
}

void NBToolImagesScaler::statePreparePNG(NBGenResState* data){
	//PNG reusable save-state
	if(data->pngSaveState == NULL){
		data->pngSaveState = (NBPngSaveState*)NBGestorMemoria::reservarMemoria(sizeof(NBPngSaveState), ENMemoriaTipo_Temporal);
		AUMapaBits::NBPngSaveStateInit(data->pngSaveState, true);
	}
}

void NBToolImagesScaler::stateFinalize(NBGenResState* data){
	//Files found
	if(data->filesFound != NULL){
		NBToolImagesScaler::filesFinalize(data->filesFound);
		data->filesFound->liberar();
		data->filesFound = NULL;
	}
	//JPEG reusable save-state
	if(data->jpegSaveState != NULL){
		AUMapaBits::jpegWriteStateDestroy(data->jpegSaveState);
		data->jpegSaveState = NULL;
	}
	//PNG reusable save-state
	if(data->pngSaveState != NULL){
		AUMapaBits::NBPngSaveStateRelease(data->pngSaveState, true);
		NBGestorMemoria::liberarMemoria(data->pngSaveState);
		data->pngSaveState = NULL;
	}
}

// Rules

const char* NBToolImagesScaler::configGetProfileNodeName(const ENGenResProfile profile){
	NBASSERT(profile < ENGenResProfile_Count)
	NBASSERT(ENGenResProfile_Count == (sizeof(___globalGenResProfilesDef) / sizeof(___globalGenResProfilesDef[0])))
	return ___globalGenResProfilesDef[profile].nodeName;
}

void NBToolImagesScaler::configScalesLoad(const AUDatosJSON* configFile, const STJsonNode* nScales, AUArregloNativoMutableP<NBGenResScale>* dst){
	SI32 iScale = 0;
	const STJsonNode* nScale = configFile->nodoHijo("imgScales", nScales);
	while(nScale != NULL){
		bool added		= false;
		NBGenResScale data;
		data.name		= new AUCadenaMutable8();
		data.root		= new AUCadenaMutable8();
		data.rootScale	= 1;
		data.dstPath	= new AUCadenaMutable8();
		data.rules		= new AUArregloNativoMutableP<NBGenResScalePath>();
		//
		configFile->nodoHijo("name", data.name, "", nScale);
		configFile->nodoHijo("root", data.root, "", nScale);
		configFile->nodoHijo("dstPath", data.dstPath, "", nScale);
		data.rootScale = configFile->nodoHijo("rootScale", 0, nScale);
		const STJsonNode* nRules = configFile->nodoHijo("rules", nScale);
		if(data.rootScale <= 0){
			PRINTF_ERROR("Scale #%d ('%s') 'rootScale' must be explicit and higher than zero (ignoring scale).\n", (iScale + 1), data.name->str());
		} else if(data.dstPath->tamano() == 0){
			PRINTF_ERROR("Scale #%d ('%s') 'dstPath' node is empty (ignoring scale).\n", (iScale + 1), data.name->str());
		} else if(nRules == NULL){
			PRINTF_ERROR("Scale #%d ('%s') 'rules' node is not defined (ignoring scale).\n", (iScale + 1), data.name->str());
		} else {
			NBToolImagesScaler::configScaleRulesLoad(configFile, nRules, data.rules, iScale, data.name->str());
			dst->agregarElemento(data);
			added = true;
			
		}
		//Release if not added
		if(!added){
			if(data.name != NULL) data.name->liberar(); data.name = NULL;
			if(data.root != NULL) data.root->liberar(); data.root = NULL;
			if(data.dstPath != NULL) data.dstPath->liberar(); data.dstPath = NULL;
			if(data.rules != NULL){
				NBToolImagesScaler::configScaleRulesRelease(data.rules);
				data.rules->vaciar();
				data.rules->liberar();
				data.rules = NULL;
			}
		}
		//
		nScale = configFile->nodoHijo("imgScales", nScales, nScale);
		iScale++;
	}
}

void NBToolImagesScaler::configScalesRelease(AUArregloNativoP<NBGenResScale>* configs){
	SI32 i; const SI32 count = configs->conteo;
	for(i = 0; i < count; i++){
		NBGenResScale* data = configs->elemPtr(i);
		if(data->name != NULL) data->name->liberar(); data->name = NULL;
		if(data->root != NULL) data->root->liberar(); data->root = NULL;
		if(data->dstPath != NULL) data->dstPath->liberar(); data->dstPath = NULL;
		if(data->rules != NULL){
			NBToolImagesScaler::configScaleRulesRelease(data->rules);
			data->rules->vaciar();
			data->rules->liberar();
			data->rules = NULL;
		}
	}
}

void NBToolImagesScaler::configScaleRulesLoad(const AUDatosJSON* configFile, const STJsonNode* nRules, AUArregloNativoMutableP<NBGenResScalePath>* rules, const SI32 iScale, const char* scaleName){
	//Load rules
	SI32 iRule = 0;
	AUCadenaMutable8* strAction = new AUCadenaMutable8();
	const STJsonNode* nRule = configFile->nodoHijo("rules", nRules);
	NBASSERT(ENGenResProfile_Count == (sizeof(___globalGenResProfilesDef) / sizeof(___globalGenResProfilesDef[0])))
	while(nRule != NULL){
		//Add rule
		NBGenResScalePath data;
		NBToolImagesScaler::configScalePathInit(&data);
		data.rule.maskExplicits	|= (configFile->nodoHijo("divider", nRule) != NULL ? NB_GEN_RES_CNFG_DIVIDER_BIT : 0);
		data.rule.divider		= configFile->nodoHijo("divider", 1, nRule);
		if(data.rule.divider <= 0){
			PRINTF_ERROR("Scale #%d ('%s'), rule #%d 'divider':%d must be higher than zero.\n", (iScale + 1), scaleName, (iRule + 1), data.rule.divider);
			NBToolImagesScaler::configScalePathFinalize(&data);
		} else {
			//Action
			if(configFile->nodoHijo("action", nRule) != NULL){
				data.rule.maskExplicits	|= NB_GEN_RES_CNFG_ACTION_BIT;
				configFile->nodoHijo("action", strAction, "", nRule);
				if(strAction->esIgual("ignore")){
					data.rule.action		= ENGenResScaleAction_Ignore;
				} else if(strAction->esIgual("process")){
					data.rule.action		= ENGenResScaleAction_Process;
				} else if(strAction->esIgual("processOnce")){
					data.rule.action		= ENGenResScaleAction_ProcessOnce;
				} else {
					data.rule.action		= ENGenResScaleAction_Unknown;
					PRINTF_WARNING("Scale #%d ('%s'), rule %d's explicit action '%s' is not valid.\n", (iScale + 1), scaleName, (iRule + 1), strAction->str());
				}
			}
			//Path
			configFile->nodoHijo("path", data.path, "", nRule);
			//Profiles
			{
				const STJsonNode* nProfiles = configFile->nodoHijo("profiles", nRule);
				if(nProfiles == NULL){
					//Load all profiles from the "rule" node
					SI32 i;
					for(i = 0; i < ENGenResProfile_Count; i++){
						NBToolImagesScaler::configScaleRuleProfileLoad(configFile, nRule, &data.rule.profiles[i], (ENGenResProfile)i, iScale, scaleName, iRule);
					}
				} else {
					NBASSERT((sizeof(___globalGenResProfilesDef) / sizeof(___globalGenResProfilesDef[0])) == ENGenResProfile_Count)
					//Try to load profiles from their corresponding node
					SI32 i;
					for(i = 0; i < ENGenResProfile_Count; i++){
						NBASSERT(___globalGenResProfilesDef[i].idEnum == i)
						const STJsonNode* nProfile = configFile->nodoHijo(___globalGenResProfilesDef[i].nodeName, nProfiles);
						NBToolImagesScaler::configScaleRuleProfileLoad(configFile, (nProfile != NULL ? nProfile : nRule), &data.rule.profiles[i], (ENGenResProfile)i, iScale, scaleName, iRule);
					}
				}
			}
			//Debug
			{
				const STJsonNode* nDebug = configFile->nodoHijo("debug", nRule);
				if(nDebug != NULL){
					data.rule.maskExplicits	|= NB_GEN_RES_CNFG_DEBUG_BIT;
					//
					data.rule.debug.maskExplicits |= (configFile->nodoHijo("testAfterSaving", nDebug) != NULL ? NB_GEN_RES_DBG_RULE_TEST_SAVING_BIT : 0);
					data.rule.debug.testAfterSaving = configFile->nodoHijo("testAfterSaving", false, nDebug);
				}
			}
			//Add
			rules->agregarElemento(data);
		}
		//
		nRule = configFile->nodoHijo("rules", nRules, nRule);
		iRule++;
	}
	strAction->liberar();
}

void NBToolImagesScaler::configScaleRuleProfileLoad(const AUDatosJSON* configFile, const STJsonNode* nParentNode, NBGenResScaleRuleProfile* data, const ENGenResProfile profile, const SI32 iScale, const char* scaleName, const SI32 iRule){
	NBASSERT(ENGenResProfile_Count == (sizeof(___globalGenResProfilesDef) / sizeof(___globalGenResProfilesDef[0])))
	//PNG-compression rule
	{
		const STJsonNode* npngCompress = configFile->nodoHijo("pngCompress", nParentNode);
		if(npngCompress != NULL){
			AUCadenaMutable8* strTmp = new AUCadenaMutable8();
			data->maskExplicits					|= NB_GEN_RES_CNFG_PNG_COMP_BIT;
			//
			data->pngCompress.maskExplicits	|= (configFile->nodoHijo("level", npngCompress) != NULL ? NB_GEN_RES_PNG_COMPLEVEL_BIT : 0);
			data->pngCompress.tonesDivider	|= (configFile->nodoHijo("tonesDivider", npngCompress) != NULL ? NB_GEN_RES_PNG_COMPTONESDIV_BIT : 0);
			data->pngCompress.maskExplicits	|= (configFile->nodoHijo("palette", npngCompress) != NULL ? NB_GEN_RES_PNG_COMPPALETTE_BIT : 0);
			//
			data->pngCompress.levelOver9	= configFile->nodoHijo("level", (profile == ENGenResProfile_Dev ? 1 : 9), npngCompress);
			data->pngCompress.tonesDivider	= configFile->nodoHijo("tonesDivider", (UI8)(profile == ENGenResProfile_Dev ? 1 : 2), npngCompress);
			configFile->nodoHijo("palette", strTmp, (profile == ENGenResProfile_Dev ? "never" : "auto"), npngCompress);
			if(strTmp->esIgual("auto")){
				data->pngCompress.paletteAction = ENGenResPngPaletteAction_Auto;
			} else if(strTmp->esIgual("never")){
				data->pngCompress.paletteAction = ENGenResPngPaletteAction_Never;
			} else {
				PRINTF_WARNING("Scale #%d ('%s'), rule #%d 'pngCompress/palette': '%s' is not valid (forcing 'never').\n", (iScale + 1), scaleName, (iRule + 1), strTmp->str());
				data->pngCompress.paletteAction = ENGenResPngPaletteAction_Never;
			}
			//
			if(data->pngCompress.levelOver9 < 0){
				PRINTF_WARNING("Scale #%d ('%s'), rule #%d 'pngCompress/level': %d is lower than 0 (forcing 0).\n", (iScale + 1), scaleName, (iRule + 1), data->pngCompress.levelOver9);
				data->pngCompress.levelOver9 = 0;
			} else if(data->pngCompress.levelOver9 > 9){
				PRINTF_WARNING("Scale #%d ('%s'), rule #%d 'pngCompress/level': %d is higher than 9 (forcing 9).\n", (iScale + 1), scaleName, (iRule + 1), data->pngCompress.levelOver9);
				data->pngCompress.levelOver9 = 9;
			}
			if(data->pngCompress.tonesDivider < 1){
				PRINTF_WARNING("Scale #%d ('%s'), rule #%d 'pngCompress/tonesDivider': %d is lower than 1 (forcing 1).\n", (iScale + 1), scaleName, (iRule + 1), data->pngCompress.tonesDivider);
				data->pngCompress.tonesDivider = 1;
			} else if(data->pngCompress.tonesDivider > 128){
				PRINTF_WARNING("Scale #%d ('%s'), rule #%d 'pngCompress/tonesDivider': %d is higher than 128 (forcing 128).\n", (iScale + 1), scaleName, (iRule + 1), data->pngCompress.tonesDivider);
				data->pngCompress.tonesDivider = 128;
			}
			strTmp->liberar();
		}
	}
	//JPEG-compression rule
	{
		const STJsonNode* njpgCompress = configFile->nodoHijo("jpgCompress", nParentNode);
		if(njpgCompress != NULL){
			data->maskExplicits					|= NB_GEN_RES_CNFG_JPG_COMP_BIT;
			//
			data->jpgCompress.maskExplicits	|= (configFile->nodoHijo("quality", njpgCompress) != NULL ? NB_GEN_RES_JPG_COMPQUALITY_BIT : 0);
			data->jpgCompress.maskExplicits	|= (configFile->nodoHijo("smooth", njpgCompress) != NULL ? NB_GEN_RES_JPG_COMPSMOOTH_BIT : 0);
			data->jpgCompress.tonesDivider	|= (configFile->nodoHijo("tonesDivider", njpgCompress) != NULL ? NB_GEN_RES_JPG_COMPTONESDIV_BIT : 0);
			//
			data->jpgCompress.qualityOver100 = configFile->nodoHijo("quality", (profile == ENGenResProfile_Dev ? 95 : 90), njpgCompress);
			data->jpgCompress.smoothOver100	= configFile->nodoHijo("smooth", (profile == ENGenResProfile_Dev ? 0 : 0), njpgCompress);
			data->jpgCompress.tonesDivider	= configFile->nodoHijo("tonesDivider", (UI8)(profile == ENGenResProfile_Dev ? 1 : 2), njpgCompress);
			//
			if(data->jpgCompress.qualityOver100 < 10){
				PRINTF_WARNING("Scale #%d ('%s'), rule #%d 'jpgCompress/quality': %d is lower than 10 (forcing 10).\n", (iScale + 1), scaleName, (iRule + 1), data->jpgCompress.qualityOver100);
				data->jpgCompress.qualityOver100 = 10;
			}
			if(data->jpgCompress.qualityOver100 > 100){
				PRINTF_WARNING("Scale #%d ('%s'), rule #%d 'jpgCompress/quality': %d is higher than 100 (forcing 100).\n", (iScale + 1), scaleName, (iRule + 1), data->jpgCompress.qualityOver100);
				data->jpgCompress.qualityOver100 = 100;
			}
			if(data->jpgCompress.smoothOver100 > 100){
				PRINTF_WARNING("Scale #%d ('%s'), rule #%d 'jpgCompress/smooth': %d is higher than 100 (forcing 100).\n", (iScale + 1), scaleName, (iRule + 1), data->jpgCompress.smoothOver100);
				data->jpgCompress.smoothOver100 = 100;
			}
			if(data->jpgCompress.tonesDivider < 1){
				PRINTF_WARNING("Scale #%d ('%s'), rule #%d 'jpgCompress/tonesDivider': %d is lower than 1 (forcing 1).\n", (iScale + 1), scaleName, (iRule + 1), data->jpgCompress.tonesDivider);
				data->jpgCompress.tonesDivider = 1;
			} else if(data->jpgCompress.tonesDivider > 128){
				PRINTF_WARNING("Scale #%d ('%s'), rule #%d 'jpgCompress/tonesDivider': %d is higher than 128 (forcing 128).\n", (iScale + 1), scaleName, (iRule + 1), data->jpgCompress.tonesDivider);
				data->jpgCompress.tonesDivider = 128;
			}
		}
	}
}

void NBToolImagesScaler::configScaleRulesRelease(AUArregloNativoP<NBGenResScalePath>* rules){
	SI32 i;
	for(i = 0; i < rules->conteo; i++){
		NBToolImagesScaler::configScalePathFinalize(rules->elemPtr(i));
	}
}

void NBToolImagesScaler::configScalePathInit(NBGenResScalePath* data){
	data->path = new AUCadenaMutable8();
	NBToolImagesScaler::configScaleRuleInit(&data->rule);
}

void NBToolImagesScaler::configScalePathFinalize(NBGenResScalePath* data){
	if(data->path != NULL) data->path->liberar(); data->path = NULL;
	NBToolImagesScaler::configScaleRuleFinalize(&data->rule);
}

void NBToolImagesScaler::configScaleRuleInit(NBGenResScaleRule* data){
	data->maskExplicits	= 0;
	data->action		= ENGenResScaleAction_Ignore;
	data->divider		= 1;
	//Profiles
	{
		SI32 i;
		for(i = 0; i < ENGenResProfile_Count; i++){
			NBToolImagesScaler::configScaleRuleProfileInit(&data->profiles[i], (ENGenResProfile)i);
		}
	}
	//Debug
	data->debug.maskExplicits	= 0;
	data->debug.testAfterSaving	= false;
}

void NBToolImagesScaler::configScaleRuleFinalize(NBGenResScaleRule* data){
	data->maskExplicits	= 0;
	data->action		= ENGenResScaleAction_Ignore;
	data->divider		= 0;
	//Profiles
	//Profiles
	{
		SI32 i;
		for(i = 0; i < ENGenResProfile_Count; i++){
			NBToolImagesScaler::configScaleRuleProfileFinalize(&data->profiles[i]);
		}
	}
	//Debug
	data->debug.maskExplicits	= 0;
	data->debug.testAfterSaving	= false;
}

void NBToolImagesScaler::configScaleRuleProfileInit(NBGenResScaleRuleProfile* data, const ENGenResProfile profile){
	data->pngCompress.maskExplicits	= 0;
	data->pngCompress.levelOver9	= (profile == ENGenResProfile_Dev ? 1 : 9);
	data->pngCompress.tonesDivider	= (profile == ENGenResProfile_Dev ? 1 : 2);
	data->pngCompress.paletteAction	= (profile == ENGenResProfile_Dev ? ENGenResPngPaletteAction_Never : ENGenResPngPaletteAction_Auto);
	//
	data->jpgCompress.maskExplicits	= 0;
	data->jpgCompress.qualityOver100 = (profile == ENGenResProfile_Dev ? 95 : 90);
	data->jpgCompress.smoothOver100	= (profile == ENGenResProfile_Dev ? 0 : 0);
	data->jpgCompress.tonesDivider	= (profile == ENGenResProfile_Dev ? 1 : 2);
}

void NBToolImagesScaler::configScaleRuleProfileFinalize(NBGenResScaleRuleProfile* data){
	data->pngCompress.maskExplicits	= 0;
	data->pngCompress.levelOver9	= 0;
	data->pngCompress.tonesDivider	= 0;
	data->pngCompress.paletteAction	= ENGenResPngPaletteAction_Never;
	//
	data->jpgCompress.maskExplicits	= 0;
	data->jpgCompress.qualityOver100 = 0;
	data->jpgCompress.smoothOver100	= 0;
	data->jpgCompress.tonesDivider	= 0;
}

void NBToolImagesScaler::configScaleRuleByExplicits(const NBGenResScaleRule* parentRule, NBGenResScaleRule* newRule){
	if(parentRule != NULL && newRule != NULL && parentRule != newRule){
		//Divider
		if((newRule->maskExplicits & NB_GEN_RES_CNFG_DIVIDER_BIT) == 0){
			newRule->divider = parentRule->divider;
		}
		//Action
		if((newRule->maskExplicits & NB_GEN_RES_CNFG_ACTION_BIT) == 0){
			newRule->action = parentRule->action;
		}
		//Profiles
		SI32 i;
		for(i = 0; i < ENGenResProfile_Count; i++){
			// PNG compression
			if((newRule->profiles[i].maskExplicits & NB_GEN_RES_CNFG_PNG_COMP_BIT) == 0){
				newRule->profiles[i].pngCompress = parentRule->profiles[i].pngCompress;
			} else {
				if((newRule->profiles[i].pngCompress.maskExplicits & NB_GEN_RES_PNG_COMPLEVEL_BIT) == 0){
					newRule->profiles[i].pngCompress.levelOver9 = parentRule->profiles[i].pngCompress.levelOver9;
				}
				if((newRule->profiles[i].pngCompress.maskExplicits & NB_GEN_RES_PNG_COMPTONESDIV_BIT) == 0){
					newRule->profiles[i].pngCompress.tonesDivider = parentRule->profiles[i].pngCompress.tonesDivider;
				}
				if((newRule->profiles[i].pngCompress.maskExplicits & NB_GEN_RES_PNG_COMPPALETTE_BIT) == 0){
					newRule->profiles[i].pngCompress.paletteAction = parentRule->profiles[i].pngCompress.paletteAction;
				}
			}
			// JPG compression
			if((newRule->profiles[i].maskExplicits & NB_GEN_RES_CNFG_JPG_COMP_BIT) == 0){
				newRule->profiles[i].jpgCompress = parentRule->profiles[i].jpgCompress;
			} else {
				if((newRule->profiles[i].jpgCompress.maskExplicits & NB_GEN_RES_JPG_COMPQUALITY_BIT) == 0){
					newRule->profiles[i].jpgCompress.qualityOver100 = parentRule->profiles[i].jpgCompress.qualityOver100;
				}
				if((newRule->profiles[i].jpgCompress.maskExplicits & NB_GEN_RES_JPG_COMPSMOOTH_BIT) == 0){
					newRule->profiles[i].jpgCompress.smoothOver100 = parentRule->profiles[i].jpgCompress.smoothOver100;
				}
				if((newRule->profiles[i].jpgCompress.maskExplicits & NB_GEN_RES_JPG_COMPTONESDIV_BIT) == 0){
					newRule->profiles[i].jpgCompress.tonesDivider = parentRule->profiles[i].jpgCompress.tonesDivider;
				}
			}
			//Validate
			{
				if(newRule->profiles[i].pngCompress.levelOver9 < 0){
					PRINTF_WARNING("Resulting rule 'pngCompress/level': %d is lower than 0 (forcing 0).\n", (SI32)newRule->profiles[i].pngCompress.levelOver9);
					newRule->profiles[i].pngCompress.levelOver9 = 0;
				} else if(newRule->profiles[i].pngCompress.levelOver9 > 9){
					PRINTF_WARNING("Resulting rule 'pngCompress/level': %d is higher than 9 (forcing 9).\n", (SI32)newRule->profiles[i].pngCompress.levelOver9);
					newRule->profiles[i].pngCompress.levelOver9 = 9;
				}
				if(newRule->profiles[i].pngCompress.tonesDivider < 1){
					PRINTF_WARNING("Resulting rule 'pngCompress/tonesDivider': %d is lower than 1 (forcing 1).\n", (SI32)newRule->profiles[i].pngCompress.tonesDivider);
					newRule->profiles[i].pngCompress.tonesDivider = 1;
				} else if(newRule->profiles[i].pngCompress.tonesDivider > 9){
					PRINTF_WARNING("Resulting rule 'pngCompress/tonesDivider': %d is higher than 128 (forcing 128).\n", (SI32)newRule->profiles[i].pngCompress.tonesDivider);
					newRule->profiles[i].pngCompress.tonesDivider = 128;
				}
			}
			//Validate
			{
				if(newRule->profiles[i].jpgCompress.qualityOver100 < 10){
					PRINTF_WARNING("Resulting rule 'jpgCompress/quality': %d is lower than 10 (forcing 10).\n", newRule->profiles[i].jpgCompress.qualityOver100);
					newRule->profiles[i].jpgCompress.qualityOver100 = 10;
				}
				if(newRule->profiles[i].jpgCompress.qualityOver100 > 100){
					PRINTF_WARNING("Resulting rule 'jpgCompress/quality': %d is higher than 100 (forcing 100).\n", newRule->profiles[i].jpgCompress.qualityOver100);
					newRule->profiles[i].jpgCompress.qualityOver100 = 100;
				}
				if(newRule->profiles[i].jpgCompress.smoothOver100 > 100){
					PRINTF_WARNING("Resulting rule 'jpgCompress/smooth': %d is higher than 100 (forcing 100).\n", newRule->profiles[i].jpgCompress.smoothOver100);
					newRule->profiles[i].jpgCompress.smoothOver100 = 100;
				}
				if(newRule->profiles[i].jpgCompress.tonesDivider < 1){
					PRINTF_WARNING("Resulting rule 'jpgCompress/tonesDivider': %d is lower than 1 (forcing 1).\n", (SI32)newRule->profiles[i].jpgCompress.tonesDivider);
					newRule->profiles[i].jpgCompress.tonesDivider = 1;
				} else if(newRule->profiles[i].jpgCompress.tonesDivider > 9){
					PRINTF_WARNING("Resulting rule 'jpgCompress/tonesDivider': %d is higher than 128 (forcing 128).\n", (SI32)newRule->profiles[i].jpgCompress.tonesDivider);
					newRule->profiles[i].jpgCompress.tonesDivider = 128;
				}

			}
		}
		//Debug
		if((newRule->maskExplicits & NB_GEN_RES_CNFG_DEBUG_BIT) == 0){
			newRule->debug = parentRule->debug;
		} else {
			if((newRule->debug.maskExplicits & NB_GEN_RES_DBG_RULE_TEST_SAVING_BIT) == 0){
				newRule->debug.testAfterSaving = parentRule->debug.testAfterSaving;
			}
		}
		//
	}
}

const NBGenResScaleRule* NBToolImagesScaler::configScaleRuleByPath(AUArregloNativoP<NBGenResScalePath>* rules, const char* path){
	const NBGenResScaleRule* r = NULL;
	SI32 i;
	for(i = 0; i < rules->conteo; i++){
		const NBGenResScalePath* data = rules->elemPtr(i);
		if(data->path != NULL){
			if(data->path->esIgual(path)){
				r = &data->rule;
				break;
			}
		}
	}
	return r;
}

//

ENGenResFileFormat NBToolImagesScaler::fileFormatByName(const char* strFilename){
	ENGenResFileFormat r = ENGenResFileFormat_NoDef;
	if(strFilename != NULL){
		const SI32 nameLen = AUCadena8::tamano(strFilename);
		//Extension 3-len ('PNG', 'JPG', ...)
		if(nameLen > 4){
			const char* buffFinal = &strFilename[nameLen - 4];
			if((buffFinal[0] == '.' && (buffFinal[1] == 'p' || buffFinal[1] == 'P') && (buffFinal[2] == 'n' || buffFinal[2] == 'N') && (buffFinal[3] == 'g' || buffFinal[3] == 'G'))){
				r = ENGenResFileFormat_Png;
			} else if((buffFinal[0] == '.' && (buffFinal[1] == 'j' || buffFinal[1] == 'J') && (buffFinal[2] == 'p' || buffFinal[2] == 'P') && (buffFinal[3] == 'g' || buffFinal[3] == 'G'))){
				r = ENGenResFileFormat_Jpeg;
			} else if(nameLen > 5){
				//Extension 4-len ('JPEG', ...)
				const char* buffFinal = &strFilename[nameLen - 5];
				if(buffFinal[0] == '.' && (buffFinal[1] == 'j' || buffFinal[1] == 'J') && (buffFinal[2] == 'p' || buffFinal[2] == 'P') && (buffFinal[3] == 'e' || buffFinal[3] == 'E') && (buffFinal[4] == 'g' || buffFinal[4] == 'G')){
					r = ENGenResFileFormat_Jpeg;
				}
			}
		}
	}
	return r;
}

SI32 NBToolImagesScaler::generateScaledImages(NBGenResState* state, const ENGenResProfile profile, const char* carpetaBaseAltaResolucion, const char* carpetaBaseDestinoResolucionMenor, const char* carpetaExtra, const SI32 escalaBase2Origen, NBGenResStats* acumularResultadosEn, AUArregloNativoP<NBGenResScalePath>* rules, const NBGenResScaleRule* curRule){
	int conteoArchivoProcesados = 0;
	AUArregloMutable* subcarpetasHD = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUArregloMutable* subcarpetasSD = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUArregloMutable* archivosHD = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUCadenaMutable8* strTmp = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	AUCadenaMutable8* strTmp2 = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	AUCadenaMutable8* strTmp3 = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	//PRINTF_INFO("PNG: procesando carpeta %s%s\n", carpetaBaseAltaResolucion, carpetaExtra);
	//--------------------------------------
	//Asegurar la existencia de las carpetas
	//--------------------------------------
	{
		strTmp->vaciar();
		strTmp->agregar(carpetaBaseAltaResolucion);
		if(carpetaExtra[0] != '\0'){ strTmp->agregar('/'); strTmp->agregar(carpetaExtra); }
		NBGestorArchivos::listarSubcarpetasDe(strTmp->str(), subcarpetasHD);
		//
		strTmp->vaciar();
		strTmp->agregar(carpetaBaseDestinoResolucionMenor);
		if(carpetaExtra[0] != '\0'){ strTmp->agregar('/'); strTmp->agregar(carpetaExtra); }
		NBGestorArchivos::listarSubcarpetasDe(strTmp->str(), subcarpetasSD);
		//
		SI32 iCarp;
		for(iCarp=0; iCarp < subcarpetasHD->conteo; iCarp++){
			AUCadena8* nombreRecurso = (AUCadena8*)subcarpetasHD->elemento[iCarp];
			//
			strTmp3->vaciar();
			strTmp3->agregar(carpetaExtra); if(carpetaExtra[0] != '\0'){ strTmp3->agregar('/'); }
			strTmp3->agregar(nombreRecurso->str());
			//
			{
				SI16 i, iRecurso = -1;
				for(i=0; i<subcarpetasSD->conteo; i++){
					if(((AUCadena8*)subcarpetasSD->elemento[i])->esIgual(nombreRecurso->str())){
						iRecurso = i; break;
					}
				}
				if(iRecurso == -1){
					strTmp->establecer(carpetaBaseDestinoResolucionMenor);
					if(carpetaExtra[0] != '\0'){ strTmp->agregar('/'); strTmp->agregar(carpetaExtra); }
					strTmp->agregar('/');
					strTmp->agregar(nombreRecurso->str());
					if(!NBGestorArchivos::crearCarpeta(strTmp->str())){
						PRINTF_ERROR("Creando subcarpeta: %s\n", strTmp->str());
					}
				}
			}
		}
	}
	//--------------------------------------
	//Procesar archivos
	//--------------------------------------
	{
		strTmp->vaciar();
		strTmp->agregar(carpetaBaseAltaResolucion);
		if(carpetaExtra[0] != '\0'){ strTmp->agregar('/'); strTmp->agregar(carpetaExtra); }
		NBGestorArchivos::listarArchivosDe(strTmp->str(), archivosHD);
		//PRINTF_INFO("%d archivos en subcarpeta '%s'.\n", archivosHD->conteo, strTmp->str());
		int iArch;
		for(iArch=0; iArch<archivosHD->conteo; iArch++){
			AUCadena8* nombreRecurso = (AUCadena8*)archivosHD->elemento[iArch];
			//Format by extension
			const ENGenResFileFormat formato = NBToolImagesScaler::fileFormatByName(nombreRecurso->str());
			if(formato == ENGenResFileFormat_NoDef){
				if(acumularResultadosEn != NULL){
					acumularResultadosEn->qNotImgs++;
				}
			} else {
				strTmp->establecer(carpetaBaseAltaResolucion);
				if(carpetaExtra[0] != '\0'){ strTmp->agregar('/'); strTmp->agregar(carpetaExtra); }
				strTmp->agregar('/');
				strTmp->agregar(nombreRecurso->str());
				//
				strTmp2->establecer(carpetaBaseDestinoResolucionMenor);
				if(carpetaExtra[0] != '\0'){ strTmp2->agregar('/'); strTmp2->agregar(carpetaExtra); }
				strTmp2->agregar('/');
				strTmp2->agregar(nombreRecurso->str());
				//
				strTmp3->vaciar();
				strTmp3->agregar(carpetaExtra); if(carpetaExtra[0] != '\0'){ strTmp3->agregar('/'); }
				strTmp3->agregar(nombreRecurso->str());
				//
				//Find rule
				NBGenResScaleRule localRuleVar;
				const NBGenResScaleRule* ruleForPath = NBToolImagesScaler::configScaleRuleByPath(rules, strTmp3->str());
				if(ruleForPath == NULL){
					ruleForPath = curRule;
				} else {
					//Set values of local var
					localRuleVar = *ruleForPath;
					//Set values of local var (inhired implicits values)
					if(curRule != NULL){
						NBToolImagesScaler::configScaleRuleByExplicits(curRule, &localRuleVar);
					}
					//Set local-var as current rule
					ruleForPath	= &localRuleVar;
				}
				//Apply rule
				if(ruleForPath == NULL){
					//PRINTF_INFO("IGNORANDO archivo '%s' (sin regla definida).\n", strTmp3->str());
					if(acumularResultadosEn != NULL){
						acumularResultadosEn->qIgnoredImgs++;
					}
				} else {
					bool ignore = true;
					bool skip = false;
					//Determine action
					if(ruleForPath->action == ENGenResScaleAction_Ignore){
						ignore = true;
					} else if(ruleForPath->action == ENGenResScaleAction_Process){
						ignore = false;
					} else if(ruleForPath->action == ENGenResScaleAction_ProcessOnce){
						ignore = false;
						AUArchivo* dstFile = NBGestorArchivos::flujoDeArchivo(ENMemoriaTipo_Temporal, strTmp2->str(), ENArchivoModo_SoloLectura);
						if(dstFile != NULL){
							dstFile->cerrar();
							skip = true;
						}
					} else {
						PRINTF_INFO("UNKNOWN ACTION DEFINED (ignoring) for file '%s'.\n", strTmp3->str());
						ignore = true;
					}
					//
					if(ignore){
						//PRINTF_INFO("IGNORANDO archivo '%s' (ignore).\n", strTmp3->str());
						if(acumularResultadosEn != NULL){
							acumularResultadosEn->qIgnoredImgs++;
						}
					} else if(skip){
						//PRINTF_INFO("SALTANDO archivo '%s' (processOnce).\n", strTmp3->str());
						if(acumularResultadosEn != NULL){
							acumularResultadosEn->qSkippedImgs++;
						}
					} else {
						//Acumulate file found
						if(state->filesFound != NULL){
							NBGenResFileTask task;
							NBToolImagesScaler::fileTaskInit(&task, strTmp2->str());
							task.rule		= *ruleForPath;
							task.dstStats	= acumularResultadosEn;
							//Register task
							NBGenResFile srch;
							srch.filePath = strTmp;
							const SI32 fileIndex = state->filesFound->indiceDe(srch);
							if(fileIndex < 0){
								//New file and task
								NBGenResFile data;
								NBToolImagesScaler::fileInit(&data, strTmp->str());
								data.scaleBase2 = escalaBase2Origen;
								if(data.tasks == NULL){
									data.tasks	= new AUArregloNativoMutableP<NBGenResFileTask>();
								}
								data.tasks->agregarElemento(task);
								state->filesFound->agregarElemento(data);
							} else {
								//Add task to existing file
								NBGenResFile* data = state->filesFound->elemPtr(fileIndex);
								NBASSERT(data->scaleBase2 == escalaBase2Origen) 
								if(data->tasks == NULL){
									data->tasks	= new AUArregloNativoMutableP<NBGenResFileTask>();
								}
								data->tasks->agregarElemento(task);
							}
						}
						//Process
						if(profile < ENGenResProfile_Count){
							NBGenResFileTask task;
							task.dstPath	= strTmp2;
							task.rule		= *ruleForPath;
							task.dstStats	= acumularResultadosEn;
							//
							conteoArchivoProcesados += NBToolImagesScaler::generateScaledImage(state, profile, escalaBase2Origen, strTmp->str(), &task, 1);
						}
					}
				}
			}
		}
	}
	//--------------------------------------
	//Procesar subcarpetas
	//--------------------------------------
	{
		SI32 iCarp;
		for(iCarp=0; iCarp<subcarpetasHD->conteo; iCarp++){
			AUCadena8* nombreRecurso = (AUCadena8*)subcarpetasHD->elemento[iCarp];
			//
			strTmp->establecer(carpetaBaseAltaResolucion);
			if(carpetaExtra[0] != '\0'){ strTmp->agregar('/'); strTmp->agregar(carpetaExtra); }
			strTmp->agregar('/');
			strTmp->agregar(nombreRecurso->str());
			//
			strTmp2->establecer(carpetaBaseDestinoResolucionMenor);
			if(carpetaExtra[0] != '\0'){ strTmp2->agregar('/'); strTmp2->agregar(carpetaExtra); }
			strTmp2->agregar('/');
			strTmp2->agregar(nombreRecurso->str());
			//
			strTmp3->vaciar();
			strTmp3->agregar(carpetaExtra); if(carpetaExtra[0] != '\0'){ strTmp3->agregar('/'); }
			strTmp3->agregar(nombreRecurso->str());
			//Find rule
			NBGenResScaleRule localRuleVar;
			const NBGenResScaleRule* ruleForPath = NBToolImagesScaler::configScaleRuleByPath(rules, strTmp3->str());
			if(ruleForPath == NULL){
				ruleForPath = curRule;
			} else {
				//Set values of local-var
				localRuleVar = *ruleForPath;
				//Set values of local-var (inhired implicits values)
				if(curRule != NULL){
					NBToolImagesScaler::configScaleRuleByExplicits(curRule, &localRuleVar);
				}
				//Set local-var as current rule
				ruleForPath	= &localRuleVar;
			}
			//Process
			//PRINTF_INFO("PROCESANDO subcarpeta '%s'%s.\n", strTmp3->str(), (ruleForPath != curRule ? " (EXPLICIT RULE)" : ""));
			conteoArchivoProcesados += NBToolImagesScaler::generateScaledImages(state, profile, carpetaBaseAltaResolucion, carpetaBaseDestinoResolucionMenor, strTmp3->str(), escalaBase2Origen, acumularResultadosEn, rules, ruleForPath);
		}
	}
	//
	strTmp3->liberar(NB_RETENEDOR_THIS);
	strTmp2->liberar(NB_RETENEDOR_THIS);
	strTmp->liberar(NB_RETENEDOR_THIS);
	subcarpetasHD->liberar(NB_RETENEDOR_THIS);
	subcarpetasSD->liberar(NB_RETENEDOR_THIS);
	archivosHD->liberar(NB_RETENEDOR_THIS);
	return conteoArchivoProcesados;
}

SI32 NBToolImagesScaler::generateScaledImage(NBGenResState* state, const ENGenResProfile profile, const SI32 escalaBase2Origen, const char* srcPath, const NBGenResFileTask* tasks, const SI32 tasksCount){
	SI32 r = 0;
	//
	bool contenidoCargado = false;
	AUMapaBitsMutable* bmpHD = new(ENMemoriaTipo_Temporal) AUMapaBitsMutable();
	AUPngChuncksMutable* extraChuncksHD = new(ENMemoriaTipo_Temporal) AUPngChuncksMutable();
	//Format by extension
	const ENGenResFileFormat formato = NBToolImagesScaler::fileFormatByName(srcPath);
	//Interpretar contenido desde archivo
	if(formato == ENGenResFileFormat_Png){
		contenidoCargado = bmpHD->cargarDesdeArchivoPNG(srcPath, true, NULL, extraChuncksHD);
	} else if(formato == ENGenResFileFormat_Jpeg){
		contenidoCargado = bmpHD->cargarDesdeArchivoJPEG(srcPath, true);
	}
	//Procesar contenido
	if(!contenidoCargado){
		PRINTF_ERROR("al interpretar los datos del archivo '%s'\n", srcPath);
		SI32 i;
		for(i = 0 ; i < tasksCount; i++){
			const NBGenResFileTask* task = &tasks[i];
			if(task->dstStats != NULL){
				task->dstStats->qErrorImgs++;
			}
		}
	} else {
		//Cargar atlas desde backup
		if(extraChuncksHD->getChunckById("nbAT") == NULL){
			AUAtlasMapaMutable* atlasMap = new(ENMemoriaTipo_Temporal) AUAtlasMapaMutable(0, 0, 0);
			AUCadenaMutable8* strTmp2 = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
			strTmp2->agregar(srcPath); strTmp2->agregar(".atlas.bak");
			if(AUAtlasMapa::cargarXmlEn(atlasMap, strTmp2->str())){
				if(atlasMap->areasOcupadas()->conteo == 0){
					//PRINTF_WARNING("Empty atlas loaded from '%s' file, ignoring.\n", strTmp2->str());
				} else {
					//PRINTF_INFO("Atlas loaded from '%s' file.\n", strTmp2->str());
					AUArchivoEnBuffer* flujo = new(ENMemoriaTipo_Temporal) AUArchivoEnBuffer();
					flujo->lock();
					if(!AUAtlasMapa::guardarEnXml(atlasMap, flujo)){
						PRINTF_ERROR("Could not generate xml of atlas.\n");
					} else {
						const AUCadena8* buffStream = flujo->buffer();
						extraChuncksHD->setChunck("nbAT", (const BYTE*)buffStream->str(), buffStream->tamano()); //NicaraguaBinaryATlas
					}
					flujo->unlock();
					flujo->liberar();
				}
			}
			strTmp2->liberar();
			atlasMap->liberar();
		}
		//Cargar  mesh desde backup
		if(extraChuncksHD->getChunckById("nbMH") == NULL){
			AUMallaMapaMutable* mallaMap = new(ENMemoriaTipo_Temporal) AUMallaMapaMutable(1);
			AUCadenaMutable8* strTmp2 = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
			strTmp2->agregar(srcPath); strTmp2->agregar(".mesh.bak");
			if(AUMallaMapa::cargarXmlEn(mallaMap, strTmp2->str())){
				if(mallaMap->cortesEnSentido(ENMallaCorteSentido_Vertical)->conteo == 0 || mallaMap->cortesEnSentido(ENMallaCorteSentido_Horizontal)->conteo == 0){
					//PRINTF_WARNING("Empty mesh loaded from '%s' file, ignoring.\n", strTmp2->str());
				} else {
					//PRINTF_INFO("Mesh loaded from '%s' file.\n", strTmp2->str());
					AUArchivoEnBuffer* flujo = new(ENMemoriaTipo_Temporal) AUArchivoEnBuffer();
					flujo->lock();
					if(!AUMallaMapa::guardarEnXml(mallaMap, flujo)){
						PRINTF_ERROR("Could not generate xml of mesh.\n");
					} else {
						const AUCadena8* buffStream = flujo->buffer();
						extraChuncksHD->setChunck("nbMH", (const BYTE*)buffStream->str(), buffStream->tamano()); //NicaraguaBinaryATlas
					}
					flujo->unlock();
					flujo->liberar();
				}
			}
			strTmp2->liberar();
			mallaMap->liberar();
		}
		//Cargar props
		AUMapaBitsPropsMutable* bmpHDProps = NULL;
		{
			if(bmpHDProps == NULL){
				const AUCadenaLarga8* chunckBP = extraChuncksHD->getChunckById("nbBP"); //NicaraguaBinaryBitmapProps
				if(chunckBP != NULL){
					AUMapaBitsPropsMutable* bitmapProps = new(ENMemoriaTipo_Temporal) AUMapaBitsPropsMutable();
					AUArchivoEnBuffer* flujo = new(ENMemoriaTipo_Temporal) AUArchivoEnBuffer(&chunckBP->str()[4], chunckBP->tamano() - 4);
					flujo->lock();
					if(!AUMapaBitsProps::cargarXmlEn(bitmapProps, flujo)){
						PRINTF_ERROR("Could not load bitmapProps from png data chunck: (%d bytes).\n", chunckBP->tamano());
					} else {
						bmpHDProps = bitmapProps;
						bitmapProps->retener();
						//PRINTF_INFO("BitmapProps loaded from png data chunck: (%d bytes).\n", chunckBP->tamano());
					}
					flujo->unlock();
					flujo->liberar();
					bitmapProps->liberar();
				}
			}
			if(bmpHDProps == NULL){
				AUMapaBitsPropsMutable* bitmapProps = new(ENMemoriaTipo_Temporal) AUMapaBitsPropsMutable();
				AUCadenaMutable8* strTmp2 = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
				strTmp2->agregar(srcPath); strTmp2->agregar(".props.bak");
				if(AUMapaBitsProps::cargarXmlEn(bitmapProps, strTmp2->str())){
					//PRINTF_INFO("BitmapProps loaded from '.png.props.bak' file.\n");
					bmpHDProps = bitmapProps;
					bitmapProps->retener();
				}
				strTmp2->liberar();
				bitmapProps->liberar();
			}
			if(bmpHDProps == NULL){
				bmpHDProps = new(ENMemoriaTipo_Temporal) AUMapaBitsPropsMutable();
			}
			//Convert to grayScale (if prefered and necesary)
			if(bmpHDProps->mascaraAlphaPreferido()){
				if(bmpHD->propiedades().color != COLOR_GRIS8){
					const STBitmapPixsDesc pixDesc = bmpHD->pixelsDesc();
					if(pixDesc.mask == (NB_BITMAP_PIXS_DESC_BIT_WHITE & NB_BITMAP_PIXS_DESC_BIT_TRANSP)){
						AUMapaBitsMutable* bmpHD8 = new(ENMemoriaTipo_Temporal) AUMapaBitsMutable(bmpHD, COLOR_ALPHA8);
						bmpHD8->establecerModoGris();
						bmpHD->liberar();
						bmpHD = bmpHD8;
					}
				}
			}
		}
		//--------------------------
		// Process all tasks
		//--------------------------
		SI32 i;
		for(i = 0 ; i < tasksCount; i++){
			const NBGenResFileTask* task = &tasks[i];
			const NBGenResScaleRule* ruleForPath = &task->rule;
			const NBGenResScaleRuleProfile* profForPath = &ruleForPath->profiles[profile];
			const char* dstPath = task->dstPath->str();
			//
			NBASSERT(profForPath->pngCompress.levelOver9 >= 0 && profForPath->pngCompress.levelOver9 <= 9)
			NBASSERT(profForPath->pngCompress.tonesDivider >= 1 && profForPath->pngCompress.tonesDivider <= 128)
			//
			NBASSERT(profForPath->jpgCompress.qualityOver100 >= 0 && profForPath->jpgCompress.qualityOver100 <= 100)
			NBASSERT(profForPath->jpgCompress.smoothOver100 >= 0 && profForPath->jpgCompress.smoothOver100 <= 100)
			NBASSERT(profForPath->jpgCompress.tonesDivider >= 1 && profForPath->jpgCompress.tonesDivider <= 128)
			//
			AUPngChuncksMutable* extraChuncks = new(ENMemoriaTipo_Temporal) AUPngChuncksMutable(extraChuncksHD);
			//Actualizar props
			{
				AUMapaBitsPropsMutable* savingProps = new(ENMemoriaTipo_Temporal) AUMapaBitsPropsMutable(bmpHDProps);
				savingProps->establecerEscalaBase2(escalaBase2Origen / (SI32)ruleForPath->divider);
				//
				AUArchivoEnBuffer* flujo = new(ENMemoriaTipo_Temporal) AUArchivoEnBuffer();
				flujo->lock();
				if(!AUMapaBitsProps::guardarEnXml(savingProps, flujo)){
					PRINTF_ERROR("Could not generate xml for props.\n");
				} else {
					const AUCadena8* buffStream = flujo->buffer();
					extraChuncks->setChunck("nbBP", (const BYTE*)buffStream->str(), buffStream->tamano()); //NicaraguaBinaryBitmapProperties
				}
				flujo->unlock();
				flujo->liberar();
				savingProps->liberar();
			}
			//Advertir sobre tamano no multiplo de divisor
			{
				const NBTamanoI tamMapaBits = bmpHD->tamano();
				if((tamMapaBits.ancho % (SI32)ruleForPath->divider) != 0 || (tamMapaBits.alto % (SI32)ruleForPath->divider) != 0){
					PRINTF_WARNING("dimension(es) (%d x %d) no multiplo de %d: %s (utilice la 'devtool' para corregir esto)\n", tamMapaBits.ancho, tamMapaBits.alto, (SI32)ruleForPath->divider, dstPath);
				}
			}
			//Crear copia escalada
			AUMapaBits* bmpScaled = NULL;
			{
				if(ruleForPath->divider == 1){
					//Optimization: use the same bitmap (avoid creating a copy).
					bmpScaled = bmpHD;
					bmpScaled->retener();
				} else {
					bmpScaled = new(ENMemoriaTipo_Temporal) AUMapaBits(bmpHD, ruleForPath->divider);
				}
			}
			//ANALIZAR MAPA DE BITS ESCALADO
			AUArregloNativoOrdenadoMutableP<NBColor8>* palette	= NULL;
			const MapaDeBitsDesciptor bmpScaledProps = bmpScaled->propiedades();
			const UI32 qScaledPixels				= (bmpScaledProps.ancho * bmpScaledProps.alto);
			const STBitmapPixsDesc pixsDesc			= bmpScaled->pixelsDesc();
			//Save image
			bool imgSaved = false;
			if(formato == ENGenResFileFormat_Jpeg){
				//-------------------
				// JPEG
				//-------------------
				//Posterize
				if(profForPath->jpgCompress.tonesDivider >= 2 && profForPath->jpgCompress.tonesDivider <= 128){
					if(bmpScaledProps.color == COLOR_RGB8 || bmpScaledProps.color == COLOR_RGBA8 || bmpScaledProps.color == COLOR_ARGB8){
						//Copy bitmap if needed (undo optimization "avoid creating a copy")
						if(bmpScaled == bmpHD){
							bmpScaled->liberar();
							bmpScaled = new(ENMemoriaTipo_Temporal) AUMapaBits(bmpHD, ruleForPath->divider);
						}
						bmpScaled->posterizeByDivider(profForPath->jpgCompress.tonesDivider);
					}
				}
				//Optimization: save using reusable-buffer
				void* reusableSaveState = NULL;
				if(state != NULL){
					NBToolImagesScaler::statePrepareJPEG(state);
					reusableSaveState = state->jpegSaveState;
				}
				//Save JPEG
				if(!bmpScaled->guardarComoJPEG(dstPath, profForPath->jpgCompress.qualityOver100, profForPath->jpgCompress.smoothOver100, reusableSaveState)){
					PRINTF_ERROR("al intentar guardar mapa de bits escalado JPEG: %s\n", dstPath);
				} else {
					imgSaved = true;
					//Test saved image
					if(ruleForPath->debug.testAfterSaving){
						AUMapaBitsMutable* testImg = new AUMapaBitsMutable();
						if(!testImg->cargarDesdeArchivoJPEG(dstPath, true)){
							PRINTF_ERROR("Load test failed for generated JPG file: '%s'.\n", dstPath);
							//NBASSERT(false);
							imgSaved = false;
						}
						testImg->liberar();
					}
				}
			} else if(formato == ENGenResFileFormat_Png){
				//-------------------
				// PNG
				//-------------------
				//Posterize
				if(profForPath->pngCompress.tonesDivider >= 2 && profForPath->pngCompress.tonesDivider <= 128){
					if(bmpScaledProps.color == COLOR_RGB8 || bmpScaledProps.color == COLOR_RGBA8 || bmpScaledProps.color == COLOR_ARGB8){
						//Copy bitmap if needed (undo optimization "avoid creating a copy")
						if(bmpScaled == bmpHD){
							bmpScaled->liberar();
							bmpScaled = new(ENMemoriaTipo_Temporal) AUMapaBits(bmpHD, ruleForPath->divider);
						}
						bmpScaled->posterizeByDivider(profForPath->pngCompress.tonesDivider);
					}
				}
				// Calculate palette
				if(profForPath->pngCompress.paletteAction == ENGenResPngPaletteAction_Auto){
					if((pixsDesc.mask & NB_BITMAP_PIXS_DESC_BIT_RGB) != 0){
						palette	= new(ENMemoriaTipo_Temporal) AUArregloNativoOrdenadoMutableP<NBColor8>(257);
						bmpScaled->pixelsPalette(palette, 257);
					}
				}
				//Optimization: save using reusable-buffer
				NBPngSaveState* reusableSaveState = NULL;
				if(state != NULL){
					NBToolImagesScaler::statePreparePNG(state);
					reusableSaveState = state->pngSaveState;
				}
				//Save PNG
				UI32 bytesDatosComprimidosDelPNG = 0;
				if(!bmpScaled->guardarComoPNG(dstPath, true, &pixsDesc, palette, &bytesDatosComprimidosDelPNG, extraChuncks, reusableSaveState, (ENPngCompressLevel)profForPath->pngCompress.levelOver9)){
					PRINTF_ERROR("al intentar guardar mapa de bits escalado PNG: %s\n", dstPath);
				} else {
					imgSaved = true;
					//Test saved image
					if(ruleForPath->debug.testAfterSaving){
						AUMapaBitsMutable* testImg = new AUMapaBitsMutable();
						if(!testImg->cargarDesdeArchivoPNG(dstPath, true)){
							PRINTF_ERROR("Load test failed for generated PNG file: '%s'.\n", dstPath);
							//NBASSERT(false);
							imgSaved = false;
						}
						testImg->liberar();
					}
				}
			}
			//Acumulate results
			{
				if(imgSaved){
					r++;
				}
				//
				if(task->dstStats != NULL){
					if(!imgSaved){
						task->dstStats->qErrorImgs++;
					} else {
						if((pixsDesc.mask & NB_BITMAP_PIXS_DESC_BIT_RGB) == 0){
							//Black, gray or white bitmap
							if((pixsDesc.mask & NB_BITMAP_PIXS_DESC_BIT_TRANSP) == 0){
								task->dstStats->conteoGris++;
								task->dstStats->pixelsGris += qScaledPixels;
							} else {
								task->dstStats->conteoGrisAlpha++;
								task->dstStats->pixelsGrisAlpha += qScaledPixels;
							}
						} else {
							//Colored bitmap (not black, gray nor white)
							if((pixsDesc.mask & NB_BITMAP_PIXS_DESC_BIT_TRANSP) == 0){
								if(palette == NULL){
									task->dstStats->conteoRGB++;
									task->dstStats->pixelsRGB += qScaledPixels;
								} else {
									if(palette->conteo > 256){
										task->dstStats->conteoRGB++;
										task->dstStats->pixelsRGB += qScaledPixels;
									} else {
										task->dstStats->conteoRGB_Paleta++;
										task->dstStats->pixelsRGB_Paleta += qScaledPixels;
									}
								}
							} else {
								if(palette == NULL){
									task->dstStats->conteoRGBA++;
									task->dstStats->pixelsRGBA += qScaledPixels;
								} else {
									if(palette->conteo > 256){
										task->dstStats->conteoRGBA++;
										task->dstStats->pixelsRGBA += qScaledPixels;
									} else {
										task->dstStats->conteoRGBA_Paleta++;
										task->dstStats->pixelsRGBA_Paleta += qScaledPixels;
									}
								}
							}
						}
						//
						if(formato == ENGenResFileFormat_Jpeg){
							task->dstStats->qFormatJpeg++;
						} else if(formato == ENGenResFileFormat_Png){
							task->dstStats->qFormatPng++;
						}
						//
						task->dstStats->qProcessedImgs++;
						task->dstStats->pixelsTotal += qScaledPixels;
					}
				}
			}
			//
			if(palette != NULL) palette->liberar(NB_RETENEDOR_THIS);
			bmpScaled->liberar(NB_RETENEDOR_THIS);
			extraChuncks->liberar(NB_RETENEDOR_THIS);
		}
		bmpHDProps->liberar(NB_RETENEDOR_THIS);
	}
	extraChuncksHD->liberar(NB_RETENEDOR_THIS);
	bmpHD->liberar(NB_RETENEDOR_THIS);
	//
	return r;
}

void NBToolImagesScaler::pngAgregarResumenMapaBits(const char* carpetaPNGsSinPlecaAlFinal, AUCadenaLargaMutable8* cadenaCompartida, AUArregloNativoMutableP<NBDevResumenMapaBits>* propiedades){
	AUArregloMutable* subcarpetasHD = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUArregloMutable* archivosHD = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUCadenaMutable8* strTmp = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	//Procesar archivos PNG
	NBGestorArchivos::listarArchivosDe(carpetaPNGsSinPlecaAlFinal, archivosHD);
	int iArch;
	for(iArch=0; iArch<archivosHD->conteo; iArch++){
		AUCadena8* nombreRecurso = (AUCadena8*)archivosHD->elemento[iArch];
		if(nombreRecurso->tamano() > 4){
			const char* ult4 = &nombreRecurso->str()[nombreRecurso->tamano() - 4];
			if(ult4[0] == '.' && (ult4[1] == 'p' || ult4[1] == 'P') && (ult4[2] == 'n' || ult4[2] == 'N') && (ult4[3] == 'g' || ult4[3] == 'G')){
				strTmp->establecer(carpetaPNGsSinPlecaAlFinal);
				strTmp->agregar("/");
				strTmp->agregar(nombreRecurso->str());
				//PRINTF_INFO("PNG: procesando archivo %s\n", strTmp->str());
				AUMapaBitsMutable* bmpHD = new(ENMemoriaTipo_Temporal) AUMapaBitsMutable();
				if(!bmpHD->cargarDesdeArchivoPNG(strTmp->str(), false, NULL)){
					PRINTF_ERROR("al interpretar los datos del archivo '%s'\n", strTmp->str());
				} else {
					NBDevResumenMapaBits datosResumen;
					datosResumen.indiceRutaPNG = cadenaCompartida->tamano(); cadenaCompartida->agregar(strTmp->str(), strTmp->tamano()+1, strCopiarPlecasCero);
					datosResumen.tamanoMapaBits = bmpHD->tamano();
					propiedades->agregarElemento(datosResumen);
				}
				bmpHD->liberar(NB_RETENEDOR_THIS);
				bmpHD = NULL;
			}
		}
	}
	//Procesar subcarpetas
	NBGestorArchivos::listarSubcarpetasDe(carpetaPNGsSinPlecaAlFinal, subcarpetasHD);
	int iCarp;
	for(iCarp=0; iCarp<subcarpetasHD->conteo; iCarp++){
		AUCadena8* nombreRecurso = (AUCadena8*)subcarpetasHD->elemento[iCarp];
		strTmp->establecer(carpetaPNGsSinPlecaAlFinal);
		strTmp->agregar("/");
		strTmp->agregar(nombreRecurso->str());
		NBToolImagesScaler::pngAgregarResumenMapaBits(strTmp->str(), cadenaCompartida, propiedades);
	}
	//
	strTmp->liberar(NB_RETENEDOR_THIS);
	subcarpetasHD->liberar(NB_RETENEDOR_THIS);
	archivosHD->liberar(NB_RETENEDOR_THIS);
}

int NBToolImagesScaler::pngAnalizarMapaBitsParecidos(const char* cadenaCompartida, NBDevResumenMapaBits* propiedadesAchivos, int conteoArchivos, UI8 maxMargenAbsPorComponenteRGBA){
	int conteoDuplicadosEncontrados = 0;
	int i;
	for(i=0; i<conteoArchivos; i++){
		int i2, conteoParecidosEncontrados = 0;
		for(i2=i+1; i2<conteoArchivos; i2++){
			NBDevResumenMapaBits props1 = propiedadesAchivos[i];
			NBDevResumenMapaBits props2 = propiedadesAchivos[i2];
			if(props1.tamanoMapaBits==props2.tamanoMapaBits){
				AUMapaBitsMutable* mapaBits1 = new(ENMemoriaTipo_Temporal) AUMapaBitsMutable();
				AUMapaBitsMutable* mapaBits2 = new(ENMemoriaTipo_Temporal) AUMapaBitsMutable();
				if(!mapaBits1->cargarDesdeArchivoPNG(&cadenaCompartida[props1.indiceRutaPNG], true, NULL)){
					PRINTF_ERROR("al interpretar los datos del archivo (1) '%s'\n", &cadenaCompartida[props1.indiceRutaPNG]);
				} else if(!mapaBits2->cargarDesdeArchivoPNG(&cadenaCompartida[props2.indiceRutaPNG], true, NULL)){
					PRINTF_ERROR("al interpretar los datos del archivo (2) '%s'\n", &cadenaCompartida[props2.indiceRutaPNG]);
				} else if(mapaBits1->esParecido(mapaBits2, maxMargenAbsPorComponenteRGBA)) {
					conteoParecidosEncontrados++;
					if(conteoParecidosEncontrados==1) PRINTF_INFO("Parecido entre archivos:\n 1) %s\n", &cadenaCompartida[props1.indiceRutaPNG]);
					PRINTF_INFO("%d) %s\n", conteoParecidosEncontrados+1, &cadenaCompartida[props2.indiceRutaPNG]);
				}
				mapaBits1->liberar();
				mapaBits2->liberar();
			}
		}
		conteoDuplicadosEncontrados += conteoParecidosEncontrados;
	}
	return conteoDuplicadosEncontrados;
}

/*int NBToolImagesScaler::listarArchivosFuenteEnUsoQueNoExistanEnDestino(const char* carpetaArchivosQueUsan, const char* carpetaPNGsFuente, const char* carpetaPNGsDestino, SI32* acumularConteoProcesadosEn){
	SI32 conteoCasos = 0;
	AUCadenaMutable8* strTmp = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	AUCadenaMutable8* strTmp2 = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	AUCadenaMutable8* strReferenciaEncontrada = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	//PRINTF_INFO("PNG: procesando carpeta %s\n", carpetaPNGsAltaResolucion);
	//Procesar archivos PNG
	AUArregloMutable* archivosFuente = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUArregloMutable* archivosDestino = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	NBGestorArchivos::listarArchivosDe(carpetaPNGsFuente, archivosFuente);
	NBGestorArchivos::listarArchivosDe(carpetaPNGsDestino, archivosDestino);
	int iArch;
	for(iArch=0; iArch<archivosFuente->conteo; iArch++){
		AUCadena8* nombreRecurso = (AUCadena8*)archivosFuente->elemento[iArch];
		if(nombreRecurso->tamano() > 4){
		const char* ult4 = &nombreRecurso->str()[nombreRecurso->tamano() - 4];
		if(ult4[0] == '.' && (ult4[1] == 'p' || ult4[1] == 'P') && (ult4[2] == 'n' || ult4[2] == 'N') && (ult4[3] == 'g' || ult4[3] == 'G')){
			*acumularConteoProcesadosEn = *acumularConteoProcesadosEn + 1;
			SI16 i, iRecurso = -1;
			for(i=0; i<archivosDestino->conteo; i++){
				if(((AUCadena8*)archivosDestino->elemento[i])->esIgual(nombreRecurso->str())){
					iRecurso = i; break;
				}
			}
			if(iRecurso==-1){
				if(NBBuscadorArchivosSinReferencias::existeReferenciaHaciaArchivoEnCarpeta(nombreRecurso->str(), carpetaArchivosQueUsan, strReferenciaEncontrada)){
					PRINTF_WARNING("no existe el archivo: '%s/%s' (primera referencia: '%s')\n", carpetaPNGsDestino, nombreRecurso->str(), strReferenciaEncontrada->str());
					conteoCasos++;
				}
			}
		}
		}
	}
	archivosDestino->liberar(NB_RETENEDOR_THIS);
	archivosFuente->liberar(NB_RETENEDOR_THIS);
	//Procesar subcarpetas
	AUArregloMutable* subcarpetasFuente = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUArregloMutable* subcarpetasDestino = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	NBGestorArchivos::listarSubcarpetasDe(carpetaPNGsFuente, subcarpetasFuente);
	NBGestorArchivos::listarSubcarpetasDe(carpetaPNGsDestino, subcarpetasDestino);
	int iCarp;
	for(iCarp=0; iCarp<subcarpetasFuente->conteo; iCarp++){
		AUCadena8* nombreRecurso = (AUCadena8*)subcarpetasFuente->elemento[iCarp];
		*acumularConteoProcesadosEn = *acumularConteoProcesadosEn + 1;
		SI16 i, iRecurso = -1;
		for(i=0; i<subcarpetasDestino->conteo; i++){
			if(((AUCadena8*)subcarpetasDestino->elemento[i])->esIgual(nombreRecurso->str())){
				iRecurso = i; break;
			}
		}
		if(iRecurso==-1){
			PRINTF_WARNING("no existe la subcarpeta: '%s/%s' en ''\n", carpetaPNGsDestino, nombreRecurso->str());
			conteoCasos++;
		} else {
			AUCadena8* nombreRecurso = (AUCadena8*)subcarpetasFuente->elemento[iCarp];
			strTmp->establecer(carpetaPNGsFuente);
			strTmp->agregar("/");
			strTmp->agregar(nombreRecurso->str());
			strTmp2->establecer(carpetaPNGsDestino);
			strTmp2->agregar("/");
			strTmp2->agregar(nombreRecurso->str());
			conteoCasos += NBToolImagesScaler::listarArchivosFuenteEnUsoQueNoExistanEnDestino(carpetaArchivosQueUsan, strTmp->str(), strTmp2->str(), acumularConteoProcesadosEn);
		}
	}
	subcarpetasDestino->liberar(NB_RETENEDOR_THIS);
	subcarpetasFuente->liberar(NB_RETENEDOR_THIS);
	//
	strReferenciaEncontrada->liberar(NB_RETENEDOR_THIS);
	strTmp2->liberar(NB_RETENEDOR_THIS);
	strTmp->liberar(NB_RETENEDOR_THIS);
	//
	return conteoCasos;
}*/

void NBToolImagesScaler::actualizarImgsHaciaTamanoMultiploDe(const char* carpetaPNGs, SI32 tamanoMultiploDe, SI32* acumularConteoRedimensionadasEn, SI32* acumularConteoIgnoradasEn){
	AUArregloMutable* subcarpetas	= new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUArregloMutable* archivos		= new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUCadenaMutable8* strTmp		= new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	//AUCadenaMutable8* strTmp2		= new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	SI32 conteoRedimensionadas		= 0;
	SI32 conteoIgnoradas			= 0;
	//Procesar archivos PNG
	NBGestorArchivos::listarArchivosDe(carpetaPNGs, archivos);
	int iArch;
	for(iArch = 0; iArch < archivos->conteo; iArch++){
		AUCadena8* nombreRecurso = (AUCadena8*)archivos->elemento[iArch];
		if(nombreRecurso->tamano() > 4){
			ENGenResFileFormat formato = ENGenResFileFormat_NoDef;
			//Verificar extension PNG o JPG
			if(formato == ENGenResFileFormat_NoDef && nombreRecurso->tamano() > 4){
				const char* buffFinal = &nombreRecurso->str()[nombreRecurso->tamano() - 4];
				if((buffFinal[0] == '.' && (buffFinal[1] == 'p' || buffFinal[1] == 'P') && (buffFinal[2] == 'n' || buffFinal[2] == 'N') && (buffFinal[3] == 'g' || buffFinal[3] == 'G'))){
					formato = ENGenResFileFormat_Png;
				} else if((buffFinal[0] == '.' && (buffFinal[1] == 'j' || buffFinal[1] == 'J') && (buffFinal[2] == 'p' || buffFinal[2] == 'P') && (buffFinal[3] == 'g' || buffFinal[3] == 'G'))){
					formato = ENGenResFileFormat_Jpeg;
				}
			}
			//Verificar extension JPEG
			if(formato == ENGenResFileFormat_NoDef && nombreRecurso->tamano() > 5){
				const char* buffFinal = &nombreRecurso->str()[nombreRecurso->tamano() - 5];
				if(buffFinal[0] == '.' && (buffFinal[1] == 'j' || buffFinal[1] == 'J') && (buffFinal[2] == 'p' || buffFinal[2] == 'P') && (buffFinal[3] == 'e' || buffFinal[3] == 'E') && (buffFinal[4] == 'g' || buffFinal[4] == 'G')){
					formato = ENGenResFileFormat_Jpeg;
				}
			}
			if(formato == ENGenResFileFormat_NoDef){
				conteoIgnoradas++;
			} else {
				strTmp->establecer(carpetaPNGs);
				strTmp->agregar("/");
				strTmp->agregar(nombreRecurso->str());
				//
				AUMapaBitsMutable* bitmapHeader = NULL;
				//---------------------
				// Load header only
				//---------------------
				if(formato == ENGenResFileFormat_Png){
					bitmapHeader = new(ENMemoriaTipo_Temporal) AUMapaBitsMutable();
					if(!bitmapHeader->cargarDesdeArchivoPNG(strTmp->str(), false, NULL, (AUPngChuncksMutable*)NULL)){
						PRINTF_ERROR("Al interpretar el encabezado del archivo PNG '%s'\n", strTmp->str());
						bitmapHeader->liberar();
						bitmapHeader = NULL;
					}
				} else if(formato == ENGenResFileFormat_Jpeg){
					bitmapHeader = new(ENMemoriaTipo_Temporal) AUMapaBitsMutable();
					if(!bitmapHeader->cargarDesdeArchivoJPEG(strTmp->str(), false)){
						PRINTF_ERROR("Al interpretar el encabezado del archivo JPEG '%s'\n", strTmp->str());
						bitmapHeader->liberar();
						bitmapHeader = NULL;
					}
				} else {
					NBASSERT(false)
				}
				//---------------------
				// Check size first
				//---------------------
				if(bitmapHeader == NULL){
					conteoIgnoradas++;
				} else {
					const NBTamanoI tamMapaBits	= bitmapHeader->tamano();
					SI32 sobranteMultiploAncho	= (tamanoMultiploDe - (tamMapaBits.ancho % tamanoMultiploDe));
					SI32 sobranteMultiploAlto	= (tamanoMultiploDe - (tamMapaBits.alto % tamanoMultiploDe));
					if(sobranteMultiploAncho == tamanoMultiploDe){
						sobranteMultiploAncho = 0;
					}
					if(sobranteMultiploAlto == tamanoMultiploDe){
						sobranteMultiploAlto = 0;
					}
					if(sobranteMultiploAncho == 0 && sobranteMultiploAlto == 0){
						conteoIgnoradas++;
					} else {
						//PRINTF_INFO("PNG: procesando archivo %s\n", strTmp->str());
						AUPngChuncksMutable* extraChuncks = new(ENMemoriaTipo_Temporal) AUPngChuncksMutable();
						AUMapaBitsMutable* bmpHD = NULL;
						//---------------------
						// Load
						//---------------------
						if(formato == ENGenResFileFormat_Png){
							bmpHD = new(ENMemoriaTipo_Temporal) AUMapaBitsMutable();
							if(!bmpHD->cargarDesdeArchivoPNG(strTmp->str(), true, NULL, extraChuncks)){
								PRINTF_ERROR("Al interpretar los datos del archivo PNG '%s'\n", strTmp->str());
								bmpHD->liberar();
								bmpHD = NULL;
							} else {
								NBASSERT(bmpHD->datos()!=NULL)
							}
						} else if(formato == ENGenResFileFormat_Jpeg){
							bmpHD = new(ENMemoriaTipo_Temporal) AUMapaBitsMutable();
							if(!bmpHD->cargarDesdeArchivoJPEG(strTmp->str(), true)){
								PRINTF_ERROR("Al interpretar los datos del archivo JPEG '%s'\n", strTmp->str());
								bmpHD->liberar();
								bmpHD = NULL;
							} else {
								NBASSERT(bmpHD->datos()!=NULL)
							}
						} else {
							NBASSERT(false)
						}
						//---------------------
						// Process
						//---------------------
						if(bmpHD == NULL){
							conteoIgnoradas++;
						} else {
							NBASSERT(bmpHD->datos() != NULL)
							const NBTamanoI tamMapaBits	= bmpHD->tamano();
							SI32 sobranteMultiploAncho	= (tamanoMultiploDe - (tamMapaBits.ancho % tamanoMultiploDe));
							SI32 sobranteMultiploAlto	= (tamanoMultiploDe - (tamMapaBits.alto % tamanoMultiploDe));
							if(sobranteMultiploAncho == tamanoMultiploDe){
								sobranteMultiploAncho = 0;
							}
							if(sobranteMultiploAlto == tamanoMultiploDe){
								sobranteMultiploAlto = 0;
							}
							if(sobranteMultiploAncho == 0 && sobranteMultiploAlto == 0){
								conteoIgnoradas++;
							} else {
								PRINTF_INFO("PROCESANDO (%d+%d x %d+%d) a multiplo de %d: %s\n", tamMapaBits.ancho, sobranteMultiploAncho, tamMapaBits.alto, sobranteMultiploAlto, tamanoMultiploDe, strTmp->str());
								const MapaDeBitsDesciptor propiedadesBMP = bmpHD->propiedades();
								const bool hasAlphaChannel = (propiedadesBMP.color==COLOR_RGBA8 || propiedadesBMP.color==COLOR_GRISALPHA8 || propiedadesBMP.color==COLOR_ALPHA8);
								//Asegurar un mapa de bits fuente que sea RGBA
								AUMapaBits* mapaBitsRGBA = NULL;
								if(propiedadesBMP.color == COLOR_RGBA8){
									mapaBitsRGBA = bmpHD;
									mapaBitsRGBA->retener();
								} else {
									mapaBitsRGBA = new(ENMemoriaTipo_Temporal) AUMapaBits(bmpHD, COLOR_RGBA8);
								}
								const SI32 tamXFinal	= (propiedadesBMP.ancho + sobranteMultiploAncho);
								const SI32 tamYFinal	= (propiedadesBMP.alto + sobranteMultiploAlto);
								AUMapaBitsLienzo* lienzoMultiplo = new(ENMemoriaTipo_Temporal) AUMapaBitsLienzo(tamXFinal, tamYFinal);
								lienzoMultiplo->formatearLienzo(0, 0, 0, (hasAlphaChannel ? 0 : 255));
								const SI32 posXDst = 0; //(sobranteMultiploAncho / 2); //Note: better conserve the image origin, otherwise invalidation of atlasMap or meshMap may ocur (in pgn-chuncks)
								const SI32 posYDst = 0; //(sobranteMultiploAlto / 2); //Note: better conserve the image origin, otherwise invalidation of atlasMap or meshMap may ocur (in pgn-chuncks)
								lienzoMultiplo->pegarMapaBitsRGBA(posXDst, posYDst, mapaBitsRGBA);
								//Analizar si tiene alphas
								bool transpExists = false;
								if(hasAlphaChannel){
									const MapaDeBitsDesciptor rgbaProps = mapaBitsRGBA->propiedades();
									const UI8* rgbaData = mapaBitsRGBA->datos();
									if(rgbaData != NULL){
										SI32 y;
										for(y = 0; y < rgbaProps.alto; y++){
											const UI8* pixData = &rgbaData[y * rgbaProps.bytesPorLinea];
											const UI8* pixNestLast = pixData + rgbaProps.bytesPorLinea;
											while(pixData < pixNestLast){
												if(pixData[3] != 255){
													transpExists = true;
													break;
												}
												pixData += 4;
											}
											if(transpExists){
												break;
											}
										}
										
									}
								}
								//Rellenar espacios vacios en los bordes, repitiendo la ultima fila o columna.
								if(!transpExists){
									SI32 i;
									//relleno izquierda
									for(i = 0; i < posXDst; i++){
										lienzoMultiplo->pegarMapaBitsRGBA(i, 0, mapaBitsRGBA, 0/*x*/, 0/*y*/, 1/*ancho*/, tamMapaBits.alto/*alto*/);
									}
									//relleno derecha
									for(i = (posXDst + tamMapaBits.ancho); i < tamXFinal; i++){
										lienzoMultiplo->pegarMapaBitsRGBA(i, 0, mapaBitsRGBA, (tamMapaBits.ancho - 1)/*x*/, 0/*y*/, 1/*ancho*/, tamMapaBits.alto/*alto*/);
									}
									//relleno arriba
									for(i = 0; i < posYDst; i++){
										lienzoMultiplo->pegarMapaBitsRGBA(0, i, mapaBitsRGBA, 0/*x*/, 0/*y*/, tamMapaBits.ancho/*ancho*/, 1/*alto*/);
									}
									//relleno abajo
									for(i = (posYDst + tamMapaBits.alto); i < tamYFinal; i++){
										lienzoMultiplo->pegarMapaBitsRGBA(0, i, mapaBitsRGBA, 0/*x*/, (tamMapaBits.alto - 1)/*y*/, tamMapaBits.ancho/*ancho*/, 1/*alto*/);
									}
								}
								//bool AUMapaBitsLienzo::pegarMapaBitsRGBA(SI32 posXDestino, SI32 posYDestino, AUMapaBits* mapaBitsFuente, SI32 posXFuente, SI32 posYFuente, SI32 anchoDibujarFuente, SI32 altoDibujarFuente){
								//---------------------
								// Save
								//---------------------
								if(formato == ENGenResFileFormat_Png){
									AUArregloNativoOrdenadoMutableP<NBColor8>* palette = NULL;
									const STBitmapPixsDesc pixsDesc			= lienzoMultiplo->pixelsDesc();
									if((pixsDesc.mask & NB_BITMAP_PIXS_DESC_BIT_RGB) != 0){
										palette	= new(ENMemoriaTipo_Temporal) AUArregloNativoOrdenadoMutableP<NBColor8>(257);
										lienzoMultiplo->pixelsPalette(palette, 257);
									}
									//
									if(!lienzoMultiplo->guardarComoPNG(strTmp->str(), false, &pixsDesc, palette, NULL, extraChuncks)){
										PRINTF_ERROR("Guardando archivo: %s\n", strTmp->str());
									}
									//
									if(palette != NULL) palette->liberar();
								} else if(formato == ENGenResFileFormat_Jpeg){
									AUMapaBits* mapaBitsRGB = new(ENMemoriaTipo_Temporal) AUMapaBits(lienzoMultiplo, COLOR_RGB8);
									if(!mapaBitsRGB->guardarComoJPEG(strTmp->str(), 98, 0)){
										PRINTF_ERROR("Guardando archivo: %s\n", strTmp->str());
									}
									mapaBitsRGB->liberar();
								} else {
									NBASSERT(false)
								}
								//
								lienzoMultiplo->liberar();
								mapaBitsRGBA->liberar();
								conteoRedimensionadas++;
							}
							bmpHD->liberar(NB_RETENEDOR_THIS);
						}
						extraChuncks->liberar();
					}
					bitmapHeader->liberar();
				}
			}
		}
	}
	//Procesar subcarpetas
	NBGestorArchivos::listarSubcarpetasDe(carpetaPNGs, subcarpetas);
	int iCarp;
	for(iCarp = 0; iCarp < subcarpetas->conteo; iCarp++){
		AUCadena8* nombreRecurso = (AUCadena8*)subcarpetas->elemento[iCarp];
		strTmp->establecer(carpetaPNGs);
		strTmp->agregar("/");
		strTmp->agregar(nombreRecurso->str());
		NBToolImagesScaler::actualizarImgsHaciaTamanoMultiploDe(strTmp->str(), tamanoMultiploDe, acumularConteoRedimensionadasEn, acumularConteoIgnoradasEn);
	}
	//
	if(acumularConteoRedimensionadasEn!=NULL) *acumularConteoRedimensionadasEn += conteoRedimensionadas;
	if(acumularConteoIgnoradasEn!=NULL) *acumularConteoIgnoradasEn += conteoIgnoradas;
	//
	//strTmp2->liberar(NB_RETENEDOR_THIS);
	strTmp->liberar(NB_RETENEDOR_THIS);
	subcarpetas->liberar(NB_RETENEDOR_THIS);
	archivos->liberar(NB_RETENEDOR_THIS);
}


int NBToolImagesScaler::generarCacheBinariaPNGs(const char* carpetaPNGs, const char* carpetaCachesBinarias){
	int conteoArchivoProcesados = 0;
	AUArregloMutable* subcarpetasPNG = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUArregloMutable* subcarpetasBIN = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUArregloMutable* archivosPNG = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUCadenaMutable8* strTmp = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	AUCadenaMutable8* strTmp2 = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	//PRINTF_INFO("PNG: procesando carpeta %s\n", carpetaPNGsAltaResolucion);
	//Asegurar la existencia de las carpetas
	NBGestorArchivos::listarSubcarpetasDe(carpetaPNGs, subcarpetasPNG);
	NBGestorArchivos::listarSubcarpetasDe(carpetaCachesBinarias, subcarpetasBIN);
	int iCarp;
	for(iCarp=0; iCarp<subcarpetasPNG->conteo; iCarp++){
		AUCadena8* nombreRecurso = (AUCadena8*)subcarpetasPNG->elemento[iCarp];
		SI16 i, iRecurso = -1;
		for(i=0; i<subcarpetasBIN->conteo; i++){
			if(((AUCadena8*)subcarpetasBIN->elemento[i])->esIgual(nombreRecurso->str())){
				iRecurso = i; break;
			}
		}
		if(iRecurso==-1){
			strTmp2->establecer(carpetaCachesBinarias);
			strTmp2->agregar("/");
			strTmp2->agregar(nombreRecurso->str());
			if(!NBGestorArchivos::crearCarpeta(strTmp2->str())){
				PRINTF_ERROR("creando subcarpeta: %s\n", strTmp2->str());
			}
		}
	}
	//Procesar archivos PNG
	NBGestorArchivos::listarArchivosDe(carpetaPNGs, archivosPNG);
	int iArch;
	for(iArch=0; iArch<archivosPNG->conteo; iArch++){
		AUCadena8* nombreRecurso = (AUCadena8*)archivosPNG->elemento[iArch];
		if(nombreRecurso->tamano() > 4){
			const char* ult4 = &nombreRecurso->str()[nombreRecurso->tamano() - 4];
			if(ult4[0] == '.' && (ult4[1] == 'p' || ult4[1] == 'P') && (ult4[2] == 'n' || ult4[2] == 'N') && (ult4[3] == 'g' || ult4[3] == 'G')){
				strTmp->establecer(carpetaPNGs);
				strTmp->agregar("/");
				strTmp->agregar(nombreRecurso->str());
				strTmp2->establecer(carpetaCachesBinarias);
				strTmp2->agregar("/");
				strTmp2->agregar(nombreRecurso->str());
				strTmp2->quitarDerecha(4); //quitar extension ".png"
				strTmp2->agregar(".mapaBits");
				//PRINTF_INFO("PNG: procesando archivo %s\n", strTmp->str());
				AUMapaBitsMutable* mapaBits = new(ENMemoriaTipo_Temporal) AUMapaBitsMutable();
				if(!mapaBits->cargarDesdeArchivoPNG(strTmp->str(), true, NULL)){
					PRINTF_ERROR("al interpretar los datos del archivo '%s'\n", strTmp->str());
				} else {
					if(!mapaBits->guardarComoMapaBits(strTmp2->str(), true)){
						PRINTF_ERROR("al intentar guardar cache BIN: %s\n", strTmp2->str());
					} else {
						conteoArchivoProcesados++;
					}
				}
				mapaBits->liberar(NB_RETENEDOR_THIS);
			}
		}
	}
	//Procesar subcarpetas
	for(iCarp=0; iCarp<subcarpetasPNG->conteo; iCarp++){
		AUCadena8* nombreRecurso = (AUCadena8*)subcarpetasPNG->elemento[iCarp];
		strTmp->establecer(carpetaPNGs);
		strTmp->agregar("/");
		strTmp->agregar(nombreRecurso->str());
		strTmp2->establecer(carpetaCachesBinarias);
		strTmp2->agregar("/");
		strTmp2->agregar(nombreRecurso->str());
		conteoArchivoProcesados += NBToolImagesScaler::generarCacheBinariaPNGs(strTmp->str(), strTmp2->str());
	}
	//
	strTmp2->liberar(NB_RETENEDOR_THIS);
	strTmp->liberar(NB_RETENEDOR_THIS);
	subcarpetasPNG->liberar(NB_RETENEDOR_THIS);
	subcarpetasBIN->liberar(NB_RETENEDOR_THIS);
	archivosPNG->liberar(NB_RETENEDOR_THIS);
	return conteoArchivoProcesados;
}
