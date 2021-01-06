//
//  wad_editor_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

WAD_EDITOR* wadEditorCreate(WK_SYSTEM* system, GRAPHICS_VIEW* outputView)
{
	WAD_EDITOR* wadEditor = malloc(sizeof(WAD_EDITOR));
	wadEditor->system = system;
	wadEditor->openedWadPath = NULL;
	wadEditor->wad = NULL;
	wadEditor->hasChanges = 0;
	wadEditor->outputView = outputView;
	wadEditor->device = graphicsViewGetDevice(outputView);
	
	return wadEditor;
}

void wadEditorRelease(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	
	free(wadEditor);
}


void wadEditorUpdateState(WAD_EDITOR* wadEditor, double timeElapsed)
{
	//
}
