//
//  wad_editor_viewport_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_viewport_interface_h
#define wad_editor_lib__wad_editor_viewport_interface_h

#include "wad_kit_link.h"
#include "we_mouse_key.h"

typedef struct WAD_EDITOR_VIEWPORT WAD_EDITOR_VIEWPORT;
typedef struct WAD_EDITOR_VIEWPORT_DELEGATE WAD_EDITOR_VIEWPORT_DELEGATE;
typedef struct RENDERER RENDERER;

WAD_EDITOR_VIEWPORT* wadEditorViewportCreate(void);
void wadEditorViewportRelease(WAD_EDITOR_VIEWPORT* viewport);

void wadEditorViewportConnectRenderer(WAD_EDITOR_VIEWPORT* viewport, RENDERER* renderer);
RENDERER* wadEditorViewportGetConnectedRenderer(WAD_EDITOR_VIEWPORT* viewport);
void wadEditorViewportDisconnectRenderer(WAD_EDITOR_VIEWPORT* viewport);

void wadEditorViewportDraw(WAD_EDITOR_VIEWPORT* viewport);

// MARK: - User interaction

void wadEditorViewportMouseDown(WAD_EDITOR_VIEWPORT* viewport, WE_MOUSE_KEY keyCode);
void wadEditorViewportMouseUp(WAD_EDITOR_VIEWPORT* viewport, WE_MOUSE_KEY keyCode);
void wadEditorViewportMouseMove(WAD_EDITOR_VIEWPORT* viewport, vector2f pointerPosition);

// MARK: - Properties

void wadEditorViewportSetDelegate(WAD_EDITOR_VIEWPORT* viewport, WAD_EDITOR_VIEWPORT_DELEGATE* delegate);

void wadEditorViewportSetSize(WAD_EDITOR_VIEWPORT* viewport, vector2f size);
vector2f wadEditorViewportGetSize(WAD_EDITOR_VIEWPORT* viewport);

#endif /* wad_editor_lib__wad_editor_viewport_interface_h */
