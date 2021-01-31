//
//  device.c
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

const char* graphicsDeviceGetName(GR_DEVICE* graphicsDevice)
{
	assert(graphicsDevice);
	return stringGetData(&graphicsDevice->name);
}

void* graphicsDeviceGetUserInfo(GR_DEVICE* graphicsDevice)
{
	assert(graphicsDevice);
	return graphicsDevice->userInfo;
}


void graphicsDeviceScheduleUpdateBuffers(GR_DEVICE* device, COMMAND_BUFFER* commandBuffer)
{
	assert(device);
	assert(commandBuffer);
	
	device->scheduleUploadBuffersFunc(device, commandBuffer);
}
