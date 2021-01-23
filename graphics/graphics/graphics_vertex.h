//
//  graphics_vertex.h
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

#ifndef graphics_vertex_h
#define graphics_vertex_h

#if !defined(WAD_KIT_GRAPHICS_MATH_REDEFINED)
#include "graphics.h"
#endif

typedef struct GRAPHICS_VERTEX
{
	vector3f position;
	//vector3f normal;
	vector2f uv;
	//float shade;
	float weights[2];	// uses GRAPHICS_MESH_UNIFORMS_DATA.modelView[x]
}
GRAPHICS_VERTEX;

#endif /* graphics_vertex_h */
