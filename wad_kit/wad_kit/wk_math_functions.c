//
//  wk_math_functions.c
//  wad_kit
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "wk_math_functions.h"

#if defined(__APPLE__) && __APPLE__

simd_float4x4 matrix4fScale(simd_float3 scale)
{
	return (simd_float4x4) {{
		{ scale.x,    0.0f,    0.0f, 0.0f },
		{    0.0f, scale.y,    0.0f, 0.0f },
		{    0.0f,    0.0f, scale.z, 0.0f },
		{    0.0f,    0.0f,    0.0f, 1.0f }
	}};
}

simd_float4x4 matrix4fTranslation(float tx, float ty, float tz)
{
	return (simd_float4x4) {{
		{ 1,   0,  0, 0 }, // -> { 1, 0, 0, tx }
		{ 0,   1,  0, 0 }, //    { 0, 1, 0, ty }
		{ 0,   0,  1, 0 }, //    { 0, 0, 1, tz }
		{ tx, ty, tz, 1 }  //    { 0, 0, 0,  1 }
	}};
}

simd_float4x4 matrix4fRotation(float radians, simd_float3 axis)
{
	axis = vector_normalize(axis);
	float ct = cosf(radians);
	float st = sinf(radians);
	float ci = 1 - ct;
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	
	return (simd_float4x4) {{
		{     ct + x * x * ci, y * x * ci + z * st, z * x * ci - y * st, 0 },
		{ x * y * ci - z * st,     ct + y * y * ci, z * y * ci + x * st, 0 },
		{ x * z * ci + y * st, y * z * ci - x * st,     ct + z * z * ci, 0 },
		{                   0,                   0,                   0, 1 }
	}};
}

simd_float4x4 matrix4fPerspectiveRightHand(float fovyRadians, float aspectRatio, float frustumNearZ, float frustumFarZ)
{
	float ys = 1.0f / tanf(fovyRadians * 0.5f);
    float xs = ys / aspectRatio;
    float zs = frustumFarZ / (frustumNearZ - frustumFarZ);
	float tz = frustumNearZ * zs;

    return (simd_float4x4) {{
        { xs,   0,  0,  0 },
        {  0,  ys,  0,  0 },
        {  0,   0, zs, -1 },
        {  0,   0, tz,  0 }
    }};
}

simd_float4x4 matrix4fLookAt_rightHand(vector_float3 eye, vector_float3 target, vector_float3 up)
{
	simd_float3 z = vector_normalize(eye - target);
	simd_float3 x = vector_normalize(vector_cross(up, z));
	simd_float3 y = vector_cross(z, x);
	simd_float3 t = simd_make_float3(-vector_dot(x, eye), -vector_dot(y, eye), -vector_dot(z, eye));

	/*return (simd_float4x4) {{
		{ x.x, x.y, x.z, t.x },
		{ y.x, y.y, y.z, t.y },
		{ z.x, z.y, z.z, t.z },
		{   0,   0,   0,   1 }
	}};*/
	
	return (simd_float4x4) {{
		{ x.x, y.x, z.x, 0 },
		{ x.y, y.y, z.y, 0 },
		{ x.z, y.z, z.z, 0 },
		{ t.x, t.y, t.z, 1 }
	}};
}

#else // IF !__APPLE__

#error Math is not implemented for this system

#endif // END !__APPLE__
