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

WAD_EDITOR* wadEditorCreate(RESOURCE_STORAGE* resourceStorage);
void wadEditorRelease(WAD_EDITOR* wadEditor);

//void wadEditorConnect(WAD_EDITOR* wadEditor, WAD_EDITOR_DELEGATE* delegate);

void wadEditorNewWad(WAD_EDITOR* wadEditor);
void wadEditorLoadWad(WAD_EDITOR* wadEditor, const char* filePath);
void wadEditorSaveWad(WAD_EDITOR* wadEditor);
void wadEditorSaveAsWad(WAD_EDITOR* wadEditor, const char* filePath);

void wadEditorUpdateState(WAD_EDITOR* wadEditor, double timeElapsed);

// MARK: - UI abstraction interface

WAD_EDITOR_ITEM* wadEditorGetRootItem(WAD_EDITOR* wadEditor);
unsigned int wadEditorGetNumChildrenInItem(WAD_EDITOR* wadEditor, WAD_EDITOR_ITEM_TYPE type, unsigned int itemIndex);
const char* wadEditorGetItemName(WAD_EDITOR* wadEditor, WAD_EDITOR_ITEM_TYPE type, unsigned int itemIndex);

void wadEditorSelectItemInSection(WAD_EDITOR* wadEditor, unsigned int sectionIndex, unsigned int itemIndex);
WAD_EDITOR_ITEM* wadEditorGetItemInSection(WAD_EDITOR* wadEditor, unsigned int sectionIndex, unsigned int itemIndex);

void wadEditorSelectItem(WAD_EDITOR* editor, WAD_EDITOR_ITEM* item);

// Menu items
//int wadEditorIsSaveAsMenuAvailable(WAD_EDITOR* wadEditor);

// MARK: - Properties

WK_WAD* wadEditorGetWad(WAD_EDITOR* wadEditor);
WAD_EDITOR_VIEWPORT* wadEditorGetMainViewport(WAD_EDITOR* wadEditor);

#endif /* wad_editor_lib__wad_editor_interface_h */
