//
//  state_change_private_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 31.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef state_change_private_interface_h
#define state_change_private_interface_h

#include "private_interface.h"

void stateChangeInitialize(WK_STATE_CHANGE* stateChange, WK_ANIMATION* animation, RAW_STATE_CHANGE* rawStateChange, WK_WAD_LOAD_INFO* loadInfo);
void stateChangeDeinitialize(WK_STATE_CHANGE* stateChange);

#endif /* state_change_private_interface_h */
