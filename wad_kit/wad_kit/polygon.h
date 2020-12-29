//
//  polygon.h
//  wad_kit
//
//  Created by Евгений Лютц on 01.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_polygon_h
#define wad_kit_polygon_h

#include "wad_kit.h"

typedef struct POLYGON
{
	MESH* mesh;
	
	int isTriangle;
	
	// Vertex indices
	unsigned short v1;
	unsigned short v2;
	unsigned short v3;
	unsigned short v4;		// optional - this value is not used when isTriangle is true
	
	unsigned short textureInfo;
	unsigned char attributes;
	unsigned char unknown;	// garbage?
	
	// Calculated values: (calculated from textureInfo. In future remove that variable)
	unsigned short flipped;
	unsigned short textureSampleShape;
	unsigned short textureSampleIndex;
}
POLYGON;

#endif /* wad_kit_polygon_h */
