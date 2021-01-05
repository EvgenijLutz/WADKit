//
//  graphics_buffer_option.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_buffer_option_h
#define graphics_buffer_option_h

#include "wad_editor_lib.h"

typedef enum GRAPHICS_BUFFER_OPTION
{
	GRAPHICS_BUFFER_OPTION_CPU_READ_WRITE = 0,
	GRAPHICS_BUFFER_OPTION_GPU_ONLY
}
GRAPHICS_BUFFER_OPTION;

#endif /* graphics_buffer_option_h */
