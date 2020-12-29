//
//  dynamic_array_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef dynamic_array_interface_h
#define dynamic_array_interface_h

#include "wad_kit.h"

void arrayInitializeWithCapacityIncrement(WK_ARRAY* array, unsigned int itemSize, unsigned int capacityIncrement);
void arrayInitialize(WK_ARRAY* array, unsigned int itemSize);
void arrayDeinitialize(WK_ARRAY* array);

void* arrayAddItem(WK_ARRAY* array);
void* arrayGetItem(WK_ARRAY* array, unsigned int itemIndex);
unsigned int arrayGetItemIndex(WK_ARRAY* array, void* item);

#endif /* dynamic_array_interface_h */
