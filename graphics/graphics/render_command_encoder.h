//
//  render_command_encoder.h
//  graphics
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef render_command_encoder_h
#define render_command_encoder_h

#include "graphics.h"

typedef struct RENDER_COMMAND_ENCODER
{
	COMMAND_BUFFER* commandBuffer;
	
	void* renderCommandEncoderId;
}
RENDER_COMMAND_ENCODER;

#endif /* render_command_encoder_h */
