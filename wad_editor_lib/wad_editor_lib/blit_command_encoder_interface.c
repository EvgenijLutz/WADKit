//
//  blit_command_encoder_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void blitCommandEncoderEndEncoding(BLIT_COMMAND_ENCODER* blitCommandEncoder)
{
	assert(blitCommandEncoder);
	GRAPHICS_DEVICE* device = blitCommandEncoder->commandBuffer->commandQueue->device;
	device->blitCommandEncoderEndEncodingFunc(blitCommandEncoder);
	
	arrayRemoveItem(&device->blitCommandEncoders, blitCommandEncoder);
}

void blitCommandEncoderScheduleCopyFromBufferToBuffer(BLIT_COMMAND_ENCODER* blitCommandEncoder, GRAPHICS_BUFFER* sourceBuffer, unsigned long sourceOffset, GRAPHICS_BUFFER* destinationBuffer, unsigned long destinationOffset, unsigned long size)
{
	assert(blitCommandEncoder);
	assert(sourceBuffer);
	assert(destinationBuffer);
	
	blitCommandEncoder->commandBuffer->commandQueue->device->blitCommandEncoderCopyFromBufferToBuffer(blitCommandEncoder, sourceBuffer, sourceOffset, destinationBuffer, destinationOffset, size);
}
