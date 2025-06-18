//
//  NBToolPackageMaker.h
//  DevTools
//
//  Created by Nicaragua Binary on 20/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef NBToolPackageMaker_H_INCLUIDO
#define NBToolPackageMaker_H_INCLUIDO

#include "AUFrameworkBase.h"

// Rules

typedef struct NBGenPkgConfigRule_ {
	AUCadenaMutable8*	path;
	AUCadenaMutable8*	action;
} NBGenPkgConfigRule;

// Stats

typedef struct NBGenPkgStats_ {
	SI32 qIncluded;
	SI32 qIgnored;
	UI32 qBytesIncluded;
} NBGenPkgStats;

// Class

class NBToolPackageMaker {
	public:
		static SI32 generarPaqueteDeArbol(const char* rutaRaizArbolConPlecaAlFinal, const char* rutaRaizEnPaquete, const char* rutaArchivoPaqueteDestino, const char* rutaArchivoTmp1, const char* rutaArchivoTmp2, bool listarArchivos, NBGenPkgStats* dstStats, AUArregloNativoP<NBGenPkgConfigRule>* rules);
		static SI32	agregarPaqueteDeArbol(const char* rutaCarpetaRaizFuenteConPlecaAlFinal, const char* rutaCarpetaRaizPaqueteConPlecaAlFinal, const char* rutaAdicionalConPlecaAlFinal, AUPaqueteIndice* indicePaquete, FILE* archivoTmpDatosPaquete, UI32* acumuladorRecorridoArchivoDatosPaquete, bool listarArchivos, NBGenPkgStats* dstStats, AUArregloNativoP<NBGenPkgConfigRule>* rules, const NBGenPkgConfigRule* curRule);
		//Stats
		static void	statsInit(NBGenPkgStats* data);
		//Configuracion
		static void configRuleInit(NBGenPkgConfigRule* data);
		static void configRuleFinalize(NBGenPkgConfigRule* data);
		static const NBGenPkgConfigRule* configRuleByPath(AUArregloNativoP<NBGenPkgConfigRule>* rules, const char* path);
};


#endif
