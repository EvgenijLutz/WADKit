//
//  graphics_buffer_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_buffer_interface_h
#define graphics_buffer_interface_h

#include "wad_editor_lib.h"

void graphicsBufferRelease(GRAPHICS_BUFFER* buffer);

void graphicsBufferSetLabel(GRAPHICS_BUFFER* buffer, const char* text);
void* graphicsBufferGetDataToCPUWrite(GRAPHICS_BUFFER* buffer);

#endif /* graphics_buffer_interface_h */
