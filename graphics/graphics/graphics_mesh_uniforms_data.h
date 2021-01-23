//
//  graphics_mesh_uniforms_data.h
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

#ifndef graphics_mesh_uniforms_data_h
#define graphics_mesh_uniforms_data_h

#if !defined(WAD_KIT_GRAPHICS_MATH_REDEFINED)
#include "graphics.h"
#endif

#define WK_MESH_UNIFORM_DATA_NUM_POINT_LIGHTS 4

typedef struct GRAPHICS_POINT_LIGHT
{
	vector3f position;
	vector4f colorAndRadius;
}
GRAPHICS_POINT_LIGHT;

typedef struct GRAPHICS_MESH_UNIFORMS_DATA
{
	//GRAPHICS_POINT_LIGHT pointLights[WK_MESH_UNIFORM_DATA_NUM_POINT_LIGHTS];
	matrix4f modelView[2];	// used by GRAPHICS_VERTEX.weights[x]
	matrix4f projection;
	vector3f ambient;
}
GRAPHICS_MESH_UNIFORMS_DATA;

#endif /* graphics_mesh_uniforms_data_h */
