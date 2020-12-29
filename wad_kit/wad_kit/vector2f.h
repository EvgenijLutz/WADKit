//
//  vector2f.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef vector2f_h
#define vector2f_h

#if defined(__APPLE__) && __APPLE__

#include <simd/simd.h>

typedef simd_float2 vector2f;

static inline vector2f vector2fCreate(float x, float y) { return simd_make_float2(x, y); }

#else

#error Implement vector2f

#endif

#endif /* vector2f_h */
