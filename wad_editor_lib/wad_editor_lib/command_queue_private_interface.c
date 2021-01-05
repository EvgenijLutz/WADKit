//
//  command_queue_private_interface.c
//  wad_editor_lib_macos
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void commandQueueReleaseCommandBuffer(COMMAND_QUEUE* commandQueue, COMMAND_BUFFER* commandBuffer)
{
	assert(commandQueue);
	assert(commandBuffer);
	assert(commandBuffer->commandQueue == commandQueue);
	
	GRAPHICS_DEVICE* device = commandQueue->device;
	device->commandQueueReleaseCommandBufferFunc(commandQueue, commandBuffer);
	arrayRemoveItem(&device->commandBuffers, commandBuffer);
}
