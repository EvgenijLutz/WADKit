//
//  wad_editor_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - WAD items list subscriber

static void _wadItemsList_itemSelected(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo)
{
	//
}

static void _wadItemsList_ItemDeselected(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo)
{
	//
}


// MARK: - Graphics view subscriber

static void _graphicsView_sizeChanged(GRAPHICS_VIEW* graphicsView, float width, float height, void* userInfo)
{
	//
}

static void _graphicsView_update(GRAPHICS_VIEW* graphicsView, void* userInfo)
{
	WAD_EDITOR* editor = (WAD_EDITOR*)userInfo;
	wadEditorRender(editor);
}


// MARK: - WAD editor interface

WAD_EDITOR* wadEditorCreate(WK_SYSTEM* system, WE_LIST* wadContentsList, GRAPHICS_VIEW* outputView)
{
	WAD_EDITOR* wadEditor = malloc(sizeof(WAD_EDITOR));
	wadEditor->system = system;
	wadEditor->openedWadPath = NULL;
	wadEditor->wad = NULL;
	wadEditor->hasChanges = 0;
	wadEditor->wadContentsList = wadContentsList;
	wadEditor->outputView = outputView;
	
	WE_LIST_SUBSCRIBER listSubscriber;
	listSubscriber.itemSelectedFunc = _wadItemsList_itemSelected;
	listSubscriber.itemDeselectedFunc = _wadItemsList_ItemDeselected;
	listSubscriber.userInfo = wadEditor;
	listSubscribe(wadEditor->wadContentsList, &listSubscriber);
	
	GRAPHICS_VIEW_SUBSCRIBER graphicsViewSubscriber;
	graphicsViewSubscriber.sizeChangedFunc = _graphicsView_sizeChanged;
	graphicsViewSubscriber.updateFunc = _graphicsView_update;
	graphicsViewSubscriber.userInfo = wadEditor;
	graphicsViewSubscribe(wadEditor->outputView, &graphicsViewSubscriber);
	
	wadEditor->device = graphicsViewGetDevice(outputView);
	wadEditor->commandQueue = graphicsDeviceCreateCommandQueue(wadEditor->device);
	wadEditor->commandBuffer = commandQueueCreateCommandBuffer(wadEditor->commandQueue);
	
	wadEditor->accessSemaphore = systemCreateSemaphore(wadEditor->system, 1);
	wadEditor->lastUpdateTime = systemGetCurrentTime(wadEditor->system);
	
	return wadEditor;
}

void wadEditorRelease(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	
	semaphoreLeave(wadEditor->accessSemaphore);
	
	commandBufferRelease(wadEditor->commandBuffer);
	commandQueueRelease(wadEditor->commandQueue);
	
	if (wadEditor->wad)
	{
		wadRelease(wadEditor->wad);
	}
	
	debug_memset(wadEditor, 0, sizeof(WAD_EDITOR));
	free(wadEditor);
}


void wadEditorLoadWad(WAD_EDITOR* editor, const char* filePath)
{
	assert(editor);
	assert(filePath);
	
	if (editor->wad)
	{
		wadRelease(editor->wad);
	}
	
	EXECUTE_RESULT executeResult;
	editor->wad = wadCreateFromContentsOfResourceFile(editor->system, "tut1", &executeResult);
	
	if (executeResultIsFailed(&executeResult))
	{
		editor->wad = NULL;
		return;
	}
	
	listClear(editor->wadContentsList);
	WE_LIST_ITEM* rootItem = listGetRootItem(editor->wadContentsList);
	
	WE_LIST_ITEM* item = listItemAddChild(rootItem, 0, NULL, "Texture pages");
	unsigned int length = wadGetNumTexturePages(editor->wad);
	char title[512];
	for (unsigned int i = 0; i < length; i++)
	{
		sprintf(title, "Texture page #%u", i);
		listItemAddChild(item, 0, NULL, title);
	}
	
	item = listItemAddChild(rootItem, 0, NULL, "Meshes");
	length = wadGetNumMeshes(editor->wad);
	for (unsigned int i = 0; i < length; i++)
	{
		sprintf(title, "Mesh #%u", i);
		listItemAddChild(item, 0, NULL, title);
	}
	
	item = listItemAddChild(rootItem, 0, NULL, "Movables");
	length = wadGetNumMovables(editor->wad);
	for (unsigned int i = 0; i < length; i++)
	{
		MOVABLE* movable = wadGetMovableByIndex(editor->wad, i);
		MOVABLE_ID movableId = movableGetId(movable);
		const char* movableName = movableIdGetName(movableId);
		listItemAddChild(item, 0, NULL, movableName);
	}
	
	item = listItemAddChild(rootItem, 0, NULL, "Statics");
	length = wadGetNumStatics(editor->wad);
	for (unsigned int i = 0; i < length; i++)
	{
		STATIC* staticObject = wadGetStaticByIndex(editor->wad, i);
		STATIC_ID staticId = staticGetId(staticObject);
		//const char* staticName = staticIdGetFabulousName(staticId);
		const char* staticName = staticIdGetTerribleName(staticId);
		listItemAddChild(item, 0, NULL, staticName);
	}
}


static void _wadEditor_updateState(WAD_EDITOR* wadEditor)
{
	const double currentTime = systemGetCurrentTime(wadEditor->system);
	const double timeElapsed = currentTime - wadEditor->lastUpdateTime;
	
	static int numUpdates = 0;
	if (numUpdates < 60)
	{
		numUpdates++;
		
		char message[500];
		sprintf(message, "Time elapsed: %f\n", timeElapsed);
		printf("%s", message);
	}
	
	wadEditor->lastUpdateTime = currentTime;
}

static void _wadEditor_render(WAD_EDITOR* wadEditor)
{
	//
}

void wadEditorRender(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	
	semaphoreEnter(wadEditor->accessSemaphore);
	
	_wadEditor_updateState(wadEditor);
	_wadEditor_render(wadEditor);
	
	semaphoreLeave(wadEditor->accessSemaphore);
}
