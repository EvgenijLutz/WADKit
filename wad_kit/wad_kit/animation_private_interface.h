//
//  animation_private_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef animation_private_interface_h
#define animation_private_interface_h

#include "private_interface.h"

void animationInitialize(WK_ANIMATION* animation, WK_MOVABLE* movable, RAW_MOVABLE* rawMovable, RAW_ANIMATION* rawAnimation, RAW_ANIMATION* nextRawAnimation, WK_WAD_LOAD_INFO* loadInfo);
void animationDeinitialize(WK_ANIMATION* animation);

#endif /* animation_private_interface_h */
