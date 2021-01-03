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

/*!
 @abstract
 Dynamic array.
 @discussion
 Yaaay, reinvention on the wheel.
 */
typedef struct WK_ARRAY
{
	unsigned int length;
	int allocatorIsReferenced;
	DATA_BLOCK_ITEM* elements;
	DATA_ALLOCATOR* allocator;
}
WK_ARRAY;

#endif /* dynamic_array_h */
