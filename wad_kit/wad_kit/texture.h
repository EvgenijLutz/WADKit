//
//  texture.h
//  wad_kit
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef texture_h
#define texture_h

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

#endif /* texture_h */
