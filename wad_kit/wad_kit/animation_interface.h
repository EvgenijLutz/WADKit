//
//  animation_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 24.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef animation_interface_h
#define animation_interface_h

#include "wad_kit.h"

unsigned int animationGetFrameDuration(ANIMATION* animation);

unsigned int animationGetNumKeyframes(ANIMATION* animation);
KEYFRAME* animationGetKeyframe(ANIMATION* animation, unsigned int keyframeIndex);

#endif /* animation_interface_h */
