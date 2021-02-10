//
//  OpaqueTriangleShaderTypes.h
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

#ifndef OpaqueTriangleShaderTypes_h
#define OpaqueTriangleShaderTypes_h

#if defined(WAD_KIT_GRAPHICS_MATH_REDEFINED)
#include <simd/simd.h>
typedef simd_float2 vector2f;
typedef simd_float3 vector3f;
typedef simd_float4 vector4f;
typedef simd_float3x3 matrix3f;
typedef simd_float4x4 matrix4f;
#else
#include "graphics.h"
#endif

#define WK_RENDER_WITH_ALPHA_CHANNEL	0
#define WK_USE_TEXTURE_FILTERING		0

#include "buffer_data.h"

#endif /* OpaqueTriangleShaderTypes_h */
