//
//  animation_internal_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void animationInitialize(ANIMATION* animation, MOVABLE* movable, RAW_MOVABLE* rawMovable, RAW_ANIMATION* rawAnimation, RAW_ANIMATION* nextRawAnimation, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(animation);
	assert(movable);
	assert(rawAnimation);
	// nextRawAnimation is nullable
	assert(loadInfo);
	
	//WK_WAD* wad = loadInfo->wad;
	WK_BUFFER* buffer = loadInfo->buffer;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	animation->movable = movable;
	animation->stateId = rawAnimation->stateId;
	arrayInitializeWithCapacityIncrement(&animation->keyframes, sizeof(KEYFRAME), 64);
	animation->frameDuration = rawAnimation->frameDuration;
	animation->moveSpeed = rawAnimation->speed;
	animation->moveAcceleration = rawAnimation->acceleration;
	animation->unknown1 = rawAnimation->unknown1;
	animation->unknown2 = rawAnimation->unknown2;
	animation->nextAnimation = rawAnimation->nextAnimation - rawMovable->animsIndex;
	assert(rawAnimation->nextAnimation >= rawMovable->animsIndex);
	
	// TODO: understand this shit
	animation->nextAnimationFrame = rawAnimation->frameIn;
	/*if (animation->nextAnimation < loadInfo->numAnimations)
	{
		RAW_ANIMATION* nextDefaultAnimation = &loadInfo->rawAnimations[rawAnimation->nextAnimation];
		if (rawAnimation->frameIn < nextDefaultAnimation->frameStart)
		{
			if (nextDefaultAnimation->keyframeSize == 0)
			{
				assert(0);
			}
			assert(0);
		}
		animation->nextAnimationFrame = rawAnimation->frameIn - nextDefaultAnimation->frameStart;
	}
	else
	{
		// WAD is just a little corrupted
		animation->nextAnimationFrame = 0;
		assert(0);
	}*/
	
	arrayInitializeWithCapacityIncrement(&animation->stateChanges, sizeof(STATE_CHANGE), 8);
	arrayInitializeWithCapacityIncrement(&animation->commands, sizeof(COMMAND), 8);
	
	// MARK: Read keyframes
	if (rawAnimation->keyframeSize > 0)
	{
		// Calculate number of keyframes. It's also tricky as reading movable's number of animations.
		unsigned int numKeyframes = 0;
		if (nextRawAnimation) {
			numKeyframes = (nextRawAnimation->keyframeOffset - rawAnimation->keyframeOffset) / (rawAnimation->keyframeSize * 2);
		}
		else {
			numKeyframes = (loadInfo->numKeyframesWords * 2 - rawAnimation->keyframeOffset) / (rawAnimation->keyframeSize * 2);
		}
		
		bufferSetEditorPosition(buffer, loadInfo->keyframesDataLocation + rawAnimation->keyframeOffset);
		for (unsigned int keyframeIndex = 0; keyframeIndex < numKeyframes; keyframeIndex++)
		{
			bufferSetEditorPosition(buffer, loadInfo->keyframesDataLocation + rawAnimation->keyframeOffset + keyframeIndex * rawAnimation->keyframeSize * 2);
			
			KEYFRAME* keyframe = arrayAddItem(&animation->keyframes);
			keyframeInitialize(keyframe, animation, rawAnimation, loadInfo);
			if (executeResultIsFailed(executeResult)) { return; }
		}
	}
	
	// MARK: Read commands
	bufferSetEditorPosition(buffer, loadInfo->commandsDataLocation + rawAnimation->commandOffsets * 2);
	for (unsigned int i = 0; i < rawAnimation->numCommands; i++)
	{
		COMMAND* command = arrayAddItem(&animation->commands);
		commandInitialize(command, animation, loadInfo);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	// MARK: Read state changes
	for (unsigned int i = 0; i < rawAnimation->numStateChanges; i++)
	{
		RAW_STATE_CHANGE* rawStateChange = &loadInfo->rawStateChanges[rawAnimation->changesIndex + i];
		STATE_CHANGE* stateChange = arrayAddItem(&animation->stateChanges);
		stateChangeInitialize(stateChange, animation, rawStateChange, loadInfo);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	executeResultSetSucceeded(executeResult);
}

void animationDeinitialize(ANIMATION* animation)
{
	assert(animation);
	
	for (unsigned int i = 0; i < animation->stateChanges.length; i++)
	{
		STATE_CHANGE* stateChange = arrayGetItem(&animation->stateChanges, i);
		stateChangeDeinitialize(stateChange);
	}
	arrayDeinitialize(&animation->stateChanges);
	
	for (unsigned int i = 0; i < animation->commands.length; i++)
	{
		COMMAND* command = arrayGetItem(&animation->commands, i);
		commandDeinitialize(command);
	}
	arrayDeinitialize(&animation->commands);
	
	for (unsigned int i = 0; i < animation->keyframes.length; i++)
	{
		KEYFRAME* keyframe = arrayGetItem(&animation->keyframes, i);
		keyframeDeinitialize(keyframe);
	}
	arrayDeinitialize(&animation->keyframes);
	
	memset(animation, 0, sizeof(ANIMATION));
}
