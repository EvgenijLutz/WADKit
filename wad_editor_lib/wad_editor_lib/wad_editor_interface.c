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
	WAD_EDITOR* editor = (WAD_EDITOR*)userInfo;
	printf("Item selected: %s\n", stringGetData(&item->title));
	
	if (item->flags == 1)
	{
		editor->selectedMesh = item->data;
	}
}

static void _wadItemsList_ItemDeselected(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo)
{
	WAD_EDITOR* editor = (WAD_EDITOR*)userInfo;
	printf("Item deselected: %s\n", stringGetData(&item->title));
	
	if (item->flags == 1)
	{
		editor->selectedMesh = NULL;
	}
}


// MARK: - Graphics view subscriber

static void _graphicsView_sizeChanged(GRAPHICS_VIEW* graphicsView, float width, float height, void* userInfo)
{
	/*WAD_EDITOR* editor = (WAD_EDITOR*)userInfo;
	semaphoreEnter(editor->accessSemaphore);
	
	printf("Size changed: %f x %f\n", width, height);
	
	semaphoreLeave(editor->accessSemaphore);*/
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
	
	wadEditor->accessSemaphore = systemCreateSemaphore(wadEditor->system, 1);
	wadEditor->lastUpdateTime = systemGetCurrentTime(wadEditor->system);
	
	wadEditor->selectedMesh = NULL;
	
	wadEditor->testMeshUniforms = NULL;
	wadEditor->testMesh = NULL;
	
	return wadEditor;
}

void wadEditorRelease(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	
	semaphoreLeave(wadEditor->accessSemaphore);
	
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
	
	semaphoreEnter(editor->accessSemaphore);
	
	if (editor->testMesh)
	{
		graphicsMeshRelease(editor->testMesh);
		graphicsMeshUniformsRelease(editor->testMeshUniforms);
	}
	
	if (editor->wad)
	{
		wadRelease(editor->wad);
	}
	
	EXECUTE_RESULT executeResult;
	editor->wad = wadCreateFromContentsOfResourceFile(editor->system, "tut1", &executeResult);
	
	if (executeResultIsFailed(&executeResult))
	{
		editor->wad = NULL;
		semaphoreLeave(editor->accessSemaphore);
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
		MESH* mesh = wadGetMesh(editor->wad, i);
		sprintf(title, "Mesh #%u", i);
		listItemAddChild(item, 1, mesh, title);
	}
	
	item = listItemAddChild(rootItem, 0, NULL, "Movables");
	length = wadGetNumMovables(editor->wad);
	for (unsigned int i = 0; i < length; i++)
	{
		MOVABLE* movable = wadGetMovableByIndex(editor->wad, i);
		MOVABLE_ID movableId = movableGetId(movable);
		const char* movableName = movableIdGetTerribleName(movableId);
		WE_LIST_ITEM* movableItem = listItemAddChild(item, 0, NULL, movableName);
		
		/*WE_LIST_ITEM* rootMeshItem =*/ listItemAddChild(movableItem, 0, NULL, "Root mesh");
		WE_LIST_ITEM* jointsItem = listItemAddChild(movableItem, 0, NULL, "Jointed meshes");
		unsigned int numJoints = movableGetNumJoints(movable);
		for (unsigned int j = 0; j < numJoints; j++)
		{
			sprintf(title, "Jointed mesh #%u", j);
			listItemAddChild(jointsItem, 0, NULL, title);
		}
		
		WE_LIST_ITEM* animationsItem = listItemAddChild(movableItem, 0, NULL, "Animations");
		unsigned int numAnimations = movableGetNumAnimations(movable);
		for (unsigned int j = 0; j < numAnimations; j++)
		{
			//ANIMATION* animation = movableGetAnimation(movable, j);
			sprintf(title, "Animation #%u", j);
			listItemAddChild(animationsItem, 0, NULL, title);
		}
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
	
	if (wadGetNumTexturePages(editor->wad) > 0)
	{
		GRAPHICS_VERTEX vertices[] = {
			{ .position=vector3fCreate(-0.5, -0.5, 0.0), .uv=vector2fCreate(0.0, 1.0), .weights={ 1.0, 0.0} },
			{ .position=vector3fCreate( 0.5, -0.5, 0.0), .uv=vector2fCreate(1.0, 1.0), .weights={ 1.0, 0.0} },
			{ .position=vector3fCreate( 0.5,  0.5, 0.0), .uv=vector2fCreate(1.0, 0.0), .weights={ 1.0, 0.0} },
			
			{ .position=vector3fCreate( 0.5,  0.5, 0.0), .uv=vector2fCreate(1.0, 0.0), .weights={ 1.0, 0.0} },
			{ .position=vector3fCreate(-0.5,  0.5, 0.0), .uv=vector2fCreate(0.0, 0.0), .weights={ 1.0, 0.0} },
			{ .position=vector3fCreate(-0.5, -0.5, 0.0), .uv=vector2fCreate(0.0, 1.0), .weights={ 1.0, 0.0} }
		};
		TEXTURE_PAGE* page = wadGetTexturePage(editor->wad, 0);
		TEXTURE2D* texture = graphicsDeviceCreateTexture2dWithData(editor->device, 256, 256, 3, TEXTURE_USAGE_SHADER_READ, texturePageGetData(page));
		
		editor->testMesh = graphicsDeviceCreateMesh(editor->device, vertices, 6, texture);
		editor->testMeshUniforms = graphicsDeviceCreateMeshUniforms(editor->device);
	}
	
	semaphoreLeave(editor->accessSemaphore);
}


static void _wadEditor_updateState(WAD_EDITOR* wadEditor)
{
	const double currentTime = systemGetCurrentTime(wadEditor->system);
	//const double timeElapsed = currentTime - wadEditor->lastUpdateTime;
	
	if (wadEditor->testMeshUniforms)
	{
		const float fovyRadians = 65.0 * (M_PI / 180.0f);
		float aspect = graphicsViewGetWidth(wadEditor->outputView) / graphicsViewGetHeight(wadEditor->outputView);
		
		GRAPHICS_MESH_UNIFORMS_DATA data;
		data.modelView[0] = matrix4fMul(matrix4fTranslation(0.0f, 0.0f, -0.8f), matrix4f_identity);
		data.modelView[1] = matrix4fMul(matrix4fTranslation(0.0f, 0.0f, -0.8f), matrix4f_identity);
		data.projection = matrix4fPerspectiveRightHand(fovyRadians, aspect, 0.01f, 100.0f);
		data.ambient = vector3fCreate(1.0f, 1.0f, 1.0f);
		graphicsMeshUniformsSetData(wadEditor->testMeshUniforms, wadEditor->commandQueue, &data);
	}
	
	wadEditor->lastUpdateTime = currentTime;
}

static void _wadEditor_commandBufferCompleted(COMMAND_BUFFER* commandBuffer, void* userInfo)
{
	GRAPHICS_DRAWABLE* drawable = userInfo;
	
	commandBufferRelease(commandBuffer);
	graphicsDrawableReturn(drawable);
}

static void _wadEditor_render(WAD_EDITOR* wadEditor)
{
	GRAPHICS_VIEW* graphicsView = wadEditor->outputView;
	GRAPHICS_DRAWABLE* drawable = graphicsViewRequestCurrentDrawable(graphicsView);
	if (!drawable) {
		return;
	}
	
	COMMAND_BUFFER* commandBuffer = commandQueueCreateCommandBuffer(wadEditor->commandQueue);
	commandBufferAddCompletionHandler(commandBuffer, _wadEditor_commandBufferCompleted, drawable);
	
	RENDER_COMMAND_ENCODER* renderEncoder = commandBufferStartRenderCommandEncoder(commandBuffer, graphicsView);
	if (wadEditor->testMesh)
	{
		renderCommandEncoderScheduleDrawMesh(renderEncoder, wadEditor->testMesh, wadEditor->testMeshUniforms);
	}
	renderCommandEncoderFinishEncoding(renderEncoder);
	
	commandBufferScheduleDisplayDrawable(commandBuffer, drawable);
	commandBufferCommit(commandBuffer);
}

void wadEditorRender(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	
	semaphoreEnter(wadEditor->accessSemaphore);
	
	_wadEditor_updateState(wadEditor);
	_wadEditor_render(wadEditor);
	
	semaphoreLeave(wadEditor->accessSemaphore);
}
