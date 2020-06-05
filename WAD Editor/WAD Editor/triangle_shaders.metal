//
//  triangle_shaders.metal
//  WAD Editor
//
//  Created by Евгений Лютц on 03.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

#include "RendererTypes.h"

#define CALCULATE_LIGHT 1

//=============================//
//===== Textured triangle =====//
//=============================//

struct TexTriangleFragmentIn
{
	float4 position [[ position ]];
	float2 uv;
};

vertex TexTriangleFragmentIn texturedTriangle_vs(uint id [[ vertex_id ]],
												 constant WE_VERTEX* vertices [[ buffer(0) ]],
												 constant WE_INDEX* indices [[ buffer(1) ]],
												 constant OBJECT_UNIFORMS& uniforms [[ buffer(2) ]])
{
	uint vertexIndex = indices[id].vertexIndex;
	
	float4 pos = float4(vertices[vertexIndex].position, 1.0f);
	float4 position = uniforms.modelViewProjection * pos;
	TexTriangleFragmentIn out =
	{
		.position = position,
		.uv = indices[id].uv
	};
	
	return out;
}

fragment float4 texturedTriangle_fs(TexTriangleFragmentIn in [[ stage_in ]],
									texture2d<half> texture [[ texture(0) ]])
{
	constexpr sampler textureSampler = sampler(mag_filter::nearest, min_filter::nearest);
	//constexpr sampler textureSampler = sampler(mag_filter::linear, min_filter::linear);
	
	float4 color = float4(texture.sample(textureSampler, in.uv));
	color.a = 1.0f;
	
	return color;
}
