//
//  animation_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 24.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

unsigned int animationGetFrameDuration(ANIMATION* animation)
{
	assert(animation);
	return animation->frameDuration;
}

unsigned int animationGetNumKeyframes(ANIMATION* animation)
{
	assert(animation);
	return animation->keyframes.length;
}

KEYFRAME* animationGetKeyframe(ANIMATION* animation, unsigned int keyframeIndex)
{
	assert(animation);
	return arrayGetItem(&animation->keyframes, keyframeIndex);
}
