//
//  buffer_interface.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_buffer_interface_h
#define graphics_buffer_interface_h

#include "graphics.h"

GR_BUFFER* graphicsDeviceCreateVertexBuffer(GR_DEVICE* device, COMMAND_QUEUE* queue, GR_T_VERTEX* vertices, unsigned int numVertices);

GR_BUFFER* graphicsDeviceCreateMeshUniformsBuffer(GR_DEVICE* device);
void meshUniformsBufferSetData(GR_BUFFER* buffer, GR_MESH_UNIFORM_DATA* data);

GR_BUFFER* graphicsDeviceCreateViewportUniformsBuffer(GR_DEVICE* device);
void viewportUniformsBufferSetData(GR_BUFFER* buffer, GR_VIEWPORT_UNIFORMS_DATA* data);

void graphicsBufferSetData(GR_BUFFER* buffer, const void* data);
void graphicsBufferRelease(GR_BUFFER* buffer);

void graphicsBufferSetLabel(GR_BUFFER* buffer, const char* text);

#endif /* graphics_buffer_interface_h */
