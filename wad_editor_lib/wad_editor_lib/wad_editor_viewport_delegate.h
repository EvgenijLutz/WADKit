//
//  wad_editor_viewport_delegate.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_viewport_delegate_h
#define wad_editor_viewport_delegate_h

#include "wad_editor_viewport_delegate_functions.h"

typedef struct WAD_EDITOR_VIEWPORT_DELEGATE
{
	WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_DOWN_FUNC mouseDownFunc;
	WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_UP_FUNC mouseUpFunc;
	WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_MOVE_FUNC mouseMoveFunc;
	
	WAD_EDITOR_VIEWPORT_DELEGATE_DRAW_FUNC renderFunc;
	
	void* userInfo;
}
WAD_EDITOR_VIEWPORT_DELEGATE;

#endif /* wad_editor_viewport_delegate_h */
