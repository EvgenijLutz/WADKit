//
//  static_object.h
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef static_object_h
#define static_object_h

#include "private_interface.h"

typedef struct STATIC
{
	WK_WAD* wad;
	
	STATIC_ID staticId;
	MESH* mesh;
	
	// Visibility box
	short vx1;
	short vy1;
	short vz1;
	
	short vx2;
	short vy2;
	short vz2;
	
	// Collision box
	short cx1;
	short cy1;
	short cz1;
	
	short cx2;
	short cy2;
	short cz2;
	
	unsigned short flags;
}
STATIC;

#endif /* static_object_h */
