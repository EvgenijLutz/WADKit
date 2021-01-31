//
//  joint_location_type.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_joint_location_type_h
#define wad_kit_joint_location_type_h

#include "wad_kit.h"

/**
 From @b documentation:
 The opCode takes the values 0, 1, 2, 3, where:
 @b 0 = stack not used. Link the current mesh to the previous mesh.
 @b 1 = pull the parent from the stack. Link the current mesh to the parent.
 @b 2 = push the parent into the stack. Link the current mesh to the parent.
 @b 3 = read the parent in the stack. Link the current mesh to the parent.
 */
typedef enum JOINT_LOCATION_TYPE
{
	JOINT_LOCATION_TYPE_LINK_TO_PREVIOUS_MESH = 0,
	JOINT_LOCATION_TYPE_PULL_PARENT_FROM_STACK_AND_CONNECT = 1,
	JOINT_LOCATION_TYPE_PUSH_PARENT_TO_STACK_AND_CONNECT = 2,
	JOINT_LOCATION_TYPE_LINK_TO_PARENT_IN_STACK = 3
}
JOINT_LOCATION_TYPE;

#endif /* wad_kit_joint_location_type_h */
