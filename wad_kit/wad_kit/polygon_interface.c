//
//  polygon_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void polygonInitializeRawFromBuffer(POLYGON* polygon, MESH* mesh, WK_BUFFER* buffer, EXECUTE_RESULT* executeResult)
{
	assert(polygon);
	assert(mesh);
	assert(buffer);
	
	polygon->mesh = mesh;
	
	polygon->isTriangle = bufferReadUInt16(buffer, executeResult) == 8 ? 1 : 0;
	if (executeResultIsFailed(executeResult)) { return; }
	
	polygon->v1 = bufferReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	polygon->v2 = bufferReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	polygon->v3 = bufferReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	if (!polygon->isTriangle) {
		polygon->v4 = bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	polygon->textureInfo = bufferReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	polygon->attributes = bufferReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	polygon->unknown = bufferReadUInt8(buffer, executeResult);
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
