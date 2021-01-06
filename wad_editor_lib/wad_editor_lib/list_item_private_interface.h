//
//  list_item_private_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef list_item_private_interface_h
#define list_item_private_interface_h

#include "private_interface.h"

void listItemInitialize(WE_LIST_ITEM* item, WE_LIST* list, WE_LIST_ITEM* parent, unsigned long flags, void* data, const char* title);
void listItemDeinitialize(WE_LIST_ITEM* listItem);

#endif /* list_item_private_interface_h */
