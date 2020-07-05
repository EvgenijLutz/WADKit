//
//  movable_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_movable_interface_h
#define wad_kit_movable_interface_h

#include "movable_id.h"

typedef struct WAD WAD;
typedef struct MOVABLE MOVABLE;
typedef struct MESH MESH;
typedef struct ANIMATION ANIMATION;

MOVABLE_ID movableGetId(MOVABLE* movable);
const char* movableIdGetName(MOVABLE_ID movableId);

unsigned int movableGetNumMeshes(MOVABLE* movable);
unsigned int movableGetMeshIndex(MOVABLE* movable, unsigned int movableMeshIndex);
MESH* movableGetMesh(MOVABLE* movable, unsigned int movableMeshIndex, WAD* wad);

unsigned int movableGetSkeletonIndex(MOVABLE* movable);

unsigned int movableGetNumAnimations(MOVABLE* movable);
ANIMATION* movableGetAnimation(MOVABLE* movable, unsigned int animationIndex);

#endif /* wad_kit_movable_interface_h */
