//
//  movable_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "movable_interface.h"
#include "movable.h"
#include "wad_interface.h"
#include <stdlib.h>
#include <assert.h>


MOVABLE_ID movableGetId(MOVABLE* movable)
{
	assert(movable);
	return movable->movableId;
}


unsigned int movableGetNumMeshes(MOVABLE* movable)
{
	assert(movable);
	return movable->numMeshes;
}

unsigned int movableGetMeshIndex(MOVABLE* movable, unsigned int movableMeshIndex)
{
	assert(movable);
	assert(movableMeshIndex < movable->numMeshes);
	return movable->meshIndices[movableMeshIndex];
}

MESH* movableGetMesh(MOVABLE* movable, unsigned int movableMeshIndex, WAD* wad)
{
	assert(movable);
	assert(movableMeshIndex < movable->numMeshes);
	assert(wad);
	
	unsigned int meshIndex = movable->meshIndices[movableMeshIndex];
	return wadGetMesh(wad, meshIndex);
}

unsigned int movableGetSkeletonIndex(MOVABLE* movable)
{
	assert(movable);
	return movable->skeletonIndex;
}
