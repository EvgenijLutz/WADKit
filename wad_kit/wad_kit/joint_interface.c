//
//  joint_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

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
