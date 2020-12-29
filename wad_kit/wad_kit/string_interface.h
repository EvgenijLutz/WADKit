//
//  string_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef string_interface_h
#define string_interface_h

#include "wad_kit.h"

void stringInitialize(WK_STRING* string, const char* data);
void stringDeinitialize(WK_STRING* string);

char* stringGetData(WK_STRING* string);
void stringSetData(WK_STRING* string, const char* data);
void stringSetString(WK_STRING* string, WK_STRING* source);

#endif /* string_interface_h */
