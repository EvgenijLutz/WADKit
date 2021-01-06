//
//  list_item_interface.c
//  wad_editor_lib_macos
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

WE_LIST_ITEM* listItemGetParent(WE_LIST_ITEM* listItem)
{
	assert(listItem);
	return listItem->parent;
}

unsigned long listItemGetFlags(WE_LIST_ITEM* listItem)
{
	assert(listItem);
	return listItem->flags;
}

void* listItemGetData(WE_LIST_ITEM* listItem)
{
	assert(listItem);
	return listItem->data;
}

const char* listItemGetTitle(WE_LIST_ITEM* listItem)
{
	assert(listItem);
	return stringGetData(&listItem->title);
}

unsigned int listItemGetIndex(WE_LIST_ITEM* listItem)
{
	assert(listItem);
	
	if (listItem->parent == NULL)
	{
		return 0;
	}
	
	return arrayGetItemIndex(&listItem->parent->children, listItem);
}


unsigned int listItemGetNumberOfChildren(WE_LIST_ITEM* listItem)
{
	assert(listItem);
	return listItem->children.length;
}

WE_LIST_ITEM* listItemGetChild(WE_LIST_ITEM* listItem, unsigned int itemIndex)
{
	assert(listItem);
	return arrayGetItem(&listItem->children, itemIndex);
}

WE_LIST_ITEM* listItemAddChild(WE_LIST_ITEM* listItem, unsigned long flags, void* data, const char* title)
{
	assert(listItem);
	
	WE_LIST_ITEM* child = arrayAddItem(&listItem->children);
	listItemInitialize(child, listItem->list, listItem, flags, data, title);
	
	child->list->delegate.itemAddedFunc(child->list, child, child->list->delegate.userInfo);
	
	return child;
}

void listItemRemoveChild(WE_LIST_ITEM* listItem, WE_LIST_ITEM* child)
{
	assert(listItem);
	
	child->list->delegate.itemWillBeRemovedFunc(child->list, child, child->list->delegate.userInfo);
	
	listItemDeinitialize(child);
	arrayRemoveItem(&listItem->children, child);
}

void listItemClearChildren(WE_LIST_ITEM* listItem)
{
	unsigned int numChildren = listItem->children.length;
	for (unsigned int i = 0; i < numChildren; i++)
	{
		WE_LIST_ITEM* item = listItemGetChild(listItem, numChildren - i - 1);
		listItemRemoveChild(listItem, item);
	}
}
