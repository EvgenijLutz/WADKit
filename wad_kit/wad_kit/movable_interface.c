//
//  movable_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"


MOVABLE_ID movableGetId(MOVABLE* movable)
{
	assert(movable);
	return movable->movableId;
}


MESH* movableGetRootMesh(MOVABLE* movable)
{
	assert(movable);
	return movable->rootMesh;
}

unsigned int movableGetNumJoints(MOVABLE* movable)
{
	assert(movable);
	return movable->joints.length;
}

JOINT* movableGetJoint(MOVABLE* movable, unsigned int jointIndex)
{
	assert(movable);
	return arrayGetItem(&movable->joints, jointIndex);
}


unsigned int movableGetNumAnimations(MOVABLE* movable)
{
	assert(movable);
	return movable->animations.length;
}

ANIMATION* movableGetAnimation(MOVABLE* movable, unsigned int animationIndex)
{
	assert(movable);
	return arrayGetItem(&movable->animations, animationIndex);
}
