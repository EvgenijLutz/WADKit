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

typedef struct /*__attribute__((__packed__))*/ WK_ROTATION
{
	unsigned short rawEulerRotationX;
	unsigned short rawEulerRotationY;
	unsigned short rawEulerRotationZ;
	
	vector3f eulerRotation;
	quaternionf quaternionRotation;
}
WK_ROTATION;

typedef struct WK_KEYFRAME
{
	WK_ANIMATION* animation;
	
	short bb1x;
	short bb1y;
	short bb1z;
	
	short bb2x;
	short bb2y;
	short bb2z;
	
	// Root mesh offset
	short rawModelOffsetX;
	short rawModelOffsetY;
	short rawModelOffsetZ;
	
	vector3f boundingBoxStart;
	vector3f boundingBoxEnd;
	vector3f modelOffset;
	
	/* Number of rotations is always equal to object's number of meshes */
	WK_ROTATION* rotations;
}
WK_KEYFRAME;

#endif /* keyframe_h */
