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

WAD_EDITOR* wadEditorCreate(WK_SYSTEM* system, GRAPHICS_VIEW* outputView);
void wadEditorRelease(WAD_EDITOR* wadEditor);

void wadEditorUpdateState(WAD_EDITOR* wadEditor, double timeElapsed);

#endif /* wad_editor_lib__wad_editor_interface_h */
