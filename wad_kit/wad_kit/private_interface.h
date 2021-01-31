//
//  private_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wk_private_interface_h
#define wk_private_interface_h

#include "wad_kit.h"
#include "../../../Foundation/Foundation/private_common.h"

// MARK: - Macros

#define WK_COORDINATE_MULTIPLIER	1024.0f
#define WK_NORMAL_MULTIPLIER		16300.0
#define WK_SHADE_MULTIPLIER			256.0
#define WK_COLOR_TO_F(color)		(1.0f / 255.0f * ((float)(color * 8)))
#define WK_COLOR_R(color)			COLOR_TO_F(((color & 0x7c00) >> 10))
#define WK_COLOR_G(color)			COLOR_TO_F(((color & 0x03e0) >> 5))
#define WK_COLOR_B(color)			COLOR_TO_F(((color & 0x001f)))

// MARK: - Declarations

// MARK: - Structures

#include "texture_page.h"
#include "texture_sample.h"
#include "vertex.h"
#include "polygon.h"
#include "mesh.h"
#include "joint.h"
#include "keyframe.h"
#include "command.h"
#include "state_change.h"
#include "animation.h"
#include "movable.h"
#include "static_object.h"
#include "raw_wad.h"
#include "wad.h"

// MARK: - Private interface

#include "texture_page_private_interface.h"
#include "texture_sample_private_interface.h"
#include "joint_private_interface.h"
#include "command_private_interface.h"
#include "state_change_private_interface.h"
#include "keyframe_internal_interface.h"
#include "animation_private_interface.h"
#include "movable_internal_interface.h"
#include "static_object_private_interface.h"
#include "wad_private_interface.h"

#endif /* wk_private_interface_h */
