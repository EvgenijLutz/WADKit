//
//  texture_page_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"


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
