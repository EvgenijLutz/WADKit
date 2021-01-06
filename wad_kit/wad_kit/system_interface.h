//
//  system_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef system_interface_h
#define system_interface_h

#include "wad_kit.h"

WK_SYSTEM* systemCreateDefaultIfAvailable(void);
void systemRelease(WK_SYSTEM* system);

void systemGetPathForResourceWithName(WK_SYSTEM* system, WK_STRING* outPath, const char* name, const char* type, EXECUTE_RESULT* executeResult);

double systemGetCurrentTime(WK_SYSTEM* system);

#endif /* system_interface_h */
