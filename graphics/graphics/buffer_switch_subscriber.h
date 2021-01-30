//
//  buffer_switch_subscriber.h
//  graphics
//
//  Created by Евгений Лютц on 28.01.21.
//

#ifndef buffer_switch_subscriber_h
#define buffer_switch_subscriber_h

#include "graphics.h"

typedef void (* BUFFER_SWITCH_SUBSCRIBER_SWITCH_FUNC)(BUFFER_SWITCH* bufferSwitch, unsigned int bufferIndex, COMMAND_BUFFER* commandEncoder, void* userInfo);

typedef struct BUFFER_SWITCH_SUBSCRIBER
{
	BUFFER_SWITCH_SUBSCRIBER_SWITCH_FUNC switchFunc;
	void* userInfo;
}
BUFFER_SWITCH_SUBSCRIBER;

#endif /* buffer_switch_subscriber_h */
