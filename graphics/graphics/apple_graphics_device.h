//
//  apple_graphics_device.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef apple_graphics_device_h
#define apple_graphics_device_h

#include "graphics_apple.h"

GRAPHICS_DEVICE* graphicsDeviceCreateDefaultMetalDevice(void);
void graphicsDeviceReleaseDefaultMetalDevice(GRAPHICS_DEVICE* graphicsDevice);

#endif /* apple_graphics_device_h */
