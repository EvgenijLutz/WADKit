//
//  graphics_device_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_device_interface_h
#define graphics_device_interface_h

#include "wad_editor_lib.h"

GRAPHICS_DEVICE* graphicsDeviceCreateDefault(void);
void graphicsDeviceRelease(GRAPHICS_DEVICE* graphicsDevice);

const char* graphicsDeviceGetName(GRAPHICS_DEVICE* graphicsDevice);
void* graphicsDeviceGetUserInfo(GRAPHICS_DEVICE* graphicsDevice);

TEXTURE2D* graphicsDeviceCreateTexture2d(GRAPHICS_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, const void* data);
void graphicsDeviceReleaseTexture2d(GRAPHICS_DEVICE* device, TEXTURE2D* texture);

#endif /* graphics_device_interface_h */
