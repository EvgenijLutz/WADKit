//
//  movable.h
//  wad_kit
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef movable_h
#define movable_h

#include "movable_id.h"

typedef struct ANIMATION ANIMATION;

typedef struct MOVABLE
{
	MOVABLE_ID movableId;
	
	unsigned short numMeshes;
	unsigned int* meshIndices;
	
	unsigned int skeletonIndex;	// TODO: maybe replace to SKELETON pointer?
	
	unsigned short numAnimations;
	ANIMATION* animations;
}
MOVABLE;

#endif /* movable_h */
