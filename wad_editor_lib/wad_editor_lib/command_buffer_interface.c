//
//  command_buffer_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void commandBufferRelease(COMMAND_BUFFER* commandBuffer)
{
	assert(commandBuffer);
	commandQueueReleaseCommandBuffer(commandBuffer->commandQueue, commandBuffer);
}


BLIT_COMMAND_ENCODER* commandBufferStartBlitCommandEncoder(COMMAND_BUFFER* commandBuffer)
{
	assert(commandBuffer);
	
	GRAPHICS_DEVICE* device = commandBuffer->commandQueue->device;
	void* blitCommandEncoderId = device->commandBufferStartBlitCommandEncoderFunc(commandBuffer);
	
	BLIT_COMMAND_ENCODER* encoder = arrayAddItem(&device->blitCommandEncoders);
	encoder->commandBuffer = commandBuffer;
	encoder->blitCommandEncoderId = blitCommandEncoderId;
	return encoder;
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
