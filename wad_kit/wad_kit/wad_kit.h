//
//  wad_kit.h
//  wad_kit
//
//  Created by Евгений Лютц on 15.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_h
#define wad_kit_h

/* * * Public WADKit interface * * */

#ifdef __cplusplus
extern "C" {
#endif

#include "execute_result.h"

#include "wk_math.h"
#include "wk_math_functions.h"

#include "wk_vertex.h"
#include "wk_polygon.h"
#include "texture_page.h"

#include "wad_interface.h"
#include "texture_page_interface.h"
#include "mesh_interface.h"

#include "joint_location_type.h"
#include "joint_interface.h"
#include "skeleton_interface.h"

#include "movable_id.h"
#include "movable_interface.h"

#include "static_object_id.h"
#include "static_object_interface.h"

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* wad_kit_h */
