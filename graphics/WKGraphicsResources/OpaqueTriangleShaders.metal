//
//  OpaqueTriangleShaders.metal
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

#include <metal_stdlib>
using namespace metal;

#define WAD_KIT_GRAPHICS_MATH_REDEFINED
#include "../graphics/OpaqueTriangleShaderTypes.h"

typedef struct FragmentIn
{
	float4 position [[ position ]];
	float2 uv;
}
FragmentIn;

vertex FragmentIn opaqueTexturedTriangle_vs(uint id [[ vertex_id ]],
											constant GRAPHICS_VERTEX* vertices [[ buffer(0) ]],
											constant GRAPHICS_MESH_UNIFORMS_DATA& uniforms [[ buffer(1) ]])
{
	constant GRAPHICS_VERTEX& vInfo = vertices[id];
	float4 vertexPosition = float4(vInfo.position, 1.0f);
	
	// Calculate vertex location with first weight
	float4x4 modelViewProjection = uniforms.projection * uniforms.modelView[0];
	float4 position1 = modelViewProjection * vertexPosition * vInfo.weights[0];
	
	// Calculate vertex location with second weight
	modelViewProjection = uniforms.projection * uniforms.modelView[1];
	float4 position2 = modelViewProjection * vertexPosition * vInfo.weights[1];
	
	FragmentIn out =
	{
		.position = position1 + position2,
		.uv = vInfo.uv
	};
	
	return out;
}

#define RENDER_WITH_ALPHA_CHANNEL 0

fragment float4 opaqueTexturedTriangle_fs(FragmentIn in [[ stage_in ]],
										  texture2d<half> texture [[ texture(0) ]],
										  constant GRAPHICS_MESH_UNIFORMS_DATA& uniforms [[ buffer(1) ]])
{
	constexpr sampler textureSampler = sampler(mag_filter::nearest, min_filter::nearest);
	//constexpr sampler textureSampler = sampler(mag_filter::linear, min_filter::linear);
	
	float4 color = float4(texture.sample(textureSampler, in.uv));
	if (color.r > 0.99 && color.g < 0.01 && color.b > 0.99)
	{
#if RENDER_WITH_ALPHA_CHANNEL
		color.a = 0.0f;
#else
		color = float4(0.0, 0.0, 0.0, 1.0);
#endif
	}

	color.x *= uniforms.ambient.x;
	color.y *= uniforms.ambient.y;
	color.z *= uniforms.ambient.z;
	
#if !RENDER_WITH_ALPHA_CHANNEL
	color.a = 1.0f;
#endif
	
	return color;
}
