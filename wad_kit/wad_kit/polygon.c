//
//  polygon.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void polygonInitializeRawFromBuffer(WK_POLYGON* polygon, WK_MESH* mesh, BUFFER_READER* buffer, EXECUTE_RESULT* executeResult)
{
	assert(polygon);
	assert(mesh);
	assert(buffer);
	
	polygon->mesh = mesh;
	
	polygon->isTriangle = bufferReaderReadUInt16(buffer, executeResult) == 8 ? 1 : 0;
	if (executeResultIsFailed(executeResult)) { return; }
	
	polygon->v1 = bufferReaderReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	polygon->v2 = bufferReaderReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	polygon->v3 = bufferReaderReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	if (!polygon->isTriangle) {
		polygon->v4 = bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	polygon->textureInfo = bufferReaderReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	polygon->attributes = bufferReaderReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	polygon->unknown = bufferReaderReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	
	
	unsigned short textureFlags = polygon->textureInfo;
	polygon->flipped = (textureFlags & 0x8000) >> 15;
	polygon->textureSampleShape = (textureFlags & 0x7000) >> 12;
	
	if (polygon->isTriangle)
	{
		const short sampleIndex = textureFlags & 0x0fff;
		assert(sampleIndex >= 0);
		polygon->textureSampleIndex = sampleIndex;
	}
	else
	{
		const short sampleIndex = textureFlags & 0xffff;
		if (polygon->flipped)
		{
			polygon->textureSampleIndex = -sampleIndex;
		}
		else
		{
			polygon->textureSampleIndex = sampleIndex;
		}
	}
	
	const int textureSampleShape = polygon->textureSampleShape;
	assert(textureSampleShape == 0 || textureSampleShape == 2 || textureSampleShape == 4 ||
		   textureSampleShape == 6 || textureSampleShape == 7);
	assert(polygon->textureSampleIndex < mesh->wad->textureSamples.length);
	
	executeResultSetSucceeded(executeResult);
}


int polygonIsTriangle(WK_POLYGON* polygon)
{
	assert(polygon);
	return polygon->isTriangle;
}

WK_VERTEX* polygonGetVertex1(WK_POLYGON* polygon)
{
	assert(polygon);
	return magicArrayGetItem(&polygon->mesh->vertices, polygon->v1);
}

WK_VERTEX* polygonGetVertex2(WK_POLYGON* polygon)
{
	assert(polygon);
	return magicArrayGetItem(&polygon->mesh->vertices, polygon->v2);
}

WK_VERTEX* polygonGetVertex3(WK_POLYGON* polygon)
{
	assert(polygon);
	return magicArrayGetItem(&polygon->mesh->vertices, polygon->v3);
}

WK_VERTEX* polygonGetVertex4(WK_POLYGON* polygon)
{
	assert(polygon);
	return magicArrayGetItem(&polygon->mesh->vertices, polygon->v4);
}


WK_TEXTURE_SAMPLE* polygonGetTextureSample(WK_POLYGON* polygon)
{
	assert(polygon);
	return magicArrayGetItem(&polygon->mesh->wad->textureSamples, polygon->textureSampleIndex);
}

unsigned short polygonGetTextureSampleShape(WK_POLYGON* polygon)
{
	assert(polygon);
	return polygon->textureSampleShape;
}

unsigned short polygonIsFlipped(WK_POLYGON* polygon)
{
	assert(polygon);
	return polygon->flipped;
}
