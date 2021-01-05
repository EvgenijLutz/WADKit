//
//  wad_editor_viewport_delegate_interface.c
//  wad_editor_lib_macos
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

WAD_EDITOR_VIEWPORT_DELEGATE* wadEditorViewportDelegateCreate(WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_DOWN_FUNC mouseDownFunc, WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_UP_FUNC mouseUpFunc, WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_MOVE_FUNC mouseMoveFunc, WAD_EDITOR_VIEWPORT_DELEGATE_DRAW_FUNC renderFunc, void* userInfo)
{
	assert(mouseDownFunc);
	assert(mouseUpFunc);
	assert(mouseMoveFunc);
	assert(renderFunc);
	
	WAD_EDITOR_VIEWPORT_DELEGATE* delegate = malloc(sizeof(WAD_EDITOR_VIEWPORT_DELEGATE));
	delegate->mouseDownFunc = mouseDownFunc;
	delegate->mouseUpFunc = mouseUpFunc;
	delegate->mouseMoveFunc = mouseMoveFunc;
	delegate->renderFunc = renderFunc;
	delegate->userInfo = userInfo;
	
	return delegate;
}

void wadEditorViewportDelegateRelease(WAD_EDITOR_VIEWPORT_DELEGATE* delegate)
{
	assert(delegate);
	free(delegate);
}
