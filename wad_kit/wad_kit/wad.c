//
//  wad.c
//  wad_kit
//
//  Created by Евгений Лютц on 31.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

MESH* _wad_findMesh(unsigned short pointerListStart, unsigned short index, WK_WAD_LOAD_INFO* loadInfo)
{
	if (pointerListStart + index >= loadInfo->numMeshPointers)
	{
		executeResultSetFailed(loadInfo->executeResult, "Cannot find mesh. Invalid index to data is given.");
		return NULL;
	}
	
	unsigned int value = loadInfo->meshPointers[pointerListStart + index];
	
	for (unsigned int i = 0; i < loadInfo->numMeshDataOffsets; i++)
	{
		if (loadInfo->meshDataOffsets[i] == value)
		{
			return wadGetMesh(loadInfo->wad, i);
		}
	}
	
	executeResultSetFailed(loadInfo->executeResult, "Cannot find mesh.");
	return NULL;
}

// MARK: - Public interface

WK_WAD* wadCreate(void)
{
	WK_WAD* wad = malloc(sizeof(WK_WAD));
	memset(wad, 0, sizeof(WK_WAD));
	wad->version = 129;
	
	wad->rawVertexAllocator = dataAllocatorCreate(sizeof(VERTEX), 4096);
	wad->rawPolygonAllocator = dataAllocatorCreate(sizeof(POLYGON), 512);
	wad->animationAllocator = dataAllocatorCreate(sizeof(ANIMATION), 512);
	wad->keyframeAllocator = dataAllocatorCreate(sizeof(KEYFRAME), 4096);
	
	magicArrayInitialize(&wad->texturePages, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(WK_TEXTURE_PAGE), 16);
	magicArrayInitialize(&wad->textureSamples, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(WK_TEXTURE_SAMPLE), 256);
	magicArrayInitialize(&wad->meshes, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(MESH), 128);
	magicArrayInitialize(&wad->movables, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(MOVABLE), 32);
	magicArrayInitialize(&wad->statics, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(STATIC), 32);
	
	return wad;
}

void wadRelease(WK_WAD* wad)
{
	for (unsigned int i = 0; i < wad->statics.length; i++)
	{
		STATIC* staticItem = magicArrayGetItem(&wad->statics, i);
		staticDeinitialize(staticItem);
	}
	magicArrayDeinitialize(&wad->statics);
	
	for (unsigned int i = 0; i < wad->movables.length; i++)
	{
		MOVABLE* movable = magicArrayGetItem(&wad->movables, i);
		movableDeinitialize(movable);
	}
	magicArrayDeinitialize(&wad->movables);
	
	for (unsigned int i = 0; i < wad->meshes.length; i++)
	{
		MESH* mesh = magicArrayGetItem(&wad->meshes, i);
		meshDeinitialize(mesh);
	}
	magicArrayDeinitialize(&wad->meshes);
	
	for (unsigned int i = 0; i < wad->textureSamples.length; i++)
	{
		WK_TEXTURE_SAMPLE* textureSample = magicArrayGetItem(&wad->textureSamples, i);
		textureSampleDeinitialize(textureSample);
	}
	magicArrayDeinitialize(&wad->textureSamples);
	
	for (unsigned int i = 0; i < wad->texturePages.length; i++)
	{
		WK_TEXTURE_PAGE* texturePage = magicArrayGetItem(&wad->texturePages, i);
		texturePageDeinitialize(texturePage);
	}
	magicArrayDeinitialize(&wad->texturePages);
	
	dataAllocatorRelease(wad->keyframeAllocator);
	dataAllocatorRelease(wad->animationAllocator);
	dataAllocatorRelease(wad->rawPolygonAllocator);
	dataAllocatorRelease(wad->rawVertexAllocator);

	debug_memset(wad, 0, sizeof(WK_WAD));
	free(wad);
}

unsigned int wadGetVersion(WK_WAD* wad)
{
	assert(wad);
	return wad->version;
}


unsigned int wadGetNumTexturePages(WK_WAD* wad)
{
	assert(wad);
	return wad->texturePages.length;
}

WK_TEXTURE_PAGE* wadGetTexturePage(WK_WAD* wad, unsigned int texturePageIndex)
{
	assert(wad);
	return magicArrayGetItem(&wad->texturePages, texturePageIndex);
}

unsigned char* wadGetTexturePageData(WK_WAD* wad, unsigned int texturePageIndex)
{
	assert(wad);
	WK_TEXTURE_PAGE* texturePage = wadGetTexturePage(wad, texturePageIndex);
	return texturePage->data;
}


unsigned int wadGetNumMeshes(WK_WAD* wad)
{
	assert(wad);
	return wad->meshes.length;
}

MESH* wadGetMesh(WK_WAD* wad, unsigned int meshIndex)
{
	assert(wad);
	return magicArrayGetItem(&wad->meshes, meshIndex);
}


unsigned int wadGetNumMovables(WK_WAD* wad)
{
	assert(wad);
	return wad->movables.length;
}

MOVABLE* wadGetMovableByIndex(WK_WAD* wad, unsigned int movableIndex)
{
	assert(wad);
	return magicArrayGetItem(&wad->movables, movableIndex);
}

MOVABLE* wadGetMovableById(WK_WAD* wad, MOVABLE_ID movableId)
{
	assert(wad);
	
	MOVABLE* movable = NULL;
	for (unsigned int i = 0; i < wad->movables.length; i++)
	{
		MOVABLE* currentMovable = magicArrayGetItem(&wad->movables, i);
		if (currentMovable->movableId == movableId)
		{
			movable = currentMovable;
			break;
		}
	}
	
	return movable;
}


unsigned int wadGetNumStatics(WK_WAD* wad)
{
	assert(wad);
	return wad->statics.length;
}

STATIC* wadGetStaticByIndex(WK_WAD* wad, unsigned int staticIndex)
{
	assert(wad);
	return magicArrayGetItem(&wad->statics, staticIndex);
}

STATIC* wadGetStaticById(WK_WAD* wad, STATIC_ID staticId)
{
	assert(wad);
	
	STATIC* staticObject = NULL;
	for (unsigned int i = 0; i < wad->statics.length; i++)
	{
		STATIC* currentStatic = magicArrayGetItem(&wad->statics, i);
		if (currentStatic->staticId == staticId)
		{
			staticObject = currentStatic;
			break;
		}
	}
	
	return staticObject;
}
