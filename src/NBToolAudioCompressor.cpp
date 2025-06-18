//
//  NBToolAudioCompressor.cpp
//  DevTools
//
//  Created by Nicaragua Binary on 27/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "AUFrameworkBaseStdAfx.h"
#include "NBToolAudioCompressor.h"

int NBToolAudioCompressor::generarAudioComprimido(const char* carpetaArchivosWAV, const char* carpetaInternaWav, const char* carpetaDestinosComprimidos, const NBCompAudTipo tipoCompresion, const ENSonidoCalidad calidad, AUArregloNativoMutableP<NBCompAudConfig>* config){
	int conteoArchivoProcesados = 0;
	AUArregloMutable* subcarpetasHD = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUArregloMutable* subcarpetasSD = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUArregloMutable* archivosHD = new(ENMemoriaTipo_Temporal) AUArregloMutable();
	AUCadenaMutable8* strTmp = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	AUCadenaMutable8* strTmp2 = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	AUCadenaMutable8* strTmp3 = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	//Asegurar la existencia de las carpetas
	strTmp->establecer(carpetaArchivosWAV);
	if(AUCadena8::tamano(carpetaInternaWav) > 0){
		strTmp->agregar("/");
		strTmp->agregar(carpetaInternaWav);
	}
	//
	NBGestorArchivos::listarSubcarpetasDe(strTmp->str(), subcarpetasHD);
	NBGestorArchivos::listarSubcarpetasDe(carpetaDestinosComprimidos, subcarpetasSD);
	int iCarp;
	for(iCarp=0; iCarp<subcarpetasHD->conteo; iCarp++){
		AUCadena8* nombreRecurso = (AUCadena8*)subcarpetasHD->elemento[iCarp];
		SI16 i, iRecurso = -1;
		for(i=0; i<subcarpetasSD->conteo; i++){
			if(((AUCadena8*)subcarpetasSD->elemento[i])->esIgual(nombreRecurso->str())){
				iRecurso = i; break;
			}
		}
		if(iRecurso==-1){
			strTmp2->establecer(carpetaDestinosComprimidos);
			strTmp2->agregar("/");
			strTmp2->agregar(nombreRecurso->str());
			if(!NBGestorArchivos::crearCarpeta(strTmp2->str())){
				PRINTF_ERROR("creando subcarpeta: %s\n", strTmp2->str());
			}
		}
	}
	//Procesar archivos PNG
	strTmp->establecer(carpetaArchivosWAV);
	if(AUCadena8::tamano(carpetaInternaWav) > 0){
		strTmp->agregar("/");
		strTmp->agregar(carpetaInternaWav);
	}
	NBGestorArchivos::listarArchivosDe(strTmp->str(), archivosHD);
	int iArch;
	for(iArch=0; iArch<archivosHD->conteo; iArch++){
		AUCadena8* nombreRecurso = (AUCadena8*)archivosHD->elemento[iArch];
		if(nombreRecurso->indiceDe(".wav")!=-1 || nombreRecurso->indiceDe(".WAV")!=-1 || nombreRecurso->indiceDe(".Wav")!=-1){
			strTmp->establecer(carpetaArchivosWAV);
			if(AUCadena8::tamano(carpetaInternaWav) > 0){
				strTmp->agregar("/");
				strTmp->agregar(carpetaInternaWav);
			}
			strTmp->agregar("/");
			strTmp->agregar(nombreRecurso->str());
			//
			strTmp3->vaciar();
			if(AUCadena8::tamano(carpetaInternaWav) > 0){
				strTmp3->agregar(carpetaInternaWav);
				strTmp3->agregar("/");
			}
			strTmp3->agregar(nombreRecurso->str());
			//
			strTmp2->establecer(carpetaDestinosComprimidos);
			strTmp2->agregar("/");
			strTmp2->agregar(nombreRecurso->str());
			strTmp2->quitarDerecha(4); //".wav"
			strTmp2->agregar(tipoCompresion == NBCompAudTipo_Ogg ? ".ogg" : ".aud");
			AUSonidoMutable* sonidoHD = new(ENMemoriaTipo_Temporal) AUSonidoMutable();
			if(!sonidoHD->cargarDesdeArchivoWav(strTmp->str(), true, NULL, 0)){
				PRINTF_ERROR("al interpretar los datos del archivo '%s'\n", strTmp->str());
			} else {
				//GUARDAR COMPRIMIDO
				SI32 deltasPorBloque = 8;
				SI32 deltasMultiplicador = 16;
				bool configEncontrado = false;
				bool ignorar = false;
				const NBCompAudConfig* configAud = NBToolAudioCompressor::configParaRecurso(config, strTmp3->str());
				if(configAud != NULL){
					if(configAud->deltasPorBloque > 0) deltasPorBloque = configAud->deltasPorBloque;
					if(configAud->multDeltas > 0) deltasMultiplicador = configAud->multDeltas;
					ignorar = configAud->ignorar;
					configEncontrado = true;
				}
				if(ignorar){
					PRINTF_INFO("Ignorando recurso '%s' (via config).\n", strTmp3->str());
				} else {
					if(tipoCompresion == NBCompAudTipo_Ogg){
						//Guardar Vorbis
						if(!sonidoHD->guardarHaciaArchivoOgg(strTmp2->str(), calidad)){
							PRINTF_ERROR("al intentar guardar audio vorbis: %s\n", strTmp2->str());
						} else {
							conteoArchivoProcesados++;
						}
					} else {
						//Guardar delta
						PRINTF_INFO("Comprimiedo '%s' en bloques de %d muestras con multiplicador %d (via %s).\n", strTmp3->str(), deltasPorBloque, deltasMultiplicador, configEncontrado ? "config" : "auto");
						if(!sonidoHD->guardarHaciaArchivoDeltas(strTmp2->str(), deltasPorBloque, deltasMultiplicador)){
							PRINTF_ERROR("al intentar guardar audio deltas_PCM: %s\n", strTmp2->str());
						} else {
							conteoArchivoProcesados++;
						}
					}
				}
			}
			sonidoHD->liberar(NB_RETENEDOR_THIS);
		}
	}
	//Procesar subcarpetas
	for(iCarp=0; iCarp<subcarpetasHD->conteo; iCarp++){
		AUCadena8* nombreRecurso = (AUCadena8*)subcarpetasHD->elemento[iCarp];
		//
		strTmp3->vaciar();
		if(AUCadena8::tamano(carpetaInternaWav) > 0){
			strTmp3->agregar(carpetaInternaWav);
			strTmp3->agregar("/");
		}
		strTmp3->agregar(nombreRecurso->str());
		//
		strTmp2->establecer(carpetaDestinosComprimidos);
		strTmp2->agregar("/");
		strTmp2->agregar(nombreRecurso->str());
		//
		bool ignorar = false;
		const NBCompAudConfig* configAud = NBToolAudioCompressor::configParaRecurso(config, strTmp3->str());
		if(configAud != NULL){
			ignorar = configAud->ignorar;
		}
		if(ignorar){
			PRINTF_INFO("Ignorando carpeta '%s' (via config).\n", strTmp3->str());
		} else {
			conteoArchivoProcesados += NBToolAudioCompressor::generarAudioComprimido(carpetaArchivosWAV, strTmp3->str(), strTmp2->str(), tipoCompresion, calidad, config);
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

//

void NBToolAudioCompressor::configRegistroInicializa(NBCompAudConfig* reg){
	reg->rutaRecurso		= new(ENMemoriaTipo_General) AUCadenaMutable8();
	reg->ignorar			= false;
	reg->multDeltas			= 16;
	reg->deltasPorBloque	= 8;
}

void NBToolAudioCompressor::configRegistroFinaliza(NBCompAudConfig* reg){
	reg->rutaRecurso->liberar(); reg->rutaRecurso = NULL;
	reg->ignorar			= false;
	reg->multDeltas			= 16;
	reg->deltasPorBloque	= 8;
}

const NBCompAudConfig* NBToolAudioCompressor::configParaRecurso(AUArregloNativoMutableP<NBCompAudConfig>* config, const char* rutaRecurso){
	UI16 i; const UI16 uso = config->conteo;
	for(i = 0; i < uso; i++){
		if(config->elemento[i].rutaRecurso->esIgual(rutaRecurso)){
			return &config->elemento[i];
		}
	}
	return NULL;
}

void NBToolAudioCompressor::configCargar(AUArregloNativoMutableP<NBCompAudConfig>* arr, const char* carpetaBaseSinPlecaAlFinal){
	AUCadenaMutable8* strTmp = new(ENMemoriaTipo_Temporal) AUCadenaMutable8(carpetaBaseSinPlecaAlFinal); strTmp->agregar("/config.txt");
	AUDatosXMLMutable8* datosXML = new(ENMemoriaTipo_Temporal) AUDatosXMLMutable8();
	if(!datosXML->cargaDatosXMLDesdeArchivo(strTmp->str())){
		PRINTF_ERROR("No se pudo abrir flujo hacia archivo: '%s'.\n", strTmp->str());
	} else {
		const sNodoXML* nodoConfig = datosXML->nodoHijo("config");
		if(nodoConfig == NULL){
			PRINTF_ERROR("El archivo no cuenta con el nodo 'config': '%s'.\n", strTmp->str());
		} else {
			UI32 conteoRess = 0;
			const sNodoXML* nodoRecursos = datosXML->nodoHijo("ress", nodoConfig);
			if(nodoRecursos != NULL){
				const sNodoXML* nodoR = datosXML->nodoHijo("r", nodoRecursos);
				while(nodoR != NULL){
					NBCompAudConfig reg;
					NBToolAudioCompressor::configRegistroInicializa(&reg);
					datosXML->nodoHijo("path", reg.rutaRecurso, "", nodoR);
					reg.ignorar				= datosXML->nodoHijo("ignr", false, nodoR);
					reg.deltasPorBloque		= datosXML->nodoHijo("bloqDeltas", 8, nodoR);
					reg.multDeltas			= datosXML->nodoHijo("multDeltas", 16, nodoR);
					if(reg.rutaRecurso->tamano() == 0){
						NBToolAudioCompressor::configRegistroFinaliza(&reg);
					} else {
						arr->agregarElemento(reg);
					}
					//
					nodoR = datosXML->nodoHijo("r", nodoRecursos, nodoR);
					conteoRess++;
				}
			}
			PRINTF_INFO("Archivo de configuracion cargado con %d nodos ress.\n", conteoRess);
		}
	}
	datosXML->liberar();
	strTmp->liberar();
}

void NBToolAudioCompressor::configFinalizar(AUArregloNativoMutableP<NBCompAudConfig>* arr){
	UI16 i; const UI16 uso = arr->conteo;
	for(i = 0; i < uso; i++){
		NBToolAudioCompressor::configRegistroFinaliza(&arr->elemento[i]);
	}
	arr->vaciar();
}


