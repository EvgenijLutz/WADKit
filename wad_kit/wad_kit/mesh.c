//
//  mesh.c
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

static void _mesh_updateSphereData(WK_MESH* mesh)
{
	//
}

void meshInitializeFromBuffer(WK_MESH* mesh, WK_WAD* wad, BUFFER_READER* buffer, EXECUTE_RESULT* executeResult)
{
	assert(mesh);
	char errorMessage[1024];
	
	mesh->wad = wad;
	mesh->numReferences = 0;
	//magicArrayInitialize(&mesh->vertices, sizeof(WK_VERTEX), 64);
	//magicArrayInitialize(&mesh->polygons, sizeof(WK_POLYGON), 32);
	magicArrayInitializeWithAllocator(&mesh->vertices, wad->rawVertexAllocator);
	magicArrayInitializeWithAllocator(&mesh->polygons, wad->rawPolygonAllocator);
	
	// MARK: Read mesh bounding sphere info
	
	mesh->cx = bufferReaderReadInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	mesh->cy = bufferReaderReadInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	mesh->cz = bufferReaderReadInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	mesh->radius = bufferReaderReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	mesh->unknown = bufferReaderReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	_mesh_updateSphereData(mesh);
	
	// MARK: Read vertices
	
	const unsigned short numVertices = bufferReaderReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	for (unsigned short vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
	{
		const unsigned short vx = bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		const unsigned short vy = bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		const unsigned short vz = bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		meshAddRawVertex(mesh, vx, vy, vz);
	}
	
	// MARK: Read normals/shades
	
	const short numNormalsShades = bufferReaderReadInt16(buffer, executeResult);
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
			const short nx = bufferReaderReadInt16(buffer, executeResult);
			if (executeResultIsFailed(executeResult)) { return; }
			
			const short ny = bufferReaderReadInt16(buffer, executeResult);
			if (executeResultIsFailed(executeResult)) { return; }
			
			const short nz = bufferReaderReadInt16(buffer, executeResult);
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
			const short shade = bufferReaderReadInt16(buffer, executeResult);
			if (executeResultIsFailed(executeResult)) { return; }
			
			meshSetRawVertexShade(mesh, shadeIndex, shade);
		}
	}
	
	// MARK: Read polygons
	
	const unsigned short numPolygons = bufferReaderReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	unsigned int numQuads = 0;
	for (unsigned int polygonIndex = 0; polygonIndex < numPolygons; polygonIndex++)
	{
		WK_POLYGON* polygon = magicArrayAddItem(&mesh->polygons);
		polygonInitializeRawFromBuffer(polygon, mesh, buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		numQuads += polygon->isTriangle == 0;
	}
	
	if (numQuads % 2)
	{
		// Skip garbage data
		bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	executeResultSetSucceeded(executeResult);
}

void meshDeinitialize(WK_MESH* mesh)
{
	assert(mesh);
	assert(mesh->numReferences == 0);
	magicArrayDeinitialize(&mesh->polygons);
	magicArrayDeinitialize(&mesh->vertices);
}

WK_WAD* meshGetWad(WK_MESH* mesh)
{
	assert(mesh);
	return mesh->wad;
}

int meshUsesNormals(WK_MESH* mesh)
{
	assert(mesh);
	return mesh->usesNormals;
}

void meshSetUsesNormals(WK_MESH* mesh, int uses)
{
	assert(mesh);
	mesh->usesNormals = (uses != 0);
}


unsigned int meshGetNumVertices(WK_MESH* mesh)
{
	assert(mesh);
	return mesh->vertices.length;
}

WK_VERTEX meshGetVertex(WK_MESH* mesh, unsigned int vertexIndex)
{
	assert(mesh);
	return *(WK_VERTEX*)magicArrayGetItem(&mesh->vertices, vertexIndex);
}

void meshAddRawVertex(WK_MESH* mesh, unsigned short vx, unsigned short vy, unsigned short vz)
{
	assert(mesh);
	WK_VERTEX* vertex = magicArrayAddItem(&mesh->vertices);
	vertexInitializeWithRawPosition(vertex, vx, vy, vz);
	
	// TODO: use mesh->usesNormals
}

void meshSetRawVertexNormal(WK_MESH* mesh, unsigned int vertexIndex, short nx, short ny, short nz)
{
	assert(mesh);
	WK_VERTEX* vertex = magicArrayAddItem(&mesh->vertices);
	vertexSetRawNormal(vertex, nx, ny, nz);
}

void meshSetRawVertexShade(WK_MESH* mesh, unsigned int vertexIndex, short shade)
{
	assert(mesh);
	WK_VERTEX* vertex = magicArrayAddItem(&mesh->vertices);
	vertexSetRawShade(vertex, shade);
}


unsigned int meshGetNumPolygons(WK_MESH* mesh)
{
	assert(mesh);
	return mesh->polygons.length;
}

WK_POLYGON* meshGetPolygon(WK_MESH* mesh, unsigned int polygonIndex)
{
	assert(mesh);
	return magicArrayGetItem(&mesh->polygons, polygonIndex);
}
