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

typedef struct WK_ANIMATION
{
	WK_MOVABLE* movable;
	
	unsigned short stateId;
	
	/*!
	 @b WK_KEYFRAME array.
	 */
	MAGIC_ARRAY keyframes;
	
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
	 Array of @b WK_STATE_CHANGE elements.
	 */
	MAGIC_ARRAY stateChanges;
	
	/*!
	 Array of @b WK_COMMAND elements.
	 */
	MAGIC_ARRAY commands;
}
WK_ANIMATION;

#endif /* animation_h */
