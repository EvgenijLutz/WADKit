//
//  texture_page.h
//  wad_kit
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_texture_page_h
#define wad_kit_texture_page_h

typedef struct TEXTURE_SAMPLE
{
	unsigned char x;
	unsigned char y;
	unsigned short page;
	char flipX;
	unsigned char addW;
	char flipY;
	unsigned char addH;
}
TEXTURE_SAMPLE;

typedef struct TEXTURE_PAGE
{
	unsigned char data[256 * 256 * 3];
}
TEXTURE_PAGE;

#endif /* wad_kit_texture_page_h */
