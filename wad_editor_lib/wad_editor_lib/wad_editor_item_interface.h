//
//  wad_editor_item_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 16.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_item_interface_h
#define wad_editor_lib__wad_editor_item_interface_h

#include "wad_editor_item_type.h"
#include "wad_editor_item_data.h"

typedef struct WAD_EDITOR_ITEM WAD_EDITOR_ITEM;

WAD_EDITOR_ITEM_TYPE wadEditorItemGetType(WAD_EDITOR_ITEM* item);
const char* wadEditorItemGetName(WAD_EDITOR_ITEM* item);

WAD_EDITOR_ITEM_DATA wadEditorItemGetData(WAD_EDITOR_ITEM* item);

WAD_EDITOR_ITEM* wadEditorItemGetParent(WAD_EDITOR_ITEM* item);

unsigned int wadEditorItemGetNumChildren(WAD_EDITOR_ITEM* item);
WAD_EDITOR_ITEM* wadEditorItemGetChild(WAD_EDITOR_ITEM* item, unsigned int childIndex);

#endif /* wad_editor_lib__wad_editor_item_interface_h */
