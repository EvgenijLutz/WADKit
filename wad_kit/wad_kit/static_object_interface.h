//
//  static_object_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef static_object_interface_h
#define static_object_interface_h

#include "static_object_id.h"

typedef struct STATIC STATIC;

STATIC_ID staticGetId(STATIC* staticObject);
unsigned int staticGetMeshIndex(STATIC* staticObject);

const char* staticIdGetName(STATIC_ID staticId);

#endif /* static_object_interface_h */
