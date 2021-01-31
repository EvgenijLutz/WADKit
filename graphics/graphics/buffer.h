//
//  buffer.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_buffer_h
#define graphics_buffer_h

#include "graphics.h"

typedef enum GR_BUFFER_TYPE
{
	GR_BUFFER_TYPE_VERTEX_BUFFER = 0,
	GR_BUFFER_TYPE_MESH_UNIFORMS,
	GR_BUFFER_TYPE_VIEWPORT_UNIFORMS,
}
GR_BUFFER_TYPE;

typedef struct GR_BUFFER
{
	GR_DEVICE* device;
	GR_BUFFER_TYPE type;
	void* bufferId;
}
GR_BUFFER;

#endif /* graphics_buffer_h */
