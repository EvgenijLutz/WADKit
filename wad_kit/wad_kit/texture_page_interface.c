//
//  texture_page_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void texturePageInitializeFromBuffer(TEXTURE_PAGE* texturePage, WK_WAD* wad, WK_BUFFER* buffer, EXECUTE_RESULT* executeResult)
{
	assert(texturePage);
	assert(wad);
	assert(buffer);
	
	const char* textureData = bufferRequestDataToRead(buffer, WK_SIZE_OF_TEXTURE_PAGE, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	texturePage->wad = wad;
	texturePage->numReferences = 0;
	texturePage->data = malloc(WK_SIZE_OF_TEXTURE_PAGE);
	memcpy(texturePage->data, textureData, WK_SIZE_OF_TEXTURE_PAGE);
}

void texturePageInitializeEmpty(TEXTURE_PAGE* texturePage, WK_WAD* wad)
{
	assert(texturePage);
	assert(wad);
	
	texturePage->wad = wad;
	texturePage->numReferences = 0;
	texturePage->data = malloc(WK_SIZE_OF_TEXTURE_PAGE);
	memset(texturePage->data, 0, WK_SIZE_OF_TEXTURE_PAGE);
}

void texturePageInitializeWithData(TEXTURE_PAGE* texturePage, WK_WAD* wad, const char* data)
{
	assert(texturePage);
	assert(wad);
	assert(data);
	
	texturePage->wad = wad;
	texturePage->numReferences = 0;
	texturePage->data = malloc(WK_SIZE_OF_TEXTURE_PAGE);
	memcpy(texturePage->data, data, WK_SIZE_OF_TEXTURE_PAGE);
}

void texturePageDeinitialize(TEXTURE_PAGE* texturePage)
{
	assert(texturePage);
	
	assert(texturePage->numReferences == 0);
	
	free(texturePage->data);
#if DEBUG
	memset(texturePage, 0, sizeof(TEXTURE_PAGE));
#endif
}


unsigned long texturePageGetNumReferences(TEXTURE_PAGE* texturePage)
{
	assert(texturePage);
	return texturePage->numReferences;
}

unsigned char* texturePageGetData(TEXTURE_PAGE* texturePage)
{
	assert(texturePage);
	return texturePage->data;
}
