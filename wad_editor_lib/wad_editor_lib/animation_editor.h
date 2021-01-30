//
//  animation_editor.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 25.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef animation_editor_h
#define animation_editor_h

#include "private_interface.h"

typedef struct ANIMATION_EDITOR
{
	WAD_EDITOR* editor;
	
	MOVABLE_DATA* movableData;
	ANIMATION_DATA* selectedAnimation;
	
	int useInterpolation;
	int useQuaternionRotations;
}
ANIMATION_EDITOR;

#endif /* animation_editor_h */
