//
//  wad_item.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 16.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_item_h
#define wad_editor_lib__wad_editor_item_h

#include "wad_editor_lib.h"

typedef struct WAD_EDITOR_ITEM
{
	WAD_EDITOR_ITEM_TYPE type;
	WAD_EDITOR_ITEM_DATA data;
	char* name;
	
	WAD_EDITOR_ITEM* parent;
	
	unsigned int numChildren;
	unsigned int childrenCapacity;
	WAD_EDITOR_ITEM** children;
}
WAD_EDITOR_ITEM;

#endif /* wad_editor_lib__wad_editor_item_h */
