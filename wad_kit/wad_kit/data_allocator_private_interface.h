//
//  data_allocator_private_interface.h
//  Foundation
//
//  Created by Евгений Лютц on 01.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef data_allocator_private_interface_h
#define data_allocator_private_interface_h

#include "private_interface.h"

void dataAllocatorInitialize(DATA_ALLOCATOR* allocator, unsigned int itemSize, unsigned int dataBlockCapacity);
void dataAllocatorDeinitialize(DATA_ALLOCATOR* allocator);

#endif /* data_allocator_private_interface_h */
