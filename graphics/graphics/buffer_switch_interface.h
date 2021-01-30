//
//  buffer_switch_interface.h
//  graphics
//
//  Created by Евгений Лютц on 28.01.21.
//

#ifndef buffer_switch_interface_h
#define buffer_switch_interface_h

#include "graphics.h"

BUFFER_SWITCH* bufferSwitchCreate(unsigned int numBuffers);
void bufferSwitchRelease(BUFFER_SWITCH* bufferSwitch);

void bufferSwitchSubscribe(BUFFER_SWITCH* bufferSwitch, BUFFER_SWITCH_SUBSCRIBER* subscriber);
void bufferSwitchSwitchAndScheduleUploadingChanges(BUFFER_SWITCH* bufferSwitch, COMMAND_BUFFER* commandEncoder);

#endif /* buffer_switch_interface_h */
