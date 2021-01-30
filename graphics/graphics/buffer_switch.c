//
//  buffer_switch.c
//  graphics
//
//  Created by Евгений Лютц on 28.01.21.
//

#include "private_interface.h"

// Oh, come on, just say somethin'
// No, go ahead, tell me what do I need?

BUFFER_SWITCH* bufferSwitchCreate(unsigned int numBuffers)
{
	BUFFER_SWITCH* bufferSwitch = malloc(sizeof(BUFFER_SWITCH));
	bufferSwitch->numBuffers = numBuffers;
	bufferSwitch->currentBuffer = 0;
	magicArrayInitialize(&bufferSwitch->subscribers, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(BUFFER_SWITCH_SUBSCRIBER), 128);
	
	return bufferSwitch;
}

void bufferSwitchRelease(BUFFER_SWITCH* bufferSwitch)
{
	assert(bufferSwitch);
	
	magicArrayDeinitialize(&bufferSwitch->subscribers);
	debug_memset(bufferSwitch, 0, sizeof(BUFFER_SWITCH));
	free(bufferSwitch);
}


void bufferSwitchSubscribe(BUFFER_SWITCH* bufferSwitch, BUFFER_SWITCH_SUBSCRIBER* subscriber)
{
	assert(bufferSwitch);
	
	BUFFER_SWITCH_SUBSCRIBER* newSubscriber = magicArrayAddItem(&bufferSwitch->subscribers);
	*newSubscriber = *subscriber;
}

void bufferSwitchSwitchAndScheduleUploadingChanges(BUFFER_SWITCH* bufferSwitch, COMMAND_BUFFER* commandEncoder)
{
	assert(bufferSwitch);
	
	bufferSwitch->currentBuffer = (bufferSwitch->currentBuffer + 1) % bufferSwitch->numBuffers;
	for (unsigned int i = 0; i < bufferSwitch->subscribers.length; i++)
	{
		BUFFER_SWITCH_SUBSCRIBER* subscriber = magicArrayGetItem(&bufferSwitch->subscribers, i);
		subscriber->switchFunc(bufferSwitch, bufferSwitch->currentBuffer, commandEncoder, subscriber->userInfo);
	}
}
