//
//  wad_editor_viewport_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "wad_editor_viewport_interface.h"
#include "wad_editor_viewport_delegate.h"
#include "wad_editor_viewport.h"
#include "common.h"

WAD_EDITOR_VIEWPORT* wadEditorViewportCreate(void)
{
	WAD_EDITOR_VIEWPORT* viewport = malloc(sizeof(WAD_EDITOR_VIEWPORT));
	viewport->delegate = NULL;
	viewport->renderer = NULL;
	viewport->size = vector2fCreate(10.0f, 10.0f);
	
	return viewport;
}

void wadEditorViewportRelease(WAD_EDITOR_VIEWPORT* viewport)
{
	assert(viewport);
	free(viewport);
}


void wadEditorViewportConnectRenderer(WAD_EDITOR_VIEWPORT* viewport, RENDERER* renderer)
{
	assert(viewport);
	assert(viewport->renderer == NULL);
	viewport->renderer = renderer;
}

RENDERER* wadEditorViewportGetConnectedRenderer(WAD_EDITOR_VIEWPORT* viewport)
{
	assert(viewport);
	return viewport->renderer;
}

void wadEditorViewportDisconnectRenderer(WAD_EDITOR_VIEWPORT* viewport)
{
	assert(viewport);
	assert(viewport->renderer);
	viewport->renderer = NULL;
}


void wadEditorViewportDraw(WAD_EDITOR_VIEWPORT* viewport)
{
	assert(viewport);
	
	if (viewport->delegate && viewport->renderer)
	{
		viewport->delegate->renderFunc(viewport->renderer, viewport->delegate->userInfo);
	}
}


// MARK: - User interaction

void wadEditorViewportMouseDown(WAD_EDITOR_VIEWPORT* viewport, WE_MOUSE_KEY keyCode)
{
	assert(viewport);
}

void wadEditorViewportMouseUp(WAD_EDITOR_VIEWPORT* viewport, WE_MOUSE_KEY keyCode)
{
	assert(viewport);
}

void wadEditorViewportMouseMove(WAD_EDITOR_VIEWPORT* viewport, vector2f pointerPosition)
{
	assert(viewport);
}


// MARK: - Properties

void wadEditorViewportSetDelegate(WAD_EDITOR_VIEWPORT* viewport, WAD_EDITOR_VIEWPORT_DELEGATE* delegate)
{
	assert(viewport);
	viewport->delegate = delegate;
}

void wadEditorViewportSetSize(WAD_EDITOR_VIEWPORT* viewport, vector2f size)
{
	assert(viewport);
	viewport->size = size;
}

vector2f wadEditorViewportGetSize(WAD_EDITOR_VIEWPORT* viewport)
{
	assert(viewport);
	return viewport->size;
}
