//
//  keyframe_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 24.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

vector3f keyframeGetRootOffset(KEYFRAME* keyframe)
{
	assert(keyframe);
	
	float x = (float)keyframe->offx / 1024.0f;
	float y = (float)keyframe->offy / 1024.0f;
	float z = (float)keyframe->offz / 1024.0f;
	
	return vector3fCreate(-x, -y, z);
}

vector3f keyframeGetInterpolatedRootOffset(KEYFRAME* firstKeyframe, KEYFRAME* secondKeyframe, float interpolationCoefficient)
{
	assert(firstKeyframe);
	assert(secondKeyframe);
	
	vector3f firstOffset = keyframeGetRootOffset(firstKeyframe);
	firstOffset = vector3fMulFloat(firstOffset, 1.0f - interpolationCoefficient);
	
	vector3f secondOffset = keyframeGetRootOffset(secondKeyframe);
	secondOffset = vector3fMulFloat(secondOffset, interpolationCoefficient);
	
	return vector3fSum(firstOffset, secondOffset);
}


quaternionf keyframeGetRotationQuaternion(KEYFRAME* keyframe, unsigned int transformIndex)
{
	assert(keyframe);
	assert(keyframe->animation->movable->joints.length + 1 > transformIndex);
	
	ROTATION* rotation = &keyframe->rotations[transformIndex];
	return rotation->rotation;
}

vector3f keyframeCalculateRotation(KEYFRAME* keyframe, unsigned int transformIndex)
{
	assert(keyframe);
	assert(keyframe->animation->movable->joints.length + 1 > transformIndex);
	
	/*
	 const float degrees = (float)(360.0 / 65536.0 * (double)angle);
	 const float radians = degrees * (M_PI / 180.0f);
	 */
	
	ROTATION* rotation = &keyframe->rotations[transformIndex];
	float x = M_PI * 2 * (float)rotation->rotx / 1024.0f;
	float y = M_PI * 2 * (float)rotation->roty / 1024.0f;
	float z = M_PI * 2 * (float)rotation->rotz / 1024.0f;
	return vector3fCreate(-x, -y, z);
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

vector3f keyframeCalculateInterpolatedRotation(KEYFRAME* firstKeyframe, KEYFRAME* secondKeyframe, unsigned int transformIndex, float interpolationCoefficient)
{
	assert(firstKeyframe);
	assert(secondKeyframe);
	//assert(firstKeyframe != secondKeyframe);
	assert(firstKeyframe->animation->movable->joints.length + 1 > transformIndex);
	assert(secondKeyframe->animation->movable->joints.length + 1 > transformIndex);
	
	/*vector3f firstRotation = keyframeCalculateRotation(firstKeyframe, transformIndex);
	firstRotation = vector3fMulFloat(firstRotation, 1.0f - interpolationCoefficient);
	
	vector3f secondRotation = keyframeCalculateRotation(secondKeyframe, transformIndex);
	secondRotation = vector3fMulFloat(secondRotation, interpolationCoefficient);
	
	return vector3fSum(firstRotation, secondRotation);*/
	
	vector3f firstRotation = keyframeCalculateRotation(firstKeyframe, transformIndex);
	vector3f secondRotation = keyframeCalculateRotation(secondKeyframe, transformIndex);
	
	float rx = _calcRot(firstRotation.x, secondRotation.x, interpolationCoefficient);
	float ry = _calcRot(firstRotation.y, secondRotation.y, interpolationCoefficient);
	float rz = _calcRot(firstRotation.z, secondRotation.z, interpolationCoefficient);
	vector3f result = vector3fCreate(rx, ry, rz);
	
	return result;
}
