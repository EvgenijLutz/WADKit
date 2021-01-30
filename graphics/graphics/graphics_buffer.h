//
//  graphics_buffer.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_buffer_h
#define graphics_buffer_h

#include "graphics.h"

typedef enum GRAPHICS_BUFFER_TYPE
{
	GRAPHICS_BUFFER_TYPE_VIEWPORT_UNIFORMS = 0,
}
GRAPHICS_BUFFER_TYPE;

typedef struct GRAPHICS_BUFFER
{
	GRAPHICS_DEVICE* device;
	GRAPHICS_BUFFER_TYPE type;
	BUFFER_SWITCH* bufferSwitch;
	
	unsigned int length;
	
	void* bufferId;
}
GRAPHICS_BUFFER;

#endif /* graphics_buffer_h */
