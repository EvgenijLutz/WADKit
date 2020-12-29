//
//  dynamic_array.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef dynamic_array_h
#define dynamic_array_h

#include "wad_kit.h"

typedef struct WK_ARRAY_ITEM
{
	void* data;
}
WK_ARRAY_ITEM;

/*!
 Yaaay, reinvention on the wheel
 */
typedef struct WK_ARRAY
{
	unsigned int capacity;
	unsigned int length;
	unsigned int itemSize;
	unsigned int capacityIncrement;
	WK_ARRAY_ITEM* items;
}
WK_ARRAY;

#endif /* dynamic_array_h */
