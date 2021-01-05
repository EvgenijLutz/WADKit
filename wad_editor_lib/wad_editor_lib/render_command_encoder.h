//
//  render_command_encoder.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef render_command_encoder_h
#define render_command_encoder_h

#include "wad_editor_lib.h"

typedef struct RENDER_COMMAND_ENCODER
{
	COMMAND_BUFFER* commandBuffer;
	
	void* renderCommandEncoderId;
}
RENDER_COMMAND_ENCODER;

#endif /* render_command_encoder_h */
