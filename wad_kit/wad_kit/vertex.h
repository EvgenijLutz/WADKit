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

typedef struct VERTEX
{
	short vx;
	short vy;
	short vz;
	
	short nx;
	short ny;
	short nz;
	
	short shade;
	
	vector3f position;
	vector3f nornal;
}
VERTEX;

#endif /* wad_kit_vertex_h */
