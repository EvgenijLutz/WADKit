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
static inline vector3f vector3fNormalize(vector3f vector) { return simd_normalize(vector); }
static inline vector3f vector3fSum(vector3f vector1, vector3f vector2) { return vector1 + vector2; }
static inline vector3f vector3fMulFloat(vector3f vector, float value) { return vector * value; }

#else

#error Implement vector2f

#endif

#endif /* vector3f_interface_h */
