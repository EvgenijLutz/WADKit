//
//  device_functions.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_device_functions_h
#define graphics_device_functions_h

#include "graphics.h"

typedef void* (* GRAPHICS_DEVICE_CREATE_TEXTURE2D_WITH_DATA_FUNC)(GR_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, TEXTURE_USAGE usage, const void* data);
typedef void (* GRAPHICS_DEVICE_RELEASE_TEXTURE2D_FUNC)(GR_DEVICE* device, void* textureId);

typedef void* (* GR_DEVICE_CREATE_VERTEX_BUFFER_FUNC)(GR_DEVICE* device, COMMAND_QUEUE* queue, GR_T_VERTEX* vertices, unsigned int numVertices);
typedef void* (* GR_DEVICE_CREATE_MESH_UNIFORMS_BUFFER_FUNC)(GR_DEVICE* device);
typedef void* (* GR_DEVICE_CREATE_VIEWPORT_UNIFORMS_BUFFER_FUNC)(GR_DEVICE* device);
typedef void (* GR_BUFFER_SET_DATA_FUNC)(GR_BUFFER* buffer, const void* data);
typedef void (* GR_BUFFER_RELEASE_FUNC)(GR_BUFFER* buffer);
typedef void (* GR_BUFFER_SET_LABEL_FUNC)(GR_BUFFER* buffer, const char* label);

typedef void (* GRAPHICS_DEVICE_SCHEDULE_UPLOAD_BUFFERS_FUNC)(GR_DEVICE* device, COMMAND_BUFFER* commandBuffer);

typedef void* (* GRAPHICS_DEVICE_CREATE_COMMAND_QUEUE_FUNC)(GR_DEVICE* device);
typedef void (* GRAPHICS_DEVICE_RELEASE_COMMAND_QUEUE_FUNC)(GR_DEVICE* device, COMMAND_QUEUE* queue);

typedef void* (* COMMAND_QUEUE_CREATE_COMMAND_BUFFER_FUNC)(COMMAND_QUEUE* commandQueue);
typedef void (* COMMAND_QUEUE_RELEASE_COMMAND_BUFFER_FUNC)(COMMAND_QUEUE* commandQueue, COMMAND_BUFFER* commandBuffer);

typedef void (* COMMAND_BUFFER_COMPLETION_FUNC)(COMMAND_BUFFER* commandBuffer, void* userInfo);
typedef void (* COMMAND_BUFFER_ADD_COMPLETION_FUNC)(COMMAND_BUFFER* commandBuffer, COMMAND_BUFFER_COMPLETION_FUNC completion, void* userInfo);
typedef void (* COMMAND_BUFFER_SCHEDULE_PRESENT_DRAWABLE_FUNC)(COMMAND_BUFFER* commandBuffer, GRAPHICS_DRAWABLE* graphicsDrawable);
typedef void (* COMMAND_BUFFER_COMMIT_FUNC)(COMMAND_BUFFER* commandBuffer);
typedef void (* COMMAND_BUFFER_WAIT_UNTIL_COMPLETED_FUNC)(COMMAND_BUFFER* commandBuffer);

typedef void* (* COMMAND_BUFFER_START_RENDER_COMMAND_ENCODER_FUNC)(COMMAND_BUFFER* commandBuffer, GRAPHICS_VIEW* graphicsView);
typedef void (* RENDER_COMMAND_ENCODER_END_ENCODING_FUNC)(RENDER_COMMAND_ENCODER* renderCommandEncoder);
typedef void (* RENDER_COMMAND_ENCODER_RENDER_TEXTURED_MESH_FUNC)(RENDER_COMMAND_ENCODER* renderCommandEncoder, GR_BUFFER* vertexBuffer, unsigned int numVertices, TEXTURE2D* texture, GR_BUFFER* meshUniforms, GR_BUFFER* viewportUniforms);

#endif /* graphics_device_functions_h */
