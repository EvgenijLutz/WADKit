//
//  movable.h
//  wad_kit
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef movable_h
#define movable_h

#include "wad_kit.h"

typedef struct MOVABLE
{
	WK_WAD* wad;
	unsigned long numReferences;
	
	MOVABLE_ID movableId;
	
	MESH* rootMesh;
	
	/*!
	 Collection of @b JOINT objects, where other meshes are located.
	 */
	WK_ARRAY joints;
	
	/*!
	 Collection of @b ANIMATION objects.
	 */
	WK_ARRAY animations;
}
MOVABLE;

#endif /* movable_h */
