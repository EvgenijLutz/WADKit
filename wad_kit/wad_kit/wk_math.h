//
//  wk_math.h
//  wad_kit
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wk_math_h
#define wk_math_h

#if defined(__APPLE__) && __APPLE__

#include <simd/simd.h>

typedef simd_float2 vector2f;
typedef simd_float3 vector3f;
typedef simd_float4 vector4f;
typedef simd_quatf quaternionf;

typedef simd_float4x4 matrix4f;

#define vector2fCreate(valueX, valueY) simd_make_float2(valueX, valueY)

#else // IF !__APPLE__

typedef struct vector2f
{
	float x;
	float y;
}
vector2f;

typedef struct vector3f
{
	float x;
	float y;
	float z;
}
vector3f;

typedef struct vector4f
{
	float x;
	float y;
	float z;
	float w;
}
vector4f;

typedef vector4f quaternionf;

typedef struct matrix4f
{
	vector4f columns[4];
}
matrix4f;

#error Math is not implemented for this platform

#endif // END !__APPLE__


#define vec_x(vec)		(vec.x)
#define vec_y(vec)		(vec.y)
#define vec_z(vec)		(vec.z)
#define vec_w(vec)		(vec.w)

#define quat_x(quat)	(quat.x)
#define quat_y(quat)	(quat.y)
#define quat_z(quat)	(quat.z)
#define quat_w(quat)	(quat.w)


#endif /* wk_math_h */
