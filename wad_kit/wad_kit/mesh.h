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

typedef struct MESH
{
	WK_WAD* wad;
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
	
	/*!
	 Array of @b VERTEX elements.
	 */
	WK_ARRAY vertices;
	
	/*!
	 Array of @b POLYGON elements.
	 */
	WK_ARRAY polygons;
}
MESH;

#endif /* mesh_h */
