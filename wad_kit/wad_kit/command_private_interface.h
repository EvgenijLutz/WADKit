//
//  command_private_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef command_private_interface_h
#define command_private_interface_h

#include "private_interface.h"

void commandInitialize(WK_COMMAND* command, WK_ANIMATION* animation, WK_WAD_LOAD_INFO* loadInfo);
void commandDeinitialize(WK_COMMAND* command);

#endif /* command_private_interface_h */
