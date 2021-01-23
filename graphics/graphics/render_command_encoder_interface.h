//
//  render_command_encoder_interface.h
//  graphics
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef render_command_encoder_interface_h
#define render_command_encoder_interface_h

#include "graphics.h"

RENDER_COMMAND_ENCODER* commandBufferStartRenderCommandEncoder(COMMAND_BUFFER* commandBuffer, GRAPHICS_VIEW* graphicsView);
void renderCommandEncoderFinishEncoding(RENDER_COMMAND_ENCODER* renderCommandEncoder);
void renderCommandEncoderScheduleDrawMesh(RENDER_COMMAND_ENCODER* encoder, GRAPHICS_MESH* mesh, GRAPHICS_MESH_UNIFORMS* uniforms);

#endif /* render_command_encoder_interface_h */
