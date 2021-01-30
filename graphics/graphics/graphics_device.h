//
//  graphics_device.h
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_device_h
#define graphics_device_h

#include "private_interface.h"

typedef struct GRAPHICS_DEVICE
{
	STRING name;
	
	SYSTEM* system;
	SEMAPHORE* accessSemaphore;
	
	MAGIC_ARRAY textures;
	MAGIC_ARRAY meshes;
	MAGIC_ARRAY meshUniforms;
	MAGIC_ARRAY commandQueues;
	MAGIC_ARRAY commandBuffers;
	MAGIC_ARRAY renderCommandEncoders;
	
	GRAPHICS_DEVICE_CREATE_TEXTURE2D_WITH_DATA_FUNC createTexture2dWithDataFunc;
	GRAPHICS_DEVICE_RELEASE_TEXTURE2D_FUNC releaseTexture2dFunc;
	
	GRAPHICS_DEVICE_CREATE_MESH_FUNC createMeshFunc;
	GRAPHICS_MESH_RELEASE_FUNC releaseMeshFunc;
	
	GRAPHICS_DEVICE_CREATE_MESH_UNIFORMS_FUNC createMeshUniformsFunc;
	GRAPHICS_MESH_UNIFORMS_RELEASE_FUNC meshUniformsReleaseFunc;
	GRAPHICS_MESH_UNIFORMS_SET_DATA_FUNC meshUniformsSetDataFunc;
	
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
GRAPHICS_DEVICE;

#endif /* graphics_device_h */
