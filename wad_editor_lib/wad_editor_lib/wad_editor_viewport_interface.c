//
//  wad_editor_viewport_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

static void _viewportRecalculateViewMatrix(WAD_EDITOR_VIEWPORT* viewport)
{
	vector3f pos = viewport->cameraPosition;
	matrix4f translation = matrix4fTranslation(-vec_x(pos), -vec_y(pos), -vec_z(pos));
	
	vector3f rot = viewport->cameraRotation;
	matrix4f rotation = matrix4f_identity;
	rotation = matrix4fMul(matrix4fRotation(vec_y(rot), vector3fCreate(0.0f, 1.0f, 0.0f)), rotation);
	rotation = matrix4fMul(matrix4fRotation(vec_x(rot), vector3fCreate(1.0f, 0.0f, 0.0f)), rotation);
	
	viewport->viewMatrix = matrix4fMul(translation, rotation);
	//viewport->viewMatrix = matrix4fTranslation(0.0f, 0.0f, -0.9f);
}

static void _viewportRecalculateProjectionMatrix(WAD_EDITOR_VIEWPORT* viewport)
{
	vector2f size = viewport->size;
	float aspectRatio = vec_x(size) / vec_y(size);
	viewport->projectionMatrix = matrix4fPerspectiveRightHand(viewport->fovyRadians, aspectRatio, viewport->frustumNear, viewport->frustumFar);
}

static void _viewportRotateCameraOrbit(WAD_EDITOR_VIEWPORT* viewport, vector2f xy)
{
	viewport->cameraRotation.x +=  xy.y / 50.0f;
	viewport->cameraRotation.y += -xy.x / 50.0f;
	
	_viewportRecalculateViewMatrix(viewport);
}

static void _viewportRotateCameraEye(WAD_EDITOR_VIEWPORT* viewport, vector2f xy)
{
	_viewportRecalculateViewMatrix(viewport);
}

// MARK: - Editor viewport interface implementation

WAD_EDITOR_VIEWPORT* wadEditorViewportCreate(WAD_EDITOR* editor)
{
	WAD_EDITOR_VIEWPORT* viewport = malloc(sizeof(WAD_EDITOR_VIEWPORT));
	viewport->editor = editor;
	viewport->delegate = NULL;
	viewport->renderer = NULL;
	viewport->size = vector2fCreate(10.0f, 10.0f);
	viewport->cameraMode = WAD_EDITOR_VIEWPORT_CAMERA_MODE_ORBIT;
	viewport->cameraPosition = vector3fCreate(0.0f, 0.0f, 1.0f);
	viewport->cameraRotation = vector3fCreate(0.0f, 0.0f, 0.0f);
	viewport->cameraDistance = 1.0f;
	viewport->fovyRadians = M_PI / 180.0f * 45.0f;
	viewport->frustumNear = 0.05f;
	viewport->frustumFar = 1000.0f;
	_viewportRecalculateViewMatrix(viewport);
	_viewportRecalculateProjectionMatrix(viewport);
	
	viewport->previousMousePosition = vector2fCreate(0.0f, 0.0f);
	for (unsigned int keyIndex = 0; keyIndex < WE_MOUSE_KEY_NUM_KEYS; keyIndex++) {
		viewport->mouseKeys[keyIndex] = 0;
	}
	
	for (unsigned int modifierKey = 0; modifierKey < WE_MODIFIER_KEY_NUM_KEYS; modifierKey++) {
		viewport->modifierKeys[modifierKey] = 0;
	}
	
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

void wadEditorViewportZoom(WAD_EDITOR_VIEWPORT* viewport, float value)
{
	assert(viewport);
}

void wadEditorViewportMouseDown(WAD_EDITOR_VIEWPORT* viewport, WE_MOUSE_KEY keyCode)
{
	assert(viewport);
	viewport->mouseKeys[keyCode] = 1;
}

void wadEditorViewportMouseUp(WAD_EDITOR_VIEWPORT* viewport, WE_MOUSE_KEY keyCode)
{
	assert(viewport);
	viewport->mouseKeys[keyCode] = 0;
}

void wadEditorViewportMouseMove(WAD_EDITOR_VIEWPORT* viewport, vector2f pointerPosition)
{
	assert(viewport);
	
	if (viewport->mouseKeys[WE_MOUSE_KEY_LEFT])
	{
		// Drag camera
		if (viewport->modifierKeys[WE_MODIFIER_KEY_SHIFT])
		{
			//
		}
		// Rotate camera
		else
		{
			if (viewport->cameraMode == WAD_EDITOR_VIEWPORT_CAMERA_MODE_ORBIT)
			{
				_viewportRotateCameraOrbit(viewport, vec_dif(viewport->previousMousePosition, pointerPosition));
			}
			else if (viewport->cameraMode == WAD_EDITOR_VIEWPORT_CAMERA_MODE_EYE)
			{
				_viewportRotateCameraEye(viewport, vec_dif(viewport->previousMousePosition, pointerPosition));
			}
		}
	}
	
	viewport->previousMousePosition = pointerPosition;
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
	_viewportRecalculateProjectionMatrix(viewport);
}

vector2f wadEditorViewportGetSize(WAD_EDITOR_VIEWPORT* viewport)
{
	assert(viewport);
	return viewport->size;
}

matrix4f wadEditorViewportGetViewMatrix(WAD_EDITOR_VIEWPORT* viewport)
{
	assert(viewport);
	return viewport->viewMatrix;
}

matrix4f wadEditorViewportGetProjectionMatrix(WAD_EDITOR_VIEWPORT* viewport)
{
	assert(viewport);
	return viewport->projectionMatrix;
}
