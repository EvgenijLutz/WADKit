//
//  command_queue_private_interface.h
//  wad_editor_lib_macos
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef command_queue_private_interface_h
#define command_queue_private_interface_h

#include "private_interface.h"

void commandQueueReleaseCommandBuffer(COMMAND_QUEUE* commandQueue, COMMAND_BUFFER* commandBuffer);

#endif /* command_queue_private_interface_h */
