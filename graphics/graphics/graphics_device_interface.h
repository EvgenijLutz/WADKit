//
//  graphics_device_interface.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_device_interface_h
#define graphics_device_interface_h

#include "graphics.h"

//GRAPHICS_DEVICE* graphicsDeviceCreateDefault(/*callbacks etc...*/);
//void graphicsDeviceRelease(GRAPHICS_DEVICE* graphicsDevice);

const char* graphicsDeviceGetName(GRAPHICS_DEVICE* graphicsDevice);
void* graphicsDeviceGetUserInfo(GRAPHICS_DEVICE* graphicsDevice);

#endif /* graphics_device_interface_h */
