//
//  vector3f.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef vector3f_h
#define vector3f_h

#include "wad_kit.h"

#if defined(__APPLE__) && __APPLE__

#include <simd/simd.h>
typedef simd_float3 vector3f;

#else

#error Implement vector3f

#endif

#endif /* vector3f_h */
