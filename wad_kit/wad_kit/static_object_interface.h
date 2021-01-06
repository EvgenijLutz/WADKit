//
//  static_object_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef static_object_interface_h
#define static_object_interface_h

#include "wad_kit.h"

STATIC_ID staticGetId(STATIC* staticObject);
MESH* staticGetMesh(STATIC* staticObject);

const char* staticIdGetFabulousName(STATIC_ID staticId);
const char* staticIdGetTerribleName(STATIC_ID staticId);

#endif /* static_object_interface_h */
