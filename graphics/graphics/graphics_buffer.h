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

typedef struct GRAPHICS_BUFFER
{
	GRAPHICS_DEVICE* device;
	
	unsigned int length;
	GRAPHICS_BUFFER_OPTION option;
	
	void* bufferId;
}
GRAPHICS_BUFFER;

#endif /* graphics_buffer_h */
