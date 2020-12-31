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

// MARK: - Declarations

// MARK: - Structures

#include "system.h"
#include "file_reader.h"

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
#include "animation_internal_interface.h"
#include "movable_internal_interface.h"
#include "static_object_private_interface.h"
#include "wad_private_interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#endif /* wk_private_interface_h */
