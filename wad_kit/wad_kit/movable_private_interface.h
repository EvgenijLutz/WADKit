//
//  movable_private_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef movable_private_interface_h
#define movable_private_interface_h

#include "private_interface.h"

void movableInitialize(WK_MOVABLE* movable, RAW_MOVABLE* rawMovable, WK_WAD_LOAD_INFO* loadInfo);
void movableDeinitialize(WK_MOVABLE* movable);

#endif /* movable_private_interface_h */
