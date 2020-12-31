//
//  keyframe_internal_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

#define ROTATION_USED_THREE_AXES	0x0000
#define ROTATION_USED_X_AXIS		0x4000
#define ROTATION_USED_Y_AXIS		0x8000
#define ROTATION_USED_Z_AXIS		0xc000

static void _keyframe_updateBoundingBox(KEYFRAME* keyframe)
{
	keyframe->boundingBoxStart = vector3fCreate(0, 0, 0);
	keyframe->boundingBoxEnd = vector3fCreate(0, 0, 0);
}

static void _keyframe_updateModelOffset(KEYFRAME* keyframe)
{
	keyframe->modelOffset = vector3fCreate(0, 0, 0);
}

static void _keyframe_updateRotation(ROTATION* rotation)
{
	//rotation->rotation = vector3fCreate(0, 0, 0);
}

void keyframeInitialize(KEYFRAME* keyframe, ANIMATION* animation, RAW_ANIMATION* rawAnimation, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(keyframe);
	assert(animation);
	assert(rawAnimation);
	assert(loadInfo);
	
	//WK_WAD* wad = loadInfo->wad;
	WK_BUFFER* buffer = loadInfo->buffer;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	keyframe->animation = animation;
	
	RAW_KEYFRAME* rawKeyframe = (RAW_KEYFRAME*)bufferRequestDataToRead(buffer, sizeof(RAW_KEYFRAME), executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	keyframe->bb1x = rawKeyframe->bb1x;
	keyframe->bb1y = rawKeyframe->bb1y;
	keyframe->bb1z = rawKeyframe->bb1z;
	keyframe->bb2x = rawKeyframe->bb2x;
	keyframe->bb2y = rawKeyframe->bb2y;
	keyframe->bb2z = rawKeyframe->bb2z;
	_keyframe_updateBoundingBox(keyframe);
	
	keyframe->offx = rawKeyframe->offx;
	keyframe->offy = rawKeyframe->offy;
	keyframe->offz = rawKeyframe->offz;
	_keyframe_updateModelOffset(keyframe);
	
	const unsigned int numMeshes = animation->movable->joints.length + 1;
	keyframe->rotations = malloc(sizeof(ROTATION) * numMeshes);
	
	/*
	 Tunrs out, movable can have 123 joints
	 ANIMATION::keyframeSize is unsigned char, where max value is 255
	 9 words go for bounding box and offset
	 other 123 pairs (123*2=246) of words - for rotation
	 */
	
	/* rawAnimation->keyframeSize represents maximum size of whole animation's keyframes.
	   Some keyframe sizes are less than others because of compression, so a garbage padding is added.
	 */
	
	//short wordsLeft = (rawAnimation->keyframeSize - 9);	// 9 words is bounding box
	
	for (unsigned int rotationIndex = 0; rotationIndex < numMeshes; rotationIndex++)
	{
		ROTATION* rotation = &keyframe->rotations[rotationIndex];
		unsigned int angleSet = bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		//wordsLeft--;
		unsigned int nextWord = 0;
		unsigned int axes = angleSet & 0xc000;
		
		switch (axes)
		{
			case ROTATION_USED_THREE_AXES:
				nextWord = bufferReadUInt16(buffer, executeResult);
				if (executeResultIsFailed(executeResult)) { return; }
				//wordsLeft--;
				angleSet = angleSet * 0x10000 + nextWord;
				rotation->rotz = (angleSet & 0x3ff);// * 16;
				angleSet >>= 10;
				rotation->roty = (angleSet & 0x3ff);// * 16;
				angleSet >>= 10;
				rotation->rotx = (angleSet & 0x3ff);// * 16;
				break;

			case ROTATION_USED_X_AXIS:
				rotation->rotx = (angleSet & 0x3fff) / 4;
				rotation->roty = 0;
				rotation->rotz = 0;
				break;

			case ROTATION_USED_Y_AXIS:
				rotation->rotx = 0;
				rotation->roty = (angleSet & 0x3fff) / 4;
				rotation->rotz = 0;
				break;

			case ROTATION_USED_Z_AXIS:
				rotation->rotx = 0;
				rotation->roty = 0;
				rotation->rotz = (angleSet & 0x3fff) / 4;
				break;
							
			default:
				/* This never should happen */
				assert(0);
				break;
		}
		
		_keyframe_updateRotation(rotation);
	}
	
	// Not every keyframe is evenly alignet. Thus sometimes the keyframeInitialize function does not reach end of keyframe buffer.
	
	// Skip garbage padding
	/*while (wordsLeft > 0)
	{
		bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		wordsLeft--;
	}*/
	
	executeResultIsSucceeded(executeResult);
}

void keyframeDeinitialize(KEYFRAME* keyframe)
{
	assert(keyframe);
	assert(keyframe->rotations);
	
	free(keyframe->rotations);
#if DEBUG
	memset(keyframe, 0, sizeof(KEYFRAME));
#endif
}
