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

#include "../../../Foundation/Foundation/foundation.h"

#ifdef __cplusplus
extern "C" {
#endif

// MARK: - Preprocessor macros

#define WK_TEXTURE_PAGE_WIDTH (256)
#define WK_TEXTURE_PAGE_HEIGHT (256)
#define WK_TEXTURE_PAGE_NUM_COMPONENTS (3)
#define WK_SIZE_OF_TEXTURE_PAGE ((sizeof(unsigned char)) * (WK_TEXTURE_PAGE_WIDTH * WK_TEXTURE_PAGE_HEIGHT * WK_TEXTURE_PAGE_NUM_COMPONENTS))

// MARK: - Declarations

typedef struct WK_TEXTURE_PAGE WK_TEXTURE_PAGE;
typedef struct WK_TEXTURE_SAMPLE WK_TEXTURE_SAMPLE;
typedef struct VERTEX VERTEX;
typedef struct POLYGON POLYGON;
typedef struct MESH MESH;
typedef struct JOINT JOINT;
typedef struct KEYFRAME KEYFRAME;
typedef struct ANIMATION ANIMATION;
typedef struct MOVABLE MOVABLE;
typedef struct STATIC STATIC;
typedef struct WK_WAD WK_WAD;

// MARK: - Enumerations

#include "joint_location_type.h"
#include "command_type.h"
#include "movable_id.h"
#include "static_object_id.h"

// MARK: - Callbacks


// MARK: - Structures


// MARK: - Public interface

#include "texture_page_interface.h"
#include "texture_sample_interface.h"
#include "vertex_interface.h"
#include "polygon_interface.h"
#include "mesh_interface.h"
#include "joint_interface.h"
#include "command_interface.h"
#include "state_change_interface.h"
#include "keyframe_interface.h"
#include "animation_interface.h"
#include "movable_interface.h"
#include "static_object_interface.h"

#include "wad_interface.h"

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* wad_kit_h */
