//
//  vector2f.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef vector2f_h
#define vector2f_h

#include "wad_kit.h"

#if defined(__APPLE__) && __APPLE__

#include <simd/simd.h>
typedef simd_float2 vector2f;

#else

#error Implement vector2f

#endif

#endif /* vector2f_h */
