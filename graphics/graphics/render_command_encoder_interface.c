//
//  render_command_encoder_interface.c
//  graphics
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

RENDER_COMMAND_ENCODER* commandBufferStartRenderCommandEncoder(COMMAND_BUFFER* commandBuffer, GRAPHICS_VIEW* graphicsView)
{
	assert(commandBuffer);
	assert(graphicsView);
	
	GRAPHICS_DEVICE* device = commandBuffer->commandQueue->device;
	void* renderCommandEncoderId = device->commandBufferStartRenderCommandEncoderFunc(commandBuffer, graphicsView);
	
	RENDER_COMMAND_ENCODER* encoder = arrayAddItem(&device->renderCommandEncoders);
	encoder->commandBuffer = commandBuffer;
	encoder->renderCommandEncoderId = renderCommandEncoderId;
	return encoder;
}

void renderCommandEncoderFinishEncoding(RENDER_COMMAND_ENCODER* renderCommandEncoder)
{
	assert(renderCommandEncoder);
	
	GRAPHICS_DEVICE* device = renderCommandEncoder->commandBuffer->commandQueue->device;
	device->renderCommandEncoderEndEncodingFunc(renderCommandEncoder);
	arrayRemoveItem(&device->renderCommandEncoders, renderCommandEncoder);
}

void renderCommandEncoderScheduleDrawMesh(RENDER_COMMAND_ENCODER* encoder, GRAPHICS_MESH* mesh, GRAPHICS_MESH_UNIFORMS* uniforms)
{
	assert(encoder);
	assert(mesh);
	assert(uniforms);
	
	GRAPHICS_DEVICE* device = encoder->commandBuffer->commandQueue->device;
	device->renderCommandEncoderRenderTexturedMeshFunc(encoder, mesh, uniforms);
}
