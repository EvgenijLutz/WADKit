//
//  buffer_switch.h
//  graphics
//
//  Created by Евгений Лютц on 28.01.21.
//

#ifndef buffer_switch_h
#define buffer_switch_h

#include "private_interface.h"

typedef struct BUFFER_SWITCH
{
	unsigned int numBuffers;
	unsigned int currentBuffer;
	
	/// Collection of @b BUFFER_SWITCH_SUBSCRIBER elements.
	MAGIC_ARRAY subscribers;
}
BUFFER_SWITCH;

#endif /* buffer_switch_h */
