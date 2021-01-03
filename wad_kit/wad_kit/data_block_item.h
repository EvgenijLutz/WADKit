//
//  data_block_item.h
//  Foundation
//
//  Created by Евгений Лютц on 01.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef data_block_item_h
#define data_block_item_h

#include "wad_kit.h"

typedef struct DATA_BLOCK_ITEM
{
	unsigned int dataBlockIndex;
	unsigned int itemIndex;
	void* data;
}
DATA_BLOCK_ITEM;

#endif /* data_block_item_h */
