//
//  graphics_buffer.c
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

/*static void _graphicsBuffer_subscribeIfNeeded(GRAPHICS_BUFFER* buffer)
{
	//
}*/

void graphicsBufferRelease(GRAPHICS_BUFFER* buffer)
{
	assert(buffer);
	
	//GRAPHICS_DEVICE* device = buffer->device;
	//device->graphicsBufferReleaseFunc(device, buffer->bufferId);
	//magicArrayRemoveItem(&device->buffers, buffer);
}

void graphicsBufferSetLabel(GRAPHICS_BUFFER* buffer, const char* text)
{
	assert(buffer);
	
	//buffer->device->graphicsBufferSetLabelFunc(buffer, text);
}
