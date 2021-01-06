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
	WK_STRING name;
	
	WK_ARRAY textures;
	WK_ARRAY buffers;
	WK_ARRAY commandQueues;
	WK_ARRAY commandBuffers;
	WK_ARRAY blitCommandEncoders;
	
	GRAPHICS_DEVICE_CREATE_TEXTURE2D_WITH_DATA_FUNC createTexture2dWithDataFunc;
	GRAPHICS_DEVICE_RELEASE_TEXTURE2D_FUNC releaseTexture2dFunc;
	
	GRAPHICS_DEVICE_CREATE_BUFFER_FUNC createBufferFunc;
	GRAPHICS_BUFFER_RELEASE_FUNC graphicsBufferReleaseFunc;
	GRAPHICS_BUFFER_SET_LABEL_FUNC graphicsBufferSetLabelFunc;
	GRAPHICS_BUFFER_GET_DATA_TO_CPU_WRITE_FUNC graphicsBufferGetDataToCPUWriteFunc;
	
	GRAPHICS_DEVICE_CREATE_COMMAND_QUEUE_FUNC createCommandQueueFunc;
	GRAPHICS_DEVICE_RELEASE_COMMAND_QUEUE_FUNC releaseCommandQueueFunc;
	
	COMMAND_QUEUE_CREATE_COMMAND_BUFFER_FUNC commandQueueCreateCommandBufferFunc;
	COMMAND_QUEUE_RELEASE_COMMAND_BUFFER_FUNC commandQueueReleaseCommandBufferFunc;
	
	COMMAND_BUFFER_ADD_COMPLETION_FUNC commandBufferAddCompletionFunc;
	COMMAND_BUFFER_SCHEDULE_PRESENT_DRAWABLE_FUNC commandBufferPresentDrawableFunc;
	COMMAND_BUFFER_COMMIT_FUNC commandBufferCommitFunc;
	COMMAND_BUFFER_WAIT_UNTIL_COMPLETED_FUNC commandBufferWaitUntilCompletedFunc;
	
	COMMAND_BUFFER_START_BLIT_COMMAND_ENCODER_FUNC commandBufferStartBlitCommandEncoderFunc;
	BLIT_COMMAND_ENCODER_END_ENCODING_FUNC blitCommandEncoderEndEncodingFunc;
	BLIT_COMMAND_ENCODER_COPY_FROM_BUFFER_TO_BUFFER_FUNC blitCommandEncoderCopyFromBufferToBuffer;
	
	int createdByDefault;
	void* userInfo;
}
GRAPHICS_DEVICE;

#endif /* graphics_device_h */
