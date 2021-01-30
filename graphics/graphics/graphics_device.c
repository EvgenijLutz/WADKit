//
//  graphics_device.c
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

const char* graphicsDeviceGetName(GRAPHICS_DEVICE* graphicsDevice)
{
	assert(graphicsDevice);
	return stringGetData(&graphicsDevice->name);
}

void* graphicsDeviceGetUserInfo(GRAPHICS_DEVICE* graphicsDevice)
{
	assert(graphicsDevice);
	return graphicsDevice->userInfo;
}
