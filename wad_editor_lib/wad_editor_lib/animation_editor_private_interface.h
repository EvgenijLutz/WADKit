//
//  animation_editor_private_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 25.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef animation_editor_private_interface_h
#define animation_editor_private_interface_h

#include "private_interface.h"

void animationEditorInitialize(ANIMATION_EDITOR* animationEditor, WAD_EDITOR* wadEditor);
void animationEditorDenitialize(ANIMATION_EDITOR* animationEditor);

#endif /* animation_editor_private_interface_h */
