//
//  wad_editor_item_private_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 16.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_item_private_interface_h
#define wad_editor_lib__wad_editor_item_private_interface_h

#include "wad_editor_item_type.h"
#include "wad_editor_item_data.h"

typedef struct WAD_EDITOR_ITEM WAD_EDITOR_ITEM;

WAD_EDITOR_ITEM* wadEditorItemCreate(WAD_EDITOR_ITEM_TYPE type, WAD_EDITOR_ITEM_DATA data, const char* name);
void wadEditorItemRelease(WAD_EDITOR_ITEM* item);

void wadEditorItemRemoveAllChildren(WAD_EDITOR_ITEM* item);

WAD_EDITOR_ITEM* wadEditorItemAddChild(WAD_EDITOR_ITEM* item, WAD_EDITOR_ITEM_TYPE type, WAD_EDITOR_ITEM_DATA data, const char* name);

#endif /* wad_editor_lib__wad_editor_item_private_interface_h */
