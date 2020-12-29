//
//  dynamic_array_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"


static void _array_initializeItems(WK_ARRAY* array, unsigned int startIndex)
{
	for (unsigned int i = startIndex; i < array->capacity; i++)
	{
		//  forgive me for this |
		//                      |
		//                     \|/
		//                      '
		array->items[i].data = malloc(array->itemSize);
	}
}

static void _array_deinitializeItems(WK_ARRAY* array)
{
	for (unsigned int i = 0; i < array->capacity; i++)
	{
		free(array->items[i].data);
	}
}

static void _array_grow(WK_ARRAY* array)
{
	unsigned int oldCapacity = array->capacity;
	
	array->capacity += array->capacityIncrement;
	array->items = realloc(array->items, sizeof(WK_ARRAY_ITEM) * array->capacity);
	assert(array->items);
	_array_initializeItems(array, oldCapacity);
}


void arrayInitializeWithCapacityIncrement(WK_ARRAY* array, unsigned int itemSize, unsigned int capacityIncrement)
{
	assert(array);
	assert(itemSize > 0);
	assert(capacityIncrement > 0);
	
	array->capacity = capacityIncrement;
	array->length = 0;
	array->itemSize = itemSize;
	array->capacityIncrement = capacityIncrement;
	array->items = malloc(sizeof(WK_ARRAY_ITEM) * array->capacity);
	_array_initializeItems(array, 0);
}

void arrayInitialize(WK_ARRAY* array, unsigned int itemSize)
{
	assert(array);
	arrayInitializeWithCapacityIncrement(array, itemSize, 8);
}

void arrayDeinitialize(WK_ARRAY* array)
{
	assert(array);
	
	_array_deinitializeItems(array);
	free(array->items);
#if DEBUG
	memset(array, 0, sizeof(WK_ARRAY));
#endif
}


void* arrayAddItem(WK_ARRAY* array)
{
	assert(array);
	
	if (array->length + 1 == array->capacity)
	{
		_array_grow(array);
	}
	
	array->length += 1;
	return array->items[array->length - 1].data;
}

void* arrayGetItem(WK_ARRAY* array, unsigned int itemIndex)
{
	assert(array);
	assert(array->length > itemIndex);
	return array->items[itemIndex].data;
}

unsigned int arrayGetItemIndex(WK_ARRAY* array, void* item)
{
	assert(array);
	
	for (unsigned int i = 0; i < array->length; i++)
	{
		if (array->items[i].data == item)
		{
			return i;
		}
	}
	
	// Item is not found
	assert(0);
	return 0;
}
