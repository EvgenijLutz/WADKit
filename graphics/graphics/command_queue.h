//
//  command_queue.h
//  graphics
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef command_queue_h
#define command_queue_h

#include "private_interface.h"

typedef struct COMMAND_QUEUE
{
	GR_DEVICE* device;
	
	void* commandQueueId;
}
COMMAND_QUEUE;

#endif /* command_queue_h */
