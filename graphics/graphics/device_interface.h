//
//  device_interface.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_device_interface_h
#define graphics_device_interface_h

#include "graphics.h"

//GR_DEVICE* graphicsDeviceCreateDefault(/*callbacks etc...*/);
//void graphicsDeviceRelease(GR_DEVICE* graphicsDevice);

const char* graphicsDeviceGetName(GR_DEVICE* graphicsDevice);
void* graphicsDeviceGetUserInfo(GR_DEVICE* graphicsDevice);

void graphicsDeviceScheduleUpdateBuffers(GR_DEVICE* graphicsDevice, COMMAND_BUFFER* commandBuffer);

#endif /* graphics_device_interface_h */
