//
//  static_object_private_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 31.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef static_object_private_interface_h
#define static_object_private_interface_h

#include "private_interface.h"

void staticInitialize(STATIC* staticObject, RAW_STATIC* rawStatic, WK_WAD_LOAD_INFO* loadInfo);
void staticDeinitialize(STATIC* staticObject);

#endif /* static_object_private_interface_h */
