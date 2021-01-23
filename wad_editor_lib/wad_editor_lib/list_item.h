//
//  list_item.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef list_item_h
#define list_item_h

#include "private_interface.h"

typedef struct WE_LIST_ITEM
{
	WE_LIST* list;
	WE_LIST_ITEM* parent;
	
	int selected;
	
	unsigned long flags;
	void* data;
	WK_STRING title;
	
	WK_ARRAY children;
}
WE_LIST_ITEM;

#endif /* list_item_h */
