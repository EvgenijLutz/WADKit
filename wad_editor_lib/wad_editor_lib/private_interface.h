//
//  private_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef private_interface_h
#define private_interface_h

#include "wad_editor_lib.h"
#include "../../wad_kit/wad_kit/private_common.h"

// MARK: - Declarations

// MARK: - Structures

#include "texture2d.h"
#include "graphics_buffer.h"
#include "command_queue.h"
#include "command_buffer.h"
#include "blit_command_encoder.h"
#include "render_settings.h"
#include "depth_stencil_state.h"
#include "render_state.h"
#include "render_command_encoder.h"
#include "graphics_view.h"
#include "graphics_drawable.h"
#include "graphics_device.h"

#include "resource_storage.h"
#include "renderer.h"
#include "wad_editor_viewport_delegate.h"
#include "wad_editor_viewport.h"
#include "wad_editor_item.h"
#include "wad_editor.h"

// MARK: - Private interface

#include "command_queue_private_interface.h"
#include "graphics_device_private_interface.h"
#include "wad_editor_item_private_interface.h"

#endif /* private_interface_h */
