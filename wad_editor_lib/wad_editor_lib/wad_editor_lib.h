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
#include "../../graphics/graphics/graphics.h"

// MARK: - Enumerations

// MARK: - Type declarations

typedef struct WE_LIST_ITEM WE_LIST_ITEM;
typedef struct WE_LIST WE_LIST;
typedef struct WAD_EDITOR WAD_EDITOR;

// MARK: - Function declarations

#include "list_functions.h"

// MARK: - Public structures

// MARK: - Public interface

#include "list_item_interface.h"
#include "list_interface.h"
#include "wad_editor_interface.h"

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* wad_editor_lib__wad_editor_lib_h */
