//
//  tnw2Shaders.metal
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
	float3 normal;
	float2 uv;
}
FragmentIn;

// Textured, with normals, each vertex has 2 weights
vertex FragmentIn tnw2_vs(uint id [[ vertex_id ]],
						  constant GR_TNW2_VERTEX* vertices [[ buffer(0) ]],
						  constant uint& transformIndex [[ buffer (1) ]],
						  constant GR_MESH_W2_UNIFORM_DATA* transforms [[ buffer(2) ]],
						  constant GR_VIEWPORT_UNIFORMS_DATA& viewportUniforms [[ buffer(3) ]])
{
	constant GR_TNW2_VERTEX& vInfo = vertices[id];
	float4 vertexPosition = float4(vInfo.position, 1.0f);
	
	constant GR_MESH_W2_UNIFORM_DATA& transform = transforms[transformIndex];
	
	// Calculate vertex location for each weight
	float4 position1 = transform.model[0] * vertexPosition * vInfo.weights[0];
	float4 position2 = transform.model[1] * vertexPosition * vInfo.weights[1];
	
	float4x4 viewProjection = viewportUniforms.projection * transform.view;
	float4 position = viewProjection * (position1 + position2);
	
	//float3x3 normalMat1 = float3x3(transform.model[0]);
	//float3x3 normalMat2 = float3x3(transform.model[1]);
	
	FragmentIn out =
	{
		.position = position,
		.normal = vInfo.normal,
		.uv = vInfo.uv
	};
	
	return out;
}

fragment float4 tnw2_fs(FragmentIn in [[ stage_in ]],
						texture2d<half> texture [[ texture(0) ]],
						constant uint& transformIndex [[ buffer (0) ]],
						constant GR_MESH_W2_UNIFORM_DATA* transforms [[ buffer(1) ]])
{
#if WK_USE_TEXTURE_FILTERING
	constexpr sampler textureSampler = sampler(mag_filter::linear, min_filter::linear);
#else
	constexpr sampler textureSampler = sampler(mag_filter::nearest, min_filter::nearest);
#endif
	
	constant GR_MESH_W2_UNIFORM_DATA& transform = transforms[transformIndex];
	
	float4 color = float4(texture.sample(textureSampler, in.uv));
	if (color.r > 0.99 && color.g < 0.01 && color.b > 0.99)
	{
#if WK_RENDER_WITH_ALPHA_CHANNEL
		color.a = 0.0f;
#else
		color = float4(0.0, 0.0, 0.0, 1.0);
#endif
	}

	color.x *= transform.ambient.x;
	color.y *= transform.ambient.y;
	color.z *= transform.ambient.z;
	
#if !WK_RENDER_WITH_ALPHA_CHANNEL
	color.a = 1.0f;
#endif
	
	return color;
}
