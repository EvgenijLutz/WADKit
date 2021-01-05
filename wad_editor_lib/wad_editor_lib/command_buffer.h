//
//  command_buffer.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef command_buffer_h
#define command_buffer_h

#include "private_interface.h"

typedef struct COMMAND_BUFFER
{
	COMMAND_QUEUE* commandQueue;
	
	void* commandBufferId;
}
COMMAND_BUFFER;

#endif /* command_buffer_h */
