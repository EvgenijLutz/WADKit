//
//  command_queue_interface.c
//  graphics
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void commandQueueRelease(COMMAND_QUEUE* commandQueue)
{
	assert(commandQueue);
	
	GRAPHICS_DEVICE* device = commandQueue->device;
	device->releaseCommandQueueFunc(device, commandQueue);
	debug_memset(commandQueue, 0, sizeof(COMMAND_QUEUE));
	arrayRemoveItem(&device->commandQueues, commandQueue);
}


COMMAND_BUFFER* commandQueueCreateCommandBuffer(COMMAND_QUEUE* commandQueue)
{
	assert(commandQueue);
	assert(commandQueue->device);
	
	GRAPHICS_DEVICE* device = commandQueue->device;
	void* commandBufferId = device->commandQueueCreateCommandBufferFunc(commandQueue);
	COMMAND_BUFFER* commandBuffer = arrayAddItem(&device->commandBuffers);
	commandBuffer->commandQueue = commandQueue;
	commandBuffer->commandBufferId = commandBufferId;
	return commandBuffer;
}
