//
//  texture_page.h
//  wad_kit
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_texture_page_h
#define wad_kit_texture_page_h

#include "wad_kit.h"

typedef struct WK_TEXTURE_PAGE
{
	WK_WAD* wad;
	unsigned long numReferences;
	
	/*!
	 Bitmap representation of 8-bit texture page. The size is always 256 * 256 * 3: width, height and number of color components.
	 */
	unsigned char* data;
}
WK_TEXTURE_PAGE;

#endif /* wad_kit_texture_page_h */
