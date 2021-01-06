//
//  list_item_interface.h
//  wad_editor_lib_macos
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef list_item_interface_h
#define list_item_interface_h

#include "wad_editor_lib.h"

WE_LIST_ITEM* listItemGetParent(WE_LIST_ITEM* listItem);
unsigned long listItemGetFlags(WE_LIST_ITEM* listItem);
void* listItemGetData(WE_LIST_ITEM* listItem);
const char* listItemGetTitle(WE_LIST_ITEM* listItem);
unsigned int listItemGetIndex(WE_LIST_ITEM* listItem);

unsigned int listItemGetNumberOfChildren(WE_LIST_ITEM* listItem);
WE_LIST_ITEM* listItemGetChild(WE_LIST_ITEM* listItem, unsigned int itemIndex);
WE_LIST_ITEM* listItemAddChild(WE_LIST_ITEM* listItem, unsigned long flags, void* data, const char* title);
void listItemRemoveChild(WE_LIST_ITEM* listItem, WE_LIST_ITEM* child);
void listItemClearChildren(WE_LIST_ITEM* listItem);

#endif /* list_item_interface_h */
