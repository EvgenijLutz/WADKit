//
//  command_queue_interface.c
//  graphics
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

COMMAND_QUEUE* graphicsDeviceCreateCommandQueue(GRAPHICS_DEVICE* device)
{
	assert(device);
	
	void* commandQueueId = device->createCommandQueueFunc(device);
	assert(commandQueueId);
	
	COMMAND_QUEUE* commandQueue = arrayAddItem(&device->commandQueues);
	commandQueue->device = device;
	commandQueue->commandQueueId = commandQueueId;
	return commandQueue;
}

void commandQueueRelease(COMMAND_QUEUE* commandQueue)
{
	assert(commandQueue);
	
	GRAPHICS_DEVICE* device = commandQueue->device;
	device->releaseCommandQueueFunc(device, commandQueue);
	debug_memset(commandQueue, 0, sizeof(COMMAND_QUEUE));
	arrayRemoveItem(&device->commandQueues, commandQueue);
}
