//
//  vector2f_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef vector2f_interface_h
#define vector2f_interface_h

#include "wad_kit.h"

#if defined(__APPLE__) && __APPLE__

static inline vector2f vector2fCreate(float x, float y) { return simd_make_float2(x, y); }

#else

#error Implement vector2f

#endif

#endif /* vector2f_interface_h */
