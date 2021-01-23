//
//  command_buffer_interface.h
//  graphics
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef command_buffer_interface_h
#define command_buffer_interface_h

#include "graphics.h"

COMMAND_BUFFER* commandQueueCreateCommandBuffer(COMMAND_QUEUE* commandQueue);
void commandBufferRelease(COMMAND_BUFFER* commandBuffer);

void commandBufferScheduleDisplayDrawable(COMMAND_BUFFER* commandBuffer, GRAPHICS_DRAWABLE* drawable);

void commandBufferAddCompletionHandler(COMMAND_BUFFER* commandBuffer, COMMAND_BUFFER_COMPLETION_FUNC completionHandler, void* userInfo);
void commandBufferWaitUntilCompleted(COMMAND_BUFFER* commandBuffer);
void commandBufferCommit(COMMAND_BUFFER* commandBuffer);

#endif /* command_buffer_interface_h */
