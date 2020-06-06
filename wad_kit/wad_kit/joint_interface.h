//
//  joint_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_joint_interface_h
#define wad_kit_joint_interface_h

#include "joint_location_type.h"

typedef struct JOINT JOINT;

JOINT_LOCATION_TYPE jointGetLocationType(JOINT* joint);
int jointGetOffsetX(JOINT* joint);
int jointGetOffsetY(JOINT* joint);
int jointGetOffsetZ(JOINT* joint);

#endif /* wad_kit_joint_interface_h */
