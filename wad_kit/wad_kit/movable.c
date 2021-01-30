//
//  movable.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

void movableInitialize(MOVABLE* movable, RAW_MOVABLE* rawMovable, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(movable);
	assert(loadInfo);
	
	WK_WAD* wad = loadInfo->wad;
	BUFFER_READER* buffer = loadInfo->buffer;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	movable->wad = wad;
	movable->numReferences = 0;
	movable->movableId = (MOVABLE_ID)rawMovable->objectId;
	magicArrayInitialize(&movable->joints, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(JOINT), 4);
	magicArrayInitializeWithAllocator(&movable->animations, wad->animationAllocator);
	
	unsigned short numPointers = rawMovable->numPointers;
	unsigned short meshPointersStart = rawMovable->pointersIndex;
	movable->rootMesh = _wad_findMesh(meshPointersStart, 0, loadInfo);
	if (executeResultIsFailed(executeResult)) { return; }
	movable->rootMesh->numReferences++;
	
	unsigned int firstLinkAddress = rawMovable->linksIndex;
	
	// MARK: Read movable joints
	bufferReaderSetEditorPosition(buffer, loadInfo->linksDataLocation + firstLinkAddress * 4);
	for (unsigned int i = 1; i < numPointers; i++)
	{
		MESH* mesh = _wad_findMesh(meshPointersStart, i, loadInfo);
		if (executeResultIsFailed(executeResult)) { return; }
		
		JOINT* joint = magicArrayAddItem(&movable->joints);
		jointInitialize(joint, movable, mesh, loadInfo);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	/*unsigned int keyframesOffset = rawMovable->keyframesOffset;*/
	
	// Determine the number of animations. It's tricky, we must calculate it.
	const unsigned int movableIndex = magicArrayGetItemIndex(&wad->movables, movable);
	unsigned int numAnimations = 0;
	if (rawMovable->animsIndex >= 0)
	{
		// numAnimations = nextRawMovable(where .animsIndex != -1).animIndex - rawMovable.animsIndex
		
		// Default number or animations if we will not be able to find it
		numAnimations = loadInfo->numAnimations - rawMovable->animsIndex;
		
		for (unsigned int i = movableIndex + 1; i < loadInfo->numMovables; i++)
		{
			RAW_MOVABLE* nextRawMovable = &loadInfo->rawMovables[i];
			if (nextRawMovable->animsIndex >= 0)
			{
				numAnimations = nextRawMovable->animsIndex - rawMovable->animsIndex;
				break;
			}
		}
	}
	
	// MARK: Read animations
	for (unsigned short animationIndex = 0; animationIndex < numAnimations; animationIndex++)
	{
		RAW_ANIMATION* rawAnimation = &loadInfo->rawAnimations[rawMovable->animsIndex + animationIndex];
		RAW_ANIMATION* nextRawAnimation = NULL;
		// Find next animation to determine number of keyframes
		/*
		 Fun story. Previously instead of using "loadInfo->numAnimations" in this condition I used by mistake just "numAnimations". So because of that the "nextRawAnimation" was always NULL. Thus, the function "animationInitialize" counted wrong the number of animation. In sum, application was 170Mb in RAM. Now, it's 28Mb.
		 */
		if (rawMovable->animsIndex + animationIndex + 1 < loadInfo->numAnimations)
		{
			nextRawAnimation = &loadInfo->rawAnimations[rawMovable->animsIndex + animationIndex + 1];
		}
		
		ANIMATION* animation = magicArrayAddItem(&movable->animations);
		animationInitialize(animation, movable, rawMovable, rawAnimation, nextRawAnimation, loadInfo);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	executeResultSetSucceeded(executeResult);
}

void movableDeinitialize(MOVABLE* movable)
{
	assert(movable);
	assert(movable->numReferences == 0);
	
	for (unsigned int i = 0; i < movable->animations.length; i++)
	{
		ANIMATION* animation = magicArrayGetItem(&movable->animations, i);
		animationDeinitialize(animation);
	}
	magicArrayDeinitialize(&movable->animations);
	
	for (unsigned int i = 0; i < movable->joints.length; i++)
	{
		JOINT* joint = magicArrayGetItem(&movable->joints, i);
		jointDeinitialize(joint);
	}
	magicArrayDeinitialize(&movable->joints);
	
	if (movable->rootMesh)
	{
		movable->rootMesh->numReferences--;
	}
	
#if DEBUF
	memset(movable, 0, sizeof(MOVABLE));
#endif
}

// MARK: - Public interface

MOVABLE_ID movableGetId(MOVABLE* movable)
{
	assert(movable);
	return movable->movableId;
}


MESH* movableGetRootMesh(MOVABLE* movable)
{
	assert(movable);
	return movable->rootMesh;
}

unsigned int movableGetNumJoints(MOVABLE* movable)
{
	assert(movable);
	return movable->joints.length;
}

JOINT* movableGetJoint(MOVABLE* movable, unsigned int jointIndex)
{
	assert(movable);
	return magicArrayGetItem(&movable->joints, jointIndex);
}


unsigned int movableGetNumAnimations(MOVABLE* movable)
{
	assert(movable);
	return movable->animations.length;
}

ANIMATION* movableGetAnimation(MOVABLE* movable, unsigned int animationIndex)
{
	assert(movable);
	return magicArrayGetItem(&movable->animations, animationIndex);
}
