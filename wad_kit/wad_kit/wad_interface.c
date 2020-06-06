//
//  wad_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include "wad_interface.h"
#include "execute_result.h"
#include "mesh.h"
#include "static_object.h"
#include "wad.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>


WAD* wadCreate(void)
{
	WAD* wad = malloc(sizeof(WAD));
	memset(wad, 0, sizeof(WAD));
	return wad;
}

void wadRelease(WAD* wad)
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
	
	if (wad->meshes)
	{
		for (unsigned int meshIndex = 0; meshIndex < wad->numMeshes; meshIndex++)
		{
			free(wad->meshes[meshIndex].polygons);
			free(wad->meshes[meshIndex].vertices);
		}
		free(wad->meshes);
	}
	
	if (wad->skeletons)
	{
		for (unsigned int skeletonIndex = 0; skeletonIndex < wad->numSkeletons; skeletonIndex++)
		{
			free(wad->skeletons[skeletonIndex].joints);
		}
		free(wad->skeletons);
	}
	
	if (wad->texturePages)
	{
		free(wad->texturePages);
	}
	
	if (wad->textureSamples)
	{
		free(wad->textureSamples);
	}
	
	free(wad);
}

unsigned int wadGetVersion(WAD* wad)
{
	assert(wad);
	return wad->version;
}


unsigned int wadGetNumTexturePages(WAD* wad)
{
	assert(wad);
	return wad->numTexturePages;
}

unsigned char* wadGetTexturePageData(WAD* wad, unsigned int texturePageIndex)
{
	assert(wad);
	assert(texturePageIndex < wad->numTexturePages);
	return wad->texturePages[texturePageIndex].data;
}


unsigned int wadGetNumTextureSamples(WAD* wad)
{
	assert(wad);
	return wad->numTextureSamples;
}

TEXTURE_SAMPLE wadGetTextureSample(WAD* wad, unsigned int textureSampleIndex)
{
	assert(wad);
	assert(textureSampleIndex < wad->numTextureSamples);
	return wad->textureSamples[textureSampleIndex];
}


unsigned int wadGetNumMeshes(WAD* wad)
{
	assert(wad);
	return wad->numMeshes;
}

MESH* wadGetMesh(WAD* wad, unsigned int meshIndex)
{
	assert(wad);
	assert(meshIndex < wad->numMeshes);
	return &(wad->meshes[meshIndex]);
}

unsigned int wadGetNumSkeletons(WAD* wad)
{
	assert(wad);
	return wad->numSkeletons;
}


unsigned int wadGetNumMovables(WAD* wad)
{
	assert(wad);
	return wad->numMovables;
}

MOVABLE* wadGetMovableByIndex(WAD* wad, unsigned int movableIndex)
{
	assert(wad);
	assert(movableIndex < wad->numMovables);
	return &(wad->movables[movableIndex]);
}

MOVABLE* wadGetMovableById(WAD* wad, MOVABLE_ID movableId)
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


unsigned int wadGetNumStatics(WAD* wad)
{
	assert(wad);
	return wad->numStatics;
}

STATIC* wadGetStaticByIndex(WAD* wad, unsigned int staticIndex)
{
	assert(wad);
	assert(staticIndex < wad->numStatics);
	return &(wad->statics[staticIndex]);
}

STATIC* wadGetStaticById(WAD* wad, STATIC_ID staticId)
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
