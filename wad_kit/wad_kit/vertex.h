//
//  vertex.h
//  wad_kit
//
//  Created by Евгений Лютц on 01.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_vertex_h
#define wad_kit_vertex_h

#include "wad_kit.h"

typedef struct WK_VERTEX
{
	short rawPositionX;
	short rawPositionY;
	short rawPositionZ;
	
	short rawNormalX;
	short rawNormalY;
	short rawNormalZ;
	
	short rawShade;
	
	vector3f position;
	vector3f normal;
	float shade;
}
WK_VERTEX;

#endif /* wad_kit_vertex_h */
