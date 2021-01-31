//
//  keyframe.c
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

#define ROTATION_USED_THREE_AXES	0x0000
#define ROTATION_USED_X_AXIS		0x4000
#define ROTATION_USED_Y_AXIS		0x8000
#define ROTATION_USED_Z_AXIS		0xc000

#define USE_BIG_ANGLES 1

#if USE_BIG_ANGLES
#define MAKE_LITTLE_ANGLE(value) value * 4
#define MAKE_BIG_ANGLE(value) value
//#define MAKE_EULER_ANGLE(value) (M_PI * 2 * (float)(value) / 4096.0f)
// Same as here:
#define MAKE_EULER_ANGLE(value) (M_PI * (float)(value) / 2048.0f)
#else
#define MAKE_LITTLE_ANGLE(value) value
#define MAKE_BIG_ANGLE(value) value / 4
//#define MAKE_EULER_ANGLE(value) (M_PI * 2 * (float)(value) / 1024.0f)
// Same as here:
#define MAKE_EULER_ANGLE(value) (M_PI * (float)(value) / 512.0f)
#endif

static void _keyframe_updateBoundingBox(WK_KEYFRAME* keyframe)
{
	//keyframe->boundingBoxStart = vector3fCreate(0, 0, 0);
	//keyframe->boundingBoxEnd = vector3fCreate(0, 0, 0);
}

static void _keyframe_updateModelOffset(WK_KEYFRAME* keyframe)
{
	//keyframe->modelOffset = vector3fCreate(0, 0, 0);
}

static void _keyframe_updateRotation(WK_ROTATION* rotation)
{
	const float x = MAKE_EULER_ANGLE(rotation->rawEulerRotationX);
	const float y = MAKE_EULER_ANGLE(rotation->rawEulerRotationY);
	const float z = MAKE_EULER_ANGLE(rotation->rawEulerRotationZ);
	
	/*
	 const float degrees = (float)(360.0 / 65536.0 * (double)angle);
	 const float radians = degrees * (M_PI / 180.0f);
	 */
	
	rotation->eulerRotation = vector3fCreate(-x, -y, z);
	
	// This code works only for xyz euler rotation, but we need zxy... [1]
	/*vector3f euler = vector3fCreate(-x, -y, z);
	rotation->quaternionRotation = quaternionfFromEulerVector3f(euler);*/
	
	// [1] ... So instead we'll create quatertions for each component and multiply them to make zxy quaternion rotation
	vector3f eulerX = vector3fCreate(-x,  0, 0);
	quaternionf qx = quaternionfFromEulerVector3f(eulerX);
	
	vector3f eulerY = vector3fCreate( 0, -y, 0);
	quaternionf qy = quaternionfFromEulerVector3f(eulerY);
	
	vector3f eulerZ = vector3fCreate( 0,  0, z);
	quaternionf qz = quaternionfFromEulerVector3f(eulerZ);
	
	quaternionf q = quaternionfMul(qx, qz);
	q = quaternionfMul(qy, q);
	
	rotation->quaternionRotation = q;
}

void keyframeInitialize(WK_KEYFRAME* keyframe, WK_ANIMATION* animation, RAW_ANIMATION* rawAnimation, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(keyframe);
	assert(animation);
	assert(rawAnimation);
	assert(loadInfo);
	
	//WK_WAD* wad = loadInfo->wad;
	BUFFER_READER* buffer = loadInfo->buffer;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	keyframe->animation = animation;
	
	RAW_KEYFRAME* rawKeyframe = (RAW_KEYFRAME*)bufferReaderRequestDataToRead(buffer, sizeof(RAW_KEYFRAME), executeResult);
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
	keyframe->rotations = malloc(sizeof(WK_ROTATION) * numMeshes);
	
	/**
	 Tunrs out, movable can have 123 joints
	 WK_ANIMATION::keyframeSize is unsigned char, where max value is 255
	 9 words go for bounding box and offset
	 other 123 pairs (123*2=246) of words - for rotation
	 */
	
	/**
	 RAW_ANIMATION.keyframeSize represents maximum size of whole animation's keyframes.
	 Some keyframe sizes are less than others because of compression, so in some cases a garbage padding can be added.
	 */
	
	//short wordsLeft = (rawAnimation->keyframeSize - 9);	// 9 words is bounding box
	
	for (unsigned int rotationIndex = 0; rotationIndex < numMeshes; rotationIndex++)
	{
		WK_ROTATION* rotation = &keyframe->rotations[rotationIndex];
		unsigned int angleSet = bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		//wordsLeft--;
		unsigned int nextWord = 0;
		unsigned int axes = angleSet & 0xc000;
		
		switch (axes)
		{
			case ROTATION_USED_THREE_AXES:
				nextWord = bufferReaderReadUInt16(buffer, executeResult);
				if (executeResultIsFailed(executeResult)) { return; }
				//wordsLeft--;
				angleSet = angleSet * 0x10000 + nextWord;
				rotation->rawEulerRotationZ = MAKE_LITTLE_ANGLE((angleSet & 0x3ff));
				angleSet >>= 10;
				rotation->rawEulerRotationY = MAKE_LITTLE_ANGLE((angleSet & 0x3ff));
				angleSet >>= 10;
				rotation->rawEulerRotationX = MAKE_LITTLE_ANGLE((angleSet & 0x3ff));
				break;

			case ROTATION_USED_X_AXIS:
				rotation->rawEulerRotationX = MAKE_BIG_ANGLE((angleSet & 0x3fff));
				rotation->rawEulerRotationY = 0;
				rotation->rawEulerRotationZ = 0;
				break;

			case ROTATION_USED_Y_AXIS:
				rotation->rawEulerRotationX = 0;
				rotation->rawEulerRotationY = MAKE_BIG_ANGLE((angleSet & 0x3fff));
				rotation->rawEulerRotationZ = 0;
				break;

			case ROTATION_USED_Z_AXIS:
				rotation->rawEulerRotationX = 0;
				rotation->rawEulerRotationY = 0;
				rotation->rawEulerRotationZ = MAKE_BIG_ANGLE((angleSet & 0x3fff));
				break;
							
			default:
				/* This never should happen */
				assert(0);
				break;
		}
		
		_keyframe_updateRotation(rotation);
	}
	
	// Not every keyframe is evenly alignet as it earlier was spoken. Thus sometimes the keyframeInitialize function does not reach end of keyframe buffer.
	
	// Skip garbage padding
	/*while (wordsLeft > 0)
	{
		bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		wordsLeft--;
	}*/
	
	executeResultIsSucceeded(executeResult);
}

void keyframeDeinitialize(WK_KEYFRAME* keyframe)
{
	assert(keyframe);
	assert(keyframe->rotations);
	
	free(keyframe->rotations);
	debug_memset(keyframe, 0, sizeof(WK_KEYFRAME));
}

// MARK: - Public interface

vector3f keyframeGetRootOffset(WK_KEYFRAME* keyframe)
{
	assert(keyframe);
	
	float x = (float)keyframe->offx / 1024.0f;
	float y = (float)keyframe->offy / 1024.0f;
	float z = (float)keyframe->offz / 1024.0f;
	
	return vector3fCreate(-x, -y, z);
}

vector3f keyframeGetInterpolatedRootOffset(WK_KEYFRAME* firstKeyframe, WK_KEYFRAME* secondKeyframe, float interpolationCoefficient)
{
	assert(firstKeyframe);
	assert(secondKeyframe);
	
	vector3f firstOffset = keyframeGetRootOffset(firstKeyframe);
	firstOffset = vector3fMulFloat(firstOffset, 1.0f - interpolationCoefficient);
	
	vector3f secondOffset = keyframeGetRootOffset(secondKeyframe);
	secondOffset = vector3fMulFloat(secondOffset, interpolationCoefficient);
	
	return vector3fSum(firstOffset, secondOffset);
}


quaternionf keyframeGetQuaternionRotation(WK_KEYFRAME* keyframe, unsigned int transformIndex)
{
	assert(keyframe);
	assert(keyframe->animation->movable->joints.length + 1 > transformIndex);
	
	WK_ROTATION* rotation = &keyframe->rotations[transformIndex];
	return rotation->quaternionRotation;
}

vector3f keyframeGetEulerRotation(WK_KEYFRAME* keyframe, unsigned int transformIndex)
{
	assert(keyframe);
	assert(keyframe->animation->movable->joints.length + 1 > transformIndex);
	
	WK_ROTATION* rotation = &keyframe->rotations[transformIndex];
	return rotation->eulerRotation;
}

static inline float _calcRot(float r1, float r2, float interpolationCoefficient)
{
	const float twoPi = M_PI * 2.0f;
	
	float dif = twoPi - r2 + r1;
	if (dif <= r2 - r1)
	{
		float res = r1 - (dif * interpolationCoefficient);
		return res;
	}
	
	dif = twoPi - r1 + r2;
	if (dif <= r1 - r2)
	{
		return r1 + (dif * interpolationCoefficient);
	}
	
	dif = r2 - r1;
	return r1 + (dif * interpolationCoefficient);
}

vector3f keyframeCalculateInterpolatedRotation(WK_KEYFRAME* firstKeyframe, WK_KEYFRAME* secondKeyframe, unsigned int transformIndex, float interpolationCoefficient)
{
	assert(firstKeyframe);
	assert(secondKeyframe);
	//assert(firstKeyframe != secondKeyframe);
	assert(firstKeyframe->animation->movable->joints.length + 1 > transformIndex);
	assert(secondKeyframe->animation->movable->joints.length + 1 > transformIndex);
		
	vector3f firstRotation = keyframeGetEulerRotation(firstKeyframe, transformIndex);
	vector3f secondRotation = keyframeGetEulerRotation(secondKeyframe, transformIndex);
	
	float rx = _calcRot(firstRotation.x, secondRotation.x, interpolationCoefficient);
	float ry = _calcRot(firstRotation.y, secondRotation.y, interpolationCoefficient);
	float rz = _calcRot(firstRotation.z, secondRotation.z, interpolationCoefficient);
	vector3f result = vector3fCreate(rx, ry, rz);
	
	return result;
}
