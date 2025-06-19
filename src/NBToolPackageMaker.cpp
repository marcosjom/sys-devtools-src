//
//  NBToolPackageMaker.cpp
//  DevTools
//
//  Created by Nicaragua Binary on 20/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "AUFrameworkBaseStdAfx.h"
#include "NBToolPackageMaker.h"

//As with all bounds-checked functions, fopen_s is only guaranteed to be available
//if STDC_LIB_EXT1 is defined by the implementation
//and if the user defines STDC_WANT_LIB_EXT1
//to the integer constant 1 before including stdio.h.
#ifdef _WIN32
#   define FOPEN(VAR, ...)  fopen_s(&(VAR), __VA_ARGS__)
#else
#   define FOPEN(VAR, ...)  VAR = fopen(__VA_ARGS__)
#endif

// Stats

void NBToolPackageMaker::statsInit(NBGenPkgStats* data){
	data->qIncluded	 = 0;
	data->qIgnored	 = 0;
	data->qBytesIncluded = 0;
}

// Rules

void NBToolPackageMaker::configRuleInit(NBGenPkgConfigRule* data){
	data->path		= new AUCadenaMutable8();
	data->action	= new AUCadenaMutable8();
}

void NBToolPackageMaker::configRuleFinalize(NBGenPkgConfigRule* data){
	if(data->path != NULL) data->path->liberar(); data->path = NULL;
	if(data->action != NULL) data->action->liberar(); data->action = NULL;
}

const NBGenPkgConfigRule* NBToolPackageMaker::configRuleByPath(AUArregloNativoP<NBGenPkgConfigRule>* rules, const char* path){
	const NBGenPkgConfigRule* r = NULL;
	SI32 i;
	for(i = 0; i < rules->conteo; i++){
		const NBGenPkgConfigRule* data = rules->elemPtr(i);
		if(data->path != NULL){
			if(data->path->esIgual(path)){
				r = data;
				break;
			}
		}
	}
	return r;
}

//

SI32 NBToolPackageMaker::generarPaqueteDeArbol(const char* rutaRaizArbolConPlecaAlFinal, const char* rutaRaizEnPaquete, const char* rutaArchivoPaqueteDestino, const char* rutaArchivoTmp1, const char* rutaArchivoTmp2, bool listarArchivos, NBGenPkgStats* dstStats, AUArregloNativoP<NBGenPkgConfigRule>* rules){
	SI32 conteoEmpaquetados = 0;
	AUCadenaMutable8* strArchDatos = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	AUCadenaMutable8* strArchIndice = new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	strArchDatos->establecer(rutaArchivoTmp1);
	strArchIndice->establecer(rutaArchivoTmp2);
	FILE* archivoPaqueteTmp = NULL;
    FOPEN(archivoPaqueteTmp, strArchDatos->str(), "wb");
	if(archivoPaqueteTmp==NULL){
		PRINTF_ERROR("abriendo archivo para escritura: '%s'\n", strArchDatos->str());
	} else {
		UI32 acumuladorRecorrido = 0;
		AUPaqueteIndice* indicePaquete = new(ENMemoriaTipo_Temporal) AUPaqueteIndice();
		//Generar archivo datos
		const NBGenPkgConfigRule* defRule = NBToolPackageMaker::configRuleByPath(rules, "");
		/*if(defRule == NULL){
			PRINTF_WARNING("No default rule defined.\n");
		}*/
		//Process
		conteoEmpaquetados += NBToolPackageMaker::agregarPaqueteDeArbol(rutaRaizArbolConPlecaAlFinal, rutaRaizEnPaquete, "", indicePaquete, archivoPaqueteTmp, &acumuladorRecorrido, listarArchivos, dstStats, rules, defRule);
		fclose(archivoPaqueteTmp);
		//
		if(conteoEmpaquetados == 0){
			PRINTF_INFO("Ignorando PAQUETE '%s' (sin archivos).\n", rutaArchivoPaqueteDestino);
		} else {
			//Generar archivo indice (binario)
			FILE* flujo = NULL;
            FOPEN(flujo, strArchIndice->str(), "wb");
			if(flujo==NULL){
				PRINTF_ERROR("no se pudo abrir el archivo indice para escritura '%s'\n", strArchIndice->str());
			} else {
				indicePaquete->agregarBitsInternosEn(flujo);
				fclose(flujo);
				//Fusionar archivos indice y binario
				FILE* archIndice = NULL;
				FILE* archDatos = NULL;
                FOPEN(archIndice, strArchIndice->str(), "rb");
                FOPEN(archDatos, strArchDatos->str(), "rb");
				if(archDatos==NULL || archIndice==NULL){
					PRINTF_ERROR("no se pudo abrir uno de los archivos (indice o datos) para lectura\n");
				} else {
					FILE* archPaquete = NULL;
                    FOPEN(archPaquete, rutaArchivoPaqueteDestino, "wb");
					if(archPaquete==NULL){
						PRINTF_ERROR("no se pudo abrir el archivo paquete para escritura\n");
					} else {
						//Tamano del indice
						fseek(archIndice, 0, SEEK_END);
						UI32 tamanoIndice = (UI32) ftell(archIndice);
						fwrite(&tamanoIndice, sizeof(tamanoIndice), 1, archPaquete);
						//Tamano de los datos
						fseek(archDatos, 0, SEEK_END);
						UI32 tamanoDatos = (UI32) ftell(archDatos);
						fwrite(&tamanoDatos, sizeof(tamanoDatos), 1, archPaquete);
						//Copiar datos del indice
						fseek(archIndice, 0, SEEK_SET);
						BYTE buffer[10240];
						UI32 bytesLeidos = 0;
						UI32 acumBytesLeidos = 0;
						do {
							bytesLeidos = (UI32) fread(buffer, sizeof(BYTE), 10240, archIndice);
							if(bytesLeidos!=0){
								fwrite(buffer, sizeof(BYTE), bytesLeidos, archPaquete);
								acumBytesLeidos += bytesLeidos;
							}
						} while(bytesLeidos!=0);
						if(acumBytesLeidos!=tamanoIndice) PRINTF_ERROR("la cantidad de bytes del indice copiados no equivalen a su tamano\n");
						NBASSERT(acumBytesLeidos==tamanoIndice);
						//Copiar datos del paquete
						fseek(archDatos, 0, SEEK_SET);
						do {
							bytesLeidos = (UI32) fread(buffer, sizeof(BYTE), 10240, archDatos);
							if(bytesLeidos!=0){
								fwrite(buffer, sizeof(BYTE), bytesLeidos, archPaquete);
								acumBytesLeidos += bytesLeidos;
							}
						} while(bytesLeidos!=0);
						if(acumBytesLeidos!=tamanoIndice+tamanoDatos) PRINTF_ERROR("la cantidad de bytes de datos copiados no equivalen a su tamano\n");
						NBASSERT(acumBytesLeidos==tamanoIndice+tamanoDatos);
						//
						fclose(archPaquete);
					}
					fclose(archDatos);
					fclose(archIndice);
				}
			}
			if(dstStats != NULL){
				dstStats->qBytesIncluded += acumuladorRecorrido;
			}
		}
		//indicePaquete->debugImpimirIndice();
		indicePaquete->liberar();
	}
	strArchDatos->liberar();
	strArchIndice->liberar();
	return conteoEmpaquetados;
}

SI32 NBToolPackageMaker::agregarPaqueteDeArbol(const char* rutaCarpetaRaizFuenteConPlecaAlFinal, const char* rutaCarpetaRaizPaqueteConPlecaAlFinal, const char* rutaAdicionalConPlecaAlFinal, AUPaqueteIndice* indicePaquete, FILE* archivoTmpDatosPaquete, UI32* acumuladorRecorridoArchivoDatosPaquete, bool listarArchivos, NBGenPkgStats* dstStats, AUArregloNativoP<NBGenPkgConfigRule>* rules, const NBGenPkgConfigRule* curRule){
	SI32 conteoEmpaquetados			= 0;
	AUCadenaMutable8* rutaCompleta	= new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	rutaCompleta->agregar(rutaCarpetaRaizFuenteConPlecaAlFinal);
	rutaCompleta->agregar(rutaAdicionalConPlecaAlFinal);
	//
	AUCadenaMutable8* strTmp		= new(ENMemoriaTipo_Temporal) AUCadenaMutable8();
	//Procesar archivos
	AUArregloMutable* archivos		= new(ENMemoriaTipo_Temporal) AUArregloMutable();
	NBGestorArchivos::listarArchivosDe(rutaCompleta->str(), archivos);
	SI32 iArch;
	for(iArch = 0; iArch < archivos->conteo; iArch++){
		AUCadena8* nombreRecurso = (AUCadena8*)archivos->elemento[iArch];
		if(nombreRecurso->tamano() != 0){
			if(nombreRecurso->str()[0] == '.'){
				if(listarArchivos){
					PRINTF_INFO("Ignorando recurso '%s%s' (archivo oculto)\n", rutaAdicionalConPlecaAlFinal, nombreRecurso->str());
				}
				if(dstStats != NULL){
					dstStats->qIgnored++;
				}
			} else {
				strTmp->establecer(rutaAdicionalConPlecaAlFinal);
				strTmp->agregar(nombreRecurso->str());
				//Search explicit rule
				const NBGenPkgConfigRule* ruleForPath = NBToolPackageMaker::configRuleByPath(rules, strTmp->str());
				if(ruleForPath == NULL){
					ruleForPath = curRule;
				}
				//Process
				if(ruleForPath == NULL){
					if(listarArchivos){
						PRINTF_WARNING("Ignorando recurso '%s' (sin regla definida)\n", strTmp->str());
					}
					if(dstStats != NULL){
						dstStats->qIgnored++;
					}
				} else if(ruleForPath->action->esIgual("ignore")){
					if(listarArchivos){
						PRINTF_INFO("Ignorando recurso '%s' (explicito)\n", strTmp->str());
					}
					if(dstStats != NULL){
						dstStats->qIgnored++;
					}
				} else if(ruleForPath->action->esIgual("include")){
					strTmp->establecer(rutaCompleta->str());
					strTmp->agregar("/");
					strTmp->agregar(nombreRecurso->str());
					//
					FILE* archivoLectura = NULL;
                    FOPEN(archivoLectura, strTmp->str(), "rb");
					if(archivoLectura==NULL){
						PRINTF_ERROR("No se pudo abrir archivo para lectura '%s'\n", strTmp->str());
					} else {
						UI32 acumBytesLeidos = 0;
						UI8 buffer[1024]; UI32 bytesLeidos = 0;
						do {
							bytesLeidos = (UI32)fread(buffer, sizeof(UI8), 1024, archivoLectura);
							if(bytesLeidos!=0){
								fwrite(buffer, sizeof(UI8), bytesLeidos, archivoTmpDatosPaquete);
								acumBytesLeidos += bytesLeidos;
							}
						} while(bytesLeidos!=0);
						strTmp->establecer(rutaCarpetaRaizPaqueteConPlecaAlFinal);
						strTmp->agregar(rutaAdicionalConPlecaAlFinal);
						strTmp->agregar(nombreRecurso->str());
						indicePaquete->agregarRegistro(strTmp->str(), *acumuladorRecorridoArchivoDatosPaquete, acumBytesLeidos);
						if(listarArchivos){
							PRINTF_INFO("ENPAQUETADO: archivo '%s%s' (indice %u, %u bytes)\n", rutaAdicionalConPlecaAlFinal, nombreRecurso->str(), *acumuladorRecorridoArchivoDatosPaquete, acumBytesLeidos);
						}
						*acumuladorRecorridoArchivoDatosPaquete = *acumuladorRecorridoArchivoDatosPaquete + acumBytesLeidos;
						conteoEmpaquetados++;
						fclose(archivoLectura);
						//
						if(dstStats != NULL){
							dstStats->qIncluded++;
						}
					}
				} else {
					if(listarArchivos){
						PRINTF_WARNING("Ignorando recurso '%s' (rule action '%s' not valid)\n", strTmp->str(), ruleForPath->action->str());
					}
					if(dstStats != NULL){
						dstStats->qIgnored++;
					}
				}
			}
		}
	}
	archivos->liberar(NB_RETENEDOR_THIS);
	//Procesar subcarpetas
	AUArregloMutable* subcarpetas	= new(ENMemoriaTipo_Temporal) AUArregloMutable();
	NBGestorArchivos::listarSubcarpetasDe(rutaCompleta->str(), subcarpetas);
	int iCarp;
	for(iCarp=0; iCarp<subcarpetas->conteo; iCarp++){
		AUCadena8* nombreRecurso = (AUCadena8*)subcarpetas->elemento[iCarp];
		if(nombreRecurso->tamano()!=0){
			if(nombreRecurso->str()[0]=='.'){
				if(listarArchivos){
					PRINTF_INFO("IGNORANDO: subcarpeta '%s%s'\n", rutaCompleta->str(), nombreRecurso->str());
				}
			} else {
				strTmp->establecer(rutaAdicionalConPlecaAlFinal);
				strTmp->agregar(nombreRecurso->str());
				//Search explicit rule
				const NBGenPkgConfigRule* ruleForPath = NBToolPackageMaker::configRuleByPath(rules, strTmp->str());
				if(ruleForPath == NULL){
					ruleForPath = curRule;
				}
				//Process
				{
					strTmp->establecer(rutaAdicionalConPlecaAlFinal);
					strTmp->agregar(nombreRecurso->str());
					strTmp->agregar("/");
					if(listarArchivos){
						PRINTF_INFO("EXPLORANDO: subcarpeta '%s%s'%s\n", rutaAdicionalConPlecaAlFinal, nombreRecurso->str(), (ruleForPath != curRule ? " (REGLA EXPLICITA)" : ""));
					}
					conteoEmpaquetados += NBToolPackageMaker::agregarPaqueteDeArbol(rutaCarpetaRaizFuenteConPlecaAlFinal, rutaCarpetaRaizPaqueteConPlecaAlFinal, strTmp->str(), indicePaquete, archivoTmpDatosPaquete, acumuladorRecorridoArchivoDatosPaquete, listarArchivos, dstStats, rules, ruleForPath);
				}
			}
		}
	}
	subcarpetas->liberar(NB_RETENEDOR_THIS);
	//
	strTmp->liberar(NB_RETENEDOR_THIS);
	rutaCompleta->liberar();
	//
	return conteoEmpaquetados;
}
