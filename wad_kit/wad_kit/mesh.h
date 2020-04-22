//
//  mesh.h
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef mesh_h
#define mesh_h

typedef struct VERTEX VERTEX;
typedef struct POLYGON POLYGON;

// hello?

typedef struct MESH
{
	//BOUNDING_SPHERE boundingSphere;
	short cx;
	short cy;
	short cz;
	unsigned short radius;
	unsigned short unknown;	// garbage?
	
	unsigned char usesNormals;
	unsigned short numVertices;
	VERTEX* vertices;
	
	unsigned short numPolygons;
	POLYGON* polygons;
}
MESH;

#endif /* mesh_h */
