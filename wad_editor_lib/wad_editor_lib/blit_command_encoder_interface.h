//
//  blit_command_encoder_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef blit_command_encoder_interface_h
#define blit_command_encoder_interface_h

#include "wad_editor_lib.h"

void blitCommandEncoderEndEncoding(BLIT_COMMAND_ENCODER* blitCommandEncoder);
void blitCommandEncoderScheduleCopyFromBufferToBuffer(BLIT_COMMAND_ENCODER* blitCommandEncoder, GRAPHICS_BUFFER* sourceBuffer, unsigned long sourceOffset, GRAPHICS_BUFFER* destinationBuffer, unsigned long destinationOffset, unsigned long size);

#endif /* blit_command_encoder_interface_h */
