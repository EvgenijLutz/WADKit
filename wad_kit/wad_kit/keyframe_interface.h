//
//  keyframe_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 24.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef keyframe_interface_h
#define keyframe_interface_h

#include "wad_kit.h"

vector3f keyframeGetRootOffset(WK_KEYFRAME* keyframe);
vector3f keyframeGetInterpolatedRootOffset(WK_KEYFRAME* firstKeyframe, WK_KEYFRAME* secondKeyframe, float interpolationCoefficient);

quaternionf keyframeGetQuaternionRotation(WK_KEYFRAME* keyframe, unsigned int transformIndex);
vector3f keyframeGetEulerZXYRotation(WK_KEYFRAME* keyframe, unsigned int transformIndex);
vector3f keyframeCalculateInterpolatedEulerZXYRotation(WK_KEYFRAME* firstKeyframe, WK_KEYFRAME* secondKeyframe, unsigned int transformIndex, float interpolationCoefficient);

#endif /* keyframe_interface_h */
