//
//  joint_private_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef joint_private_interface_h
#define joint_private_interface_h

#include "private_interface.h"

#define JOINT_COORDINATE_MULTIPLIER	1024.0f

void jointInitialize(JOINT* joint, MOVABLE* movable, MESH* mesh, WK_WAD_LOAD_INFO* loadInfo);
void jointDeinitialize(JOINT* joint);

void jointUpdateOffset(JOINT* joint);

#endif /* joint_private_interface_h */
