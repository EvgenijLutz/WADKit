//
//  wad_editor_lib.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_lib_h
#define wad_editor_lib__wad_editor_lib_h

// Public WAD editor interface

#ifdef __cplusplus
extern "C" {
#endif

#include "../../wad_kit/wad_kit/wad_kit.h"

// MARK: - Enumerations

#include "pixel_format.h"
#include "graphics_buffer_option.h"
#include "texture_usage.h"
#include "we_mouse_key.h"
#include "we_modifier_key.h"
#include "wad_editor_viewport_camera_mode.h"
#include "wad_editor_item_type.h"

// MARK: - Type declarations

typedef struct TEXTURE2D TEXTURE2D;
typedef struct GRAPHICS_BUFFER GRAPHICS_BUFFER;
/// Device pipeline, reusable resource
typedef struct COMMAND_QUEUE COMMAND_QUEUE;
typedef struct COMMAND_BUFFER COMMAND_BUFFER;
typedef struct BLIT_COMMAND_ENCODER BLIT_COMMAND_ENCODER;
typedef struct RENDER_SETTINGS RENDER_SETTINGS;
typedef struct DEPTH_STENCIL_STATE DEPTH_STENCIL_STATE;
typedef struct RENDER_STATE RENDER_STATE;
typedef struct RENDER_COMMAND_ENCODER RENDER_COMMAND_ENCODER;
typedef struct GRAPHICS_VIEW GRAPHICS_VIEW;
typedef struct GRAPHICS_DRAWABLE GRAPHICS_DRAWABLE;
typedef struct GRAPHICS_DEVICE GRAPHICS_DEVICE;

typedef struct RENDERER RENDERER;
typedef struct RESOURCE_STORAGE RESOURCE_STORAGE;
typedef struct WAD_EDITOR WAD_EDITOR;
typedef struct WAD_EDITOR_ITEM WAD_EDITOR_ITEM;
typedef struct WAD_EDITOR_VIEWPORT WAD_EDITOR_VIEWPORT;
typedef struct WAD_EDITOR_VIEWPORT_DELEGATE WAD_EDITOR_VIEWPORT_DELEGATE;

// MARK: - Function declarations

#include "graphics_view_functions.h"
#include "graphics_device_functions.h"

#include "resource_storage_functions.h"
#include "renderer_functions.h"

// MARK: - Public structures

#include "wad_editor_item_data.h"

// MARK: - Public interface

#include "texture2d_interface.h"
#include "graphics_buffer_interface.h"
#include "command_queue_interface.h"
#include "command_buffer_interface.h"
#include "blit_command_encoder_interface.h"
#include "render_command_encoder_interface.h"
#include "graphics_view_interface.h"
#include "graphics_drawable_interface.h"
#include "graphics_device_interface.h"

#include "resource_storage_interface.h"
#include "renderer_interface.h"
#include "wad_editor_viewport_delegate_interface.h"
#include "wad_editor_viewport_interface.h"
#include "wad_editor_item_interface.h"
#include "wad_editor_interface.h"

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* wad_editor_lib__wad_editor_lib_h */
