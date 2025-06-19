//
//  main.cpp
//  devtools
//
//  Created by Marcos Ortega on 2/3/17.
//  Copyright © 2017 NIBSA. All rights reserved.
//

#include "AUFrameworkBaseStdAfx.h"
#include "AUFrameworkBase.h"
#include "NBToolAppIconsGenerator.h"
#include "NBToolLaunchscreensGenerator.h"
#include "NBToolImagesScaler.h"
#include "NBToolPackageMaker.h"
#include "nb/core/NBJson.h"
#include "nb/core/NBStruct.h"
#include "nb/core/NBMngrStructMaps.h"
#include "nb/core/NBMngrProcess.h"

void makePackagesFromFilename(const char* filename);
void makeHeadersFromBinaries(const char* filename);
void makeImagesScalesPNGX8();
void makeImagesScalesFromFilename(const char* filename, const ENGenResProfile profId);
void makeAppIconsFromFilename(const char* filename);
void makeLaunchScreensFromFilename(const char* filename);

#ifdef _WIN32
#   define SCANF    scanf_s
#else
#   define SCANF    scanf
#endif


int main(int argc, const char * argv[]) {
	NBMngrProcess_init();
	NBMngrStructMaps_init();
	//NBSocket_initEngine();
	//NBSocket_initWSA();
	//
	//Link to methods
	NBGlueLibJpegWrite_linkToMethods();
	NBGlueLibJpegRead_linkToMethods();
	//Endiens test
	{
		UI16 datoPruebaEndianes = 1; UI8* valoresBytes = (UI8*)&datoPruebaEndianes;
		PRINTF_CONSOLE("This device is %s-ENDIEN (%d bytes per pointer).\n", (valoresBytes[0]==0 && valoresBytes[1]!=0)?"BIG":"LITTLE", (SI32)sizeof(void*));
	}
	//
	NBGestorMemoria::inicializar(1, (1024*1024*4), 8191,	//Zonas de memoria NUCLEO/AGIL: cantidadPregeneradas, bytesMinPorZona, registrosIndicesMinimoPorZona
								 1, (1024*1024*40), 8191);	//Zonas de memoria TEMPORAL
	NBGestorAUObjetos::inicializar(10240);	//10240 autorelease-pointers = 4KBs
	//RegistrarClasesAUFrameworkBase();
	//RegistrarClasesAUFrameworkMedia();
	//RegistrarClasesDodoTownNucleo();
	//RegistrarClasesEditores();
	NBGestorArchivos::inicializar(); NBGestorArchivos::establecerRutaPrefijoPaquete("./");
	{
		//NBGestorDatos::inicializar();
		//NBGestorDatos::cargarDesdeArchivos(); //No es necesario cargar datos
		//NBGestorIdioma::inicializar();
		NBGestorFuentes::inicializar();
		NBGestorFuentes::establecerPrefijoRutasFuentes("Fuentes/", "");
		{
			//PRINT TITLE
			PRINTF_CONSOLE("---------------------------------------\n");
			PRINTF_CONSOLE("--- DevTool / Nicaragua Binary S.A. ---\n");
			PRINTF_CONSOLE("---           Mayo / 2012           ---\n");
			PRINTF_CONSOLE("---------------------------------------\n");
			//PRINTF_CONSOLE("Notas:\n");
			//PRINTF_CONSOLE(" - Si brinda como parametro un archivo SWF, se extraeran\n");
			//PRINTF_CONSOLE("   las fuentes y animaciones-con-nombre en la escena principal.\n");
			//PRINTF_CONSOLE(" - Si brinda como parametro un archivo PNG, se segmentara\n");
			//PRINTF_CONSOLE("   en una malla.\n");
			//PRINTF_CONSOLE("\n\n");
			{
				const char* cfgFilename = "devtools.config.json";
				const UI32 CMDS_BUFF_SIZE = 128;
				SI32 iCmd = 0; char cmdsBuff[CMDS_BUFF_SIZE];
				memset(cmdsBuff, 0, sizeof(cmdsBuff[0]) * CMDS_BUFF_SIZE);
				while(TRUE){
					const char cmd = cmdsBuff[iCmd++];
					//Actions
					if(cmd == '0'){
						break;
					} else if(cmd == '1'){
						makeImagesScalesPNGX8();
					} else if(cmd == '2'){
						makeImagesScalesFromFilename(cfgFilename, ENGenResProfile_Dev);
					} else if(cmd == '3'){
						makeImagesScalesFromFilename(cfgFilename, ENGenResProfile_Pkg);
					} else if(cmd == '4'){
						makeAppIconsFromFilename(cfgFilename);
					} else if(cmd == '5'){
						makeLaunchScreensFromFilename(cfgFilename);
					} else if(cmd == '8'){
						makePackagesFromFilename(cfgFilename);
					} else if(cmd == '9'){
						makeHeadersFromBinaries(cfgFilename);
					}
					//SOLICITAR ENTRADA
					if(cmd == '\0' || iCmd == CMDS_BUFF_SIZE){
						//aplicarAutoliberaciones();
						PRINTF_CONSOLE("Que desea hacer?\n");
						PRINTF_CONSOLE("--- CONVERTIR PNGS\n");
						PRINTF_CONSOLE("1 - Convertir las 'PNGX8' a tamanos multiplos de 8\n");
						PRINTF_CONSOLE("2 - Generar imagenes escaladas perfil DEV (incluye firmas)\n");
						PRINTF_CONSOLE("3 - Generar imagenes escaladas perfil PKG (incluye firmas)\n");
						PRINTF_CONSOLE("4 - Generar iconos de app\n");
						PRINTF_CONSOLE("5 - Generar launchscreens de app\n");
						//PRINTF_CONSOLE("6 - Comprimir 'Musica' a 'MusicaOgg'\n");
						//PRINTF_CONSOLE("7 - Comprimir 'Sonidos' a 'SonidosOgg'\n");
						//PRINTF_CONSOLE("5 - Generar cache-binarias de PNGs (X2 y X1)\n");
						//PRINTF_CONSOLE("6 - Generar cache-binarias de Animaciones\n");
						//PRINTF_CONSOLE("7 - Generar cache-binarias de Cuerpos\n");
						//PRINTF_CONSOLE("8 - Generar cache-binarias de Niveles\n");
						PRINTF_CONSOLE("8 - Generar paquetes de recursos\n");
						PRINTF_CONSOLE("9 - Generar *.h de binarios\n");
						//PRINTF_CONSOLE("--- VERIFICAR ARCHIVOS\n");
						//PRINTF_CONSOLE("A - Identificar archivos sin uso\n");
						//PRINTF_CONSOLE("B - Identificar duplicados (firma SHA1)\n");
						//PRINTF_CONSOLE("C - Identificar duplicados (tamano y valores RGBA con margen)\n");
						//PRINTF_CONSOLE("D - Verificar etiquetas de PILA en codigo fuente\n");
						//PRINTF_CONSOLE("E - XML:: Estadisticas de etiquetas XML\n");
						//PRINTF_CONSOLE("F - XML:: Actualizar formato de contenido\n");
						//PRINTF_CONSOLE("G - XML:: Reidentar XMLs\n");
						//PRINTF_CONSOLE("H - XML:: Compactar XMLs (quitar identacion)\n");
						//PRINTF_CONSOLE("J - XML:: Cambiar rutas '(PNG/)Varios/XXXX.png' hacia sus archivos originales\n");
						PRINTF_CONSOLE("---\n");
						PRINTF_CONSOLE("0 - Salir\n");
						//LEER CONSOLA
						iCmd = 0;
						memset(cmdsBuff, 0, sizeof(cmdsBuff[0]) * CMDS_BUFF_SIZE);
                        SCANF("%s", cmdsBuff);
					}
				}
			}
		}
		NBGestorFuentes::finalizar();
		//NBGestorIdioma::finalizar();
		//NBGestorDatos::guardarHaciaArchivos();
		//NBGestorDatos::finalizar();
	}
	NBGestorArchivos::finalizar();
	NBGestorAUObjetos::finalizar();
	NBGestorMemoria::finalizar();
	//
	//NBSocket_finishWSA();
	//NBSocket_releaseEngine();
	NBMngrStructMaps_release();
	NBMngrProcess_release();
	//
	PRINTF_CONSOLE("Presione ENTER para terminar\n");
	char c; SCANF("%c", &c);
	//
    return 0;
}


void makePackagesFromFilename(const char* filename){
	CICLOS_CPU_TIPO cicleStart, cicleEnd, ciclesPerSec;
	CICLOS_CPU_HILO(cicleStart);
	CICLOS_CPU_POR_SEGUNDO(ciclesPerSec);
	//
	PRINTF_CONSOLE("DEVTOOL --- GENERANDO PAQUETE DE ARCHIVOS RECURSOS\n");
	UI32 totalEmpaquetados = 0;
	AUDatosJSONMutable* configFile = new AUDatosJSONMutable();
	if(!configFile->cargaDatosJsonDesdeArchivo(filename)){
		PRINTF_ERROR("No se pudo cargar la configuracion '%s'.\n", filename);
	} else {
		const STJsonNode* nRoot = configFile->nodoHijo();
		if(nRoot == NULL){
			PRINTF_ERROR("Config-file is empty.\n");
		} else {
			const STJsonNode* nConfig = configFile->nodoHijo("config", nRoot);
			if(nConfig == NULL){
				PRINTF_ERROR("Config-file is missing '/config' node.\n");
			} else {
				const STJsonNode* nPackages = configFile->nodoHijo("packages", nConfig);
				if(nPackages == NULL){
					PRINTF_ERROR("Config-file is missing '/config/packages' node.\n");
				} else {
					SI32 iScale = 0;
					AUCadenaMutable8* name = new AUCadenaMutable8();
					AUCadenaMutable8* root = new AUCadenaMutable8();
					AUCadenaMutable8* pkgRoot = new AUCadenaMutable8();
					AUCadenaMutable8* dstFile = new AUCadenaMutable8();
					const STJsonNode* nPackage = configFile->nodoHijo("packages", nPackages);
					while(nPackage != NULL){
						configFile->nodoHijo("name", name, "", nPackage);
						configFile->nodoHijo("root", root, "", nPackage);
						configFile->nodoHijo("pkgRoot", pkgRoot, "", nPackage);
						configFile->nodoHijo("dstFile", dstFile, "", nPackage);
						const STJsonNode* nRules = configFile->nodoHijo("rules", nPackage);
						if(dstFile->tamano() == 0){
							PRINTF_ERROR("Package #%d ('%s') 'dstFile' node is empty (ignoring package).\n", (iScale + 1), name->str());
						} else if(nRules == NULL){
							PRINTF_ERROR("Package #%d ('%s') 'rules' node is not defined (ignoring package).\n", (iScale + 1), name->str());
						} else {
							//Load rules
							SI32 iRule = 0;
							AUArregloNativoMutableP<NBGenPkgConfigRule>* rules = new AUArregloNativoMutableP<NBGenPkgConfigRule>();
							const STJsonNode* nRule = configFile->nodoHijo("rules", nRules);
							while(nRule != NULL){
								//Add rule
								NBGenPkgConfigRule data;
								NBToolPackageMaker::configRuleInit(&data);
								configFile->nodoHijo("path", data.path, "", nRule);
								configFile->nodoHijo("action", data.action, "", nRule);
								rules->agregarElemento(data);
								//
								nRule = configFile->nodoHijo("rules", nRules, nRule);
								iRule++;
							}
							//Process package
							{
								//Ensure root format as "./path/", "path/" or "/path/"
								if(root->tamano() > 0){
									if(root->str()[root->tamano() - 1] != '/'){
										root->agregar('/');
									}
								}
								//Ensure pkgRoot format as "path/to/path/"
								if(pkgRoot->tamano() > 0){
									//Remove "./" at the start
									while(pkgRoot->tamano() > 0 && (pkgRoot->str()[0] == '.' || pkgRoot->str()[0] == '/' || pkgRoot->str()[0] == '\\')){
										pkgRoot->quitarIzquierda(1);
									}
									//Add "/" ath the end
									if(pkgRoot->tamano() > 0){
										if(pkgRoot->str()[pkgRoot->tamano() - 1] != '/'){
											pkgRoot->agregar('/');
										}
									}
								}
								//Process
								NBGenPkgStats stats;
								NBToolPackageMaker::statsInit(&stats);
								NBGestorArchivos::asegurarExistenciaCarpetasEnRutaArchivo(dstFile->str());
								totalEmpaquetados += NBToolPackageMaker::generarPaqueteDeArbol(root->str(), pkgRoot->str(), dstFile->str(), "./paqTmpDatos.tmp", "./paqTmpIndice.tmp", false, &stats, rules);
								PRINTF_CONSOLE("PAQUETE '%s' (%.2f MBs, %d archivos, %d ignorados)\n", dstFile->str(), (float)((float)stats.qBytesIncluded/(1024.0f*1024.0f)), stats.qIncluded, stats.qIgnored);
							}
							//Release rules
							{
								SI32 i;
								for(i = 0; i < rules->conteo; i++){
									NBToolPackageMaker::configRuleFinalize(rules->elemPtr(i));
								}
								rules->vaciar();
							}
							rules->liberar();
						}
						//
						nPackage = configFile->nodoHijo("packages", nPackages, nPackage);
						iScale++;
					}
					dstFile->liberar();
					pkgRoot->liberar();
					root->liberar();
					name->liberar();
					//Delete tmp files
					NBGestorArchivos::eliminarArchivo("./paqTmpDatos.tmp");
					NBGestorArchivos::eliminarArchivo("./paqTmpIndice.tmp");
				}
			}
		}
	}
	configFile->liberar();
	PRINTF_CONSOLE("%d archivo empaquetados en total\n", totalEmpaquetados);
	CICLOS_CPU_HILO(cicleEnd);
	PRINTF_CONSOLE("DEVTOOL --- FIN GENERANDO PAQUETE DE ARCHIVOS RECURSOS (%.2f segs)\n", ((float)(cicleEnd - cicleStart) / (float)ciclesPerSec));
}

void makeHeadersFromBinaries(const char* filename){
	CICLOS_CPU_TIPO cicleStart, cicleEnd, ciclesPerSec;
	CICLOS_CPU_HILO(cicleStart);
	CICLOS_CPU_POR_SEGUNDO(ciclesPerSec);
	//
	PRINTF_CONSOLE("DEVTOOL --- GENERANDO ENCABEZADOS DE ARCHIVOS BINARIOS\n");
	UI32 totalProcesados = 0;
	AUDatosJSONMutable* configFile = new AUDatosJSONMutable();
	if(!configFile->cargaDatosJsonDesdeArchivo(filename)){
		PRINTF_ERROR("No se pudo cargar la configuracion '%s'.\n", filename);
	} else {
		const STJsonNode* nRoot = configFile->nodoHijo();
		if(nRoot == NULL){
			PRINTF_ERROR("Config-file is empty.\n");
		} else {
			const STJsonNode* nConfig = configFile->nodoHijo("config", nRoot);
			if(nConfig == NULL){
				PRINTF_ERROR("Config-file is missing '/config' node.\n");
			} else {
				const STJsonNode* nFiles = configFile->nodoHijo("binToHeader", nConfig);
				if(nFiles == NULL){
					PRINTF_ERROR("Config-file is missing '/config/packages' node.\n");
				} else {
					AUCadenaMutable8* srcFile = new AUCadenaMutable8();
					AUCadenaMutable8* dstFile = new AUCadenaMutable8();
					const STJsonNode* nFile = configFile->nodoHijo("binToHeader", nFiles);
					while(nFile != NULL){
						configFile->nodoHijo("srcFile", srcFile, "", nFile);
						configFile->nodoHijo("dstFileC", dstFile, "", nFile);
						if(srcFile->tamano() == 0){
							PRINTF_ERROR("binToHeader 'srcFile' node is empty (ignoring binary).\n");
						} else if(dstFile->tamano() == 0){
							PRINTF_ERROR("binToHeader 'dstFileC' node is empty (ignoring binary).\n");
						} else {
							//Open src file
                            STNBFileRef fSrc = NBFile_alloc(NULL);
							if(!NBFile_open(fSrc, srcFile->str(), ENNBFileMode_Read)){
								PRINTF_ERROR("binToHeader 'srcFile' could not be opened: %s.\n", srcFile->str());
							} else {
								//Open dst file
								{
                                    STNBFileRef fDst = NBFile_alloc(NULL);
									if(!NBFile_open(fDst, dstFile->str(), ENNBFileMode_Write)){
										PRINTF_ERROR("binToHeader 'dstFile' could not be opened: %s.\n", dstFile->str());
									} else {
										NBFile_lock(fSrc);
										//Process
										{
											STNBString strCode, strNameC;
											NBString_initWithSz(&strCode, 1024 * 1024, 1024 * 1024, 0.20f);
											NBString_init(&strNameC);
											//Create "c" normalized name
											{
												const char* ptrStr = dstFile->str();
												while(*ptrStr != '\0'){
													if((*ptrStr >= 'a' && *ptrStr <= 'z') || (*ptrStr >= 'A' && *ptrStr <= 'Z') || *ptrStr == '_'){
														NBString_concatByte(&strNameC, *ptrStr);
													} else if((*ptrStr >= '0' && *ptrStr <= '9')){
														if(strNameC.length <= 0){
															//Cannot start with a digit
															NBString_concatByte(&strNameC, '_');
														} else {
															NBString_concatByte(&strNameC, *ptrStr);
														}
													} else {
														NBString_concatByte(&strNameC, '_');
													}
													ptrStr++;
												}
											}
											{
												NBString_concat(&strCode, "//File autogenerated by devtool.\n");
												NBString_concat(&strCode, "#ifndef "); NBString_concat(&strCode, strNameC.str); NBString_concat(&strCode, "_h \n");
												NBString_concat(&strCode, "#define "); NBString_concat(&strCode, strNameC.str); NBString_concat(&strCode, "_h \n");
												NBString_concat(&strCode, "\n");
												NBString_concat(&strCode, "static unsigned char "); NBString_concat(&strCode, strNameC.str); NBString_concat(&strCode, "[] = {\n");
												//Concat
												{
													const char* hexDigits = "0123456789ABCDEF";
													UI32 totalBytes = 0, totalBytesLine = 0;
													BYTE buff[4096];
													while(TRUE){
														const SI32 readCount = NBFile_read(fSrc, buff, sizeof(buff));
														if(readCount <= 0){
															break;
														} else {
															SI32 i; for(i = 0; i < readCount; i++){
																if(totalBytesLine > 32){
																	NBString_concat(&strCode, "\n\t");
																	totalBytesLine = 0;
																}
																NBString_concat(&strCode, (totalBytes <= 0 ? "\t" : ", "));
																{
																	NBString_concat(&strCode, "0x");
																	NBString_concatByte(&strCode, hexDigits[((buff[i] >> 4) & 0xF)]);
																	NBString_concatByte(&strCode, hexDigits[(buff[i] & 0xF)]);
																}
																//{
																//	NBString_concatUI32(&strCode, (UI32)buff[i]);
																//}
																totalBytesLine++;
																totalBytes++;
															}
														}
													}
													if(totalBytes > 0){
														NBString_concat(&strCode, "\n");
													}
												}
												NBString_concat(&strCode, "};\n");
												NBString_concat(&strCode, "\n");
												NBString_concat(&strCode, "#endif\n");
												//Write content
												{
													NBFile_lock(fDst);
													if(NBFile_write(fDst, strCode.str, strCode.length) != strCode.length){
														PRINTF_ERROR("binToHeader %d bytes could not be written: %s.\n", strCode.length, dstFile->str());
													} else {
														//PRINTF_CONSOLE("binToHeader %d written to: %s.\n", strCode.length, dstFile->str());
														totalProcesados++;
													}
													NBFile_unlock(fDst);
												}
											}
											NBString_release(&strNameC);
											NBString_release(&strCode);
										}
										NBFile_unlock(fSrc);
										NBFile_close(fDst);
									}
									NBFile_release(&fDst);
								}
								NBFile_close(fSrc);
							}
							NBFile_release(&fSrc);
						}
						//Next
						nFile = configFile->nodoHijo("binToHeader", nFiles, nFile);
					}
					dstFile->liberar();
					srcFile->liberar();
				}
			}
		}
	}
	configFile->liberar();
	PRINTF_CONSOLE("%d archivos procesados en total.\n", totalProcesados);
	CICLOS_CPU_HILO(cicleEnd);
	PRINTF_CONSOLE("DEVTOOL --- FIN GENERANDO ENCABEZADOS DE ARCHIVOS BINARIOS (%.2f segs).\n", ((float)(cicleEnd - cicleStart) / (float)ciclesPerSec));
}

void makeImagesScalesPNGX8(){
	CICLOS_CPU_TIPO cicleStart, cicleEnd, ciclesPerSec;
	CICLOS_CPU_HILO(cicleStart);
	CICLOS_CPU_POR_SEGUNDO(ciclesPerSec);
	//
	PRINTF_CONSOLE("DEVTOOL --- ASEGURANDO QUE LAS IMGs X8 SEAN MULTIPLO DE 8\n");
	SI32 conteoRedimensionadas = 0, conteoIgnoradas = 0;
	NBToolImagesScaler::actualizarImgsHaciaTamanoMultiploDe("./PNGX8", 8, &conteoRedimensionadas, &conteoIgnoradas);
	PRINTF_CONSOLE("Tamano multiplo-de-8: %d de %d IMGs redimensionadas\n", conteoRedimensionadas, (conteoIgnoradas+conteoRedimensionadas));
	//
	CICLOS_CPU_HILO(cicleEnd);
	PRINTF_CONSOLE("DEVTOOL --- FIN ASEGURANDO QUE LAS IMGs X8 SEAN MULTIPLO DE 8 (%.2f segs)\n", ((float)(cicleEnd - cicleStart) / (float)ciclesPerSec));
}

void makeImagesScalesFromFilename(const char* filename, const ENGenResProfile profile){
	const char* profileName = NBToolImagesScaler::configGetProfileNodeName(profile);
	CICLOS_CPU_TIPO cicleStart, cicleEnd, ciclesPerSec;
	CICLOS_CPU_HILO(cicleStart);
	CICLOS_CPU_POR_SEGUNDO(ciclesPerSec);
	//
	PRINTF_CONSOLE("DEVTOOL --- GENERANDO ESCALAS DE IMAGENES CON FIRMA SHA1 (%s PROFILE)\n", profileName);
	AUDatosJSONMutable* configFile	= new AUDatosJSONMutable();
	if(!configFile->cargaDatosJsonDesdeArchivo(filename)){
		PRINTF_ERROR("No se pudo cargar la configuracion '%s'.\n", filename);
	} else {
		const STJsonNode* nRoot = configFile->nodoHijo();
		if(nRoot == NULL){
			PRINTF_ERROR("Config-file is empty.\n");
		} else {
			const STJsonNode* nConfig = configFile->nodoHijo("config", nRoot);
			if(nConfig == NULL){
				PRINTF_ERROR("Config-file is missing '/config' node.\n");
			} else {
				const STJsonNode* nScales = configFile->nodoHijo("imgScales", nConfig);
				if(nScales == NULL){
					PRINTF_ERROR("Config-file is missing '/config/imgScales' node.\n");
				} else {
					AUCadenaMutable8* scalesMode = new AUCadenaMutable8();
					configFile->nodoHijo("imgScalesMode", scalesMode, "parallel", nConfig);
					//
					AUArregloNativoMutableP<NBGenResScale>* scales = new AUArregloNativoMutableP<NBGenResScale>();
					NBToolImagesScaler::configScalesLoad(configFile, nScales, scales);
					if(scales->conteo <= 0){
						PRINTF_WARNING("No scales loaded from config file.\n");
					} else {
						//
						if(scalesMode->esIgual("seq") || scalesMode->esIgual("sequence") || scalesMode->esIgual("sequential")){
							//---------------------
							//- Run tasks in SEQUENTIAL mode
							//- (same file may be loaded multiple times, once per scale)
							//---------------------
							PRINTF_CONSOLE("%d scales loaded from config file (SEQUENTIAL processing).\n", scales->conteo);
							PRINTF_CONSOLE("\n");
							NBGenResState state; NBToolImagesScaler::stateInit(&state);
							SI32 i; const SI32 count = scales->conteo;
							for(i = 0; i < count; i++){
								NBGenResScale* data = scales->elemPtr(i);
								//Process scale
								if(data->rules->conteo <= 0){
									PRINTF_WARNING("Scale #%d ('%s'), no path-rules for scale.\n", (i + 1), data->name->str());
								} else {
									const NBGenResScaleRule* defRule = NBToolImagesScaler::configScaleRuleByPath(data->rules, "");
									PRINTF_CONSOLE("Scale #%d ('%s'), processing %d '%s' path-rules%s...\n", (i + 1), data->name->str(), data->rules->conteo, profileName, (defRule == NULL ? " (NO DEFAULT RULE DEFINED)":""));
									SI32 conteoPNGsProcesadas;
									NBGenResStats stats;
									NBToolImagesScaler::statsInit(&stats);
									NBGestorArchivos::asegurarExistenciaCarpetasEnRutaCarpeta(data->dstPath->str());
									conteoPNGsProcesadas = NBToolImagesScaler::generateScaledImages(&state, profile, data->root->str(), data->dstPath->str(), "", data->rootScale, /*NULL*/ &stats, data->rules, defRule);
									NBToolImagesScaler::statsPrint(&stats);
									PRINTF_CONSOLE("\n");
								}
							}
							NBToolImagesScaler::stateFinalize(&state);
						} else {
							//---------------------
							//- Run tasks in PARALLEL mode (default)
							//- (each file will be loaded only once, and each scale applied)
							//---------------------
							PRINTF_CONSOLE("%d scales loaded from config file (PARALLEL processing).\n", scales->conteo);
							NBGenResState state; NBToolImagesScaler::stateInit(&state);
							NBGenResStats* stats = (NBGenResStats*) NBGestorMemoria::reservarMemoria(sizeof(NBGenResStats) * scales->conteo, ENMemoriaTipo_Temporal);
							//Find files-tasks
							{
								SI32 i; const SI32 count = scales->conteo;
								for(i = 0; i < count; i++){
									NBGenResScale* data = scales->elemPtr(i);
									NBToolImagesScaler::statsInit(&stats[i]);
									//Process scale
									if(data->rules->conteo <= 0){
										PRINTF_WARNING("Scale #%d ('%s'), no path-rules for scale.\n", (i + 1), data->name->str());
									} else {
										const NBGenResScaleRule* defRule = NBToolImagesScaler::configScaleRuleByPath(data->rules, "");
										PRINTF_CONSOLE("Scale #%d ('%s'), scanning %d path-rules%s...\n", (i + 1), data->name->str(), data->rules->conteo, (defRule == NULL ? " (NO DEFAULT RULE DEFINED)":""));
										SI32 conteoPNGsProcesadas;
										NBGestorArchivos::asegurarExistenciaCarpetasEnRutaCarpeta(data->dstPath->str());
										conteoPNGsProcesadas = NBToolImagesScaler::generateScaledImages(&state, ENGenResProfile_Count, data->root->str(), data->dstPath->str(), "", data->rootScale, /*NULL*/ &stats[i], data->rules, defRule);
									}
								}
							}
							//Process files-tasks
							{
								PRINTF_CONSOLE("Executing '%s' tasks for %d files found...\n", profileName, state.filesFound->conteo);
								const SI32 qTasks = NBToolImagesScaler::filesExecuteTasks(&state, profile);
								PRINTF_CONSOLE("... %d tasks executed.\n", qTasks);
							}
							//Print stats
							{
								PRINTF_CONSOLE("\n");
								SI32 i; const SI32 count = scales->conteo;
								for(i = 0; i < count; i++){
									NBGenResScale* data = scales->elemPtr(i);
									PRINTF_CONSOLE("Scale #%d ('%s'), processed %d '%s' path-rules ...\n", (i + 1), data->name->str(), data->rules->conteo, profileName);
									NBToolImagesScaler::statsPrint(&stats[i]);
									PRINTF_CONSOLE("\n");
								}
							}
							//
							NBGestorMemoria::liberarMemoria(stats);
							NBToolImagesScaler::stateFinalize(&state);
						}
					}
					NBToolImagesScaler::configScalesRelease(scales);
					scales->liberar();
					scalesMode->liberar();
				}
			}
		}
	}
	configFile->liberar();
	CICLOS_CPU_HILO(cicleEnd);
	PRINTF_CONSOLE("DEVTOOL --- FIN GENERANDO ESCALAS DE IMAGENES CON FIRMA SHA1 (%s PROFILE, %.2f segs)\n", profileName, ((float)(cicleEnd - cicleStart) / (float)ciclesPerSec));
}

void makeAppIconsFromFilename(const char* filename){
	CICLOS_CPU_TIPO cicleStart, cicleEnd, ciclesPerSec;
	CICLOS_CPU_HILO(cicleStart);
	CICLOS_CPU_POR_SEGUNDO(ciclesPerSec);
	//
	PRINTF_CONSOLE("DEVTOOL --- GENERANDO ICONOS\n");
	{
		STNBJson json;
		NBJson_init(&json);
		if(!NBJson_loadFromFilePath(&json, filename)){
			PRINTF_ERROR("Could not load JSON file: '%s'.\n", filename);
		} else {
			const STNBJsonNode* appIcons = NBJson_childNode(&json, "appIcons", NBJson_rootMember(&json), NULL);
			if(appIcons == NULL){
				PRINTF_ERROR("Expected 'appIcons' node at: '%s'.\n", filename);
			} else {
				NBToolGenIconsJobs jobs;
				NBMemory_setZero(jobs);
				if(!NBStruct_stReadFromJsonNode(&json, appIcons, NBToolGenIconsJobs_getSharedStructMap(), &jobs, sizeof(jobs))){
					PRINTF_ERROR("Could not not parse 'config/appIcons/jobs' data at: '%s'.\n", filename);
				} else {
					UI32 i; for(i = 0 ; i < jobs.jobsSz; i++){
						const NBToolGenIconsJob* job = &jobs.jobs[i];
						if(!job->enabled){
							PRINTF_CONSOLE("Ignoring job '%s' (not-enabled).\n", job->name);
						} else {
							PRINTF_CONSOLE("Running job '%s'.\n", job->name);
							NBToolAppIconsGenerator::generateAppIcons(job->baseIcon, job->dstFolder, job->namesPrefix, job->sizes, job->sizesSz);
						}
					}
					NBStruct_stRelease(NBToolGenIconsJobs_getSharedStructMap(), &jobs, sizeof(jobs));
				}
			}
		}
		NBJson_release(&json);
	}
	CICLOS_CPU_HILO(cicleEnd);
	PRINTF_CONSOLE("DEVTOOL --- FIN GENERANDO ICONOS (%.2f segs)\n", ((float)(cicleEnd - cicleStart) / (float)ciclesPerSec));
}

void makeLaunchScreensFromFilename(const char* filename){
	CICLOS_CPU_TIPO cicleStart, cicleEnd, ciclesPerSec;
	CICLOS_CPU_HILO(cicleStart);
	CICLOS_CPU_POR_SEGUNDO(ciclesPerSec);
	//
	PRINTF_CONSOLE("DEVTOOL --- GENERANDO LAUNCHSCREENS\n");
	{
		STNBJson json;
		NBJson_init(&json);
		if(!NBJson_loadFromFilePath(&json, filename)){
			PRINTF_ERROR("Could not load JSON file: '%s'.\n", filename);
		} else {
			const STNBJsonNode* launchScreens = NBJson_childNode(&json, "launchScreens", NBJson_rootMember(&json), NULL);
			if(launchScreens == NULL){
				PRINTF_ERROR("Expected 'launchScreens' node at: '%s'.\n", filename);
			} else {
				NBToolGenLaunchJobs jobs;
				NBMemory_setZero(jobs);
				if(!NBStruct_stReadFromJsonNode(&json, launchScreens, NBToolGenLaunchJobs_getSharedStructMap(), &jobs, sizeof(jobs))){
					PRINTF_ERROR("Could not not parse 'config/appIcons/jobs' data at: '%s'.\n", filename);
				} else {
					UI32 i; for(i = 0 ; i < jobs.jobsSz; i++){
						const NBToolGenLaunchJob* job = &jobs.jobs[i];
						if(!job->enabled){
							PRINTF_CONSOLE("Ignoring job '%s' (not-enabled).\n", job->name);
						} else {
							PRINTF_CONSOLE("Running job '%s'.\n", job->name);
							NBToolLaunchscreensGenerator::generateLaunchscreens(job);
						}
					}
					NBStruct_stRelease(NBToolGenLaunchJobs_getSharedStructMap(), &jobs, sizeof(jobs));
				}
			}
		}
		NBJson_release(&json);
	}
	CICLOS_CPU_HILO(cicleEnd);
	PRINTF_CONSOLE("DEVTOOL --- FIN GENERANDO LAUNCHSCREENS (%.2f segs)\n", ((float)(cicleEnd - cicleStart) / (float)ciclesPerSec));
}
