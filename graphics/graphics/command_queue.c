//
//  command_queue.c
//  graphics
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

void commandQueueReleaseCommandBuffer(COMMAND_QUEUE* commandQueue, COMMAND_BUFFER* commandBuffer)
{
	assert(commandQueue);
	assert(commandBuffer);
	assert(commandBuffer->commandQueue == commandQueue);
	
	GR_DEVICE* device = commandQueue->device;
	device->commandQueueReleaseCommandBufferFunc(commandQueue, commandBuffer);
	magicArrayRemoveItem(&device->commandBuffers, commandBuffer);
}

// MARK: - Public interface

COMMAND_QUEUE* graphicsDeviceCreateCommandQueue(GR_DEVICE* device)
{
	assert(device);
	
	void* commandQueueId = device->createCommandQueueFunc(device);
	assert(commandQueueId);
	
	COMMAND_QUEUE* commandQueue = magicArrayAddItem(&device->commandQueues);
	commandQueue->device = device;
	commandQueue->commandQueueId = commandQueueId;
	return commandQueue;
}

void commandQueueRelease(COMMAND_QUEUE* commandQueue)
{
	assert(commandQueue);
	
	GR_DEVICE* device = commandQueue->device;
	device->releaseCommandQueueFunc(device, commandQueue);
	debug_memset(commandQueue, 0, sizeof(COMMAND_QUEUE));
	magicArrayRemoveItem(&device->commandQueues, commandQueue);
}
