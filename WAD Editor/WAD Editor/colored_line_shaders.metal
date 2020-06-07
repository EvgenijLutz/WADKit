//
//  colored_line_shaders.metal
//  WAD Editor
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

#include "RendererTypes.h"

#define DISCARD_MAGENTA 0
#define MAGENTA_TO_BLACK 1
#define MAGENTA_TO_TRANSPARENT 0


struct VertexOut
{
	float4 position [[ position ]];
};

vertex VertexOut coloredLine_vs(uint id [[ vertex_id ]],
								constant WE_LINE_VERTEX* vertices [[ buffer(0) ]],
								constant WE_LINE_INDEX* indices [[ buffer(1) ]],
								constant WE_LINE_UNIFORMS& uniforms [[ buffer(2) ]])
{
	uint index = indices[id].index;
	float4 pos = float4(vertices[index].position * uniforms.scale, 1.0f);
	float4 position = uniforms.modelViewProjection * pos;
	VertexOut out =
	{
		.position = position
	};
	
	return out;
}

fragment float4 coloredLine_fs(VertexOut in [[ stage_in ]],
							   constant WE_LINE_UNIFORMS& uniforms [[ buffer(0) ]])
{
	return uniforms.color;
}
