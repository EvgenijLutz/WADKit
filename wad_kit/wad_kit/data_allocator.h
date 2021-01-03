//
//  data_allocator.h
//  Foundation
//
//  Created by Евгений Лютц on 01.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef data_allocator_h
#define data_allocator_h

#include "private_interface.h"

typedef struct DATA_BLOCK
{
	unsigned int firstAvailableItem;
	char* engagedItems;
	union
	{
		void* data;
		DATA_BLOCK* dataBlocks;
	};
}
DATA_BLOCK;

typedef struct DATA_ALLOCATOR
{
	unsigned int itemSize;
	unsigned int dataBlockCapacity;
	unsigned int numDataBlocks;
	DATA_BLOCK dataBlocksContainer;
}
DATA_ALLOCATOR;

#endif /* data_allocator_h */
