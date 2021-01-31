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

WK_MESH* jointGetMesh(WK_JOINT* joint);
JOINT_LOCATION_TYPE jointGetLocationType(WK_JOINT* joint);
int jointGetRawOffsetX(WK_JOINT* joint);
int jointGetRawOffsetY(WK_JOINT* joint);
int jointGetRawOffsetZ(WK_JOINT* joint);
vector3f jointGetOffset(WK_JOINT* joint);

void jointSetOffset(WK_JOINT* joint, vector3f offset);

#endif /* wad_kit_joint_interface_h */
