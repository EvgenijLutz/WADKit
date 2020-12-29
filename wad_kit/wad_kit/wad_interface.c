//
//  wad_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"


WK_WAD* wadCreate(void)
{
	WK_WAD* wad = malloc(sizeof(WK_WAD));
	memset(wad, 0, sizeof(WK_WAD));
	wad->version = 129;
	
	arrayInitializeWithCapacityIncrement(&wad->texturePages, sizeof(TEXTURE_PAGE), 16);
	arrayInitializeWithCapacityIncrement(&wad->textureSamples, sizeof(TEXTURE_SAMPLE), 256);
	arrayInitializeWithCapacityIncrement(&wad->meshes, sizeof(MESH), 128);
	arrayInitializeWithCapacityIncrement(&wad->skeletons, sizeof(SKELETON), 64);
	
	return wad;
}

void wadRelease(WK_WAD* wad)
{
	if (wad->statics)
	{
		free(wad->statics);
	}
	
	if (wad->movables)
	{
		for (unsigned int movableIndex = 0; movableIndex < wad->numMovables; movableIndex++)
		{
			MOVABLE* movable = &(wad->movables[movableIndex]);
			if (movable->animations)
			{
				for (unsigned int animIndex = 0; animIndex < movable->numAnimations; animIndex++)
				{
					ANIMATION* animation = &(movable->animations[animIndex]);
					if (animation->commands)
					{
						free(animation->commands);
					}
					
					if (animation->stateChanges)
					{
						for (unsigned int stateChangeIndex = 0; stateChangeIndex < animation->numStateChanges; stateChangeIndex++)
						{
							STATE_CHANGE* stateChange = &(animation->stateChanges[stateChangeIndex]);
							free(stateChange->dispatches);
						}
						free(animation->stateChanges);
					}
					
					if (animation->keyframes)
					{
						for (unsigned int keyframeIndex = 0; keyframeIndex < animation->numKeyframes; keyframeIndex++)
						{
							free(animation->keyframes[keyframeIndex].rotations);
						}
						free(animation->keyframes);
					}
				}
				free(movable->animations);
			}
			free(movable->meshIndices);
		}
		free(wad->movables);
	}
	
	for (unsigned int i = 0; i < wad->skeletons.length; i++)
	{
		SKELETON* skeleton = arrayGetItem(&wad->skeletons, i);
		skeletonDeinitialize(skeleton);
	}
	arrayDeinitialize(&wad->skeletons);
	
	for (unsigned int i = 0; i < wad->meshes.length; i++)
	{
		MESH* mesh = arrayGetItem(&wad->meshes, i);
		meshDeinitialize(mesh);
	}
	arrayDeinitialize(&wad->meshes);
	
	for (unsigned int i = 0; i < wad->textureSamples.length; i++)
	{
		TEXTURE_SAMPLE* textureSample = arrayGetItem(&wad->textureSamples, i);
		textureSampleDeinitialize(textureSample);
	}
	arrayDeinitialize(&wad->textureSamples);
	
	for (unsigned int i = 0; i < wad->texturePages.length; i++)
	{
		TEXTURE_PAGE* texturePage = arrayGetItem(&wad->texturePages, i);
		texturePageDeinitialize(texturePage);
	}
	arrayDeinitialize(&wad->texturePages);

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

TEXTURE_PAGE* wadGetTexturePage(WK_WAD* wad, unsigned int texturePageIndex)
{
	assert(wad);
	return arrayGetItem(&wad->texturePages, texturePageIndex);
}

unsigned char* wadGetTexturePageData(WK_WAD* wad, unsigned int texturePageIndex)
{
	assert(wad);
	TEXTURE_PAGE* texturePage = wadGetTexturePage(wad, texturePageIndex);
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
	return arrayGetItem(&wad->meshes, meshIndex);
}

unsigned int wadGetNumSkeletons(WK_WAD* wad)
{
	assert(wad);
	return wad->skeletons.length;
}

SKELETON* wadGetSkeleton(WK_WAD* wad, unsigned int skeletonIndex)
{
	assert(wad);
	return arrayGetItem(&wad->skeletons, skeletonIndex);
}


unsigned int wadGetNumMovables(WK_WAD* wad)
{
	assert(wad);
	return wad->numMovables;
}

MOVABLE* wadGetMovableByIndex(WK_WAD* wad, unsigned int movableIndex)
{
	assert(wad);
	assert(movableIndex < wad->numMovables);
	return &(wad->movables[movableIndex]);
}

MOVABLE* wadGetMovableById(WK_WAD* wad, MOVABLE_ID movableId)
{
	assert(wad);
	
	MOVABLE* movable = NULL;
	for (unsigned int i = 0; i < wad->numMovables; i++)
	{
		MOVABLE* currentMovable = &(wad->movables[i]);
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
	return wad->numStatics;
}

STATIC* wadGetStaticByIndex(WK_WAD* wad, unsigned int staticIndex)
{
	assert(wad);
	assert(staticIndex < wad->numStatics);
	return &(wad->statics[staticIndex]);
}

STATIC* wadGetStaticById(WK_WAD* wad, STATIC_ID staticId)
{
	assert(wad);
	
	STATIC* staticObject = NULL;
	for (unsigned int i = 0; i < wad->numStatics; i++)
	{
		STATIC* currentStatic = &(wad->statics[i]);
		if (currentStatic->staticId == staticId)
		{
			staticObject = currentStatic;
			break;
		}
	}
	
	return staticObject;
}
