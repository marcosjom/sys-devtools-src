//
//  main.cpp
//  tool-sourcetranslator
//
//  Created by Marcos Ortega on 13/01/15.
//  Copyright (c) 2015 NIBSA. All rights reserved.
//

#include "AUFrameworkBaseStdAfx.h"
#include "AUFrameworkBase.h"
//
#include "NBSourceTranslator.h"
// Testing json-parser
#include "NBParserJsonP.h"

int main(int argc, const char * argv[]) {
	NBGestorMemoria::inicializar(1, (1024 * 1024 * (2.2f)), 8191, 1, (1024 * 1024 * (2.2f)), 8191);
	NBGestorArchivos::inicializar();
	//
	// Testing json-parser
	//
	/*{
		AUDatosJSONMutableP<char>* jsonData = new AUDatosJSONMutableP<char>();
		if(!jsonData->cargaDatosJsonDesdeArchivo("jsonExample.txt")){
			NBASSERT(false)
		} else {
			jsonData->guardarJSONIdentadoHaciaArchivo("jsonExample.idented.txt");
			jsonData->guardarJSONSinEspaciosHaciaArchivo("jsonExample.compact.txt");
			/*AUDatosJSONMutableP<char>* jsonData2 = new AUDatosJSONMutableP<char>();
			if(!jsonData2->cargaDatosJsonDesdeArchivo("jsonExample.idented.txt")){
				NBASSERT(false)
			} else {
				jsonData2->guardarJSONIdentadoHaciaArchivo("jsonExample.idented.idented.txt");
				jsonData2->guardarJSONSinEspaciosHaciaArchivo("jsonExample.idented.compact.txt");
			}
			if(!jsonData2->cargaDatosJsonDesdeArchivo("jsonExample.compact.txt")){
				NBASSERT(false)
			} else {
				jsonData2->guardarJSONIdentadoHaciaArchivo("jsonExample.compact.idented.txt");
				jsonData2->guardarJSONSinEspaciosHaciaArchivo("jsonExample.compact.compact.txt");
			}
			jsonData2->liberar();* /
		}
		jsonData->liberar();
	}*/
	//
	//
	//Configuration
	const char* ignoreFile = "./tool-sourcetranslator-ignore.xml";
	//const char* pairsFileXml = "./tool-sourcetranslator-pairs.xml";
	const char* pairsFileCsv = "./tool-sourcetranslator-pairs.csv";
	const UI32 defaultOptMask = (TRANSLATE_OPT_NONE | TRANSLATE_OPT_TRANSLATE_OUTPUT_FILENAME | TRANSLATE_OPT_TRANSLATE_PATHS_IN_LITERALS | TRANSLATE_OPT_TRANSLATE_PATHS_IN_COMMENTS);
	const STSrcExtConfig extsGlobal[] = {
		//Source files
		{1, "h", ENSrcFileFormat_cpp, ENSrcAction_process, defaultOptMask},
		{1, "c", ENSrcFileFormat_c, ENSrcAction_process, defaultOptMask},
		{1, "hpp", ENSrcFileFormat_cpp, ENSrcAction_process, defaultOptMask},
		{1, "cpp", ENSrcFileFormat_cpp, ENSrcAction_process, defaultOptMask},
		{1, "m", ENSrcFileFormat_objc, ENSrcAction_process, defaultOptMask},
		{1, "mm", ENSrcFileFormat_objcpp, ENSrcAction_process, defaultOptMask},
		{1, "php", ENSrcFileFormat_php, ENSrcAction_process, defaultOptMask},
		{1, "sql", ENSrcFileFormat_mysql, ENSrcAction_process, defaultOptMask},
		{3, "java", ENSrcFileFormat_java, ENSrcAction_ignore, defaultOptMask}, //temporal ignore
		{3, "xml", ENSrcFileFormat_plain, ENSrcAction_ignore, defaultOptMask}, //temporal ignore
		{3, "txt", ENSrcFileFormat_plain, ENSrcAction_ignore, defaultOptMask}, //temporal ignore
		//Generated files
		{3, "o", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "obj", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "idb", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "pdb", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "log", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "tlog", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "lib", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "a", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "exe", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "jar", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "bin", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		//Resources files
		{3, "otf", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "png", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "jpg", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "jpeg", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "wav", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "mp3", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
		{3, "ogg", ENSrcFileFormat_plain, ENSrcAction_ignore, TRANSLATE_OPT_NONE},
	};
	const STSrcExtConfig extOnlySql[] = {
		//Source files
		{10, "sql", ENSrcFileFormat_mysql, ENSrcAction_process, defaultOptMask},
	};
	const STSrcPathConfig foldersAU[] = {
		//{2, "projects", ENSrcAction_process, (TRANSLATE_OPT_TRANSLATE_ONLY_PATHS | TRANSLATE_OPT_TRANSLATE_PATHS_IN_LITERALS | TRANSLATE_OPT_TRANSLATE_PATHS_IN_COMMENTS) },
		{2, "src/c", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/c/freetype", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/c/geotiff", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/c/libjpeg", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/c/libtiff", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/c/ogg", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/c/vorbis", ENSrcAction_copy, TRANSLATE_OPT_NONE},
	};
	const STSrcPathConfig foldersAUMedia[] = {
		//{2, "projects", ENSrcAction_process, (TRANSLATE_OPT_TRANSLATE_ONLY_PATHS | TRANSLATE_OPT_TRANSLATE_PATHS_IN_LITERALS | TRANSLATE_OPT_TRANSLATE_PATHS_IN_COMMENTS)},
		{2, "include/glew", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/c/glew", ENSrcAction_copy, TRANSLATE_OPT_NONE},
	};
	const STSrcPathConfig foldersAUApp[] = {
		//{2, "projects", ENSrcAction_copy, (TRANSLATE_OPT_TRANSLATE_ONLY_PATHS | TRANSLATE_OPT_TRANSLATE_PATHS_IN_LITERALS | TRANSLATE_OPT_TRANSLATE_PATHS_IN_COMMENTS)},
	};
	const STSrcPathConfig foldersAURefranero[] = {
		{2, "projects/xcode/app-refraneronica/facebook-ios-sdk-master", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "projects/xcode/app-refraneronica/GoogleSignIn.bundle", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "projects/xcode/app-refraneronica/GoogleSignIn.framework", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/cpp/app-refraneronica-protector", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/cpp/util-refraneronica-generador", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/php/sis-refranero-web/2015-09-16-api-antes-de-limpiar", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/php/sis-refranero-web/api/phpprotect_obfuscated", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/php/sis-refranero-web/api/phpprotect_obfuscated", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		//
		{2, "projects/xcode/app-refraneronica/deprecated_GooglePlus.framework", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "projects/xcode/app-refraneronica/deprecated_GooglePlus.bundle", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "projects/xcode/app-refraneronica/deprecated_GoogleOpenSource.framework", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "projects/xcode/app-refraneronica/deprecated_google-plus-ios-sdk", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "projects/visual-studio", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "projects/eclipse-workspace", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "projects/android-studio", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "projects/xcode", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "include/util-refraneronica-generador", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "include/lib-refraneronica-generador", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/cpp/util-refraneronica-generador", ENSrcAction_copy, TRANSLATE_OPT_NONE},
		{2, "src/cpp/lib-refraneronica-generador", ENSrcAction_copy, TRANSLATE_OPT_NONE},
	};
	const STSrcTraslationTask translationTasks[] = {
		{
			"lib-auframework"
			, "./lib-auframework/lib-auframework", "./lib-auframework/lib-auframework-en"
			, "./translation-pairs-lib-auframework.csv"
			,0 , ENSrcAction_copy, TRANSLATE_OPT_NONE
			, NULL, 0
			, foldersAU, (sizeof(foldersAU) / sizeof(foldersAU[0]))
		},
		{
			"lib-auframework-media"
			, "./lib-auframework-media/lib-auframework-media", "./lib-auframework-media/lib-auframework-media-en"
			, "./translation-pairs-lib-auframework-media.csv"
			,0 , ENSrcAction_copy, TRANSLATE_OPT_NONE
			, NULL, 0
			, foldersAUMedia, (sizeof(foldersAUMedia) / sizeof(foldersAUMedia[0]))
		},
		{
			"lib-auframework-app"
			, "./lib-auframework-app/lib-auframework-app", "./lib-auframework-app/lib-auframework-app-en"
			, "./translation-pairs-lib-auframework-app.csv"
			,0 , ENSrcAction_copy, TRANSLATE_OPT_NONE
			, NULL, 0
			, NULL, 0 //, foldersAUApp, (sizeof(foldersAUApp) / sizeof(foldersAUApp[0]))
		}/*,
		{
			"sis-refraneronica"
			, "../CltAbelOrtega/sis-refraneronica", "../CltAbelOrtega/sis-refraneronica-en"
			, "./translation-pairs-sys-app-refranero-nica.csv"
			,9 , ENSrcAction_copy, TRANSLATE_OPT_NONE
			, extOnlySql, (sizeof(extOnlySql) / sizeof(extOnlySql[0]))
			, foldersAURefranero, (sizeof(foldersAURefranero) / sizeof(foldersAURefranero[0]))
		},*/
	};
	const STSrcTranslatorParams translationParams = {
		//tasks
		translationTasks, (sizeof(translationTasks) / sizeof(translationTasks[0]))
		//global extensions set
		, extsGlobal, (sizeof(extsGlobal) / sizeof(extsGlobal[0]))
	};
	//Init
	STStringsLib strlib;
	StringLib_init(&strlib);
	//Load from file
	Parser_loadPairsFromCSVs(&translationParams, &strlib);
	//Determine repeated new values
	//(posible cause of compilation/runtime/program-logic problems on translated code)
	{
		AUArregloNativoMutableP<STStrLibIndex>* found = new(ENMemoriaTipo_Temporal) AUArregloNativoMutableP<STStrLibIndex>();
		StringLib_findDuplicatedTranslations(&strlib, found, true);
		if(found->conteo == 0){
			PRINTF_INFO("No repeated tramslations found (every translation is unique).\n");
		} else {
			PRINTF_WARNING("Repeated translations found!\n")
			PRINTF_WARNING("This may produce translated code with problems to\n")
			PRINTF_WARNING("compile, run or change the program's logic.\n")
			PRINTF_WARNING("Please avoid all repeated translations:\n")
			SI32 i;
			for(i = 0; i < found->conteo; i++){
				const STStrLibIndex* itm	= found->elemPtr(i);
				const char* taskName		= translationParams.transTasks[itm->iArray].taskName;
				const char* strsArr			= strlib.pairsArr[itm->iArray].strings->str();
				const char* orgWord			= &strsArr[strlib.pairsArr[itm->iArray].pairs->elemPtr(itm->iItem)->iOrig];
				const char* newWord			= &strsArr[strlib.pairsArr[itm->iArray].pairs->elemPtr(itm->iItem)->iNew];
				PRINTF_WARNING("#%d '%s' <- '%s' <- '%s'.\n", (i + 1), newWord, orgWord, taskName);
			}
		}
	}
	//Process tasks
	{
		Parser_init();
		//---------------------------
		//--- Collecting words
		//---------------------------
		{
			const SI32 pairsBefore = StringLib_size(&strlib);
			PRINTF_INFO("COLLECTING WORDS (starting with %d pairs)...\n", pairsBefore);
			STTranslStatsGrouped grpdStats;			TranslStatsGrouped_init(&grpdStats);
			STTranslStatsPerTask grpdStatsPerTask;	TranslStatsPerTask_init(&grpdStatsPerTask);
			const STTranslStat tasksStats			= Parser_processTasks(&translationParams, &strlib, TRANSLATE_ACTION_COLLECT_WORDS, &grpdStats, &grpdStatsPerTask);
			{
				const SI32 pairsAfter = StringLib_size(&strlib);
				PRINTF_INFO("---------------------------------\n");
				PRINTF_INFO("%d WORDS AFTER COLLECTING (%d new).\n", pairsAfter, pairsAfter - pairsBefore);
				PRINTF_INFO("%d files (%d processed, %d to copy, %d ignored).\n", tasksStats.countFiles, tasksStats.countFilesProcess, tasksStats.countFilesCopy, tasksStats.countFilesIgnore);
				PRINTF_INFO("%d lines read, %d lines written.\n", tasksStats.countLinesRead, tasksStats.countLinesWritten);
				PRINTF_INFO("%llu bytes read, %llu written.\n", tasksStats.countBytesRead, tasksStats.countBytesWritten);
				PRINTF_INFO("---------------------------------\n");
			}
			//Collect missing translations from backup files
			/*{
				UI32 r = 0;
				//translation-pairs-lib-auframework.bk.csv
				r = StringLib_loadMissingTranslationsFromPairsCsv(&strlib, "./translation-pairs-lib-auframework.bk.csv");
				PRINTF_INFO("%d missing translations loaded from './translation-pairs-lib-auframework.bk.csv'\n", r);
				//translation-pairs-lib-auframework-media.bk.csv
				r = StringLib_loadMissingTranslationsFromPairsCsv(&strlib, "./translation-pairs-lib-auframework-media.bk.csv");
				PRINTF_INFO("%d missing translations loaded from './translation-pairs-lib-auframework-media.bk.csv'\n", r);
				//translation-pairs-lib-auframework-app.bk.csv
				r = StringLib_loadMissingTranslationsFromPairsCsv(&strlib, "./translation-pairs-lib-auframework-app.bk.csv");
				PRINTF_INFO("%d missing translations loaded from './translation-pairs-lib-auframework-app.bk.csv'\n", r);
			}*/
			//Save to file
			Parser_savePairsToCSVs(&translationParams, &strlib, true);
			//Print stas
			Parser_printfGroupedStatsPerTask(&translationParams, &grpdStatsPerTask);
			PRINTF_INFO("\n");
			PRINTF_INFO("\n");
			PRINTF_INFO("\n");
			PRINTF_INFO("++++++++++++++++++++++++++\n")
			PRINTF_INFO("++ ALL TASKS STATS (start)\n");
			PRINTF_INFO("++++++++++++++++++++++++++\n")
			TranslStatsGrouped_printf(&grpdStats);
			PRINTF_INFO("++++++++++++++++++++++++++\n")
			PRINTF_INFO("++ ALL TASKS STATS (end)\n");
			PRINTF_INFO("++++++++++++++++++++++++++\n")
			PRINTF_INFO("\n");
			PRINTF_INFO("\n");
			PRINTF_INFO("\n");
			//
			TranslStatsGrouped_finalize(&grpdStats);
			TranslStatsPerTask_finalize(&grpdStatsPerTask);
		}
		//---------------------------
		//--- Apply translations
		//---------------------------
		/*{
			PRINTF_INFO("TRANSLATING FILES....\n");
			STTranslStatsGrouped grpdStats;			TranslStatsGrouped_init(&grpdStats);
			STTranslStatsPerTask grpdStatsPerTask;	TranslStatsPerTask_init(&grpdStatsPerTask);
			const STTranslStat tasksStats = Parser_processTasks(&translationParams, &strlib, TRANSLATE_ACTION_TRANSLATE, &grpdStats, &grpdStatsPerTask);
			PRINTF_INFO("---------------------------------\n");
			PRINTF_INFO("END OF TRANSLATING ACTION.\n");
			PRINTF_INFO("%d files (%d processed, %d copied, %d ignored).\n", tasksStats.countFiles, tasksStats.countFilesProcess, tasksStats.countFilesCopy, tasksStats.countFilesIgnore);
			PRINTF_INFO("%d lines read, %d lines written.\n", tasksStats.countLinesRead, tasksStats.countLinesWritten);
			PRINTF_INFO("%llu bytes read, %llu written.\n", tasksStats.countBytesRead, tasksStats.countBytesWritten);
			PRINTF_INFO("---------------------------------\n");
			//Print stas
			Parser_printfGroupedStatsPerTask(&translationParams, &grpdStatsPerTask);
			PRINTF_INFO("\n");
			PRINTF_INFO("\n");
			PRINTF_INFO("\n");
			PRINTF_INFO("++++++++++++++++++++++++++\n")
			PRINTF_INFO("++ ALL TASKS STATS\n");
			PRINTF_INFO("++++++++++++++++++++++++++\n")
			TranslStatsGrouped_printf(&grpdStats);
			PRINTF_INFO("\n");
			PRINTF_INFO("\n");
			PRINTF_INFO("\n");
			//
			TranslStatsGrouped_finalize(&grpdStats);
			TranslStatsPerTask_finalize(&grpdStatsPerTask);
		}*/
		Parser_finalize();
	}
	//Finalize
	StringLib_finalize(&strlib);
	NBGestorArchivos::finalizar();
	NBGestorMemoria::finalizar();
    return 0;
}
