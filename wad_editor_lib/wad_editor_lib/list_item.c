//
//  list_item.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

void listItemInitialize(WE_LIST_ITEM* item, WE_LIST* list, WE_LIST_ITEM* parent, unsigned long flags, void* data, const char* title)
{
	assert(item);
	
	item->list = list;
	item->parent = parent;
	item->selected = 0;
	item->flags = flags;
	item->data = data;
	stringInitializeWithUTF8String(&item->title, title);
	magicArrayInitializeWithAllocator(&item->children, list->itemAllocator);
}

void listItemDeinitialize(WE_LIST_ITEM* listItem)
{
	for (unsigned int i = 0; i < listItem->children.length; i++)
	{
		WE_LIST_ITEM* item = magicArrayGetItem(&listItem->children, i);
		listItemDeinitialize(item);
	}
	magicArrayClear(&listItem->children);
	magicArrayDeinitialize(&listItem->children);
	
	stringDeinitialize(&listItem->title);
	debug_memset(listItem, 0, sizeof(WE_LIST_ITEM));
}

// MARK: - Public interface

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
	
	return magicArrayGetItemIndex(&listItem->parent->children, listItem);
}


void listItemSelect(WE_LIST_ITEM* listItem)
{
	assert(listItem);
	listSelectItem(listItem->list, listItem);
}


unsigned int listItemGetNumberOfChildren(WE_LIST_ITEM* listItem)
{
	assert(listItem);
	return listItem->children.length;
}

WE_LIST_ITEM* listItemGetChild(WE_LIST_ITEM* listItem, unsigned int itemIndex)
{
	assert(listItem);
	return magicArrayGetItem(&listItem->children, itemIndex);
}

WE_LIST_ITEM* listItemAddChild(WE_LIST_ITEM* listItem, unsigned long flags, void* data, const char* title)
{
	assert(listItem);
	
	WE_LIST_ITEM* child = magicArrayAddItem(&listItem->children);
	listItemInitialize(child, listItem->list, listItem, flags, data, title);
	
	child->list->delegate.itemAddedFunc(child->list, child, child->list->delegate.userInfo);
	
	return child;
}

void listItemRemoveChild(WE_LIST_ITEM* listItem, WE_LIST_ITEM* child)
{
	assert(listItem);
	
	child->list->delegate.itemWillBeRemovedFunc(child->list, child, child->list->delegate.userInfo);
	
	listItemDeinitialize(child);
	magicArrayRemoveItem(&listItem->children, child);
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
