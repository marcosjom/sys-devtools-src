//
//  NBToolLaunchscreensGenerator.cpp
//  DevTools
//
//  Created by Nicaragua Binary on 20/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "AUFrameworkBaseStdAfx.h"
#include "NBToolLaunchscreensGenerator.h"
#include "nb/2d/NBBitmap.h"
#include "nb/2d/NBPng.h"

//Launchscreen config

static STNBStructMap* NBToolGenLaunchSize_sharedStructMap = NULL;

const STNBStructMap* NBToolGenLaunchSize_getSharedStructMap(void){
	if(NBToolGenLaunchSize_sharedStructMap == NULL){
		NBToolGenLaunchSize s;
		STNBStructMap* map = NBMemory_allocType(STNBStructMap);
		NBStructMap_init(map, sizeof(s));
		NBStructMap_addUIntM(map, s, w);
		NBStructMap_addUIntM(map, s, h);
		NBStructMap_addStrPtrM(map, s, pre);
		NBStructMap_addStrPtrM(map, s, post);
		NBToolGenLaunchSize_sharedStructMap = map;
	}
	return NBToolGenLaunchSize_sharedStructMap;
}

static STNBStructMap* NBToolGenLaunchJob_sharedStructMap = NULL;

const STNBStructMap* NBToolGenLaunchJob_getSharedStructMap(void){
	if(NBToolGenLaunchJob_sharedStructMap == NULL){
		NBToolGenLaunchJob s;
		STNBStructMap* map = NBMemory_allocType(STNBStructMap);
		NBStructMap_init(map, sizeof(s));
		NBStructMap_addStrPtrM(map, s, name);
		NBStructMap_addBoolM(map, s, enabled);
		NBStructMap_addStrPtrM(map, s, dstFolder);
		NBStructMap_addStrPtrM(map, s, namesPrefix);
		NBStructMap_addStructM(map, s, bgColor, NBColor8_getSharedStructMap());
		NBStructMap_addStructM(map, s, imgRelPos, NBPoint_getSharedStructMap());
		NBStructMap_addStructM(map, s, imgRelSz, NBSize_getSharedStructMap());
		NBStructMap_addPtrToArrayOfStrPtrM(map, s, imgVers, imgVersSz, ENNBStructMapSign_Unsigned);
		NBStructMap_addPtrToArrayOfStructM(map, s, sizes, sizesSz, ENNBStructMapSign_Unsigned, NBToolGenLaunchSize_getSharedStructMap());
		NBToolGenLaunchJob_sharedStructMap = map;
	}
	return NBToolGenLaunchJob_sharedStructMap;
}

static STNBStructMap* NBToolGenLaunchJobs_sharedStructMap = NULL;

const STNBStructMap* NBToolGenLaunchJobs_getSharedStructMap(void){
	if(NBToolGenLaunchJobs_sharedStructMap == NULL){
		NBToolGenLaunchJobs s;
		STNBStructMap* map = NBMemory_allocType(STNBStructMap);
		NBStructMap_init(map, sizeof(s));
		NBStructMap_addPtrToArrayOfStructM(map, s, jobs, jobsSz, ENNBStructMapSign_Unsigned, NBToolGenLaunchJob_getSharedStructMap());
		NBToolGenLaunchJobs_sharedStructMap = map;
	}
	return NBToolGenLaunchJobs_sharedStructMap;
}

//

BOOL NBToolLaunchscreensGenerator::generateLaunchscreens(const NBToolGenLaunchJob* job){
	BOOL r = FALSE;
	if(job->imgVersSz > 0){
		STNBSizeI* imgsSizes = NBMemory_allocTypes(STNBSizeI, job->imgVersSz);
		STNBBitmap** imgsPtrs = NBMemory_allocTypes(STNBBitmap*, job->imgVersSz);
		NBMemory_set(imgsSizes, 0, sizeof(STNBSizeI) * job->imgVersSz);
		NBMemory_set(imgsPtrs, 0, sizeof(STNBBitmap*) * job->imgVersSz);
		//Preload img-versions sizes
		{
			r = TRUE;
			SI32 i; for(i = 0 ; i < job->imgVersSz; i++){
				STNBBitmap bmp;
				NBBitmap_init(&bmp);
				if(!NBPng_loadFromPath(job->imgVers[i], FALSE, &bmp, NULL)){
					PRINTF_ERROR("Could not load imgVer's header: '%s'.\n", job->imgVers[i]);
					r = FALSE;
				} else {
					const STNBBitmapProps props = NBBitmap_getProps(&bmp);
					imgsSizes[i] = props.size;
					//PRINTF_INFO("ImgVer is %dx%d: '%s'.\n", props.size.width, props.size.height, job->imgVers[i]);
				}
				NBBitmap_release(&bmp);
			}
		}
		//Process sizes
		if(r){
			STNBString strPath;
			NBString_init(&strPath);
			SI32 i; for(i = 0; i < job->sizesSz && r; i++){
				const NBToolGenLaunchSize* sz = &job->sizes[i];
				//Determine best imgVer
				UI32 iImgVer = 0;
				{
					SI32 i; for(i = 1; i < job->imgVersSz; i++){
						const STNBSizeI* curImgSz = &imgsSizes[iImgVer];
						const STNBSizeI* imgSz = &imgsSizes[i];
						if(imgSz->width > 0 && imgSz->height > 0 && imgSz->width <= (job->imgRelSz.width * sz->w) && imgSz->height <= (job->imgRelSz.height * sz->h) && ((curImgSz->width * curImgSz->height) <= (imgSz->width * imgSz->height))){
							iImgVer = i;
						}
					}
				}
				//Load imgVer
				STNBBitmap* imgBmp = imgsPtrs[iImgVer];
				if(imgBmp == NULL){
					imgBmp = NBMemory_allocType(STNBBitmap);
					NBBitmap_init(imgBmp);
					if(!NBPng_loadFromPath(job->imgVers[iImgVer], TRUE, imgBmp, NULL)){
						PRINTF_ERROR("Could not load imgVer's data: '%s'.\n", job->imgVers[iImgVer]);
						NBBitmap_release(imgBmp);
						NBMemory_free(imgBmp);
						imgBmp = NULL;
						r = FALSE;
					} else {
						NBASSERT(imgsSizes[iImgVer].width == NBBitmap_getProps(imgBmp).size.width && imgsSizes[iImgVer].height == NBBitmap_getProps(imgBmp).size.height)
						imgsPtrs[iImgVer] = imgBmp;
					}
				}
				//Build launchscreen
				if(imgBmp == NULL){
					PRINTF_ERROR("No imgVer's data loaded: '%s'.\n", job->imgVers[iImgVer]);
					r = FALSE;
				} else {
					//Build filepath
					{
						NBString_set(&strPath, job->dstFolder);
						if(strPath.length > 0){
							if(strPath.str[strPath.length - 1] != '/' && strPath.str[strPath.length - 1] != '\\'){
								NBString_concatByte(&strPath, '/');
							}
						}
						NBString_concat(&strPath, job->namesPrefix);
						if(sz->pre != NULL){
							if(sz->pre[0] != '\0'){
								NBString_concat(&strPath, "_");
								NBString_concat(&strPath, sz->pre);
							}
						}
						NBString_concat(&strPath, "_");
						NBString_concatUI32(&strPath, sz->w);
						NBString_concat(&strPath, "x");
						NBString_concatUI32(&strPath, sz->h);
						if(sz->post != NULL){
							if(sz->post[0] != '\0'){
								NBString_concat(&strPath, "_");
								NBString_concat(&strPath, sz->post);
							}
						}
						NBString_concat(&strPath, ".png");
					}
					//Build image
					{
						STNBBitmap bmp2;
						NBBitmap_init(&bmp2);
						if(!NBBitmap_create(&bmp2, sz->w, sz->h, ENNBBitmapColor_RGBA8)){
							PRINTF_ERROR("Could not create bitmap: %dx%d, '%s'.\n", sz->w, sz->h, sz->post);
							r = FALSE;
						} else if(!NBBitmap_clearWithColor(&bmp2, job->bgColor)){
							PRINTF_ERROR("Could not clear bitmap: %dx%d, '%s'.\n", sz->w, sz->h, sz->post);
							r = FALSE;
						} else {
							const STNBSizeI* imgSz = &imgsSizes[iImgVer];
							STNBPointI pos;
							pos.x = (sz->w - imgSz->width) * job->imgRelPos.x;
							pos.y = (sz->h - imgSz->height) * job->imgRelPos.y;
							if(!NBBitmap_pasteBitmap(&bmp2, pos, imgBmp, NBST_P(STNBColor8, 255, 255, 255, 255))){
								PRINTF_ERROR("Could not paste img: %dx%d, '%s'.\n", sz->w, sz->h, sz->post);
								r = FALSE;
							}
							if(r){
								if(!NBPng_saveToPath(&bmp2, strPath.str, ENPngCompressLvl_9)){
									PRINTF_ERROR("Could not save bitmap to path: %dx%d, '%s' (compression %d).\n", sz->w, sz->h, sz->post, (SI32)ENPngCompressLvl_9);
									r = FALSE;
								} else {
									PRINTF_INFO("Bitmap saved: %dx%d, '%s' (compression %d).\n", sz->w, sz->h, sz->post, (SI32)ENPngCompressLvl_9)
								}
							}
						}
						NBBitmap_release(&bmp2);
					}
				}
			}
			NBString_release(&strPath);
		}
		//Release loadad imgs
		NBMemory_free(imgsSizes);
		{
			SI32 i; for(i = 0 ; i < job->imgVersSz; i++){
				STNBBitmap* bmp = imgsPtrs[i];
				if(bmp != NULL){
					NBBitmap_release(bmp);
					NBMemory_free(bmp);
					imgsPtrs[i] = NULL;
				}
			}
			NBMemory_free(imgsPtrs);
		}
		
	}
	//
	return r;
}

