//
//  NBToolAudioCompressor.h
//  DevTools
//
//  Created by Nicaragua Binary on 27/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef DevTools_NBToolAudioCompressor_h
#define DevTools_NBToolAudioCompressor_h

#include "AUFrameworkBase.h"

enum NBCompAudTipo {
	NBCompAudTipo_Deltas = 0,	//Compresion por delta y multiplicador entre muestras
	NBCompAudTipo_Ogg			//Compresion Vorbis (de los ogg)
};

struct NBCompAudConfig {
	AUCadenaMutable8*	rutaRecurso;
	bool				ignorar;
	UI32				multDeltas;
	UI32				deltasPorBloque;
};

class NBToolAudioCompressor {
	public:
		static int	generarAudioComprimido(const char* carpetaArchivosWAV, const char* carpetaInternaWav, const char* carpetaDestinosComprimidos, const NBCompAudTipo tipoCompresion, const ENSonidoCalidad calidad, AUArregloNativoMutableP<NBCompAudConfig>* config);
		//Configuracion
		static void configCargar(AUArregloNativoMutableP<NBCompAudConfig>* arr, const char* carpetaBaseSinPlecaAlFinal);
		static void configFinalizar(AUArregloNativoMutableP<NBCompAudConfig>* arr);
	private:
		static void configRegistroInicializa(NBCompAudConfig* reg);
		static void configRegistroFinaliza(NBCompAudConfig* reg);
		static const NBCompAudConfig* configParaRecurso(AUArregloNativoMutableP<NBCompAudConfig>* config, const char* rutaRecurso);
};

#endif
