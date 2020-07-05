//
//  wad_editor_item_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 16.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "wad_editor_item_interface.h"
#include "wad_editor_item_private_interface.h"
#include "wad_editor_item.h"
#include "common.h"
#include <string.h>

// MARK: - Private interface

WAD_EDITOR_ITEM* wadEditorItemCreate(WAD_EDITOR_ITEM_TYPE type, WAD_EDITOR_ITEM_DATA data, const char* name)
{
	WAD_EDITOR_ITEM* item = malloc(sizeof(WAD_EDITOR_ITEM));
	item->type = type;
	item->data = data;
	
	item->name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(item->name, name);
	
	item->parent = NULL;
	item->numChildren = 0;
	item->childrenCapacity = 8;
	item->children = malloc(sizeof(WAD_EDITOR_ITEM*) * item->childrenCapacity);
	
	return item;
}

void wadEditorItemRelease(WAD_EDITOR_ITEM* item)
{
	assert(item);
	
	for (unsigned int childIndex = 0; childIndex < item->numChildren; childIndex++)
	{
		wadEditorItemRelease(item->children[childIndex]);
	}
	free(item->children);
	
	free(item->name);
	free(item);
}

void wadEditorItemRemoveAllChildren(WAD_EDITOR_ITEM* item)
{
	assert(item);
	for (unsigned int childIndex = 0; childIndex < item->numChildren; childIndex++)
	{
		wadEditorItemRelease(item->children[childIndex]);
	}
	item->numChildren = 0;
}

WAD_EDITOR_ITEM* wadEditorItemAddChild(WAD_EDITOR_ITEM* item, WAD_EDITOR_ITEM_TYPE type, WAD_EDITOR_ITEM_DATA data, const char* name)
{
	assert(item);
	
	if (item->numChildren + 1 >= item->childrenCapacity)
	{
		item->childrenCapacity += 64;
		WAD_EDITOR_ITEM** newChildren = realloc(item->children, sizeof(WAD_EDITOR_ITEM*) * item->childrenCapacity);
		assert(newChildren);
		item->children = newChildren;
	}
	
	WAD_EDITOR_ITEM* child = wadEditorItemCreate(type, data, name);
	child->parent = item;
	item->children[item->numChildren] = child;
	item->numChildren++;
	
	return child;
}

// MARK: - Public interface

WAD_EDITOR_ITEM_TYPE wadEditorItemGetType(WAD_EDITOR_ITEM* item)
{
	assert(item);
	return item->type;
}

const char* wadEditorItemGetName(WAD_EDITOR_ITEM* item)
{
	assert(item);
	return item->name;
}

WAD_EDITOR_ITEM_DATA wadEditorItemGetData(WAD_EDITOR_ITEM* item)
{
	assert(item);
	return item->data;
}

WAD_EDITOR_ITEM* wadEditorItemGetParent(WAD_EDITOR_ITEM* item)
{
	assert(item);
	return item->parent;
}

unsigned int wadEditorItemGetNumChildren(WAD_EDITOR_ITEM* item)
{
	assert(item);
	return item->numChildren;
}

WAD_EDITOR_ITEM* wadEditorItemGetChild(WAD_EDITOR_ITEM* item, unsigned int childIndex)
{
	assert(item);
	assert(childIndex < item->numChildren);
	return item->children[childIndex];
}
