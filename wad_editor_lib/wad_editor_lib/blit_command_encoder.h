//
//  blit_command_encoder.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef blit_command_encoder_h
#define blit_command_encoder_h

#include "wad_editor_lib.h"

typedef struct BLIT_COMMAND_ENCODER
{
	COMMAND_BUFFER* commandBuffer;
	
	void* blitCommandEncoderId;
}
BLIT_COMMAND_ENCODER;

#endif /* blit_command_encoder_h */
