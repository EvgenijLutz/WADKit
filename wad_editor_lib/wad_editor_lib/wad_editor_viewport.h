//
//  wad_editor_viewport.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_viewport_h
#define wad_editor_lib__wad_editor_viewport_h

#include "wad_kit_link.h"
#include "we_mouse_key.h"
#include "we_modifier_key.h"
#include "wad_editor_viewport_camera_mode.h"

typedef struct WAD_EDITOR WAD_EDITOR;
typedef struct WAD_EDITOR_VIEWPORT_DELEGATE WAD_EDITOR_VIEWPORT_DELEGATE;
typedef struct RENDERER RENDERER;

typedef struct WAD_EDITOR_VIEWPORT
{
	WAD_EDITOR* editor;
	
	WAD_EDITOR_VIEWPORT_DELEGATE* delegate;
	RENDERER* renderer;
	
	vector2f size;
	
	WAD_EDITOR_VIEWPORT_CAMERA_MODE cameraMode;
	vector3f cameraPosition;
	vector3f cameraRotation;
	float cameraDistance;
	float fovyRadians;
	float frustumNear;
	float frustumFar;
	matrix4f viewMatrix;
	matrix4f projectionMatrix;
	
	// Mouse stashed data
	//vector2f mouseDelta;
	vector2f previousMousePosition;
	WE_MOUSE_KEY mouseKeys[WE_MOUSE_KEY_NUM_KEYS];
	
	WE_MOUSE_KEY modifierKeys[WE_MODIFIER_KEY_NUM_KEYS];
}
WAD_EDITOR_VIEWPORT;

#endif /* wad_editor_lib__wad_editor_viewport_h */
