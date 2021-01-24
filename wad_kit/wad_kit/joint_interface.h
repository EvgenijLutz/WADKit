//
//  joint_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_joint_interface_h
#define wad_kit_joint_interface_h

#include "wad_kit.h"

MESH* jointGetMesh(JOINT* joint);
JOINT_LOCATION_TYPE jointGetLocationType(JOINT* joint);
int jointGetRawOffsetX(JOINT* joint);
int jointGetRawOffsetY(JOINT* joint);
int jointGetRawOffsetZ(JOINT* joint);
vector3f jointGetOffset(JOINT* joint);

void jointSetOffset(JOINT* joint, vector3f offset);

#endif /* wad_kit_joint_interface_h */
