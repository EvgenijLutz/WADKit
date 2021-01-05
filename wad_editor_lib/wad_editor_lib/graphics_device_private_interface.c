//
//  graphics_device_private_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "graphics_device_private_interface.h"

void graphicsDeviceReleaseCommandQueue(GRAPHICS_DEVICE* device, COMMAND_QUEUE* commandQueue)
{
	assert(device);
	assert(commandQueue);
	assert(commandQueue->device == device);
	
	device->releaseCommandQueueFunc(device, commandQueue);
	debug_memset(commandQueue, 0, sizeof(COMMAND_QUEUE));
	free(commandQueue);
}
