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

#endif /* RendererTypes_h */
