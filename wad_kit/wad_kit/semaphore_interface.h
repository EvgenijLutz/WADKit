//
//  semaphore_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef semaphore_interface_h
#define semaphore_interface_h

#include "wad_kit.h"

WK_SEMAPHORE* systemCreateSemaphore(WK_SYSTEM* system, int value);
void semaphoreRelease(WK_SEMAPHORE* semaphore);

void semaphoreEnter(WK_SEMAPHORE* semaphore);
void semaphoreLeave(WK_SEMAPHORE* semaphore);

#endif /* semaphore_interface_h */
