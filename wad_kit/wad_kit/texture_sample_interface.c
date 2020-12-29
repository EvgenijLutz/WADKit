//
//  texture_sample_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"


static void _textureSample_calculateUVData(TEXTURE_SAMPLE* textureSample)
{
	const float coefficientX = 1.0f / WK_TEXTURE_PAGE_WIDTH;
	const float coefficientY = 1.0f / WK_TEXTURE_PAGE_HEIGHT;
	
	textureSample->uvLocation.x = (float)textureSample->x * coefficientX;
	textureSample->uvLocation.y = (float)textureSample->y * coefficientY;
	
	textureSample->uvSize.x = (float)(textureSample->addW + 1) * coefficientX;
	textureSample->uvSize.y = (float)(textureSample->addH + 1) * coefficientY;
	
	if (textureSample->flipX) {
		textureSample->uvLocation.x += textureSample->uvSize.x;
		textureSample->uvSize.x = -textureSample->uvSize.x;
	}
	
	if (textureSample->flipY) {
		textureSample->uvLocation.y += textureSample->uvSize.y;
		textureSample->uvSize.y = -textureSample->uvSize.y;
	}
}

void textureSampleInitializeFromBuffer(TEXTURE_SAMPLE* textureSample, WK_WAD* wad, WK_BUFFER* buffer, EXECUTE_RESULT* executeResult)
{
	assert(textureSample);
	
	textureSample->wad = wad;
	textureSample->numReferences = 0;
	
	textureSample->x = bufferReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	textureSample->y = bufferReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	unsigned short page = bufferReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	if (page > wad->texturePages.length) {
		executeResultSetFailed(executeResult, "Index of destination texture page exceeded number of pages in WAD.");
		return;
	}
	
	textureSample->page = wadGetTexturePage(wad, page);
	textureSample->page->numReferences++;
	
	textureSample->flipX = bufferReadInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	textureSample->addW = bufferReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	textureSample->flipY = bufferReadInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	textureSample->addH = bufferReadUInt8(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	_textureSample_calculateUVData(textureSample);
	
	executeResultSetSucceeded(executeResult);
}

void textureSampleDeinitialize(TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	assert(textureSample->numReferences == 0);
	textureSample->page->numReferences--;
}
