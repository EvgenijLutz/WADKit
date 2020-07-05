//
//  wad_editor_viewport_delegate_functions.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_viewport_delegate_functions_h
#define wad_editor_viewport_delegate_functions_h

#include "wad_kit_link.h"
#include "we_mouse_key.h"

typedef struct RENDERER RENDERER;
typedef struct WAD_EDITOR_VIEWPORT WAD_EDITOR_VIEWPORT;

typedef void (* WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_DOWN_FUNC)(WE_MOUSE_KEY mouseKey, void* userInfo);
typedef void (* WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_UP_FUNC)(WE_MOUSE_KEY mouseKey, void* userInfo);
typedef void (* WAD_EDITOR_VIEWPORT_DELEGATE_MOUSE_MOVE_FUNC)(vector2f position, void* userInfo);

typedef void (* WAD_EDITOR_VIEWPORT_DELEGATE_DRAW_FUNC)(RENDERER* renderer, void* userInfo);

#endif /* wad_editor_viewport_delegate_functions_h */
