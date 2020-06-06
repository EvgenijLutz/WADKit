//
//  joint_location_type.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_joint_location_type_h
#define wad_kit_joint_location_type_h

typedef enum JOINT_LOCATION_TYPE
{
	JOINT_LOCATION_TYPE_LINK_TO_PREVIOUS_MESH = 0,
	JOINT_LOCATION_TYPE_PULL_PARENT_FROM_STACK_AND_CONNECT,
	JOINT_LOCATION_TYPE_PUSH_PARENT_TO_STACK_AND_CONNECT,
	JOINT_LOCATION_TYPE_LINK_TO_PARENT_IN_STACK
}
JOINT_LOCATION_TYPE;

#endif /* wad_kit_joint_location_type_h */
