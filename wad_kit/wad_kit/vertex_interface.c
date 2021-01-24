//
//  vertex_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

static void _vertex_updatePosition(VERTEX* vertex)
{
	float x = vertex->vx / 1024.0;
	float y = vertex->vy / 1024.0;
	float z = vertex->vz / 1024.0;
	vertex->position = vector3fCreate(-x, -y, z);
}

static void _vertex_updateNormal(VERTEX* vertex)
{
	float x = ((double)vertex->nx) / 16300.0;
	float y = ((double)vertex->ny) / 16300.0;
	float z = ((double)vertex->nz) / 16300.0;
	vertex->normal = vector3fNormalize(vector3fCreate(-x, -y, z));
}

static void _vertex_updateShade(VERTEX* vertex)
{
	//
}


void vertexInitializeWithRawPosition(VERTEX* vertex, short vx, short vy, short vz)
{
	assert(vertex);
	
	vertex->vx = vx;
	vertex->vy = vy;
	vertex->vz = vz;
	
	vertex->nx = 16300;
	vertex->ny = 0;
	vertex->nz = 0;
	
	vertex->shade = 0;
	
	_vertex_updatePosition(vertex);
	_vertex_updateNormal(vertex);
	_vertex_updateShade(vertex);
}

void vertexSetRawPosition(VERTEX* vertex, short vx, short vy, short vz)
{
	assert(vertex);
	
	vertex->vx = vx;
	vertex->vy = vy;
	vertex->vz = vz;
	_vertex_updatePosition(vertex);
}

void vertexSetRawNormal(VERTEX* vertex, short nx, short ny, short nz)
{
	assert(vertex);
	
	vertex->nx = nx;
	vertex->ny = ny;
	vertex->nz = nz;
	_vertex_updateNormal(vertex);
}

void vertexSetRawShade(VERTEX* vertex, short shade)
{
	assert(vertex);
	
	vertex->shade = shade;
	_vertex_updateShade(vertex);
}


vector3f vertexGetPosition(VERTEX* vertex)
{
	assert(vertex);
	return vertex->position;
}
