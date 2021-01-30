//
//  graphics_buffer_interface.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_buffer_interface_h
#define graphics_buffer_interface_h

#include "graphics.h"

GRAPHICS_BUFFER* deviceCreateViewportBuffer(GRAPHICS_DEVICE* device, BUFFER_SWITCH* bufferSwitch);
void graphicsBufferRelease(GRAPHICS_BUFFER* buffer);

void graphicsBufferSetLabel(GRAPHICS_BUFFER* buffer, const char* text);

#endif /* graphics_buffer_interface_h */
