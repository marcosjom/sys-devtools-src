//
//  NBToolAppIconsGenerator.cpp
//  DevTools
//
//  Created by Nicaragua Binary on 20/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "AUFrameworkBaseStdAfx.h"
#include "NBToolAppIconsGenerator.h"
#include "nb/2d/NBBitmap.h"
#include "nb/2d/NBPng.h"

//Icons scales config

static STNBStructMap* NBToolGenIconSize_sharedStructMap = NULL;

const STNBStructMap* NBToolGenIconSize_getSharedStructMap(void){
	if(NBToolGenIconSize_sharedStructMap == NULL){
		NBToolGenIconSize s;
		STNBStructMap* map = NBMemory_allocType(STNBStructMap);
		NBStructMap_init(map, sizeof(s));
		NBStructMap_addFloatM(map, s, szPoints);
		NBStructMap_addFloatM(map, s, scale);
		NBStructMap_addUIntM(map, s, width);
		NBStructMap_addUIntM(map, s, height);
		NBStructMap_addStrPtrM(map, s, post);
		NBToolGenIconSize_sharedStructMap = map;
	}
	return NBToolGenIconSize_sharedStructMap;
}

static STNBStructMap* NBToolGenIconsJob_sharedStructMap = NULL;

const STNBStructMap* NBToolGenIconsJob_getSharedStructMap(void){
	if(NBToolGenIconsJob_sharedStructMap == NULL){
		NBToolGenIconsJob s;
		STNBStructMap* map = NBMemory_allocType(STNBStructMap);
		NBStructMap_init(map, sizeof(s));
		NBStructMap_addStrPtrM(map, s, name);
		NBStructMap_addBoolM(map, s, enabled);
		NBStructMap_addStrPtrM(map, s, baseIcon);
		NBStructMap_addStrPtrM(map, s, dstFolder);
		NBStructMap_addStrPtrM(map, s, namesPrefix);
		NBStructMap_addPtrToArrayOfStructM(map, s, sizes, sizesSz, ENNBStructMapSign_Unsigned, NBToolGenIconSize_getSharedStructMap());
		NBToolGenIconsJob_sharedStructMap = map;
	}
	return NBToolGenIconsJob_sharedStructMap;
}

static STNBStructMap* NBToolGenIconsJobs_sharedStructMap = NULL;

const STNBStructMap* NBToolGenIconsJobs_getSharedStructMap(void){
	if(NBToolGenIconsJobs_sharedStructMap == NULL){
		NBToolGenIconsJobs s;
		STNBStructMap* map = NBMemory_allocType(STNBStructMap);
		NBStructMap_init(map, sizeof(s));
		NBStructMap_addPtrToArrayOfStructM(map, s, jobs, jobsSz, ENNBStructMapSign_Unsigned, NBToolGenIconsJob_getSharedStructMap());
		NBToolGenIconsJobs_sharedStructMap = map;
	}
	return NBToolGenIconsJobs_sharedStructMap;
}

//

BOOL NBToolAppIconsGenerator::generateAppIcons(const char* basePng, const char* dstFolder, const char* iconsBaseName, const NBToolGenIconSize* sizes, const SI32 sizesLen){
	BOOL r = FALSE;
	STNBBitmap bmp;
	NBBitmap_init(&bmp);
	if(!NBPng_loadFromPath(basePng, FALSE, &bmp, NULL)){
		PRINTF_ERROR("Could not load basePng's header: '%s'.\n", basePng);
	} else {
		const STNBBitmapProps orgProps = NBBitmap_getProps(&bmp);
		if(!NBPng_loadFromPath(basePng, TRUE, &bmp, NULL)){
			PRINTF_ERROR("Could not load basePng's data: '%s'.\n", basePng);
		} else {
			r = TRUE;
			//Verify config
			{
				SI32 i; for(i = 0 ; i < sizesLen; i++){
					const NBToolGenIconSize* cfg = &sizes[i];
					if(cfg->szPoints > 0 && cfg->scale > 0 && orgProps.size.width >= 1024 && orgProps.size.width == orgProps.size.height){
						const float pxSize = (cfg->szPoints * cfg->scale);
						if(pxSize != (float)((SI32)pxSize)){
							PRINTF_ERROR("szPoints(%f) x scale(%f) must result not-decimal: %f.\n", cfg->szPoints, cfg->scale, pxSize);
							r = FALSE;
							break;
						}
					} else if(cfg->width > 0 && cfg->height > 0){
						//
					}
				}
			}
			//Execute
			if(r){
				STNBString strPath;
				NBString_init(&strPath);
				SI32 i; for(i = 0 ; i < sizesLen && r; i++){
					const NBToolGenIconSize* cfg = &sizes[i];
					if(cfg->szPoints > 0 && cfg->scale > 0 && orgProps.size.width >= 1024 && orgProps.size.width == orgProps.size.height){
						//
						//Scale by Apple format
						//
						const float pxSize = (cfg->szPoints * cfg->scale);
						NBASSERT(pxSize == (float)((SI32)pxSize));
						//Build filepath
						{
							NBString_set(&strPath, dstFolder);
							if(strPath.length > 0){
								if(strPath.str[strPath.length - 1] != '/' && strPath.str[strPath.length - 1] != '\\'){
									NBString_concatByte(&strPath, '/');
								}
							}
							NBString_concat(&strPath, iconsBaseName);
							NBString_concatByte(&strPath, '_');
							NBString_concatFloat(&strPath, cfg->szPoints, (cfg->szPoints == (float)((SI32)cfg->szPoints) ? 0 : 2));
							NBString_concat(&strPath, "pt_");
							NBString_concatFloat(&strPath, cfg->scale, (cfg->scale == (float)((SI32)cfg->scale) ? 0 : 2));
							NBString_concat(&strPath, "x_");
							NBString_concat(&strPath, cfg->post);
							NBString_concat(&strPath, ".png");
						}
						//Scale image
						{
							STNBBitmap bmp2;
							NBBitmap_init(&bmp2);
							if(!NBBitmap_create(&bmp2, pxSize, pxSize, ENNBBitmapColor_RGBA8)){
								PRINTF_ERROR("Could not create resized-bitmap: '%s' (%f px).\n", basePng, pxSize);
								r = FALSE;
							} else if(!NBBitmap_clearWithColor(&bmp2, NBST_P(STNBColor8, 255, 255, 255, 0))){
								PRINTF_ERROR("Could not clear resized-bitmap: '%s' (%f px).\n", basePng, pxSize);
								r = FALSE;
							} else {
								if(orgProps.size.width == orgProps.size.height && orgProps.size.width == pxSize){
									if(!NBBitmap_pasteBitmap(&bmp2, NBST_P(STNBPointI, 0, 0), &bmp, NBST_P(STNBColor8, 255, 255, 255, 255))){
										PRINTF_ERROR("Could not paste orig-bitmap: '%s' (%f px).\n", basePng, pxSize);
										r = FALSE;
									}
								} else {
									if(!NBBitmap_drawBitmapSmoothScaledRect(&bmp2, NBST_P(STNBRectI, 0, 0, (SI32)pxSize, (SI32)pxSize), &bmp, NBST_P(STNBRectI, 0, 0, orgProps.size.width, orgProps.size.height), NBST_P(STNBColor8, 255, 255, 255, 255))){
										PRINTF_ERROR("Could not drawBitmap orig-bitmap: '%s' (%f px).\n", basePng, pxSize);
										r = FALSE;
									}
								}
								if(r){
									if(!NBPng_saveToPath(&bmp2, strPath.str, ENPngCompressLvl_9)){
										PRINTF_ERROR("Could not save resized-bitmap to path: '%s' (%f px, compression %d).\n", strPath.str, pxSize, (SI32)ENPngCompressLvl_9);
										r = FALSE;
									} else {
										PRINTF_INFO("Resized-icon saved: '%s' (%f px, compression %d).\n", strPath.str, pxSize, (SI32)ENPngCompressLvl_9)
									}
								}
							}
							NBBitmap_release(&bmp2);
						}
					} else if(cfg->width > 0 && cfg->height > 0){
						//
						//Scale by fixed size
						//
						//Build filepath
						{
							NBString_set(&strPath, dstFolder);
							if(strPath.length > 0){
								if(strPath.str[strPath.length - 1] != '/' && strPath.str[strPath.length - 1] != '\\'){
									NBString_concatByte(&strPath, '/');
								}
							}
							NBString_concat(&strPath, iconsBaseName);
							NBString_concat(&strPath, cfg->post);
							NBString_concat(&strPath, ".png");
						}
						//Scale image
						{
							STNBBitmap bmp2;
							NBBitmap_init(&bmp2);
							if(!NBBitmap_create(&bmp2, cfg->width, cfg->height, ENNBBitmapColor_RGBA8)){
								PRINTF_ERROR("Could not create resized-bitmap: '%s' (%d x %dpx).\n", basePng, cfg->width, cfg->height);
								r = FALSE;
							} else if(!NBBitmap_clearWithColor(&bmp2, NBST_P(STNBColor8, 255, 255, 255, 0))){
								PRINTF_ERROR("Could not clear resized-bitmap: '%s' (%d x %dpx).\n", basePng, cfg->width, cfg->height);
								r = FALSE;
							} else {
								if(orgProps.size.width == orgProps.size.height && orgProps.size.width == cfg->width && orgProps.size.height == cfg->height){
									if(!NBBitmap_pasteBitmap(&bmp2, NBST_P(STNBPointI, 0, 0), &bmp, NBST_P(STNBColor8, 255, 255, 255, 255))){
										PRINTF_ERROR("Could not paste orig-bitmap: '%s' (%d x %dpx).\n", basePng, cfg->width, cfg->height);
										r = FALSE;
									}
								} else {
									if(!NBBitmap_drawBitmapSmoothScaledRect(&bmp2, NBST_P(STNBRectI, 0, 0, (SI32)cfg->width, (SI32)cfg->height), &bmp, NBST_P(STNBRectI, 0, 0, orgProps.size.width, orgProps.size.height), NBST_P(STNBColor8, 255, 255, 255, 255))){
										PRINTF_ERROR("Could not drawBitmap orig-bitmap: '%s' (%d x %dpx).\n", basePng, cfg->width, cfg->height);
										r = FALSE;
									}
								}
								if(r){
									if(!NBPng_saveToPath(&bmp2, strPath.str, ENPngCompressLvl_9)){
										PRINTF_ERROR("Could not save resized-bitmap to path: '%s' (%d x %dpx, compression %d).\n", strPath.str, cfg->width, cfg->height, (SI32)ENPngCompressLvl_9);
										r = FALSE;
									} else {
										PRINTF_INFO("Resized-icon saved: '%s' (%d x %dpx, compression %d).\n", strPath.str, cfg->width, cfg->height, (SI32)ENPngCompressLvl_9)
									}
								}
							}
							NBBitmap_release(&bmp2);
						}
					} else {
						PRINTF_INFO("Ignored-icon #%d, no valid square-1024+scale nor explicit-size configurations.\n", (i + 1));
					}
				}
				NBString_release(&strPath);
			}
		}
	}
	NBBitmap_release(&bmp);
	return r;
}

