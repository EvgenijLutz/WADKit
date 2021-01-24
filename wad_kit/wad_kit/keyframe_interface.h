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

vector3f keyframeGetRootOffset(KEYFRAME* keyframe);
vector3f keyframeGetInterpolatedRootOffset(KEYFRAME* firstKeyframe, KEYFRAME* secondKeyframe, float interpolationCoefficient);

quaternionf keyframeGetRotationQuaternion(KEYFRAME* keyframe, unsigned int transformIndex);
vector3f keyframeCalculateRotation(KEYFRAME* keyframe, unsigned int transformIndex);
vector3f keyframeCalculateInterpolatedRotation(KEYFRAME* firstKeyframe, KEYFRAME* secondKeyframe, unsigned int transformIndex, float interpolationCoefficient);

#endif /* keyframe_interface_h */
