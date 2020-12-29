//
//  texture_sample.h
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef texture_sample_h
#define texture_sample_h

#include "private_interface.h"

typedef struct TEXTURE_SAMPLE
{
	WK_WAD* wad;
	TEXTURE_PAGE* page;
	unsigned long numReferences;
	
	unsigned char x;
	unsigned char y;
	unsigned char addW;
	unsigned char addH;
	char flipX;
	char flipY;
	
	/*!
	 Top left coordinate of the tile.
	 */
	vector2f uvLocation;
	
	/*!
	 Size of the tile, direction to bottom right.
	 */
	vector2f uvSize;
}
TEXTURE_SAMPLE;

#endif /* texture_sample_h */
