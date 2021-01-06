//
//  wad_editor_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_interface_h
#define wad_editor_lib__wad_editor_interface_h

#include "wad_editor_lib.h"

WAD_EDITOR* wadEditorCreate(WK_SYSTEM* system, WE_LIST* wadContentsList, GRAPHICS_VIEW* outputView);
void wadEditorRelease(WAD_EDITOR* wadEditor);

void wadEditorLoadWad(WAD_EDITOR* editor, const char* filePath);

void wadEditorRender(WAD_EDITOR* wadEditor);

#endif /* wad_editor_lib__wad_editor_interface_h */
