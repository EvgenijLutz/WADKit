//
//  wad_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include "wad_interface.h"
#include "wad.h"
#include "raw_wad.h"
#include "execute_result.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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

