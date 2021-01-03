//
//  keyframe.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef keyframe_h
#define keyframe_h

#include "private_interface.h"

typedef struct /*__attribute__((__packed__))*/ ROTATION
{
	unsigned short rotx;
	unsigned short roty;
	unsigned short rotz;
	
	quaternionf rotation;
}
ROTATION;

typedef struct KEYFRAME
{
	ANIMATION* animation;
	
	short bb1x;
	short bb1y;
	short bb1z;
	
	short bb2x;
	short bb2y;
	short bb2z;
	
	// Root mesh offset
	short offx;
	short offy;
	short offz;
	
	vector3f boundingBoxStart;
	vector3f boundingBoxEnd;
	vector3f modelOffset;
	
	/* Number of rotations is always equal to object's number of meshes */
	ROTATION* rotations;
}
KEYFRAME;

#endif /* keyframe_h */
