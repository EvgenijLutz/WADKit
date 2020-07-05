//
//  mesh_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include "mesh_interface.h"
#include "mesh.h"
#include <stdlib.h>
#include <assert.h>

WAD* meshGetWad(MESH* mesh)
{
	assert(mesh);
	return mesh->wad;
}

int meshUsesNormals(MESH* mesh)
{
	assert(mesh);
	return mesh->usesNormals;
}


unsigned int meshGetNumVertices(MESH* mesh)
{
	assert(mesh);
	return mesh->numVertices;
}

VERTEX meshGetVertex(MESH* mesh, unsigned int vertexIndex)
{
	assert(mesh);
	assert(vertexIndex < mesh->numVertices);
	return mesh->vertices[vertexIndex];
}


unsigned int meshGetNumPolygons(MESH* mesh)
{
	assert(mesh);
	return mesh->numPolygons;
}

POLYGON meshGetPolygon(MESH* mesh, unsigned int polygonIndex)
{
	assert(mesh);
	assert(polygonIndex < mesh->numPolygons);
	return mesh->polygons[polygonIndex];
}
