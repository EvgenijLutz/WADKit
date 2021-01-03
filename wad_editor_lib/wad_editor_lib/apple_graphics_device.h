//
//  apple_graphics_device.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef apple_graphics_device_h
#define apple_graphics_device_h

#include "private_interface.h"

GRAPHICS_DEVICE* apple_createDefaultGraphicsDevice(void);
void apple_releaseDefaultGraphicsDevice(GRAPHICS_DEVICE* graphicsDevice);

#endif /* apple_graphics_device_h */
