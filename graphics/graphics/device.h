//
//  device.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_device_h
#define graphics_device_h

#include "private_interface.h"

typedef struct GR_DEVICE
{
	STRING name;
	
	SYSTEM* system;
	SEMAPHORE* accessSemaphore;
	
	MAGIC_ARRAY textures;
	MAGIC_ARRAY buffers;
	MAGIC_ARRAY commandQueues;
	MAGIC_ARRAY commandBuffers;
	MAGIC_ARRAY renderCommandEncoders;
	
	GRAPHICS_DEVICE_CREATE_TEXTURE2D_WITH_DATA_FUNC createTexture2dWithDataFunc;
	GRAPHICS_DEVICE_RELEASE_TEXTURE2D_FUNC releaseTexture2dFunc;
	
	GR_DEVICE_CREATE_VERTEX_BUFFER_FUNC createVertexBufferFunc;
	GR_DEVICE_CREATE_MESH_UNIFORMS_BUFFER_FUNC createMeshUniformsBufferFunc;
	GR_DEVICE_CREATE_VIEWPORT_UNIFORMS_BUFFER_FUNC createViewportUniformsBufferFunc;
	GR_BUFFER_SET_DATA_FUNC setBufferDataFunc;
	GR_BUFFER_RELEASE_FUNC releaseBufferFunc;
	GR_BUFFER_SET_LABEL_FUNC setBufferLabelFunc;
	
	GRAPHICS_DEVICE_SCHEDULE_UPLOAD_BUFFERS_FUNC scheduleUploadBuffersFunc;
	
	GRAPHICS_DEVICE_CREATE_COMMAND_QUEUE_FUNC createCommandQueueFunc;
	GRAPHICS_DEVICE_RELEASE_COMMAND_QUEUE_FUNC releaseCommandQueueFunc;
	
	COMMAND_QUEUE_CREATE_COMMAND_BUFFER_FUNC commandQueueCreateCommandBufferFunc;
	COMMAND_QUEUE_RELEASE_COMMAND_BUFFER_FUNC commandQueueReleaseCommandBufferFunc;
	
	COMMAND_BUFFER_ADD_COMPLETION_FUNC commandBufferAddCompletionFunc;
	COMMAND_BUFFER_SCHEDULE_PRESENT_DRAWABLE_FUNC commandBufferPresentDrawableFunc;
	COMMAND_BUFFER_COMMIT_FUNC commandBufferCommitFunc;
	COMMAND_BUFFER_WAIT_UNTIL_COMPLETED_FUNC commandBufferWaitUntilCompletedFunc;
	
	COMMAND_BUFFER_START_RENDER_COMMAND_ENCODER_FUNC commandBufferStartRenderCommandEncoderFunc;
	RENDER_COMMAND_ENCODER_END_ENCODING_FUNC renderCommandEncoderEndEncodingFunc;
	RENDER_COMMAND_ENCODER_RENDER_TEXTURED_MESH_FUNC renderCommandEncoderRenderTexturedMeshFunc;
	
	void* userInfo;
}
GR_DEVICE;

#endif /* graphics_device_h */
