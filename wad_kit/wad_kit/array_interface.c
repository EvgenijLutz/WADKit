//
//  dynamic_array_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void arrayInitializeWithAllocator(WK_ARRAY* array, DATA_ALLOCATOR* allocator)
{
	assert(array);
	assert(allocator);
	
	array->length = 0;
	array->allocatorIsReferenced = 1;
	array->elements = malloc(sizeof(DATA_BLOCK_ITEM) * allocator->dataBlockCapacity * allocator->numDataBlocks);
	array->allocator = allocator;
}

void arrayInitializeWithCapacityIncrement(WK_ARRAY* array, unsigned int itemSize, unsigned int capacityIncrement)
{
	assert(array);
	assert(itemSize > 0);
	assert(capacityIncrement > 0);
	
	array->length = 0;
	array->allocatorIsReferenced = 0;
	array->elements = malloc(sizeof(DATA_BLOCK_ITEM) * capacityIncrement);
	array->allocator = dataAllocatorCreate(itemSize, capacityIncrement);
}

void arrayInitialize(WK_ARRAY* array, unsigned int itemSize)
{
	assert(array);
	arrayInitializeWithCapacityIncrement(array, itemSize, 8);
}

void arrayDeinitialize(WK_ARRAY* array)
{
	assert(array);
	
	if (array->allocatorIsReferenced)
	{
		for (unsigned long i = 0; i < array->length; i++)
		{
			DATA_BLOCK_ITEM* item = array->elements + i;
			dataAllocatorRemoveItem(array->allocator, item);
		}
	}
	else
	{
		dataAllocatorRelease(array->allocator);
	}
	free(array->elements);
	
	debug_memset(array, 0, sizeof(WK_ARRAY));
}


// How are you feeling today


void* arrayAddItem(WK_ARRAY* array)
{
	assert(array);
	
	DATA_ALLOCATOR* allocator = array->allocator;
	const long lastCapacity = allocator->dataBlockCapacity * allocator->numDataBlocks;
	DATA_BLOCK_ITEM item;
	dataAllocatorAddItem(allocator, &item);
	const long currentCapacity = allocator->dataBlockCapacity * allocator->numDataBlocks;
	if (currentCapacity != lastCapacity)
	{
		array->elements = realloc(array->elements, sizeof(DATA_BLOCK_ITEM) * currentCapacity);
		assert(array->elements);
	}
	
	array->elements[array->length] = item;
	array->length++;
	
	return item.data;
}

void* arrayGetItem(WK_ARRAY* array, unsigned int itemIndex)
{
	assert(array);
	assert(array->length > itemIndex);
	
	return array->elements[itemIndex].data;
}

unsigned int arrayGetItemIndex(WK_ARRAY* array, void* item)
{
	assert(array);
	
	for (unsigned int i = 0; i < array->length; i++)
	{
		if (array->elements[i].data == item)
		{
			return i;
		}
	}
	
	// Item is not found
	assert(0);
	return 0;
}

void arrayRemoveItemByIndex(WK_ARRAY* array, unsigned int itemIndex)
{
	assert(array);
	assert(itemIndex < array->length);
	
	DATA_BLOCK_ITEM* magicItem = &array->elements[itemIndex];
	dataAllocatorRemoveItem(array->allocator, magicItem);
	debug_memset(magicItem->data, 0, array->allocator->itemSize);
	
	// Shift elements
	for (unsigned int i = itemIndex + 1; i < array->length; i++)
	{
		array->elements[i - 1] = array->elements[i];
	}
	
	array->length--;
}

void arrayRemoveItem(WK_ARRAY* array, void* item)
{
	unsigned int itemIndex = arrayGetItemIndex(array, item);
	arrayRemoveItemByIndex(array, itemIndex);
	debug_memset(item, 0, array->allocator->itemSize);
}

void arrayClear(WK_ARRAY* array)
{
	assert(array);
	
	if (array->length == 0) {
		return;
	}
	
	if (array->allocatorIsReferenced)
	{
		for (unsigned long i = 0; i < array->length; i++)
		{
			DATA_BLOCK_ITEM* item = array->elements + i;
			dataAllocatorRemoveItem(array->allocator, item);
		}
	}
	else
	{
		dataAllocatorReset(array->allocator);
	}
	
	array->length = 0;
}
