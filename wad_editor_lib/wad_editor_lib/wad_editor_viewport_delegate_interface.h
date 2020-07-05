//
//  wad_editor_viewport_delegate_interface.h
//  wad_editor_lib_macos
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_viewport_delegate_interface_h
#define wad_editor_viewport_delegate_interface_h

#include "wad_editor_viewport_delegate_functions.h"

typedef struct WAD_EDITOR_VIEWPORT_DELEGATE WAD_EDITOR_VIEWPORT_DELEGATE;

WAD_EDITOR_VIEWPORT_DELEGATE* wadEditorViewportDelegateCreate(WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_DOWN_FUNC mouseDownFunc, WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_UP_FUNC mouseUpFunc, WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_MOVE_FUNC mouseMoveFunc, WAD_EDITOR_VIEWPORT_DELEGATE_DRAW_FUNC renderFunc, void* userInfo);
void wadEditorViewportDelegateRelease(WAD_EDITOR_VIEWPORT_DELEGATE* delegate);

#endif /* wad_editor_viewport_delegate_interface_h */
