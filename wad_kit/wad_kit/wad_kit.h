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

// MARK: - Preprocessor macros

#define WK_TEXTURE_PAGE_WIDTH (256)
#define WK_TEXTURE_PAGE_HEIGHT (256)
#define WK_TEXTURE_PAGE_NUM_COMPONENTS (3)
#define WK_SIZE_OF_TEXTURE_PAGE ((sizeof(unsigned char)) * (WK_TEXTURE_PAGE_WIDTH * WK_TEXTURE_PAGE_HEIGHT * WK_TEXTURE_PAGE_NUM_COMPONENTS))

// MARK: - Declarations

typedef struct EXECUTE_RESULT EXECUTE_RESULT;

typedef struct WK_BUFFER WK_BUFFER;
typedef struct DATA_BLOCK DATA_BLOCK;
typedef struct DATA_ALLOCATOR DATA_ALLOCATOR;
typedef struct WK_STRING WK_STRING;
typedef struct WK_ARRAY WK_ARRAY;

typedef struct WK_SYSTEM WK_SYSTEM;
typedef struct WK_SEMAPHORE WK_SEMAPHORE;
typedef struct WK_FILE_READER WK_FILE_READER;

typedef struct TEXTURE_PAGE TEXTURE_PAGE;
typedef struct TEXTURE_SAMPLE TEXTURE_SAMPLE;
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

#include "file_mode.h"

#include "joint_location_type.h"
#include "command_type.h"
#include "movable_id.h"
#include "static_object_id.h"

// MARK: - Callbacks

#include "system_functions.h"

// MARK: - Structures

#include "execute_result.h"
#include "buffer.h"
#include "data_block_item.h"
#include "buffer_reader.h"
#include "string.h"
#include "array.h"

#include "vector2f.h"
#include "vector3f.h"
#include "wk_math.h"

// MARK: - Public interface

#include "buffer_interface.h"
#include "buffer_reader_interface.h"
#include "data_allocator_interface.h"
#include "string_interface.h"
#include "array_interface.h"

#include "system_interface.h"
#include "semaphore_interface.h"
#include "file_reader_interface.h"

#include "vector2f_interface.h"
#include "vector3f_interface.h"
#include "wk_math_functions.h"

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
