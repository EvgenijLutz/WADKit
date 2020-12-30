//
//  movable_internal_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef movable_internal_interface_h
#define movable_internal_interface_h

#include "private_interface.h"

void movableInitialize(MOVABLE* movable, RAW_MOVABLE* rawMovable, WK_WAD_LOAD_INFO* loadInfo);
void movableDeinitialize(MOVABLE* movable);

#endif /* movable_internal_interface_h */
