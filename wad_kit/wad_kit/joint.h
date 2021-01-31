//
//  joint.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_joint_h
#define wad_kit_joint_h

#include "private_interface.h"

typedef struct WK_JOINT
{
	WK_MOVABLE* movable;
	WK_MESH* mesh;
	
	JOINT_LOCATION_TYPE opCode;
	
	int dx;
	int dy;
	int dz;
	
	vector3f offset;
}
WK_JOINT;

#endif /* wad_kit_joint_h */
