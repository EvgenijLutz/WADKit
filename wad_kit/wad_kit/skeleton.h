//
//  skeleton.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_skeleton_h
#define wad_kit_skeleton_h

typedef struct JOINT JOINT;

/**
 Skeleton information. Consists of skeleton joints except root joint.
 */
typedef struct SKELETON
{
	unsigned int numJoints;
	JOINT* joints;
}
SKELETON;

#endif /* wad_kit_skeleton_h */
