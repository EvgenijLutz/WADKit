//
//  blit_command_encoder_interface.h
//  graphics
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef blit_command_encoder_interface_h
#define blit_command_encoder_interface_h

#include "graphics.h"

void blitCommandEncoderEndEncoding(BLIT_COMMAND_ENCODER* blitCommandEncoder);
void blitCommandEncoderScheduleCopyFromBufferToBuffer(BLIT_COMMAND_ENCODER* blitCommandEncoder, GRAPHICS_BUFFER* sourceBuffer, unsigned long sourceOffset, GRAPHICS_BUFFER* destinationBuffer, unsigned long destinationOffset, unsigned long size);

#endif /* blit_command_encoder_interface_h */
