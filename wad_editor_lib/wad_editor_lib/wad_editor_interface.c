//
//  wad_editor_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

#define LI_FLAG_MESH	(1)

// MARK: - WAD items list subscriber

static void _wadItemsList_itemSelected(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo)
{
	WAD_EDITOR* editor = (WAD_EDITOR*)userInfo;
	printf("Item selected: %s\n", stringGetData(&item->title));
	
	if (item->flags == LI_FLAG_MESH)
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


// MARK: - Private interface

typedef struct TEXTURE_LINK
{
	TEXTURE_PAGE* page;
	TEXTURE2D* texture;
}
TEXTURE_LINK;

static TEXTURE2D* _wadEditor_getTextureForTexturePage(WAD_EDITOR* editor, TEXTURE_PAGE* page)
{
	for (unsigned int i = 0; i < editor->textures.length; i++)
	{
		TEXTURE_LINK* link = arrayGetItem(&editor->textures, i);
		if (link->page == page)
		{
			return link->texture;
		}
	}
	
	// This should never happen
	assert(0);
	return NULL;
}


typedef struct SUBMESH_DATA
{
	TEXTURE_PAGE* page;
	TEXTURE2D* texture;
	GRAPHICS_MESH* mesh;
}
SUBMESH_DATA;

typedef struct MESH_DATA
{
	MESH* mesh;
	WK_ARRAY submeshes;
}
MESH_DATA;

/*static MESH_DATA* _wadEditor_getMeshDataForMesh(WAD_EDITOR* editor, MESH* mesh)
{
	for (unsigned int i = 0; i < editor->textures.length; i++)
	{
		MESH_DATA* meshData = arrayGetItem(&editor->meshes, i);
		if (meshData->mesh == mesh)
		{
			return meshData;
		}
	}
	
	// This should never happen
	assert(0);
	return NULL;
}*/


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
	
	arrayInitialize(&wadEditor->textures, sizeof(TEXTURE_LINK));
	
	wadEditor->submeshAllocator = dataAllocatorCreate(sizeof(SUBMESH_DATA), 256);
	arrayInitializeWithCapacityIncrement(&wadEditor->meshes, sizeof(MESH_DATA), 256);
	
	wadEditor->selectedMesh = NULL;
	
	wadEditor->testMeshUniforms = NULL;
	
	return wadEditor;
}

void wadEditorRelease(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	
	assert(wadEditor->textures.length == 0);
	arrayDeinitialize(&wadEditor->textures);
	
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
	
	// Close opened WAD
	if (editor->wad)
	{
		// Clear temporary data
		graphicsMeshUniformsRelease(editor->testMeshUniforms);
		editor->testMeshUniforms = NULL;
		
		// Clear meshes
		for (unsigned int i = 0; i < editor->meshes.length; i++)
		{
			MESH_DATA* meshData = arrayGetItem(&editor->meshes, i);
			for (unsigned int j = 0; j < meshData->submeshes.length; j++)
			{
				SUBMESH_DATA* submesh = arrayGetItem(&meshData->submeshes, j);
				graphicsMeshRelease(submesh->mesh);
			}
			arrayDeinitialize(&meshData->submeshes);
		}
		arrayClear(&editor->meshes);
		
		// Clear textures
		for (unsigned int i = 0; i < editor->textures.length; i++)
		{
			TEXTURE_LINK* link = arrayGetItem(&editor->textures, i);
			texture2dRelease(link->texture);
		}
		arrayClear(&editor->textures);
		
		wadRelease(editor->wad);
	}
	
	// Load wad
	EXECUTE_RESULT executeResult;
	editor->wad = wadCreateFromContentsOfResourceFile(editor->system, "tut1", &executeResult);
	
	if (executeResultIsFailed(&executeResult))
	{
		editor->wad = NULL;
		semaphoreLeave(editor->accessSemaphore);
		return;
	}
	
	// Create test preview uniforms
	editor->testMeshUniforms = graphicsDeviceCreateMeshUniforms(editor->device);
	
	listClear(editor->wadContentsList);
	WE_LIST_ITEM* rootItem = listGetRootItem(editor->wadContentsList);
	
	// Initialize textures
	WE_LIST_ITEM* item = listItemAddChild(rootItem, 0, NULL, "Texture pages");
	unsigned int length = wadGetNumTexturePages(editor->wad);
	char title[512];
	for (unsigned int i = 0; i < length; i++)
	{
		TEXTURE_PAGE* texturePage = wadGetTexturePage(editor->wad, i);
		TEXTURE2D* texture = graphicsDeviceCreateTexture2dWithData(editor->device, 256, 256, 3, TEXTURE_USAGE_SHADER_READ, texturePageGetData(texturePage));
		
		TEXTURE_LINK* textureLink = arrayAddItem(&editor->textures);
		textureLink->page = texturePage;
		textureLink->texture = texture;
		
		sprintf(title, "Texture page #%u", i);
		listItemAddChild(item, 0, NULL, title);
	}
	
	// Initialize meshes
	item = listItemAddChild(rootItem, 0, NULL, "Meshes");
	length = wadGetNumMeshes(editor->wad);
	for (unsigned int i = 0; i < length; i++)
	{
		// Initialize renderer data
		
		MESH* mesh = wadGetMesh(editor->wad, i);
		unsigned int numPolygons = meshGetNumPolygons(mesh);
		
		TEXTURE_PAGE* page = NULL;
		TEXTURE2D* texture = NULL;
		
		GRAPHICS_VERTEX* vertices = malloc(sizeof(GRAPHICS_VERTEX) * numPolygons * 6);
		unsigned int numVertices = 0;
		for (unsigned int i = 0; i < numPolygons; i++)
		{
			POLYGON* polygon = meshGetPolygon(mesh, i);
			TEXTURE_SAMPLE* textureSample = polygonGetTextureSample(polygon);
			
			if (!texture)
			{
				page = textureSampleGetTexturePage(textureSample);
				texture = _wadEditor_getTextureForTexturePage(editor, page);
			}
			
			vector2f uvLocation = textureSampleGetUVLocation(textureSample);
			vector2f uvSize = textureSampleGetUVSize(textureSample);
			float su = uvLocation.x;
			float sv = uvLocation.y;
			float eu = su + uvSize.x;
			float ev = sv + uvSize.y;
			
			/*float su = (float)(textureSampleGetX(textureSample)) / 256.0f;
			float sv = (float)(textureSampleGetY(textureSample)) / 256.0f;
			float eu = su + (float)(textureSampleGetAddX(textureSample)) / 256.0f;
			float ev = sv + (float)(textureSampleGetAddY(textureSample)) / 256.0f;
			
			const int flipX = textureSampleGetFlipX(textureSample);
			if (!flipX)
			{
				const float temp = su;
				su = eu;
				eu = temp;
			}
			
			const int flipY = textureSampleGetFlipY(textureSample);
			if (!flipY)
			{
				const float temp = sv;
				sv = ev;
				ev = temp;
			}*/
			
			if (polygonIsFlipped(polygon))
			{
				const float temp = su;
				su = eu;
				eu = temp;
			}
			
			vector2f uvA = vector2fCreate(su, sv);
			vector2f uvB = vector2fCreate(eu, sv);
			vector2f uvC = vector2fCreate(eu, ev);
			vector2f uvD = vector2fCreate(su, ev);
			
			vector2f uv1 = vector2fCreate(0.0f, 0.0f);
			vector2f uv2 = vector2fCreate(1.0f, 0.0f);
			vector2f uv3 = vector2fCreate(1.0f, 1.0f);
			vector2f uv4 = vector2fCreate(0.0f, 1.0f);
			
			if (polygonIsTriangle(polygon))
			{
				unsigned short shape = polygonGetTextureSampleShape(polygon);
				if (shape == 0)
				{
					uv1 = uvA;
					uv2 = uvB;
					uv3 = uvD;
				}
				else if (shape == 2)
				{
					uv1 = uvB;
					uv2 = uvC;
					uv3 = uvA;
				}
				else if (shape == 4)
				{
					uv1 = uvC;
					uv2 = uvD;
					uv3 = uvB;
				}
				else if (shape == 6)
				{
					uv1 = uvD;
					uv2 = uvA;
					uv3 = uvC;
				}
			}
			else
			{
				uv1 = uvA;
				uv2 = uvB;
				uv3 = uvC;
				uv4 = uvD;
			}
			
			VERTEX* v1 = polygonGetVertex1(polygon);
			VERTEX* v2 = polygonGetVertex2(polygon);
			VERTEX* v3 = polygonGetVertex3(polygon);
						
			vertices[numVertices + 0].position = vertexGetPosition(v1);
			vertices[numVertices + 0].uv = uv1;
			
			vertices[numVertices + 1].position = vertexGetPosition(v3);
			vertices[numVertices + 1].uv = uv3;
			
			vertices[numVertices + 2].position = vertexGetPosition(v2);
			vertices[numVertices + 2].uv = uv2;
			
			numVertices += 3;
			
			if (!polygonIsTriangle(polygon))
			{
				VERTEX* v4 = polygonGetVertex4(polygon);
				
				vertices[numVertices + 0].position = vertexGetPosition(v1);
				vertices[numVertices + 0].uv = uv1;
				
				vertices[numVertices + 1].position = vertexGetPosition(v4);
				vertices[numVertices + 1].uv = uv4;
				
				vertices[numVertices + 2].position = vertexGetPosition(v3);
				vertices[numVertices + 2].uv = uv3;
				
				numVertices += 3;
			}
		}
		
		for (unsigned int i = 0; i < numVertices; i++)
		{
			vertices[i].weights[0] = 1.0f;
			vertices[i].weights[1] = 0.0f;
		}
		
		MESH_DATA* meshData = arrayAddItem(&editor->meshes);
		meshData->mesh = mesh;
		arrayInitializeWithAllocator(&meshData->submeshes, editor->submeshAllocator);
		
		if (texture)
		{
			SUBMESH_DATA* submesh = arrayAddItem(&meshData->submeshes);
			submesh->page = page;
			submesh->texture = texture;
			submesh->mesh = graphicsDeviceCreateMesh(editor->device, vertices, numVertices, texture);
		}
		
		sprintf(title, "Mesh #%u", i);
		listItemAddChild(item, LI_FLAG_MESH, meshData, title);
		
		free(vertices);
	}
	editor->selectedMesh = arrayGetItem(&editor->meshes, 287);
	
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
	
	if (wadGetNumTexturePages(editor->wad) > 5 && wadGetNumMeshes(editor->wad) > 300)
	{
		/*GRAPHICS_VERTEX vertices[] = {
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
		editor->testMeshUniforms = graphicsDeviceCreateMeshUniforms(editor->device);*/
	}
	
	semaphoreLeave(editor->accessSemaphore);
}


static void _wadEditor_updateState(WAD_EDITOR* wadEditor)
{
	const double currentTime = systemGetCurrentTime(wadEditor->system);
	const double timeElapsed = currentTime - wadEditor->lastUpdateTime;
	
	static float rotation = 0.0f;
	static float rotationSpeed = 1.0f;
	
	rotation += (float)(timeElapsed * rotationSpeed);
	
	if (wadEditor->testMeshUniforms)
	{
		const float fovyRadians = 65.0 * (M_PI / 180.0f);
		float aspect = graphicsViewGetWidth(wadEditor->outputView) / graphicsViewGetHeight(wadEditor->outputView);
		
		matrix4f rotationMatrix = matrix4fRotation(rotation, vector3fCreate(0.0f, 1.0f, 0.0f));
		
		GRAPHICS_MESH_UNIFORMS_DATA data;
		data.modelView[0] = matrix4fMul(matrix4fTranslation(0.0f, -0.5f, -1.5f), rotationMatrix);
		data.modelView[1] = matrix4fMul(matrix4fTranslation(0.0f, -0.5f, -1.5f), rotationMatrix);
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
	if (wadEditor->selectedMesh)
	{
		for (unsigned int i = 0; i < wadEditor->selectedMesh->submeshes.length; i++)
		{
			SUBMESH_DATA* submesh = arrayGetItem(&wadEditor->selectedMesh->submeshes, i);
			GRAPHICS_MESH* graphicsMesh = submesh->mesh;
			renderCommandEncoderScheduleDrawMesh(renderEncoder, graphicsMesh, wadEditor->testMeshUniforms);
		}
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
