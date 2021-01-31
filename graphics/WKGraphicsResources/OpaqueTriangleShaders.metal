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
	float3 ambient;
	float2 uv;
}
FragmentIn;

vertex FragmentIn opaqueTexturedTriangle_vs(uint id [[ vertex_id ]],
											constant GR_T_VERTEX* vertices [[ buffer(0) ]],
											constant uint& transformIndex [[ buffer (1) ]],
											constant GR_MESH_UNIFORM_DATA* transforms [[ buffer(2) ]],
											constant GR_VIEWPORT_UNIFORMS_DATA& viewportUniforms [[ buffer(3) ]])
{
	constant GR_T_VERTEX& vInfo = vertices[id];
	float4 vertexPosition = float4(vInfo.position, 1.0f);
	
	constant GR_MESH_UNIFORM_DATA& transform = transforms[transformIndex];
	
	// Calculate vertex location
	float4x4 modelViewProjection = viewportUniforms.projection * transform.modelView;
	float4 position = modelViewProjection * vertexPosition;
	
	/*// Calculate vertex location with first weight
	float4x4 modelViewProjection = uniforms.projection * uniforms.modelView[0];
	float4 position1 = modelViewProjection * vertexPosition * vInfo.weights[0];
	
	// Calculate vertex location with second weight
	modelViewProjection = uniforms.projection * uniforms.modelView[1];
	float4 position2 = modelViewProjection * vertexPosition * vInfo.weights[1];*/
	
	FragmentIn out =
	{
		//.position = position1 + position2,
		.position = position,
		.ambient = transform.ambient,
		.uv = vInfo.uv
	};
	
	return out;
}

#define RENDER_WITH_ALPHA_CHANNEL 0

fragment float4 opaqueTexturedTriangle_fs(FragmentIn in [[ stage_in ]],
										  texture2d<half> texture [[ texture(0) ]])
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

	color.x *= in.ambient.x;
	color.y *= in.ambient.y;
	color.z *= in.ambient.z;
	
#if !RENDER_WITH_ALPHA_CHANNEL
	color.a = 1.0f;
#endif
	
	return color;
}
