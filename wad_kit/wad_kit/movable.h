//
//  movable.h
//  wad_kit
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef movable_h
#define movable_h

typedef struct ANIMATION ANIMATION;

typedef struct MOVABLE
{
	unsigned int movableId;		// TODO: replace with enum?
	unsigned short numMeshes;
	unsigned int* meshIndices;
	unsigned int skeletonIndex;
	unsigned short numAnimations;
	ANIMATION* animations;
}
MOVABLE;

#endif /* movable_h */
