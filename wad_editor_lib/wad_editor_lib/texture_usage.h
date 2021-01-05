//
//  texture_usage.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef texture_usage_h
#define texture_usage_h

#include "wad_editor_lib.h"

typedef enum TEXTURE_USAGE
{
	TEXTURE_USAGE_UNKNOWN		= 0b00000000,
	TEXTURE_USAGE_SHADER_READ	= 0b00000001,
	TEXTURE_USAGE_SHADER_WRITE	= 0b00000010,
	TEXTURE_USAGE_RENDER_TARGET	= 0b00000100
}
TEXTURE_USAGE;

#endif /* texture_usage_h */
