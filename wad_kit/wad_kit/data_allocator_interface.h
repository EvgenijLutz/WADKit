//
//  data_allocator_interface.h
//  Foundation
//
//  Created by Евгений Лютц on 01.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef data_allocator_interface_h
#define data_allocator_interface_h

#include "wad_kit.h"

DATA_ALLOCATOR* dataAllocatorCreate(unsigned int itemSize, unsigned int dataBlockCapacity);
void dataAllocatorRelease(DATA_ALLOCATOR* allocator);

void dataAllocatorAddItem(DATA_ALLOCATOR* allocator, DATA_BLOCK_ITEM* outItemData);
void dataAllocatorRemoveItem(DATA_ALLOCATOR* allocator, DATA_BLOCK_ITEM* itemData);

void dataAllocatorReset(DATA_ALLOCATOR* allocator);

#endif /* data_allocator_interface_h */
