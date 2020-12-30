//
//  animation_internal_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef animation_internal_interface_h
#define animation_internal_interface_h

#include "private_interface.h"

void animationInitialize(ANIMATION* animation, MOVABLE* movable, RAW_MOVABLE* rawMovable, RAW_ANIMATION* rawAnimation, RAW_ANIMATION* nextRawAnimation, WK_WAD_LOAD_INFO* loadInfo);
void animationDeinitialize(ANIMATION* animation);

#endif /* animation_internal_interface_h */
