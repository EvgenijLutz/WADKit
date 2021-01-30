//
//  command_buffer.c
//  graphics
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

COMMAND_BUFFER* commandQueueCreateCommandBuffer(COMMAND_QUEUE* commandQueue)
{
	assert(commandQueue);
	assert(commandQueue->device);
	
	GRAPHICS_DEVICE* device = commandQueue->device;
	
	semaphoreEnter(device->accessSemaphore);
	
	void* commandBufferId = device->commandQueueCreateCommandBufferFunc(commandQueue);
	if (commandBufferId == NULL)
	{
		semaphoreLeave(device->accessSemaphore);
		return NULL;
	}
	
	COMMAND_BUFFER* commandBuffer = magicArrayAddItem(&device->commandBuffers);
	commandBuffer->commandQueue = commandQueue;
	commandBuffer->commandBufferId = commandBufferId;
	
	semaphoreLeave(device->accessSemaphore);
	
	return commandBuffer;
}

void commandBufferRelease(COMMAND_BUFFER* commandBuffer)
{
	assert(commandBuffer);
	//commandQueueReleaseCommandBuffer(commandBuffer->commandQueue, commandBuffer);
	
	COMMAND_QUEUE* commandQueue = commandBuffer->commandQueue;
	GRAPHICS_DEVICE* device = commandQueue->device;
	
	semaphoreEnter(device->accessSemaphore);
	
	device->commandQueueReleaseCommandBufferFunc(commandQueue, commandBuffer);
	magicArrayRemoveItem(&device->commandBuffers, commandBuffer);
	
	semaphoreLeave(device->accessSemaphore);
}

void commandBufferScheduleDisplayDrawable(COMMAND_BUFFER* commandBuffer, GRAPHICS_DRAWABLE* drawable)
{
	assert(commandBuffer);
	assert(drawable);
	
	commandBuffer->commandQueue->device->commandBufferPresentDrawableFunc(commandBuffer, drawable);
}


void commandBufferAddCompletionHandler(COMMAND_BUFFER* commandBuffer, COMMAND_BUFFER_COMPLETION_FUNC completionHandler, void* userInfo)
{
	assert(commandBuffer);
	assert(completionHandler);
	commandBuffer->commandQueue->device->commandBufferAddCompletionFunc(commandBuffer, completionHandler, userInfo);
}

void commandBufferWaitUntilCompleted(COMMAND_BUFFER* commandBuffer)
{
	assert(commandBuffer);
	commandBuffer->commandQueue->device->commandBufferWaitUntilCompletedFunc(commandBuffer);
}

void commandBufferCommit(COMMAND_BUFFER* commandBuffer)
{
	assert(commandBuffer);
	commandBuffer->commandQueue->device->commandBufferCommitFunc(commandBuffer);
}
