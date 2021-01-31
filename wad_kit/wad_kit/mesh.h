//
//  mesh.h
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef mesh_h
#define mesh_h

#include "private_interface.h"

/*typedef struct WK_TEXTURE WK_TEXTURE;
typedef struct WK_MESH_EXTENSION WK_MESH_EXTENSION;

typedef struct WK_SUBMESH_EXTENSION
{
	WK_MESH_EXTENSION* meshExtension;
	
	WK_TEXTURE* texture;
	WK_ARRAY vertices;
	WK_ARRAY polygons;
}
WK_SUBMESH_EXTENSION;

typedef struct WK_MESH_EXTENSION
{
	WK_MESH* mesh;
	
	vector3f sphereLocation;
	float sphereRadius;
	
	WK_ARRAY submeshes;
}
WK_MESH_EXTENSION;*/

typedef struct WK_MESH
{
	WK_WAD* wad;
	//WK_MESH_EXTENSION* extension;
	unsigned long numReferences;
	
	short cx;
	short cy;
	short cz;
	unsigned short radius;
	
	/*!
	 Presumably garbage.
	 */
	unsigned short unknown;
	
	int usesNormals;
	
	/// Array of @b WK_VERTEX elements.
	MAGIC_ARRAY vertices;
	
	/// Array of @b WK_POLYGON elements.
	MAGIC_ARRAY polygons;
}
WK_MESH;

#endif /* mesh_h */
