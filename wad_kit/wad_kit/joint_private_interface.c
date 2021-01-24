//
//  joint_private_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void jointInitialize(JOINT* joint, MOVABLE* movable, MESH* mesh, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(joint);
	assert(movable);
	assert(mesh);
	assert(loadInfo);
	
	WK_BUFFER* buffer = loadInfo->buffer;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	joint->movable = movable;
	joint->mesh = mesh;
	joint->mesh->numReferences++;
	
	joint->opCode = (JOINT_LOCATION_TYPE)bufferReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	joint->dx = bufferReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	joint->dy = bufferReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	joint->dz = bufferReadUInt32(buffer, executeResult);
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
