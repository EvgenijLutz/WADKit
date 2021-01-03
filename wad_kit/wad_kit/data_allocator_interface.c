//
//  data_allocator_interface.c
//  Foundation
//
//  Created by Евгений Лютц on 01.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

static void _dataBlock_initialize(DATA_BLOCK* dataBlock, DATA_ALLOCATOR* allocator)
{
	dataBlock->firstAvailableItem = 0;
	dataBlock->engagedItems = malloc(sizeof(char) * allocator->dataBlockCapacity);
	memset(dataBlock->engagedItems, 0, sizeof(char) * allocator->dataBlockCapacity);
	dataBlock->data = malloc(allocator->itemSize * allocator->dataBlockCapacity);
	debug_memset(dataBlock->data, 0, allocator->itemSize * allocator->dataBlockCapacity);
}

static void _dataBlock_deinitialize(DATA_BLOCK* dataBlock)
{
	free(dataBlock->data);
	free(dataBlock->engagedItems);
	debug_memset(dataBlock, 0, sizeof(DATA_BLOCK));
}

DATA_ALLOCATOR* dataAllocatorCreate(unsigned int itemSize, unsigned int dataBlockCapacity)
{
	DATA_ALLOCATOR* allocator = malloc(sizeof(DATA_ALLOCATOR));
	dataAllocatorInitialize(allocator, itemSize, dataBlockCapacity);
	return allocator;
}

void dataAllocatorInitialize(DATA_ALLOCATOR* allocator, unsigned int itemSize, unsigned int dataBlockCapacity)
{
	assert(allocator);
	assert(itemSize > 0);
	assert(dataBlockCapacity > 0);
	
	allocator->itemSize = itemSize;
	allocator->dataBlockCapacity = dataBlockCapacity;
	allocator->numDataBlocks = 1;
	
	DATA_BLOCK* container = &allocator->dataBlocksContainer;
	container->firstAvailableItem = 0;
	container->engagedItems = malloc(sizeof(char) * allocator->numDataBlocks);
	memset(container->engagedItems, 0, sizeof(char) * allocator->numDataBlocks);
	container->dataBlocks = malloc(sizeof(DATA_BLOCK) * allocator->numDataBlocks);
	for (unsigned int i = 0; i < allocator->numDataBlocks; i++)
	{
		DATA_BLOCK* dataBlock = container->data + i;
		_dataBlock_initialize(dataBlock, allocator);
	}
}

void dataAllocatorDeinitialize(DATA_ALLOCATOR* allocator)
{
	assert(allocator);
	
	for (unsigned int i = 0; i < allocator->numDataBlocks; i++)
	{
		DATA_BLOCK* dataBlock = &((DATA_BLOCK*)allocator->dataBlocksContainer.data)[i];
		_dataBlock_deinitialize(dataBlock);
	}
	_dataBlock_deinitialize(&allocator->dataBlocksContainer);
}

void dataAllocatorRelease(DATA_ALLOCATOR* allocator)
{
	assert(allocator);
	dataAllocatorDeinitialize(allocator);
	free(allocator);
}


void dataAllocatorAddItem(DATA_ALLOCATOR* allocator, DATA_BLOCK_ITEM* outItemData)
{
	assert(allocator);
	assert(outItemData);
	
	DATA_BLOCK* container = &allocator->dataBlocksContainer;
	
	if (allocator->numDataBlocks == allocator->dataBlocksContainer.firstAvailableItem)
	{
		const unsigned long lastItemIndex = allocator->numDataBlocks;
		allocator->numDataBlocks++;
		
		container->engagedItems = realloc(container->engagedItems, sizeof(char) * allocator->numDataBlocks);
		assert(container->engagedItems);
		container->engagedItems[lastItemIndex] = 0;
		
		container->dataBlocks = realloc(container->dataBlocks, sizeof(DATA_BLOCK) * allocator->numDataBlocks);
		assert(container->dataBlocks);
		DATA_BLOCK* dataBlock = container->dataBlocks + lastItemIndex;
		_dataBlock_initialize(dataBlock, allocator);
	}
	
	DATA_BLOCK* dataBlock = container->dataBlocks + container->firstAvailableItem;
	outItemData->dataBlockIndex = container->firstAvailableItem;
	outItemData->itemIndex = dataBlock->firstAvailableItem;
	unsigned long dataIndex = dataBlock->firstAvailableItem * allocator->itemSize;
	outItemData->data = ((char*)dataBlock->data) + dataIndex;
	
	dataBlock->engagedItems[dataBlock->firstAvailableItem] = 1;
	while (dataBlock->firstAvailableItem < allocator->dataBlockCapacity && dataBlock->engagedItems[dataBlock->firstAvailableItem])
	{
		dataBlock->firstAvailableItem++;
	}
	if (dataBlock->firstAvailableItem == allocator->dataBlockCapacity)
	{
		container->engagedItems[container->firstAvailableItem] = 1;
		while (container->firstAvailableItem < allocator->numDataBlocks && container->engagedItems[container->firstAvailableItem])
		{
			container->firstAvailableItem++;
		}
	}
}

void dataAllocatorRemoveItem(DATA_ALLOCATOR* allocator, DATA_BLOCK_ITEM* itemData)
{
	assert(allocator);
	assert(itemData);
	assert(itemData->dataBlockIndex < allocator->numDataBlocks);
	assert(itemData->itemIndex < allocator->dataBlockCapacity);
	
	DATA_BLOCK* container = &allocator->dataBlocksContainer;
	DATA_BLOCK* dataBlock = &container->dataBlocks[itemData->dataBlockIndex];
	assert(dataBlock->engagedItems[itemData->itemIndex]);
	
	dataBlock->engagedItems[itemData->itemIndex] = 0;
	if (itemData->itemIndex < dataBlock->firstAvailableItem) {
		dataBlock->firstAvailableItem = itemData->itemIndex;
	}
	
	container->engagedItems[itemData->dataBlockIndex] = 0;
	if (itemData->dataBlockIndex < container->firstAvailableItem) {
		container->firstAvailableItem = itemData->dataBlockIndex;
	}
}


void dataAllocatorReset(DATA_ALLOCATOR* allocator)
{
	assert(allocator);
	
	DATA_BLOCK* container = &allocator->dataBlocksContainer;
	for (unsigned long i = 0; i < allocator->numDataBlocks; i++)
	{
		DATA_BLOCK* dataBlock = &((DATA_BLOCK*)container->data)[i];
		dataBlock->firstAvailableItem = 0;
		memset(dataBlock->engagedItems, 0, sizeof(char) * allocator->dataBlockCapacity);
		debug_memset(dataBlock->data, 0, allocator->itemSize * allocator->dataBlockCapacity);
	}
	allocator->dataBlocksContainer.firstAvailableItem = 0;
	memset(container->engagedItems, 0, sizeof(char) * allocator->numDataBlocks);
	debug_memset(container->dataBlocks, 0, sizeof(DATA_BLOCK) * allocator->numDataBlocks);
}
