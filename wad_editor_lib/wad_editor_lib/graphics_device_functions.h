//
//  graphics_device_functions.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_device_functions_h
#define graphics_device_functions_h

#include "wad_editor_lib.h"

typedef void* (* GRAPHICS_DEVICE_CREATE_TEXTURE2D_FUNC)(GRAPHICS_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, const void* data);
typedef void (* GRAPHICS_DEVICE_RELEASE_TEXTURE2D_FUNC)(GRAPHICS_DEVICE* device, void* textureId);

#endif /* graphics_device_functions_h */
