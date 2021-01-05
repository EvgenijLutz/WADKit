//
//  graphics_device_functions.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_device_functions_h
#define graphics_device_functions_h

#include "wad_editor_lib.h"

typedef void* (* GRAPHICS_DEVICE_CREATE_TEXTURE2D_FUNC)(GRAPHICS_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, TEXTURE_USAGE usage, const void* data);
typedef void (* GRAPHICS_DEVICE_RELEASE_TEXTURE2D_FUNC)(GRAPHICS_DEVICE* device, void* textureId);

typedef void* (* GRAPHICS_DEVICE_CREATE_BUFFER_FUNC)(GRAPHICS_DEVICE* device, unsigned int length, GRAPHICS_BUFFER_OPTION option);
typedef void (* GRAPHICS_BUFFER_RELEASE_FUNC)(GRAPHICS_DEVICE* device, void* bufferId);
typedef void (* GRAPHICS_BUFFER_SET_LABEL_FUNC)(GRAPHICS_BUFFER* graphicsBuffer, const char* text);
typedef void* (* GRAPHICS_BUFFER_GET_DATA_TO_CPU_WRITE_FUNC)(GRAPHICS_BUFFER* buffer);

typedef void* (* GRAPHICS_DEVICE_CREATE_COMMAND_QUEUE_FUNC)(GRAPHICS_DEVICE* device);
typedef void (* GRAPHICS_DEVICE_RELEASE_COMMAND_QUEUE_FUNC)(GRAPHICS_DEVICE* device, COMMAND_QUEUE* queue);

typedef void* (* COMMAND_QUEUE_CREATE_COMMAND_BUFFER_FUNC)(COMMAND_QUEUE* commandQueue);
typedef void (* COMMAND_QUEUE_RELEASE_COMMAND_BUFFER_FUNC)(COMMAND_QUEUE* commandQueue, COMMAND_BUFFER* commandBuffer);

typedef void (* COMMAND_BUFFER_COMPLETION_FUNC)(COMMAND_BUFFER* commandBuffer, void* userInfo);
typedef void (* COMMAND_BUFFER_ADD_COMPLETION_FUNC)(COMMAND_BUFFER* commandBuffer, COMMAND_BUFFER_COMPLETION_FUNC completion, void* userInfo);
typedef void (* COMMAND_BUFFER_WAIT_UNTIL_COMPLETED_FUNC)(COMMAND_BUFFER* commandBuffer);
typedef void (* COMMAND_BUFFER_COMMIT_FUNC)(COMMAND_BUFFER* commandBuffer);

typedef void* (* COMMAND_BUFFER_START_BLIT_COMMAND_ENCODER_FUNC)(COMMAND_BUFFER* commandBuffer);
typedef void (* BLIT_COMMAND_ENCODER_END_ENCODING_FUNC)(BLIT_COMMAND_ENCODER* blitCommandEncoder);
typedef void (* BLIT_COMMAND_ENCODER_COPY_FROM_BUFFER_TO_BUFFER_FUNC)(BLIT_COMMAND_ENCODER* blitCommandEncoder, GRAPHICS_BUFFER* sourceBuffer, unsigned long sourceOffset, GRAPHICS_BUFFER* destinationBuffer, unsigned long destinationOffset, unsigned long size);

#endif /* graphics_device_functions_h */
