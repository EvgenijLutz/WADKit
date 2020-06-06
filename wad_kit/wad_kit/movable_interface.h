//
//  movable_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_movable_interface_h
#define wad_kit_movable_interface_h

#include "movable_id.h"

typedef struct MOVABLE MOVABLE;

MOVABLE_ID movableGetId(MOVABLE* movable);
const char* movableIdGetName(MOVABLE_ID movableId);

#endif /* wad_kit_movable_interface_h */
