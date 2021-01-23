//
//  movable_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_movable_interface_h
#define wad_kit_movable_interface_h

#include "wad_kit.h"

MOVABLE_ID movableGetId(MOVABLE* movable);
const char* movableIdGetTerribleName(MOVABLE_ID movableId);

MESH* movableGetRootMesh(MOVABLE* movable);
unsigned int movableGetNumJoints(MOVABLE* movable);
JOINT* movableGetJoint(MOVABLE* movable, unsigned int jointIndex);

unsigned int movableGetNumAnimations(MOVABLE* movable);
ANIMATION* movableGetAnimation(MOVABLE* movable, unsigned int animationIndex);

#endif /* wad_kit_movable_interface_h */
