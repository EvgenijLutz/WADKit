//
//  math_utils.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef math_utils_h
#define math_utils_h

#include <simd/simd.h>

simd_float4x4 matrix4fTranslation(float tx, float ty, float tz);
simd_float4x4 matrix4fRotation(float radians, simd_float3 axis);
simd_float4x4 matrix4fPerspectiveRightHand(float fovyRadians, float aspect, float nearZ, float farZ);
simd_float4x4 matrix4fLookAt_rightHand(vector_float3 eye, vector_float3 target, vector_float3 up);

#endif /* math_utils_h */
