//
//  graphics_mesh.h
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

#ifndef graphics_mesh_h
#define graphics_mesh_h

#include "private_interface.h"

typedef struct GRAPHICS_MESH
{
	GRAPHICS_DEVICE* device;
	unsigned int numVertices;
	void* meshId;
}
GRAPHICS_MESH;

typedef struct GRAPHICS_MESH_UNIFORMS
{
	GRAPHICS_DEVICE* device;
	void* uniformsId;
}
GRAPHICS_MESH_UNIFORMS;

#endif /* graphics_mesh_h */
