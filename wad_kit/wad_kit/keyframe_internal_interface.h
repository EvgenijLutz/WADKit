//
//  keyframe_internal_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef keyframe_internal_interface_h
#define keyframe_internal_interface_h

#include "private_interface.h"

void keyframeInitialize(KEYFRAME* keyframe, ANIMATION* animation, RAW_ANIMATION* rawAnimation, WK_WAD_LOAD_INFO* loadInfo);
void keyframeDeinitialize(KEYFRAME* keyframe);

#endif /* keyframe_internal_interface_h */
