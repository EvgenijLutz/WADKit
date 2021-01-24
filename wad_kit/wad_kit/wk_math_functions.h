//
//  wk_math_functions.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wk_math_functions_h
#define wk_math_functions_h

#include "wad_kit.h"

matrix4f matrix4fScale(vector3f scale);
matrix4f matrix4fTranslation(float tx, float ty, float tz);
matrix4f matrix4fRotation(float radians, vector3f axis);
matrix4f matrix4fPerspectiveRightHand(float fovyRadians, float aspectRatio, float frustumNearZ, float frustumFarZ);
matrix4f matrix4fLookAt_rightHand(vector3f eye, vector3f target, vector3f up);

matrix4f matrix4fFromQuaternion(quaternionf quaternion);

#endif /* wk_math_functions_h */
