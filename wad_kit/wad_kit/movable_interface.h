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

MOVABLE_ID movableGetId(WK_MOVABLE* movable);
const char* movableIdGetTerribleName(MOVABLE_ID movableId);

WK_MESH* movableGetRootMesh(WK_MOVABLE* movable);
unsigned int movableGetNumJoints(WK_MOVABLE* movable);
WK_JOINT* movableGetJoint(WK_MOVABLE* movable, unsigned int jointIndex);

unsigned int movableGetNumAnimations(WK_MOVABLE* movable);
WK_ANIMATION* movableGetAnimation(WK_MOVABLE* movable, unsigned int animationIndex);

#endif /* wad_kit_movable_interface_h */
