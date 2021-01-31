//
//  command_queue_interface.h
//  graphics
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef command_queue_interface_h
#define command_queue_interface_h

#include "graphics.h"

COMMAND_QUEUE* graphicsDeviceCreateCommandQueue(GR_DEVICE* device);
void commandQueueRelease(COMMAND_QUEUE* commandQueue);

#endif /* command_queue_interface_h */
