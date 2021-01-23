//
//  mesh_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

static void _mesh_updateSphereData(MESH* mesh)
{
	//
}

void meshInitializeFromBuffer(MESH* mesh, WK_WAD* wad, WK_BUFFER* buffer, EXECUTE_RESULT* executeResult)
{
	assert(mesh);
	char errorMessage[1024];
	
	mesh->wad = wad;
	mesh->numReferences = 0;
	//arrayInitializeWithCapacityIncrement(&mesh->vertices, sizeof(VERTEX), 64);
	//arrayInitializeWithCapacityIncrement(&mesh->polygons, sizeof(POLYGON), 32);
	arrayInitializeWithAllocator(&mesh->vertices, wad->rawVertexAllocator);
	arrayInitializeWithAllocator(&mesh->polygons, wad->rawPolygonAllocator);
	
	// MARK: Read mesh bounding sphere info
	
	mesh->cx = bufferReadInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	mesh->cy = bufferReadInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	mesh->cz = bufferReadInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	mesh->radius = bufferReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	mesh->unknown = bufferReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	_mesh_updateSphereData(mesh);
	
	// MARK: Read vertices
	
	const unsigned short numVertices = bufferReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	for (unsigned short vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
	{
		const unsigned short vx = bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		const unsigned short vy = bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		const unsigned short vz = bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		meshAddRawVertex(mesh, vx, vy, vz);
	}
	
	// MARK: Read normals/shades
	
	const short numNormalsShades = bufferReadInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	int usesNormals = numNormalsShades > 0;
	meshSetUsesNormals(mesh, usesNormals);
	if (usesNormals)
	{
		const unsigned short numNormals = numNormalsShades;
		if (numVertices != numNormals)
		{
			sprintf(errorMessage, "Number of vertices(%d) does not match the number of normals(%d)", numVertices, numNormals);
			executeResultSetFailed(executeResult, errorMessage);
			return;
		}
		
		for (unsigned int normalIndex = 0; normalIndex < numNormals; normalIndex++)
		{
			const short nx = bufferReadInt16(buffer, executeResult);
			if (executeResultIsFailed(executeResult)) { return; }
			
			const short ny = bufferReadInt16(buffer, executeResult);
			if (executeResultIsFailed(executeResult)) { return; }
			
			const short nz = bufferReadInt16(buffer, executeResult);
			if (executeResultIsFailed(executeResult)) { return; }
			
			meshSetRawVertexNormal(mesh, normalIndex, nx, ny, nz);
		}
	}
	else
	{
		const unsigned short numShades = -numNormalsShades;
		if (numVertices != numShades)
		{
			sprintf(errorMessage, "Number of vertices(%d) does not match with number of shades(%d)", numVertices, numShades);
			executeResultSetFailed(executeResult, errorMessage);
			return;
		}
		
		for (unsigned int shadeIndex = 0; shadeIndex < numShades; shadeIndex++)
		{
			const short shade = bufferReadInt16(buffer, executeResult);
			if (executeResultIsFailed(executeResult)) { return; }
			
			meshSetRawVertexShade(mesh, shadeIndex, shade);
		}
	}
	
	// MARK: Read polygons
	
	const unsigned short numPolygons = bufferReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	unsigned int numQuads = 0;
	for (unsigned int polygonIndex = 0; polygonIndex < numPolygons; polygonIndex++)
	{
		POLYGON* polygon = arrayAddItem(&mesh->polygons);
		polygonInitializeRawFromBuffer(polygon, mesh, buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		numQuads += polygon->isTriangle == 0;
	}
	
	if (numQuads % 2)
	{
		// Skip garbage data
		bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	executeResultSetSucceeded(executeResult);
}

void meshDeinitialize(MESH* mesh)
{
	assert(mesh);
	assert(mesh->numReferences == 0);
	arrayDeinitialize(&mesh->polygons);
	arrayDeinitialize(&mesh->vertices);
}

WK_WAD* meshGetWad(MESH* mesh)
{
	assert(mesh);
	return mesh->wad;
}

int meshUsesNormals(MESH* mesh)
{
	assert(mesh);
	return mesh->usesNormals;
}

void meshSetUsesNormals(MESH* mesh, int uses)
{
	assert(mesh);
	mesh->usesNormals = (uses != 0);
}


unsigned int meshGetNumVertices(MESH* mesh)
{
	assert(mesh);
	return mesh->vertices.length;
}

VERTEX meshGetVertex(MESH* mesh, unsigned int vertexIndex)
{
	assert(mesh);
	return *(VERTEX*)arrayGetItem(&mesh->vertices, vertexIndex);
}

void meshAddRawVertex(MESH* mesh, unsigned short vx, unsigned short vy, unsigned short vz)
{
	assert(mesh);
	VERTEX* vertex = arrayAddItem(&mesh->vertices);
	vertexInitializeWithRawPosition(vertex, vx, vy, vz);
	
	// TODO: use mesh->usesNormals
}

void meshSetRawVertexNormal(MESH* mesh, unsigned int vertexIndex, short nx, short ny, short nz)
{
	assert(mesh);
	VERTEX* vertex = arrayAddItem(&mesh->vertices);
	vertexSetRawNormal(vertex, nx, ny, nz);
}

void meshSetRawVertexShade(MESH* mesh, unsigned int vertexIndex, short shade)
{
	assert(mesh);
	VERTEX* vertex = arrayAddItem(&mesh->vertices);
	vertexSetRawShade(vertex, shade);
}


unsigned int meshGetNumPolygons(MESH* mesh)
{
	assert(mesh);
	return mesh->polygons.length;
}

POLYGON meshGetPolygon(MESH* mesh, unsigned int polygonIndex)
{
	assert(mesh);
	return *(POLYGON*)arrayGetItem(&mesh->polygons, polygonIndex);
}
