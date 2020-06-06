//
//  joint_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "joint_interface.h"
#include "joint.h"
#include <stdlib.h>
#include <assert.h>


JOINT_LOCATION_TYPE jointGetLocationType(JOINT* joint)
{
	assert(joint);
	return (JOINT_LOCATION_TYPE)joint->opCode;
}

int jointGetOffsetX(JOINT* joint)
{
	assert(joint);
	return joint->dx;
}

int jointGetOffsetY(JOINT* joint)
{
	assert(joint);
	return joint->dy;
}

int jointGetOffsetZ(JOINT* joint)
{
	assert(joint);
	return joint->dz;
}
