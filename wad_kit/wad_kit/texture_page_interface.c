//
//  texture_page_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "texture_page_interface.h"
#include "texture_page.h"
#include "common.h"

unsigned char* texturePageGetData(TEXTURE_PAGE* texturePage)
{
	assert(texturePage);
	return texturePage->data;
}
