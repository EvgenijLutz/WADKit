//
//  graphics_mesh.h
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

#ifndef graphics_mesh_h
#define graphics_mesh_h

#include "private_interface.h"

typedef struct GR_DRAWABLE
{
	int type;
	int visible;
	
	TEXTURE2D* texture;
	GR_BUFFER* vertexBuffer;
	GR_BUFFER* meshUniforms;
}
GR_DRAWABLE;

#endif /* graphics_mesh_h */
