//
//  graphics_buffer_interface.c
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void graphicsBufferRelease(GRAPHICS_BUFFER* buffer)
{
	assert(buffer);
	
	GRAPHICS_DEVICE* device = buffer->device;
	device->graphicsBufferReleaseFunc(device, buffer->bufferId);
	arrayRemoveItem(&device->buffers, buffer);
}

void graphicsBufferSetLabel(GRAPHICS_BUFFER* buffer, const char* text)
{
	assert(buffer);
	
	buffer->device->graphicsBufferSetLabelFunc(buffer, text);
}

void* graphicsBufferGetDataToCPUWrite(GRAPHICS_BUFFER* buffer)
{
	assert(buffer);
	assert(buffer->option == GRAPHICS_BUFFER_OPTION_CPU_READ_WRITE);
	
	return buffer->device->graphicsBufferGetDataToCPUWriteFunc(buffer);
}
