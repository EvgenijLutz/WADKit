//
//  buffer.c
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

GR_BUFFER* graphicsDeviceCreateVertexBuffer(GR_DEVICE* device, COMMAND_QUEUE* queue, GR_T_VERTEX* vertices, unsigned int numVertices)
{
	assert(device);
	assert(queue);
	assert(vertices);
	assert(numVertices > 0);
	
	void* bufferId = device->createVertexBufferFunc(device, queue, vertices, numVertices);
	GR_BUFFER* buffer = magicArrayAddItem(&device->buffers);
	buffer->device = device;
	buffer->type = GR_BUFFER_TYPE_VERTEX_BUFFER;
	buffer->bufferId = bufferId;
	
	return buffer;
}


GR_BUFFER* graphicsDeviceCreateMeshUniformsBuffer(GR_DEVICE* device)
{
	assert(device);
	
	void* bufferId = device->createMeshUniformsBufferFunc(device);
	GR_BUFFER* buffer = magicArrayAddItem(&device->buffers);
	buffer->device = device;
	buffer->type = GR_BUFFER_TYPE_MESH_UNIFORMS;
	buffer->bufferId = bufferId;
	
	return buffer;
}

void meshUniformsBufferSetData(GR_BUFFER* buffer, GR_MESH_UNIFORM_DATA* data)
{
	assert(buffer);
	assert(data);
	assert(buffer->type == GR_BUFFER_TYPE_MESH_UNIFORMS);
	graphicsBufferSetData(buffer, data);
}


GR_BUFFER* graphicsDeviceCreateViewportUniformsBuffer(GR_DEVICE* device)
{
	assert(device);
	
	void* bufferId = device->createViewportUniformsBufferFunc(device);
	GR_BUFFER* buffer = magicArrayAddItem(&device->buffers);
	buffer->device = device;
	buffer->type = GR_BUFFER_TYPE_VIEWPORT_UNIFORMS;
	buffer->bufferId = bufferId;
	
	return buffer;
}

void viewportUniformsBufferSetData(GR_BUFFER* buffer, GR_VIEWPORT_UNIFORMS_DATA* data)
{
	assert(buffer);
	assert(data);
	assert(buffer->type == GR_BUFFER_TYPE_VIEWPORT_UNIFORMS);
	graphicsBufferSetData(buffer, data);
}


void graphicsBufferSetData(GR_BUFFER* buffer, const void* data)
{
	assert(buffer);
	assert(data);
	buffer->device->setBufferDataFunc(buffer, data);
}

void graphicsBufferRelease(GR_BUFFER* buffer)
{
	assert(buffer);
	buffer->device->releaseBufferFunc(buffer);
	
	magicArrayRemoveItem(&buffer->device->buffers, buffer);
}


void graphicsBufferSetLabel(GR_BUFFER* buffer, const char* text)
{
	assert(buffer);
	buffer->device->setBufferLabelFunc(buffer, text);
}
