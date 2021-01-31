//
//  buffer_data.h
//  graphics
//
//  Created by Евгений Лютц on 31.01.21.
//

#ifndef graphics_buffer_data_h
#define graphics_buffer_data_h

#if !defined(WAD_KIT_GRAPHICS_MATH_REDEFINED)
#include "graphics.h"
#endif

/// Vertex with UV data for texturing
typedef struct GR_T_VERTEX
{
	vector3f position;
	vector2f uv;
}
GR_T_VERTEX;

typedef struct GR_MESH_UNIFORM_DATA
{
	matrix4f modelView;
	vector3f ambient;
}
GR_MESH_UNIFORM_DATA;

typedef struct GR_VIEWPORT_UNIFORMS_DATA
{
	matrix4f projection;
}
GR_VIEWPORT_UNIFORMS_DATA;


// TODO: Later...

typedef struct GR_TNW2_VERTEX
{
	vector3f position;
	vector3f normal;
	vector2f uv;
	float weights[2];	// uses GR_W2_MESH_UNIFORMS_DATA.modelView[x]
}
GR_TNW2_VERTEX;

#define WK_MESH_UNIFORM_DATA_NUM_POINT_LIGHTS 4

typedef struct GRAPHICS_POINT_LIGHT
{
	vector3f position;
	vector4f colorAndRadius;
}
GRAPHICS_POINT_LIGHT;

typedef struct GR_W2_MESH_UNIFORMS_DATA
{
	//GRAPHICS_POINT_LIGHT pointLights[WK_MESH_UNIFORM_DATA_NUM_POINT_LIGHTS];
	matrix4f modelView[2];	// used by GR_T_VERTEX.weights[x]
	matrix4f projection;
	vector3f ambient;
}
GR_W2_MESH_UNIFORMS_DATA;

#endif /* graphics_buffer_data_h */
