//
//  mesh_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef mesh_interface_h
#define mesh_interface_h

#include "wk_vertex.h"
#include "wk_polygon.h"

typedef struct WAD WAD;
typedef struct MESH MESH;

WAD* meshGetWad(MESH* mesh);

int meshUsesNormals(MESH* mesh);

unsigned int meshGetNumVertices(MESH* mesh);
VERTEX meshGetVertex(MESH* mesh, unsigned int vertexIndex);

unsigned int meshGetNumPolygons(MESH* mesh);
POLYGON meshGetPolygon(MESH* mesh, unsigned int polygonIndex);

#endif /* mesh_interface_h */
