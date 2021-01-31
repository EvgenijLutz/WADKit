//
//  vertex.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

static void _vertex_updatePosition(WK_VERTEX* vertex)
{
	float x = vertex->rawPositionX / WK_COORDINATE_MULTIPLIER;
	float y = vertex->rawPositionY / WK_COORDINATE_MULTIPLIER;
	float z = vertex->rawPositionZ / WK_COORDINATE_MULTIPLIER;
	vertex->position = vector3fCreate(-x, -y, z);
}

static void _vertex_updateNormal(WK_VERTEX* vertex)
{
	float x = vertex->rawNormalX / WK_NORMAL_MULTIPLIER;
	float y = vertex->rawNormalY / WK_NORMAL_MULTIPLIER;
	float z = vertex->rawNormalZ / WK_NORMAL_MULTIPLIER;
	vertex->normal = vector3fNormalize(vector3fCreate(-x, -y, z));
}

static void _vertex_updateShade(WK_VERTEX* vertex)
{
	//#define COLOR_TO_F(color) (1.0f / 255.0f * ((float)(color * 8)))
	// grey_intensity = 255 - shade * 255 / 8191
	
	// TODO: look at shade values, chech it
	//assert(0);
	
	float shade = 1.0f / 255.0f * ((float)vertex->rawShade * 8.0f);
	if (shade > 1.0f)
	{
		shade = 1.0f;
	}
	if (shade < 0.0f)
	{
		shade = 0.0f;
	}
	vertex->shade = shade;
}


void vertexInitializeWithRawPosition(WK_VERTEX* vertex, short vx, short vy, short vz)
{
	assert(vertex);
	
	vertex->rawPositionX = vx;
	vertex->rawPositionY = vy;
	vertex->rawPositionZ = vz;
	
	vertex->rawNormalX = (short)WK_NORMAL_MULTIPLIER;
	vertex->rawNormalY = 0;
	vertex->rawNormalZ = 0;
	
	vertex->rawShade = 0;
	
	_vertex_updatePosition(vertex);
	_vertex_updateNormal(vertex);
	_vertex_updateShade(vertex);
}

void vertexSetRawPosition(WK_VERTEX* vertex, short vx, short vy, short vz)
{
	assert(vertex);
	
	vertex->rawPositionX = vx;
	vertex->rawPositionY = vy;
	vertex->rawPositionZ = vz;
	_vertex_updatePosition(vertex);
}

void vertexSetRawNormal(WK_VERTEX* vertex, short nx, short ny, short nz)
{
	assert(vertex);
	
	vertex->rawNormalX = nx;
	vertex->rawNormalY = ny;
	vertex->rawNormalZ = nz;
	_vertex_updateNormal(vertex);
}

void vertexSetRawShade(WK_VERTEX* vertex, short shade)
{
	assert(vertex);
	
	vertex->rawShade = shade;
	_vertex_updateShade(vertex);
}


vector3f vertexGetPosition(WK_VERTEX* vertex)
{
	assert(vertex);
	return vertex->position;
}
