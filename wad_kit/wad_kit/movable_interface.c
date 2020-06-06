//
//  movable_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "movable_interface.h"
#include "movable.h"
#include <stdlib.h>
#include <assert.h>


MOVABLE_ID movableGetId(MOVABLE* movable)
{
	assert(movable);
	return movable->movableId;
}
