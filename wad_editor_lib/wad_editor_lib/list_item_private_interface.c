//
//  list_item_private_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void listItemInitialize(WE_LIST_ITEM* item, WE_LIST* list, WE_LIST_ITEM* parent, unsigned long flags, void* data, const char* title)
{
	assert(item);
	
	item->list = list;
	item->parent = parent;
	item->selected = 0;
	item->flags = flags;
	item->data = data;
	stringInitialize(&item->title, title);
	arrayInitializeWithAllocator(&item->children, list->itemAllocator);
}

void listItemDeinitialize(WE_LIST_ITEM* listItem)
{
	for (unsigned int i = 0; i < listItem->children.length; i++)
	{
		WE_LIST_ITEM* item = arrayGetItem(&listItem->children, i);
		listItemDeinitialize(item);
	}
	arrayClear(&listItem->children);
	arrayDeinitialize(&listItem->children);
	
	stringDeinitialize(&listItem->title);
	debug_memset(listItem, 0, sizeof(WE_LIST_ITEM));
}
