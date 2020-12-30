//
//  animation.h
//  wad_kit
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef animation_h
#define animation_h

#include "private_interface.h"

typedef struct ANIMATION
{
	MOVABLE* movable;
	
	unsigned short stateId;
	
	/*!
	 @b KEYFRAME array.
	 */
	WK_ARRAY keyframes;
	
	unsigned char frameDuration;
	short unknown1;
	short moveSpeed;
	int moveAcceleration;
	long unknown2;
	
	unsigned short frameStart;
	unsigned short frameEnd;
	unsigned short nextAnimation;
	
	/// ?????????????????????????????????
	unsigned short nextAnimationFrame;
	
	/*!
	 Array of @b STATE_CHANGE elements.
	 */
	WK_ARRAY stateChanges;
	
	/*!
	 Array of @b COMMAND elements.
	 */
	WK_ARRAY commands;
}
ANIMATION;

#endif /* animation_h */
