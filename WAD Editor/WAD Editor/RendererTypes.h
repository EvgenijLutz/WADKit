//
//  RendererTypes.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef RendererTypes_h
#define RendererTypes_h

#include <simd/simd.h>

// ===== Textured triangle shaders =====

typedef struct WE_VERTEX
{
	simd_float3 position;
	simd_float3 normal;
	float shade;
}
WE_VERTEX;

typedef struct WE_INDEX
{
	unsigned int vertexIndex;
	simd_float2 uv;
}
WE_INDEX;

typedef struct OBJECT_UNIFORMS
{
	// simd_float4x4 modelView;
	// simd_float4x4 projection;
	
	simd_float4x4 modelViewProjection;
}
OBJECT_UNIFORMS;


// ----- Colored line shaders -----

typedef struct WE_LINE_VERTEX
{
	simd_float3 position;
}
WE_LINE_VERTEX;

typedef struct WE_LINE_INDEX
{
	unsigned int index;
}
WE_LINE_INDEX;

typedef struct WE_LINE_UNIFORMS
{
	float scale;
	simd_float4 color;
	simd_float4x4 modelViewProjection;
}
WE_LINE_UNIFORMS;


// ----- Sphere shaders -----

#endif /* RendererTypes_h */
