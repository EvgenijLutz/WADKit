//
//  render_command_encoder.c
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
	
	GR_DEVICE* device = commandBuffer->commandQueue->device;
	semaphoreEnter(device->accessSemaphore);
	void* renderCommandEncoderId = device->commandBufferStartRenderCommandEncoderFunc(commandBuffer, graphicsView);
	RENDER_COMMAND_ENCODER* encoder = magicArrayAddItem(&device->renderCommandEncoders);
	encoder->commandBuffer = commandBuffer;
	encoder->renderCommandEncoderId = renderCommandEncoderId;
	semaphoreLeave(device->accessSemaphore);
	
	return encoder;
}

void renderCommandEncoderFinishEncoding(RENDER_COMMAND_ENCODER* renderCommandEncoder)
{
	assert(renderCommandEncoder);
	
	GR_DEVICE* device = renderCommandEncoder->commandBuffer->commandQueue->device;
	semaphoreEnter(device->accessSemaphore);
	device->renderCommandEncoderEndEncodingFunc(renderCommandEncoder);
	magicArrayRemoveItem(&device->renderCommandEncoders, renderCommandEncoder);
	semaphoreLeave(device->accessSemaphore);
}

void renderCommandEncoderScheduleDrawMesh(RENDER_COMMAND_ENCODER* encoder, GR_BUFFER* vertexBuffer, unsigned int numVertices, TEXTURE2D* texture, GR_BUFFER* meshUniforms, GR_BUFFER* viewportUniforms)
{
	assert(encoder);
	assert(vertexBuffer);
	assert(meshUniforms);
	assert(viewportUniforms);
	
	GR_DEVICE* device = encoder->commandBuffer->commandQueue->device;
	device->renderCommandEncoderRenderTexturedMeshFunc(encoder, vertexBuffer, numVertices, texture, meshUniforms, viewportUniforms);
}
