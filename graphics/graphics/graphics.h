//
//  graphics.h
//  graphics
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef graphics__graphics_h
#define graphics__graphics_h

// Public WAD editor interface

#ifdef __cplusplus
extern "C" {
#endif

#include "../../wad_kit/wad_kit/wad_kit.h"

// MARK: - Enumerations

#include "pixel_format.h"
#include "texture_usage.h"

// MARK: - Type declarations

typedef struct TEXTURE2D TEXTURE2D;
typedef struct GRAPHICS_MESH_UNIFORMS GRAPHICS_MESH_UNIFORMS;
typedef struct GRAPHICS_MESH GRAPHICS_MESH;
/// Device pipeline, reusable resource
typedef struct COMMAND_QUEUE COMMAND_QUEUE;
typedef struct COMMAND_BUFFER COMMAND_BUFFER;
typedef struct RENDER_COMMAND_ENCODER RENDER_COMMAND_ENCODER;
typedef struct GRAPHICS_VIEW GRAPHICS_VIEW;
typedef struct GRAPHICS_DRAWABLE GRAPHICS_DRAWABLE;
typedef struct GRAPHICS_DEVICE GRAPHICS_DEVICE;

// MARK: - Public structures

#include "graphics_vertex.h"
#include "graphics_mesh_uniforms_data.h"

// MARK: - Function declarations

#include "graphics_view_functions.h"
#include "graphics_device_functions.h"

// MARK: - Public interface

#include "texture2d_interface.h"
#include "graphics_mesh_interface.h"
#include "command_queue_interface.h"
#include "command_buffer_interface.h"
#include "render_command_encoder_interface.h"
#include "graphics_view_interface.h"
#include "graphics_drawable_interface.h"
#include "graphics_device_interface.h"

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* graphics__graphics_h */
