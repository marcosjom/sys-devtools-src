//
//  main.cpp
//  srcanalyzer
//
//  Created by Marcos Ortega on 14/6/17.
//  Copyright © 2017 NIBSA. All rights reserved.
//

#include "nb/NBFrameworkPch.h"
#include "nb/NBFrameworkDefs.h"
//
#include "nb/core/NBMngrProcess.h"
#include "nb/core/NBMngrStructMaps.h"
#include "nb/core/NBSintaxParser.h"
#include "nb/core/NBSintaxParserC.h"
//
#include "NBSrcAnlzParser.h"		//for 'STSrcAnlzParser'
#include "NBSrcAnlzCodeC.h"			//for 'STSrcAnlzCodeC'
#include "NBSrcAnlzCodeCParser.h"	//for 'STSrcAnlzCodeCParser'
//
#include "NBSrcAnlzSintxCPlain.h"
#include "NBSrcAnlzSintxC.h"

//
/*#define ALGO 1
#define NADA 0
#define MI_FUNC(X)	0*/

//#if MI_FUNC([wh,at])
//#warning WHATTTT
//#endif

/*#include "NBSrcAnlzTokn.h"*/
//#define MY_FUNC(P1)	P1
//#define MY_FUNC	"hola-obj"

//#define MY_		MY_
//#define NAME	NAME
//#define MY_NAME "hola"

/*#define MACRO_TXT		"txt"
#define MACRO_A(TXT) 	TXT NBLEVEL5
#define MACRO_B(TXT)	MACRO_A(TXT) "b"*/

/*#define NBLEVEL1		"1"
#define NBLEVEL2		NBLEVEL1
#define NBLEVEL3		NBLEVEL2
#define NBLEVEL4		NBLEVEL3
#define NBLEVEL5		NBLEVEL4*/

/*#define FUSION_DEF		MACRO_A ## MACRO_B
#define HASH_HASH		# ## #
#define MACRO_A			"a"
#define MACRO_B			"b"
#define MACRO_AMACRO_B	"xyz"
#define FUSION(P1, P2, P3)	P1 ## P2 P3*/

#define P1				999
#define PARAMS_3		HOLA, HOLA2, HOLA3
#define FUNC3(P1, P2, P3) #P1 #P2 #P3
#define FUNC3_2(P1, P2, P3) FUNC3(P ## 1 , P2, P3)
#define FUNC3_DEAFULT FUNC3_2(PARAMS_3)

#if defined(P1)

#endif

void lcdPrintInt(int col, int row, int value, int digits){
  bool printed = false, isNeg = false;
  int base = 1, baseTmp = 1, digitsTmp = 1;
  //Nothing to print
  if(digits <= 0) return;
  //Print sign
  if(value < 0){
    isNeg = true;
    value = -value;
  }
  //Move up (determine maxBase and filter values outside digits limits)
  while(baseTmp <= value || digitsTmp <= digits){
    //Remove extra value
    if(digitsTmp > digits){
      //filter values outside digits limits
      value /= baseTmp;
    } else {
      //determine maxBase 
      base = baseTmp;
	  baseTmp *= 10;
    }
    //increase
    digitsTmp++;
  }
  //Print digits
  while(base != 0){
	  col++;
    if(value >= base || base == 1 || printed){
      if(isNeg && !printed){
		  PRINTF_INFO("row(%d), col(%d): '%s'.\n", row, col, "-");
      } else {
		  PRINTF_INFO("row(%d), col(%d): '%d'.\n", row, col, (value / base));
      }
      value = (value % base);
      printed = true;
      base /= 10;
    } else {
      base /= 10;
      if(isNeg && (value >= base || base == 1)){
        PRINTF_INFO("row(%d), col(%d): '%s'.\n", row, col, "-");
        printed = true;
      } else {
        PRINTF_INFO("row(%d), col(%d): '%s'.\n", row, col, " ");
      }
    }
  }
}

int main(int argc, const char * argv[]) {
    NBMngrProcess_init();
    NBMngrStructMaps_init();
	//PRINTF_CONSOLE("%s.\n", MY_FUNC("HOLA1"));
	//PRINTF_CONSOLE("%s.\n", MY_NAME);
	//const char* texto = MACRO_A(MACRO_A(MACRO_B("txt")));
	//const char* texto = FUSION(MACRO_A MACRO_A, MACRO_B MACRO_B, FUSION(MACRO_A, MACRO_B, ));
	//const char* texto = FUSION(MACRO_A MACRO_A, MACRO_B MACRO_B, FUSION(MACRO_A, MACRO_B,));
	const char* texto  = FUNC3_2(1, 2, 3);
	PRINTF_CONSOLE("%s.\n", texto);
	//
	//lcdPrintInt(0, 0, 0, 1);
	//lcdPrintInt(0, 0, -123456789, 5);
	//
	{
		//Testing rules completeness at constant-expression
		const char* rulesStr =
		"initializer-list:\n"\
		"designation(opt) initializer\n"\
		"initializer-list [,] designation(opt) initializer\n"\
		"\n"\
		"type-name:\n"\
		"specifier-qualifier-list abstract-declarator(opt)\n"\
		"\n"\
		"argument-expression-list:\n"\
		"assignment-expression\n"\
		"argument-expression-list [,] assignment-expression\n"\
		"\n"\
		"primary-expression:\n"\
		"identifier\n"\
		"constant\n"\
		"string-literal\n"\
		"[(] expression [)]\n"\
		"\n"\
		"postfix-expression:\n"\
		"primary-expression\n"\
		"postfix-expression [[] expression []]\n"\
		"postfix-expression [(] argument-expression-list(opt) [)]\n"\
		"postfix-expression [.] identifier\n"\
		"postfix-expression [->] identifier\n"\
		"postfix-expression [++]\n"\
		"postfix-expression [--]\n"\
		"[(] type-name [)] [{] initializer-list [}]\n"\
		"[(] type-name [)] [{] initializer-list [,] [}]\n"\
		"\n"\
		"unary-expression:\n"\
		"postfix-expression\n"\
		"[++] unary-expression\n"\
		"[--] unary-expression\n"\
		"unary-operator cast-expression\n"\
		"[sizeof] unary-expression\n"\
		"[sizeof] [(] type-name [)]\n"\
		"\n"\
		"unary-operator:\n"\
		"[&]\n"\
		"[*]\n"\
		"[+]\n"\
		"[-]\n"\
		"[~]\n"\
		"[!]\n"\
		"\n"\
		"cast-expression:\n"\
		"unary-expression\n"\
		"[(] type-name [)] cast-expression\n"\
		"\n"\
		"multiplicative-expression:\n"\
		"cast-expression\n"\
		"multiplicative-expression [*] cast-expression\n"\
		"multiplicative-expression [/] cast-expression\n"\
		"multiplicative-expression [%] cast-expression\n"\
		"\n"\
		"additive-expression:\n"\
		"multiplicative-expression\n"\
		"additive-expression [+] multiplicative-expression\n"\
		"additive-expression [-] multiplicative-expression\n"\
		"\n"\
		"shift-expression:\n"\
		"additive-expression\n"\
		"shift-expression [<<] additive-expression\n"\
		"shift-expression [>>] additive-expression\n"\
		"\n"\
		"relational-expression:\n"\
		"shift-expression\n"\
		"relational-expression [<] shift-expression\n"\
		"relational-expression [>] shift-expression\n"\
		"relational-expression [<=] shift-expression\n"\
		"relational-expression [>=] shift-expression\n"\
		"\n"\
		"equality-expression:\n"\
		"relational-expression\n"\
		"equality-expression [==] relational-expression\n"\
		"equality-expression [!=] relational-expression\n"\
		"\n"\
		"AND-expression:\n"\
		"equality-expression\n"\
		"AND-expression [&] equality-expression\n"\
		"\n"\
		"exclusive-OR-expression:\n"\
		"AND-expression\n"\
		"exclusive-OR-expression [^] AND-expression\n"\
		"\n"\
		"inclusive-OR-expression:\n"\
		"exclusive-OR-expression\n"\
		"inclusive-OR-expression [|] exclusive-OR-expression\n"\
		"\n"\
		"logical-AND-expression:\n"\
		"inclusive-OR-expression\n"\
		"logical-AND-expression [&&] inclusive-OR-expression\n"\
		"\n"\
		"logical-OR-expression:\n"\
		"logical-AND-expression\n"\
		"logical-OR-expression [||] logical-AND-expression\n"\
		"\n"\
		"conditional-expression:\n"\
		"logical-OR-expression\n"\
		"logical-OR-expression [?] expression [:] conditional-expression\n"\
		"\n"\
		"assignment-expression:\n"\
		"conditional-expression\n"\
		"unary-expression assignment-operator assignment-expression\n"\
		"\n"\
		"assignment-operator:\n"\
		"[=]\n"\
		"[*=]\n"\
		"[/=]\n"\
		"[%=]\n"\
		"[+=]\n"\
		"[-=]\n"\
		"[<<=]\n"\
		"[>>=]\n"\
		"[&=]\n"\
		"[^=]\n"\
		"[|=]\n"\
		"\n"\
		"expression:\n"\
		"assignment-expression\n"\
		"expression [,] assignment-expression\n"\
		"\n"\
		"constant-expression:\n"\
		"conditional-expression";
		STNBSintaxParser parser;
		NBSintaxParser_init(&parser);
		NBSintaxParser_rulesFeedStart(&parser);
		if(!NBSintaxParser_rulesFeed(&parser, rulesStr)){
			PRINTF_INFO("main, NBSintaxParser_rulesFeed failed.\n");
		} else if(!NBSintaxParser_rulesFeedEnd(&parser)){
			PRINTF_INFO("main, NBSintaxParser_rulesFeedEnd failed.\n");
		}
		NBSintaxParser_release(&parser);
	}
	{
		STNBSintaxParserC parser;
		NBSintaxParserC_init(&parser);
		//Config
		{
			NBSintaxParserC_cfgIncludePathsUsrAdd(&parser, "/Users/mortegam/Desktop/NIBSA_proyectos/CltNicaraguaBinary/sys-nbframework/lib-nbframework-src/include/");
			NBSintaxParserC_cfgIncludePathsUsrAdd(&parser, "/Users/mortegam/Desktop/NIBSA_proyectos/CltNicaraguaBinary/sys-auframework/lib-auframework-src/include/");
			NBSintaxParserC_cfgIncludePathsUsrAdd(&parser, "/Users/mortegam/Desktop/NIBSA_proyectos/CltNicaraguaBinary/sys-auframework/sys-devtools-src/include/");
			NBSintaxParserC_cfgIncludePathsSysAdd(&parser, "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/");
		}
		if(!NBSintaxParserC_feedStart(&parser, "")){
			PRINTF_ERROR("NBSintaxParserC_feedStart failed.\n");
		} else {
			//Feed static code
			/*{
				const char* codeC = "#	define VAL_A	/ *comment* / \"A\" "; //
				if(!NBSintaxParserC_feed(&parser, codeC)){
					PRINTF_ERROR("NBSintaxParserC_feed failed.\n");
				}
			}*/
			//Feed file
			{
				const char* filepath = "/Users/mortegam/Desktop/NIBSA_proyectos/CltNicaraguaBinary/sys-nbframework/lib-nbframework-src/src/core/NBSintaxParser.c";
                STNBFileRef file = NBFile_alloc(NULL);
				if(!NBFile_open(file, filepath, ENNBFileMode_Read)){
					PRINTF_ERROR("NBFile_open failed.\n");
				} else {
					NBFile_lock(file);
					{
						char buff[4096];
						while(TRUE){
							const SI32 buffRead = NBFile_read(file, buff, sizeof(buff));
							if(buffRead <= 0) break;
							if(!NBSintaxParserC_feedBytes(&parser, buff, buffRead)){
								PRINTF_ERROR("NBSintaxParserC_feed failed.\n");
								break;
							}
						}
					}
					NBFile_unlock(file);
				}
				NBFile_release(&file);
			}
			if(!NBSintaxParserC_feedEnd(&parser)){
				PRINTF_ERROR("NBSintaxParserC_feedEnd failed.\n");
			}
		}
		NBSintaxParserC_release(&parser);
	}
/*#	define VAL_A	"A"
#	define VAL_B	"B"
#	define VAL_AB	"AB"
#	define VAL_ABC	"ABZ"
#	define XYC		"XYC_MACRO"
#	define ARG1_MERGED_XYC(X, Y)	X ## Y
#	define ARG1_MERGED(X, Y, XY, XYC)	X ## Y ## C
#	define VA_ARGS_STR(MY...)	#MY
#	define VA_ARGS_STR2(MY...)	, ## MY*/
	//PRINTF_INFO("'%s'.\n", ARG1_MERGED_XYC(VA_ARGS_STR,(VAL_A, B)));
	//PRINTF_INFO("'%s'.\n", "" ARG1_MERGED(VAL_A, B, "TERCERO"));
	//PRINTF_INFO("'%s'.\n", VA_ARGS_STR(ARG1_MERGED(VAL_A, B, "TERCERO")));
	//PlainSintax -> BasicSintaxsDefs
	//
	/*{
		SrcAnlzSintx_genBasicStructuresFromPlainDefs(
										  __globalLangSintaxDefsPlainC, (sizeof(__globalLangSintaxDefsPlainC) / sizeof(__globalLangSintaxDefsPlainC[0])),
										  __globalKeywords_c, (sizeof(__globalKeywords_c) / sizeof(__globalKeywords_c[0])),
										  __globalKeywords_c_lenRngs, (sizeof(__globalKeywords_c_lenRngs) / sizeof(__globalKeywords_c_lenRngs[0])),
										  __globalPunctuators_c, (sizeof(__globalPunctuators_c) / sizeof(__globalPunctuators_c[0])),
										  __globalPunctuators_c_lenRngs, (sizeof(__globalPunctuators_c_lenRngs) / sizeof(__globalPunctuators_c_lenRngs[0]))
										  );
	}*/
	//
	// BasicSintaxsDefs -> AdvancedSintaxsDefs
	//
	/*{
		SrcAnlzSintx_genAdvancedStructuresFromBasicStructures(__globalSintxCDefSeqs, (sizeof(__globalSintxCDefSeqs) / sizeof(__globalSintxCDefSeqs[0])), __globalSintxCDefs);
	}*/
	//
	//
	//
	//
	//CICLOS_CPU_TIPO startTime, endTime, ciclesPerSec;
	//CICLOS_CPU_POR_SEGUNDO(ciclesPerSec)
	//CICLOS_CPU_HILO(startTime)
	//FILE* stream = fopen("/Users/mortegam/Desktop/NIBSA_proyectos/CltNicaraguaBinary/sys-auframework/sys-devtools-src/projects/xcode/devtools/srcanalyzer/NBSrcAnlz_TestCode.c", "rb+");
	/*FILE* stream = fopen("/Users/mortegam/Desktop/NIBSA_proyectos/CltNicaraguaBinary/sys-auframework/sys-devtools-src/src/NBSrcAnlzParser.c", "rb+");
	if(stream == NULL){
		PRINTF_ERROR("Could not open file to parse.\n");
	} else {
		STSrcAnlzParserItfs glue;
		STSrcAnlzParser parser;
		STSrcAnlzCodeCParser sntxParser;
		STSrcAnlzCodeC dstCode;
		SrcAnlzCodeC_init(&dstCode);
		SrcAnlzCodeCParser_init(&sntxParser, &glue, &dstCode);
		SrcAnlzParser_init(&parser, &glue);
		//
		SrcAnlzCodeCParser_addIncludePathUsr(&sntxParser, "/Users/mortegam/Desktop/NIBSA_proyectos/CltNicaraguaBinary/sys-nbframework/lib-nbframework-src/include/");
		SrcAnlzCodeCParser_addIncludePathUsr(&sntxParser, "/Users/mortegam/Desktop/NIBSA_proyectos/CltNicaraguaBinary/sys-auframework/lib-auframework-src/include/");
		SrcAnlzCodeCParser_addIncludePathUsr(&sntxParser, "/Users/mortegam/Desktop/NIBSA_proyectos/CltNicaraguaBinary/sys-auframework/sys-devtools-src/include/");
		SrcAnlzCodeCParser_addIncludePathSys(&sntxParser, "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/");
		//Predefined MACROS
		SrcAnlzParser_processLines(&parser, "\
								   typedef void* __builtin_va_list;\n"
"								   #define OBJC_NEW_PROPERTIES 1\n"
"								   #define _LP64 1\n"
"								   #define __APPLE_CC__ 6000\n"
"								   #define __APPLE__ 1\n"
"								   #define __ATOMIC_ACQUIRE 2\n"
"								   #define __ATOMIC_ACQ_REL 4\n"
"								   #define __ATOMIC_CONSUME 1\n"
"								   #define __ATOMIC_RELAXED 0\n"
"								   #define __ATOMIC_RELEASE 3\n"
"								   #define __ATOMIC_SEQ_CST 5\n"
"								   #define __BIGGEST_ALIGNMENT__ 16\n"
"								   #define __BLOCKS__ 1\n"
"								   #define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__\n"
"								   #define __CHAR16_TYPE__ unsigned short\n"
"								   #define __CHAR32_TYPE__ unsigned int\n"
"								   #define __CHAR_BIT__ 8\n"
"								   #define __CONSTANT_CFSTRINGS__ 1\n"
"								   #define __DBL_DECIMAL_DIG__ 17\n"
"								   #define __DBL_DENORM_MIN__ 4.9406564584124654e-324\n"
"								   #define __DBL_DIG__ 15\n"
"								   #define __DBL_EPSILON__ 2.2204460492503131e-16\n"
"								   #define __DBL_HAS_DENORM__ 1\n"
"								   #define __DBL_HAS_INFINITY__ 1\n"
"								   #define __DBL_HAS_QUIET_NAN__ 1\n"
"								   #define __DBL_MANT_DIG__ 53\n"
"								   #define __DBL_MAX_10_EXP__ 308\n"
"								   #define __DBL_MAX_EXP__ 1024\n"
"								   #define __DBL_MAX__ 1.7976931348623157e+308\n"
"								   #define __DBL_MIN_10_EXP__ (-307)\n"
"								   #define __DBL_MIN_EXP__ (-1021)\n"
"								   #define __DBL_MIN__ 2.2250738585072014e-308\n"
"								   #define __DECIMAL_DIG__ __LDBL_DECIMAL_DIG__\n"
"								   #define __DYNAMIC__ 1\n"
"								   #define __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ 101200\n"
"								   #define __FINITE_MATH_ONLY__ 0\n"
"								   #define __FLT_DECIMAL_DIG__ 9\n"
"								   #define __FLT_DENORM_MIN__ 1.40129846e-45F\n"
"								   #define __FLT_DIG__ 6\n"
"								   #define __FLT_EPSILON__ 1.19209290e-7F\n"
"								   #define __FLT_EVAL_METHOD__ 0\n"
"								   #define __FLT_HAS_DENORM__ 1\n"
"								   #define __FLT_HAS_INFINITY__ 1\n"
"								   #define __FLT_HAS_QUIET_NAN__ 1\n"
"								   #define __FLT_MANT_DIG__ 24\n"
"								   #define __FLT_MAX_10_EXP__ 38\n"
"								   #define __FLT_MAX_EXP__ 128\n"
"								   #define __FLT_MAX__ 3.40282347e+38F\n"
"								   #define __FLT_MIN_10_EXP__ (-37)\n"
"								   #define __FLT_MIN_EXP__ (-125)\n"
"								   #define __FLT_MIN__ 1.17549435e-38F\n"
"								   #define __FLT_RADIX__ 2\n"
"								   #define __FXSR__ 1\n"
"								   #define __GCC_ATOMIC_BOOL_LOCK_FREE 2\n"
"								   #define __GCC_ATOMIC_CHAR16_T_LOCK_FREE 2\n"
"								   #define __GCC_ATOMIC_CHAR32_T_LOCK_FREE 2\n"
"								   #define __GCC_ATOMIC_CHAR_LOCK_FREE 2\n"
"								   #define __GCC_ATOMIC_INT_LOCK_FREE 2\n"
"								   #define __GCC_ATOMIC_LLONG_LOCK_FREE 2\n"
"								   #define __GCC_ATOMIC_LONG_LOCK_FREE 2\n"
"								   #define __GCC_ATOMIC_POINTER_LOCK_FREE 2\n"
"								   #define __GCC_ATOMIC_SHORT_LOCK_FREE 2\n"
"								   #define __GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1\n"
"								   #define __GCC_ATOMIC_WCHAR_T_LOCK_FREE 2\n"
"								   #define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1\n"
"								   #define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16 1\n"
"								   #define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1\n"
"								   #define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1\n"
"								   #define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 1\n"
"								   #define __GNUC_MINOR__ 2\n"
"								   #define __GNUC_PATCHLEVEL__ 1\n"
"								   #define __GNUC_STDC_INLINE__ 1\n"
"								   #define __GNUC__ 4\n"
"								   #define __GXX_ABI_VERSION 1002\n"
"								   #define __INT16_C_SUFFIX__ \n"
"								   #define __INT16_FMTd__ \"hd\"\n"
"								   #define __INT16_FMTi__ \"hi\"\n"
"								   #define __INT16_MAX__ 32767\n"
"								   #define __INT16_TYPE__ short\n"
"								   #define __INT32_C_SUFFIX__ \n"
"								   #define __INT32_FMTd__ \"d\"\n"
"								   #define __INT32_FMTi__ \"i\"\n"
"								   #define __INT32_MAX__ 2147483647\n"
"								   #define __INT32_TYPE__ int\n"
"								   #define __INT64_C_SUFFIX__ LL\n"
"								   #define __INT64_FMTd__ \"lld\"\n"
"								   #define __INT64_FMTi__ \"lli\"\n"
"								   #define __INT64_MAX__ 9223372036854775807LL\n"
"								   #define __INT64_TYPE__ long long int\n"
"								   #define __INT8_C_SUFFIX__ \n"
"								   #define __INT8_FMTd__ \"hhd\"\n"
"								   #define __INT8_FMTi__ \"hhi\"\n"
"								   #define __INT8_MAX__ 127\n"
"								   #define __INT8_TYPE__ signed char\n"
"								   #define __INTMAX_C_SUFFIX__ L\n"
"								   #define __INTMAX_FMTd__ \"ld\"\n"
"								   #define __INTMAX_FMTi__ \"li\"\n"
"								   #define __INTMAX_MAX__ 9223372036854775807L\n"
"								   #define __INTMAX_TYPE__ long int\n"
"								   #define __INTMAX_WIDTH__ 64\n"
"								   #define __INTPTR_FMTd__ \"ld\"\n"
"								   #define __INTPTR_FMTi__ \"li\"\n"
"								   #define __INTPTR_MAX__ 9223372036854775807L\n"
"								   #define __INTPTR_TYPE__ long int\n"
"								   #define __INTPTR_WIDTH__ 64\n"
"								   #define __INT_FAST16_FMTd__ \"hd\"\n"
"								   #define __INT_FAST16_FMTi__ \"hi\"\n"
"								   #define __INT_FAST16_MAX__ 32767\n"
"								   #define __INT_FAST16_TYPE__ short\n"
"								   #define __INT_FAST32_FMTd__ \"d\"\n"
"								   #define __INT_FAST32_FMTi__ \"i\"\n"
"								   #define __INT_FAST32_MAX__ 2147483647\n"
"								   #define __INT_FAST32_TYPE__ int\n"
"								   #define __INT_FAST64_FMTd__ \"ld\"\n"
"								   #define __INT_FAST64_FMTi__ \"li\"\n"
"								   #define __INT_FAST64_MAX__ 9223372036854775807L\n"
"								   #define __INT_FAST64_TYPE__ long int\n"
"								   #define __INT_FAST8_FMTd__ \"hhd\"\n"
"								   #define __INT_FAST8_FMTi__ \"hhi\"\n"
"								   #define __INT_FAST8_MAX__ 127\n"
"								   #define __INT_FAST8_TYPE__ signed char\n"
"								   #define __INT_LEAST16_FMTd__ \"hd\"\n"
"								   #define __INT_LEAST16_FMTi__ \"hi\"\n"
"								   #define __INT_LEAST16_MAX__ 32767\n"
"								   #define __INT_LEAST16_TYPE__ short\n"
"								   #define __INT_LEAST32_FMTd__ \"d\"\n"
"								   #define __INT_LEAST32_FMTi__ \"i\"\n"
"								   #define __INT_LEAST32_MAX__ 2147483647\n"
"								   #define __INT_LEAST32_TYPE__ int\n"
"								   #define __INT_LEAST64_FMTd__ \"ld\"\n"
"								   #define __INT_LEAST64_FMTi__ \"li\"\n"
"								   #define __INT_LEAST64_MAX__ 9223372036854775807L\n"
"								   #define __INT_LEAST64_TYPE__ long int\n"
"								   #define __INT_LEAST8_FMTd__ \"hhd\"\n"
"								   #define __INT_LEAST8_FMTi__ \"hhi\"\n"
"								   #define __INT_LEAST8_MAX__ 127\n"
"								   #define __INT_LEAST8_TYPE__ signed char\n"
"								   #define __INT_MAX__ 2147483647\n"
"								   #define __LDBL_DECIMAL_DIG__ 21\n"
"								   #define __LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L\n"
"								   #define __LDBL_DIG__ 18\n"
"								   #define __LDBL_EPSILON__ 1.08420217248550443401e-19L\n"
"								   #define __LDBL_HAS_DENORM__ 1\n"
"								   #define __LDBL_HAS_INFINITY__ 1\n"
"								   #define __LDBL_HAS_QUIET_NAN__ 1\n"
"								   #define __LDBL_MANT_DIG__ 64\n"
"								   #define __LDBL_MAX_10_EXP__ 4932\n"
"								   #define __LDBL_MAX_EXP__ 16384\n"
"								   #define __LDBL_MAX__ 1.18973149535723176502e+4932L\n"
"								   #define __LDBL_MIN_10_EXP__ (-4931)\n"
"								   #define __LDBL_MIN_EXP__ (-16381)\n"
"								   #define __LDBL_MIN__ 3.36210314311209350626e-4932L\n"
"								   #define __LITTLE_ENDIAN__ 1\n"
"								   #define __LONG_LONG_MAX__ 9223372036854775807LL\n"
"								   #define __LONG_MAX__ 9223372036854775807L\n"
"								   #define __LP64__ 1\n"
"								   #define __MACH__ 1\n"
"								   #define __MMX__ 1\n"
"								   #define __NO_INLINE__ 1\n"
"								   #define __NO_MATH_INLINES 1\n"
"								   #define __OBJC_BOOL_IS_BOOL 0\n"
"								   #define __ORDER_BIG_ENDIAN__ 4321\n"
"								   #define __ORDER_LITTLE_ENDIAN__ 1234\n"
"								   #define __ORDER_PDP_ENDIAN__ 3412\n"
"								   #define __PIC__ 2\n"
"								   #define __POINTER_WIDTH__ 64\n"
"								   #define __PRAGMA_REDEFINE_EXTNAME 1\n"
"								   #define __PTRDIFF_FMTd__ \"ld\"\n"
"								   #define __PTRDIFF_FMTi__ \"li\"\n"
"								   #define __PTRDIFF_MAX__ 9223372036854775807L\n"
"								   #define __PTRDIFF_TYPE__ long int\n"
"								   #define __PTRDIFF_WIDTH__ 64\n"
"								   #define __REGISTER_PREFIX__ \n"
"								   #define __SCHAR_MAX__ 127\n"
"								   #define __SHRT_MAX__ 32767\n"
"								   #define __SIG_ATOMIC_MAX__ 2147483647\n"
"								   #define __SIG_ATOMIC_WIDTH__ 32\n"
"								   #define __SIZEOF_DOUBLE__ 8\n"
"								   #define __SIZEOF_FLOAT__ 4\n"
"								   #define __SIZEOF_INT128__ 16\n"
"								   #define __SIZEOF_INT__ 4\n"
"								   #define __SIZEOF_LONG_DOUBLE__ 16\n"
"								   #define __SIZEOF_LONG_LONG__ 8\n"
"								   #define __SIZEOF_LONG__ 8\n"
"								   #define __SIZEOF_POINTER__ 8\n"
"								   #define __SIZEOF_PTRDIFF_T__ 8\n"
"								   #define __SIZEOF_SHORT__ 2\n"
"								   #define __SIZEOF_SIZE_T__ 8\n"
"								   #define __SIZEOF_WCHAR_T__ 4\n"
"								   #define __SIZEOF_WINT_T__ 4\n"
"								   #define __SIZE_FMTX__ \"lX\"\n"
"								   #define __SIZE_FMTo__ \"lo\"\n"
"								   #define __SIZE_FMTu__ \"lu\"\n"
"								   #define __SIZE_FMTx__ \"lx\"\n"
"								   #define __SIZE_MAX__ 18446744073709551615UL\n"
"								   #define __SIZE_TYPE__ long unsigned int\n"
"								   #define __SIZE_WIDTH__ 64\n"
"								   #define __SSE2_MATH__ 1\n"
"								   #define __SSE2__ 1\n"
"								   #define __SSE3__ 1\n"
"								   #define __SSE4_1__ 1\n"
"								   #define __SSE_MATH__ 1\n"
"								   #define __SSE__ 1\n"
"								   #define __SSP__ 1\n"
"								   #define __SSSE3__ 1\n"
"								   #define __STDC_HOSTED__ 1\n"
"								   #define __STDC_NO_THREADS__ 1\n"
"								   #define __STDC_UTF_16__ 1\n"
"								   #define __STDC_UTF_32__ 1\n"
"								   #define __STDC_VERSION__ 201112L\n"
"								   #define __STDC__ 1\n"
"								   #define __UINT16_C_SUFFIX__ \n"
"								   #define __UINT16_FMTX__ \"hX\"\n"
"								   #define __UINT16_FMTo__ \"ho\"\n"
"								   #define __UINT16_FMTu__ \"hu\"\n"
"								   #define __UINT16_FMTx__ \"hx\"\n"
"								   #define __UINT16_MAX__ 65535\n"
"								   #define __UINT16_TYPE__ unsigned short\n"
"								   #define __UINT32_C_SUFFIX__ U\n"
"								   #define __UINT32_FMTX__ \"X\"\n"
"								   #define __UINT32_FMTo__ \"o\"\n"
"								   #define __UINT32_FMTu__ \"u\"\n"
"								   #define __UINT32_FMTx__ \"x\"\n"
"								   #define __UINT32_MAX__ 4294967295U\n"
"								   #define __UINT32_TYPE__ unsigned int\n"
"								   #define __UINT64_C_SUFFIX__ ULL\n"
"								   #define __UINT64_FMTX__ \"llX\"\n"
"								   #define __UINT64_FMTo__ \"llo\"\n"
"								   #define __UINT64_FMTu__ \"llu\"\n"
"								   #define __UINT64_FMTx__ \"llx\"\n"
"								   #define __UINT64_MAX__ 18446744073709551615ULL\n"
"								   #define __UINT64_TYPE__ long long unsigned int\n"
"								   #define __UINT8_C_SUFFIX__ \n"
"								   #define __UINT8_FMTX__ \"hhX\"\n"
"								   #define __UINT8_FMTo__ \"hho\"\n"
"								   #define __UINT8_FMTu__ \"hhu\"\n"
"								   #define __UINT8_FMTx__ \"hhx\"\n"
"								   #define __UINT8_MAX__ 255\n"
"								   #define __UINT8_TYPE__ unsigned char\n"
"								   #define __UINTMAX_C_SUFFIX__ UL\n"
"								   #define __UINTMAX_FMTX__ \"lX\"\n"
"								   #define __UINTMAX_FMTo__ \"lo\"\n"
"								   #define __UINTMAX_FMTu__ \"lu\"\n"
"								   #define __UINTMAX_FMTx__ \"lx\"\n"
"								   #define __UINTMAX_MAX__ 18446744073709551615UL\n"
"								   #define __UINTMAX_TYPE__ long unsigned int\n"
"								   #define __UINTMAX_WIDTH__ 64\n"
"								   #define __UINTPTR_FMTX__ \"lX\"\n"
"								   #define __UINTPTR_FMTo__ \"lo\"\n"
"								   #define __UINTPTR_FMTu__ \"lu\"\n"
"								   #define __UINTPTR_FMTx__ \"lx\"\n"
"								   #define __UINTPTR_MAX__ 18446744073709551615UL\n"
"								   #define __UINTPTR_TYPE__ long unsigned int\n"
"								   #define __UINTPTR_WIDTH__ 64\n"
"								   #define __UINT_FAST16_FMTX__ \"hX\"\n"
"								   #define __UINT_FAST16_FMTo__ \"ho\"\n"
"								   #define __UINT_FAST16_FMTu__ \"hu\"\n"
"								   #define __UINT_FAST16_FMTx__ \"hx\"\n"
"								   #define __UINT_FAST16_MAX__ 65535\n"
"								   #define __UINT_FAST16_TYPE__ unsigned short\n"
"								   #define __UINT_FAST32_FMTX__ \"X\"\n"
"								   #define __UINT_FAST32_FMTo__ \"o\"\n"
"								   #define __UINT_FAST32_FMTu__ \"u\"\n"
"								   #define __UINT_FAST32_FMTx__ \"x\"\n"
"								   #define __UINT_FAST32_MAX__ 4294967295U\n"
"								   #define __UINT_FAST32_TYPE__ unsigned int\n"
"								   #define __UINT_FAST64_FMTX__ \"lX\"\n"
"								   #define __UINT_FAST64_FMTo__ \"lo\"\n"
"								   #define __UINT_FAST64_FMTu__ \"lu\"\n"
"								   #define __UINT_FAST64_FMTx__ \"lx\"\n"
"								   #define __UINT_FAST64_MAX__ 18446744073709551615UL\n"
"								   #define __UINT_FAST64_TYPE__ long unsigned int\n"
"								   #define __UINT_FAST8_FMTX__ \"hhX\"\n"
"								   #define __UINT_FAST8_FMTo__ \"hho\"\n"
"								   #define __UINT_FAST8_FMTu__ \"hhu\"\n"
"								   #define __UINT_FAST8_FMTx__ \"hhx\"\n"
"								   #define __UINT_FAST8_MAX__ 255\n"
"								   #define __UINT_FAST8_TYPE__ unsigned char\n"
"								   #define __UINT_LEAST16_FMTX__ \"hX\"\n"
"								   #define __UINT_LEAST16_FMTo__ \"ho\"\n"
"								   #define __UINT_LEAST16_FMTu__ \"hu\"\n"
"								   #define __UINT_LEAST16_FMTx__ \"hx\"\n"
"								   #define __UINT_LEAST16_MAX__ 65535\n"
"								   #define __UINT_LEAST16_TYPE__ unsigned short\n"
"								   #define __UINT_LEAST32_FMTX__ \"X\"\n"
"								   #define __UINT_LEAST32_FMTo__ \"o\"\n"
"								   #define __UINT_LEAST32_FMTu__ \"u\"\n"
"								   #define __UINT_LEAST32_FMTx__ \"x\"\n"
"								   #define __UINT_LEAST32_MAX__ 4294967295U\n"
"								   #define __UINT_LEAST32_TYPE__ unsigned int\n"
"								   #define __UINT_LEAST64_FMTX__ \"lX\"\n"
"								   #define __UINT_LEAST64_FMTo__ \"lo\"\n"
"								   #define __UINT_LEAST64_FMTu__ \"lu\"\n"
"								   #define __UINT_LEAST64_FMTx__ \"lx\"\n"
"								   #define __UINT_LEAST64_MAX__ 18446744073709551615UL\n"
"								   #define __UINT_LEAST64_TYPE__ long unsigned int\n"
"								   #define __UINT_LEAST8_FMTX__ \"hhX\"\n"
"								   #define __UINT_LEAST8_FMTo__ \"hho\"\n"
"								   #define __UINT_LEAST8_FMTu__ \"hhu\"\n"
"								   #define __UINT_LEAST8_FMTx__ \"hhx\"\n"
"								   #define __UINT_LEAST8_MAX__ 255\n"
"								   #define __UINT_LEAST8_TYPE__ unsigned char\n"
"								   #define __USER_LABEL_PREFIX__ _\n"
"								   #define __VERSION__ \"4.2.1 Compatible Apple LLVM 9.0.0 (clang-900.0.39.2)\"\n"
"								   #define __WCHAR_MAX__ 2147483647\n"
"								   #define __WCHAR_TYPE__ int\n"
"								   #define __WCHAR_WIDTH__ 32\n"
"								   #define __WINT_TYPE__ int\n"
"								   #define __WINT_WIDTH__ 32\n"
"								   #define __amd64 1\n"
"								   #define __amd64__ 1\n"
"								   #define __apple_build_version__ 9000039\n"
"								   #define __block __attribute__((__blocks__(byref)))\n"
"								   #define __clang__ 1\n"
"								   #define __clang_major__ 9\n"
"								   #define __clang_minor__ 0\n"
"								   #define __clang_patchlevel__ 0\n"
"								   #define __clang_version__ \"9.0.0 (clang-900.0.39.2)\"\n"
"								   #define __core2 1\n"
"								   #define __core2__ 1\n"
"								   #define __llvm__ 1\n"
"								   #define __nonnull _Nonnull\n"
"								   #define __null_unspecified _Null_unspecified\n"
"								   #define __nullable _Nullable\n"
"								   #define __pic__ 2\n"
"								   #define __strong \n"
"								   #define __tune_core2__ 1\n"
"								   #define __unsafe_unretained \n"
"								   #define __weak __attribute__((objc_gc(weak)))\n"
"								   #define __x86_64 1\n"
"								   #define __x86_64__ 1\n"
"		");
		//
		SrcAnlzParser_processStream(&parser, stream);
		SrcAnlzParser_end(&parser);
		//Print parsed code
		{
			PRINTF_INFO("------------------\n");
			PRINTF_INFO("-- CODE PARSED: --\n");
			PRINTF_INFO("------------------\n");
			SrcAnlzCodeC_print(&dstCode);
			PRINTF_INFO("------------------\n");
		}
		//
		SrcAnlzParser_release(&parser);
		SrcAnlzCodeCParser_release(&sntxParser);
		SrcAnlzCodeC_release(&dstCode);
	}*/
	//CICLOS_CPU_HILO(endTime)
	//PRINTF_INFO("%.4f secs.\n", (float)(endTime - startTime) / (float)ciclesPerSec);
	//
	NBMngrStructMaps_release();
	NBMngrProcess_release();
	//
	return 0;
}


