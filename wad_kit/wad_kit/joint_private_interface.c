//
//  joint_private_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

static void _joint_updateOffset(JOINT* joint)
{
	//
}

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
	
	_joint_updateOffset(joint);
}

void jointDeinitialize(JOINT* joint)
{
	assert(joint);
	joint->mesh->numReferences--;
}
