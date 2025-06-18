//
//  NBToolImagesScaler.h
//  DevTools
//
//  Created by Nicaragua Binary on 20/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef NBToolImagesScaler_H_INCLUIDO
#define NBToolImagesScaler_H_INCLUIDO

#include "AUFrameworkBase.h"

typedef enum ENGenResFileFormat_ {
	ENGenResFileFormat_NoDef = 0,
	ENGenResFileFormat_Png,
	ENGenResFileFormat_Jpeg
} ENGenResFileFormat;

//

typedef enum ENGenResProfile_ {
	ENGenResProfile_Dev = 0,	//Usually for fast results (fastest image compresion, skip some bitmaps analysis)
	ENGenResProfile_Pkg,		//Usually for final results (better image compression, more bitmaps analysis)
	ENGenResProfile_Count
} ENGenResProfile;

// Rules

#define NB_GEN_RES_DBG_RULE_TEST_SAVING_BIT	1

typedef struct NBGenResDbgRule_ {
	UI32	maskExplicits;
	//
	bool	testAfterSaving;
} NBGenResDbgRule;

#define NB_GEN_RES_JPG_COMPQUALITY_BIT	1
#define NB_GEN_RES_JPG_COMPSMOOTH_BIT	2
#define NB_GEN_RES_JPG_COMPTONESDIV_BIT	4

typedef struct NBGenResJpgCompressRule_ {
	UI32	maskExplicits;
	//
	UI8		qualityOver100;	//Lossy compression
	UI8		smoothOver100;	//Smoothen image (like blur)
	UI8		tonesDivider;	//Posterization
} NBGenResJpgCompressRule;

#define NB_GEN_RES_PNG_COMPLEVEL_BIT	1
#define NB_GEN_RES_PNG_COMPPALETTE_BIT	2
#define NB_GEN_RES_PNG_COMPTONESDIV_BIT	4

typedef enum ENGenResPngPaletteAction_ {
	ENGenResPngPaletteAction_Auto = 0,	//Calculate if palette is posible.
	ENGenResPngPaletteAction_Never,		//Never use palette
	ENGenResPngPaletteAction_Count
} ENGenResPngPaletteAction;

typedef struct NBGenResPngCompressRule_ {
	UI32	maskExplicits;
	//
	UI8		levelOver9;						//zlib compression level (0 to 9)
	UI8		tonesDivider;					//Posterization
	ENGenResPngPaletteAction paletteAction;	//8 bits palette
} NBGenResPngCompressRule;

#define NB_GEN_RES_CNFG_ACTION_BIT		1
#define NB_GEN_RES_CNFG_DIVIDER_BIT		2
#define NB_GEN_RES_CNFG_PNG_COMP_BIT	4
#define NB_GEN_RES_CNFG_JPG_COMP_BIT	8
#define NB_GEN_RES_CNFG_DEBUG_BIT		16

typedef struct NBGenResScaleRuleProfile_ {
	UI32					maskExplicits;
	//
	NBGenResPngCompressRule	pngCompress;
	NBGenResJpgCompressRule	jpgCompress;
} NBGenResScaleRuleProfile;

typedef enum ENGenResScaleAction_ {
	ENGenResScaleAction_Unknown = 0,
	ENGenResScaleAction_Ignore,			//Ignore
	ENGenResScaleAction_Process,		//Process normal
	ENGenResScaleAction_ProcessOnce		//Process if output file not exists
} ENGenResScaleAction;

typedef struct NBGenResScaleRule_ {
    UI32					maskExplicits;
	//
	UI8						divider;
	ENGenResScaleAction		action;
	NBGenResScaleRuleProfile profiles[ENGenResProfile_Count];
	//
	NBGenResDbgRule			debug;
} NBGenResScaleRule;

typedef struct NBGenResScalePath_ {
	AUCadenaMutable8*		path;
	NBGenResScaleRule		rule;
} NBGenResScalePath;

typedef struct NBGenResScale_ {
	AUCadenaMutable8*		name;
	AUCadenaMutable8*		root;
	SI32					rootScale;
	AUCadenaMutable8*		dstPath;
	//
	AUArregloNativoMutableP<NBGenResScalePath>* rules;
} NBGenResScale;

// Stats

typedef struct NBGenResStats_ {
	//Per format
	SI32 qFormatPng;
	SI32 qFormatJpeg;
	//Per pixel format
	SI32 conteoRGBA;			//Mapas de bits RGBA sin paleta
	UI32 pixelsRGBA;			//Cantidad de pixeles
	SI32 conteoRGBA_Paleta;		//Mapas de bits RGBA con paleta
	UI32 pixelsRGBA_Paleta;		//Cantidad de pixeles
	SI32 conteoRGB;				//Mapas de bits RGB sin paleta
	UI32 pixelsRGB;				//Cantidad de pixeles
	SI32 conteoRGB_Paleta;		//Mapas de bits RGB con paleta
	UI32 pixelsRGB_Paleta;		//Cantidad de pixeles
	SI32 conteoGris;			//Mapas de bits GRIS sin paleta
	UI32 pixelsGris;			//Cantidad de pixeles
	SI32 conteoGrisAlpha;		//Mapas de bits GRIS+Alpha sin paleta
	UI32 pixelsGrisAlpha;		//Cantidad de pixeles
	//
	SI32 qProcessedImgs;
	UI32 pixelsTotal;
	//
	SI32 qNotImgs;				//archivos no imagenes
	SI32 qIgnoredImgs;			//archivos imagenes ignorados
	SI32 qSkippedImgs;			//archivos imagenes previamente procesados
	SI32 qErrorImgs;			//archivos con error
} NBGenResStats;

struct NBDevResumenMapaBits {
	SI32		indiceRutaPNG;
	NBTamanoI	tamanoMapaBits;
	//
	bool operator==(const NBDevResumenMapaBits &otro) const {
		return (indiceRutaPNG==otro.indiceRutaPNG);
	}
	bool operator!=(const NBDevResumenMapaBits &otro) const {
		return !(indiceRutaPNG==otro.indiceRutaPNG);
	}
};


// Files/tasks found

typedef struct NBGenResFileTask_ {
	AUCadena8*			dstPath;
	NBGenResScaleRule	rule;
	NBGenResStats*		dstStats;
} NBGenResFileTask;

typedef struct NBGenResFile_ {
	AUCadena8*		filePath;
	SI32			scaleBase2;
	AUArregloNativoMutableP<NBGenResFileTask>* tasks;
	//Ordering
	bool operator==(const NBGenResFile_ &otro) const { return AUCadena8::esIgual(filePath->str(), otro.filePath->str()); }
	bool operator!=(const NBGenResFile_ &otro) const { return !AUCadena8::esIgual(filePath->str(), otro.filePath->str()); }
	bool operator<(const NBGenResFile_ &otro) const { return AUCadena8::esMenor(filePath->str(), otro.filePath->str()); }
	bool operator<=(const NBGenResFile_ &otro) const { return AUCadena8::esMenorIgual(filePath->str(), otro.filePath->str()); }
	bool operator>(const NBGenResFile_ &otro) const { return AUCadena8::esMayor(filePath->str(), otro.filePath->str()); }
	bool operator>=(const NBGenResFile_ &otro) const { return AUCadena8::esMayorIgual(filePath->str(), otro.filePath->str()); }
} NBGenResFile;

// State

typedef struct NBGenResState_ {
	//Array of files found
	AUArregloNativoOrdenadoMutableP<NBGenResFile>* filesFound;
	//JPEG reusable save-state (with internal buffers)
	void* jpegSaveState;
	//PNG reusable save-state (with internal buffers)
	NBPngSaveState* pngSaveState;
} NBGenResState;

// Class

class NBToolImagesScaler {
	public:
		static SI32	generateScaledImages(NBGenResState* state, const ENGenResProfile profile, const char* carpetaBaseAltaResolucion, const char* carpetaBaseDestinoResolucionMenor, const char* carpetaExtra, const SI32 escalaBase2Origen, NBGenResStats* acumularResultadosEn, AUArregloNativoP<NBGenResScalePath>* rules, const NBGenResScaleRule* curRule);
		static SI32 generateScaledImage(NBGenResState* state, const ENGenResProfile profile, const SI32 escalaBase2Origen, const char* srcPath, const NBGenResFileTask* tasks, const SI32 tasksCount);
		static ENGenResFileFormat fileFormatByName(const char* strFilename);
		//Comparar archivos mediante Tamano y valores RGBA con margen de error
		static void	pngAgregarResumenMapaBits(const char* carpetaPNGsSinPlecaAlFinal, AUCadenaLargaMutable8* cadenaCompartida, AUArregloNativoMutableP<NBDevResumenMapaBits>* propiedades);
		static int pngAnalizarMapaBitsParecidos(const char* cadenaCompartida, NBDevResumenMapaBits* propiedadesAchivos, int conteoArchivos, UI8 maxMargenAbsPorComponenteRGBA);
		//static int listarArchivosFuenteEnUsoQueNoExistanEnDestino(const char* carpetaArchivosQueUsan, const char* carpetaPNGsFuente, const char* carpetaPNGsDestino, SI32* acumularConteoProcesadosEn);
		static void	actualizarImgsHaciaTamanoMultiploDe(const char* carpetaPNGs, SI32 tamanoMultiploDe, SI32* acumularConteoRedimensionadasEn, SI32* acumularConteoIgnoradasEn);
		static int	generarCacheBinariaPNGs(const char* carpetaPNGs, const char* carpetaCachesBinarias);
		//Found files
		static void fileInit(NBGenResFile* data, const char* filePath);
		static void fileFinalize(NBGenResFile* data);
		static SI32 filesExecuteTasks(NBGenResState* state, const ENGenResProfile profile);
		static void filesFinalize(AUArregloNativoOrdenadoMutableP<NBGenResFile>* filesFound);
		//Found file tasks
		static void fileTaskInit(NBGenResFileTask* data, const char* filePath);
		static void fileTaskFinalize(NBGenResFileTask* data);
		//Stats
		static void	statsInit(NBGenResStats* data);
		static void	statsPrint(const NBGenResStats* data);
		//State
		static void stateInit(NBGenResState* data);
		static void statePrepareJPEG(NBGenResState* data);
		static void statePreparePNG(NBGenResState* data);
		static void stateFinalize(NBGenResState* data);
		//Configuracion
		static const char* configGetProfileNodeName(const ENGenResProfile profile);
		static void configScalesLoad(const AUDatosJSON* configFile, const STJsonNode* nScales, AUArregloNativoMutableP<NBGenResScale>* dst);
		static void configScalesRelease(AUArregloNativoP<NBGenResScale>* configs);
		static void configScaleRulesLoad(const AUDatosJSON* configFile, const STJsonNode* nRules, AUArregloNativoMutableP<NBGenResScalePath>* dst, const SI32 iScale, const char* scaleName);
		static void configScaleRulesRelease(AUArregloNativoP<NBGenResScalePath>* rules);
		static void configScaleRuleProfileLoad(const AUDatosJSON* configFile, const STJsonNode* nParentNode, NBGenResScaleRuleProfile* data, const ENGenResProfile profile, const SI32 iScale, const char* scaleName, const SI32 iRule);
		static void configScalePathInit(NBGenResScalePath* data);
		static void configScalePathFinalize(NBGenResScalePath* data);
		static void configScaleRuleInit(NBGenResScaleRule* data);
		static void configScaleRuleFinalize(NBGenResScaleRule* data);
		static void configScaleRuleProfileInit(NBGenResScaleRuleProfile* data, const ENGenResProfile profile);
		static void configScaleRuleProfileFinalize(NBGenResScaleRuleProfile* data);
		static void configScaleRuleByExplicits(const NBGenResScaleRule* parentRule, NBGenResScaleRule* newRule);
		static const NBGenResScaleRule* configScaleRuleByPath(AUArregloNativoP<NBGenResScalePath>* rules, const char* path);
};


#endif
