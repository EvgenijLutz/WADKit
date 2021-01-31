//
//  texture2d_interface.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef texture2d_interface_h
#define texture2d_interface_h

#include "graphics.h"

TEXTURE2D* graphicsDeviceCreateTexture2dWithData(GR_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, TEXTURE_USAGE usage, const void* data);
void texture2dRelease(TEXTURE2D* texture);

#endif /* texture2d_interface_h */
