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

void jointInitialize(WK_JOINT* joint, WK_MOVABLE* movable, WK_MESH* mesh, WK_WAD_LOAD_INFO* loadInfo);
void jointDeinitialize(WK_JOINT* joint);

void jointUpdateOffset(WK_JOINT* joint);

#endif /* joint_private_interface_h */
