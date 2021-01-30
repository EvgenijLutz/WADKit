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
#include "../../../Foundation/Foundation/private_common.h"

// MARK: - Declarations

typedef struct TEXTURE_DATA TEXTURE_DATA;
typedef struct MESH_DATA MESH_DATA;
typedef struct MOVABLE_DATA MOVABLE_DATA;
typedef struct ANIMATION_DATA ANIMATION_DATA;
typedef struct STATIC_DATA STATIC_DATA;
typedef struct ANIMATION_EDITOR ANIMATION_EDITOR;

// MARK: - Structures

#include "list_item.h"
#include "list.h"
#include "animation_editor.h"
#include "wad_editor.h"

// MARK: - Private interface

#include "list_item_private_interface.h"
#include "animation_editor_private_interface.h"

#endif /* private_interface_h */
