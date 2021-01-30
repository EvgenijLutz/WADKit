//
//  joint.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

void jointInitialize(JOINT* joint, MOVABLE* movable, MESH* mesh, WK_WAD_LOAD_INFO* loadInfo)
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
	
	joint->dx = bufferReaderReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	joint->dy = bufferReaderReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	joint->dz = bufferReaderReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	jointUpdateOffset(joint);
}

void jointDeinitialize(JOINT* joint)
{
	assert(joint);
	joint->mesh->numReferences--;
}


void jointUpdateOffset(JOINT* joint)
{
	const float x = (float)joint->dx / JOINT_COORDINATE_MULTIPLIER;
	const float y = (float)joint->dy / JOINT_COORDINATE_MULTIPLIER;
	const float z = (float)joint->dz / JOINT_COORDINATE_MULTIPLIER;
	joint->offset = vector3fCreate(-x, -y, z);
}

// MARK: - Public interface

MESH* jointGetMesh(JOINT* joint)
{
	assert(joint);
	return joint->mesh;
}

JOINT_LOCATION_TYPE jointGetLocationType(JOINT* joint)
{
	assert(joint);
	return joint->opCode;
}

int jointGetRawOffsetX(JOINT* joint)
{
	assert(joint);
	return joint->dx;
}

int jointGetRawOffsetY(JOINT* joint)
{
	assert(joint);
	return joint->dy;
}

int jointGetRawOffsetZ(JOINT* joint)
{
	assert(joint);
	return joint->dz;
}

vector3f jointGetOffset(JOINT* joint)
{
	assert(joint);
	return joint->offset;
}


void jointSetOffset(JOINT* joint, vector3f offset)
{
	assert(joint);
	
	joint->offset = offset;
	joint->dx = -(int)(offset.x * JOINT_COORDINATE_MULTIPLIER);
	joint->dy = -(int)(offset.y * JOINT_COORDINATE_MULTIPLIER);
	joint->dz = (int)(offset.z * JOINT_COORDINATE_MULTIPLIER);
}

