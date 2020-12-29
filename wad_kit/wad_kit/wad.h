//
//  wad.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//
// Achtung! These structures do not reflect .wad file

#ifndef wad_kit_wad_h
#define wad_kit_wad_h

#include "private_interface.h"

typedef struct WK_WAD
{
	unsigned int version;
	
	/*!
	 Array of @b TEXTURE_PAGE elements.
	 */
	WK_ARRAY texturePages;
	
	/*!
	 Array of @b TEXTURE_SAMPLE elements.
	 */
	WK_ARRAY textureSamples;
	
	/*!
	 Array of @b WK_ARRAY elements.
	 */
	WK_ARRAY meshes;
	
	/*!
	 Array of @b SKELETON elements.
	 In theory, movables can refer to the same skeleton. Let's check it in future.
	 If not, then move each skeleton instance to related movable. */
	WK_ARRAY skeletons;
	
	unsigned int numMovables;
	MOVABLE* movables;
	
	unsigned int numStatics;
	STATIC* statics;
}
WK_WAD;

#endif /* wad_kit_wad_h */
