//
//  texture_sample.c
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

static void _textureSample_calculateUVData(WK_TEXTURE_SAMPLE* textureSample)
{
	const float onePixelX = 1.0f / WK_TEXTURE_PAGE_WIDTH;
	const float onePixelY = 1.0f / WK_TEXTURE_PAGE_HEIGHT;
	
	textureSample->uvLocation.x = (float)textureSample->x * onePixelX;
	textureSample->uvLocation.y = (float)textureSample->y * onePixelY;
	
	textureSample->uvSize.x = (float)(textureSample->addW + 1) * onePixelX;
	textureSample->uvSize.y = (float)(textureSample->addH + 1) * onePixelY;
	
	if (!textureSample->flipX) {
		textureSample->uvLocation.x += textureSample->uvSize.x;
		textureSample->uvSize.x = -textureSample->uvSize.x;
	}
	
	if (!textureSample->flipY) {
		textureSample->uvLocation.y += textureSample->uvSize.y;
		textureSample->uvSize.y = -textureSample->uvSize.y;
	}
	
	// Add a very-very tiny micro little inset in a uv square to avoid texture seams if we are using texture filtering
	// Offset 1/20 of a pixel
	const float tinyOffsetX = onePixelX / 20.0f /*+ onePixelX*/;
	const float tinyOffsetY = onePixelY / 20.0f /*+ onePixelY*/;
	
	if (textureSample->uvSize.x < 0) {
		textureSample->uvLocation.x -= tinyOffsetX;
		textureSample->uvSize.x += tinyOffsetX * 2.0f;
	}
	else {
		textureSample->uvLocation.x += tinyOffsetX;
		textureSample->uvSize.x -= tinyOffsetX * 2.0f;
	}
	
	if (textureSample->uvSize.y < 0) {
		textureSample->uvLocation.y -= tinyOffsetY;
		textureSample->uvSize.y += tinyOffsetY * 2.0f;
	}
	else {
		textureSample->uvLocation.y += tinyOffsetY;
		textureSample->uvSize.y -= tinyOffsetY * 2.0f;
	}
}

void textureSampleInitializeFromBuffer(WK_TEXTURE_SAMPLE* textureSample, WK_WAD* wad, BUFFER_READER* buffer, EXECUTE_RESULT* executeResult)
{
	assert(textureSample);
	
	textureSample->wad = wad;
	textureSample->numReferences = 0;
	
	textureSample->x = bufferReaderReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	textureSample->y = bufferReaderReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	unsigned short page = bufferReaderReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	if (page > wad->texturePages.length) {
		executeResultSetFailed(executeResult, "Index of destination texture page exceeded number of pages in WAD.");
		return;
	}
	
	textureSample->page = wadGetTexturePage(wad, page);
	textureSample->page->numReferences++;
	
	textureSample->flipX = bufferReaderReadInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	textureSample->addW = bufferReaderReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	textureSample->flipY = bufferReaderReadInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	textureSample->addH = bufferReaderReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	_textureSample_calculateUVData(textureSample);
	
	executeResultSetSucceeded(executeResult);
}

void textureSampleDeinitialize(WK_TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	assert(textureSample->numReferences == 0);
	textureSample->page->numReferences--;
}

// MARK: - Public interface

unsigned char textureSampleGetX(WK_TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->x;
}

unsigned char textureSampleGetY(WK_TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->y;
}

unsigned char textureSampleGetAddX(WK_TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->addW;
}

unsigned char textureSampleGetAddY(WK_TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->addH;
}

char textureSampleGetFlipX(WK_TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->flipX;
}

char textureSampleGetFlipY(WK_TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->flipY;
}


WK_TEXTURE_PAGE* textureSampleGetTexturePage(WK_TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->page;
}

vector2f textureSampleGetUVLocation(WK_TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->uvLocation;
}

vector2f textureSampleGetUVSize(WK_TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->uvSize;
}
