//
//  joint.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

void jointInitialize(WK_JOINT* joint, WK_MOVABLE* movable, WK_MESH* mesh, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(joint);
	assert(movable);
	assert(mesh);
	assert(loadInfo);
	
	BUFFER_READER* buffer = loadInfo->buffer;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	joint->movable = movable;
	joint->mesh = mesh;
	joint->mesh->numReferences++;
	
	joint->opCode = (JOINT_LOCATION_TYPE)bufferReaderReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	joint->rawOffsetX = bufferReaderReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	joint->rawOffsetY = bufferReaderReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	joint->rawOffsetZ = bufferReaderReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	jointUpdateOffset(joint);
}

void jointDeinitialize(WK_JOINT* joint)
{
	assert(joint);
	joint->mesh->numReferences--;
}


void jointUpdateOffset(WK_JOINT* joint)
{
	const float x = (float)joint->rawOffsetX / WK_COORDINATE_MULTIPLIER;
	const float y = (float)joint->rawOffsetY / WK_COORDINATE_MULTIPLIER;
	const float z = (float)joint->rawOffsetZ / WK_COORDINATE_MULTIPLIER;
	joint->offset = vector3fCreate(-x, -y, z);
}

// MARK: - Public interface

WK_MESH* jointGetMesh(WK_JOINT* joint)
{
	assert(joint);
	return joint->mesh;
}

JOINT_LOCATION_TYPE jointGetLocationType(WK_JOINT* joint)
{
	assert(joint);
	return joint->opCode;
}

int jointGetRawOffsetX(WK_JOINT* joint)
{
	assert(joint);
	return joint->rawOffsetX;
}

int jointGetRawOffsetY(WK_JOINT* joint)
{
	assert(joint);
	return joint->rawOffsetY;
}

int jointGetRawOffsetZ(WK_JOINT* joint)
{
	assert(joint);
	return joint->rawOffsetZ;
}

vector3f jointGetOffset(WK_JOINT* joint)
{
	assert(joint);
	return joint->offset;
}


void jointSetOffset(WK_JOINT* joint, vector3f offset)
{
	assert(joint);
	
	joint->offset = offset;
	
	joint->rawOffsetX = -(int)(offset.x * WK_COORDINATE_MULTIPLIER);
	joint->rawOffsetY = -(int)(offset.y * WK_COORDINATE_MULTIPLIER);
	joint->rawOffsetZ = (int)(offset.z * WK_COORDINATE_MULTIPLIER);
}
