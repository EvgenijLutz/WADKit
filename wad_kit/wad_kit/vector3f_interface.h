//
//  vector3f_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef vector3f_interface_h
#define vector3f_interface_h

#include "wad_kit.h"

#if defined(__APPLE__) && __APPLE__

static inline vector3f vector3fCreate(float x, float y, float z) { return simd_make_float3(x, y, z); }

#else

#error Implement vector2f

#endif

#endif /* vector3f_interface_h */
