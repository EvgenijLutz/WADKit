//
//  wad_editor.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

#define LI_FLAG_TEXTURE		(1)
#define LI_FLAG_MESH		(2)
#define LI_FLAG_MOVABLE		(3)
#define LI_FLAG_ANIMATION	(4)
#define LI_FLAG_STATIC		(5)
#define LI_FLAG_LAWA_KWOWT	(6)

// MARK: - Structures

typedef struct TEXTURE_DATA
{
	WK_TEXTURE_PAGE* page;
	TEXTURE2D* texture;
	GR_BUFFER* vertexBuffer;
}
TEXTURE_DATA;

typedef struct SUBMESH_DATA
{
	WK_TEXTURE_PAGE* page;
	TEXTURE2D* texture;
	GR_BUFFER* vertexBuffer;
	unsigned int numVertices;
}
SUBMESH_DATA;

typedef struct MESH_DATA
{
	WK_MESH* mesh;
	MAGIC_ARRAY submeshes;
}
MESH_DATA;

typedef struct MOVABLE_JOINT_DATA
{
	WK_JOINT* joint;
	MESH_DATA* mesh;
	MESH_DATA* lataSkinJointMesh;
}
MOVABLE_JOINT_DATA;

typedef struct MOVABLE_DATA
{
	WK_MOVABLE* movable;
	MESH_DATA* rootMesh;
	MAGIC_ARRAY jointedMeshes;
}
MOVABLE_DATA;

typedef struct ANIMATION_DATA
{
	MOVABLE_DATA* movable;
	WK_ANIMATION* animation;
}
ANIMATION_DATA;

typedef struct STATIC_DATA
{
	STATIC* staticObject;
	MESH_DATA* mesh;
}
STATIC_DATA;

// MARK: - WAD items list subscriber

static void _wadItemsList_itemSelected(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo)
{
	WAD_EDITOR* editor = (WAD_EDITOR*)userInfo;
	printf("Item selected: %s\n", stringGetData(&item->title));
	
	semaphoreEnter(editor->accessSemaphore);
	
	if (item->flags == LI_FLAG_TEXTURE)
	{
		editor->selectedTexture = item->data;
		editor->numTranslations = 0;
	}
	else if (item->flags == LI_FLAG_MESH)
	{
		editor->selectedMesh = item->data;
		editor->numTranslations = 1;
		editor->transforms[0].mesh = editor->selectedMesh;
		editor->transforms[0].skinJointMesh = NULL;
	}
	else if (item->flags == LI_FLAG_MOVABLE)
	{
		editor->selectedMovable = item->data;
		editor->numTranslations = 0;
		
		WK_MOVABLE* movable = editor->selectedMovable->movable;
		if (movableGetNumAnimations(movable))
		{
			editor->selectedAnimation = movableGetAnimation(movable, 0);
		}
	}
	else if (item->flags == LI_FLAG_ANIMATION)
	{
		ANIMATION_DATA* animationData = item->data;
		editor->selectedMovable = animationData->movable;
		editor->selectedAnimation = animationData->animation;
		editor->numTranslations = 0;
	}
	else if (item->flags == LI_FLAG_STATIC)
	{
		editor->selectedStatic = item->data;
		editor->numTranslations = 1;
		editor->transforms[0].mesh = editor->selectedStatic->mesh;
		editor->transforms[0].skinJointMesh = NULL;
	}
	else
	{
		editor->numTranslations = 0;
	}
	
	semaphoreLeave(editor->accessSemaphore);
}

static void _wadItemsList_ItemDeselected(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo)
{
	WAD_EDITOR* editor = (WAD_EDITOR*)userInfo;
	printf("Item deselected: %s\n", stringGetData(&item->title));
	
	semaphoreEnter(editor->accessSemaphore);
	switch (item->flags)
	{
		case LI_FLAG_TEXTURE: editor->selectedTexture = NULL; break;
		case LI_FLAG_MESH: editor->selectedMesh = NULL; break;
		case LI_FLAG_MOVABLE: editor->selectedMovable = NULL; editor->selectedAnimation = NULL; break;
		case LI_FLAG_ANIMATION: editor->selectedMovable = NULL; editor->selectedAnimation = NULL; break;
		case LI_FLAG_STATIC: editor->selectedStatic = NULL; break;
		default: break;
	}
	semaphoreLeave(editor->accessSemaphore);
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

static TEXTURE2D* _wadEditor_getTextureForTexturePage(WAD_EDITOR* editor, WK_TEXTURE_PAGE* page)
{
	for (unsigned int i = 0; i < editor->textures.length; i++)
	{
		TEXTURE_DATA* link = magicArrayGetItem(&editor->textures, i);
		if (link->page == page)
		{
			return link->texture;
		}
	}
	
	// This should never happen
	assert(0);
	return NULL;
}


static MESH_DATA* _wadEditor_getMeshDataForMesh(WAD_EDITOR* editor, WK_MESH* mesh)
{
	for (unsigned int i = 0; i < editor->meshes.length; i++)
	{
		MESH_DATA* meshData = magicArrayGetItem(&editor->meshes, i);
		if (meshData->mesh == mesh)
		{
			return meshData;
		}
	}
	
	// This should never happen
	assert(0);
	return NULL;
}


// MARK: - WAD editor interface

WAD_EDITOR* wadEditorCreate(SYSTEM* system, WE_LIST* wadContentsList, GRAPHICS_VIEW* outputView)
{
	WAD_EDITOR* wadEditor = malloc(sizeof(WAD_EDITOR));
	wadEditor->system = system;
	wadEditor->accessSemaphore = systemCreateSemaphore(wadEditor->system, 1);
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
	
	wadEditor->lastUpdateTime = systemGetCurrentTime(wadEditor->system);
	
	magicArrayInitialize(&wadEditor->textures, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(TEXTURE_DATA), 8);
	
	wadEditor->submeshAllocator = dataAllocatorCreate(sizeof(SUBMESH_DATA), 256);
	magicArrayInitialize(&wadEditor->meshes, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(MESH_DATA), 128);
	
	wadEditor->movableJointsAllocator = dataAllocatorCreate(sizeof(MOVABLE_JOINT_DATA), 256);
	magicArrayInitialize(&wadEditor->movables, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(MOVABLE_DATA), 16);
	
	magicArrayInitialize(&wadEditor->statics, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(STATIC_DATA), 16);
	
	magicArrayInitialize(&wadEditor->animations, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(ANIMATION_DATA), 128);
	
	wadEditor->selectedTexture = NULL;
	wadEditor->selectedMesh = NULL;
	wadEditor->selectedMovable = NULL;
	wadEditor->selectedStatic = NULL;
	
	wadEditor->numTranslations = 0;
	wadEditor->translationsCapacity = 64;
	wadEditor->transforms = malloc(sizeof(TRANSFORM_DATA) * wadEditor->translationsCapacity);
	for (unsigned int i = 0; i < wadEditor->translationsCapacity; i++)
	{
		TRANSFORM_DATA* translation = &wadEditor->transforms[i];
		translation->modelMatrix = matrix4fIdentity();
		translation->transformUniforms = graphicsDeviceCreateMeshUniformsBuffer(wadEditor->device);
	}
	
	wadEditor->selectedAnimation = NULL;
	wadEditor->animationTime = 0.0f;
	
	wadEditor->useInterpolation = 1;
	wadEditor->useQuaternionRotations = 1;
	
	wadEditor->viewportUniforms = graphicsDeviceCreateViewportUniformsBuffer(wadEditor->device);
	
	return wadEditor;
}

void wadEditorRelease(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	
	graphicsBufferRelease(wadEditor->viewportUniforms);
	
	for (unsigned int i = 0; i < wadEditor->translationsCapacity; i++)
	{
		TRANSFORM_DATA* translation = &wadEditor->transforms[i];
		//graphicsMeshUniformsRelease(translation->uniforms);
		graphicsBufferRelease(translation->transformUniforms);
	}
	free(wadEditor->transforms);
	
	assert(wadEditor->statics.length == 0);
	magicArrayDeinitialize(&wadEditor->statics);
	
	assert(wadEditor->animations.length == 0);
	magicArrayDeinitialize(&wadEditor->animations);
	
	assert(wadEditor->meshes.length == 0);
	magicArrayDeinitialize(&wadEditor->meshes);
	dataAllocatorRelease(wadEditor->submeshAllocator);
	
	assert(wadEditor->textures.length == 0);
	magicArrayDeinitialize(&wadEditor->textures);
	
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
		editor->selectedTexture = NULL;
		editor->selectedMesh = NULL;
		editor->selectedMovable = NULL;
		editor->selectedStatic = NULL;
		editor->selectedAnimation = NULL;
		
		editor->numTranslations = 0;
		
		// Clear static
		magicArrayClear(&editor->statics);
		
		// Clear animations
		magicArrayClear(&editor->animations);
		
		// Clear movables
		for (unsigned int i = 0; i < editor->movables.length; i++)
		{
			MOVABLE_DATA* movableData = magicArrayGetItem(&editor->movables, i);
			magicArrayDeinitialize(&movableData->jointedMeshes);
		}
		magicArrayClear(&editor->movables);
		
		// Clear meshes
		for (unsigned int i = 0; i < editor->meshes.length; i++)
		{
			MESH_DATA* meshData = magicArrayGetItem(&editor->meshes, i);
			for (unsigned int j = 0; j < meshData->submeshes.length; j++)
			{
				SUBMESH_DATA* submesh = magicArrayGetItem(&meshData->submeshes, j);
				//graphicsMeshRelease(submesh->mesh);
				graphicsBufferRelease(submesh->vertexBuffer);
			}
			magicArrayDeinitialize(&meshData->submeshes);
		}
		magicArrayClear(&editor->meshes);
		
		// Clear textures
		for (unsigned int i = 0; i < editor->textures.length; i++)
		{
			TEXTURE_DATA* link = magicArrayGetItem(&editor->textures, i);
			//graphicsMeshRelease(link->mesh);
			graphicsBufferRelease(link->vertexBuffer);
			texture2dRelease(link->texture);
		}
		magicArrayClear(&editor->textures);
		
		wadRelease(editor->wad);
	}
	
	// Load wad
	EXECUTE_RESULT executeResult;
	//editor->wad = wadCreateFromContentsOfResourceFile(editor->system, "tut1", &executeResult);
	editor->wad = wadCreateFromContentsOfResourceFile(editor->system, "1-tutorial", &executeResult);
	
	if (executeResultIsFailed(&executeResult))
	{
		editor->wad = NULL;
		semaphoreLeave(editor->accessSemaphore);
		return;
	}
	
	listClear(editor->wadContentsList);
	WE_LIST_ITEM* rootItem = listGetRootItem(editor->wadContentsList);
	
	// Initialize textures
	WE_LIST_ITEM* item = listItemAddChild(rootItem, 0, NULL, "Texture pages");
	unsigned int length = wadGetNumTexturePages(editor->wad);
	char title[512];
	for (unsigned int i = 0; i < length; i++)
	{
		WK_TEXTURE_PAGE* texturePage = wadGetTexturePage(editor->wad, i);
		TEXTURE2D* texture = graphicsDeviceCreateTexture2dWithData(editor->device, 256, 256, 3, TEXTURE_USAGE_SHADER_READ, texturePageGetData(texturePage));
		
		GR_T_VERTEX vertices[] = {
			{ .position=vector3fCreate(-0.5, -0.5, 0.0), .uv=vector2fCreate(0.0, 1.0)/*,.weights={ 1.0, 0.0 }*/},
			{ .position=vector3fCreate( 0.5, -0.5, 0.0), .uv=vector2fCreate(1.0, 1.0)/*,.weights={ 1.0, 0.0 }*/},
			{ .position=vector3fCreate( 0.5,  0.5, 0.0), .uv=vector2fCreate(1.0, 0.0)/*,.weights={ 1.0, 0.0 }*/},
			
			{ .position=vector3fCreate( 0.5,  0.5, 0.0), .uv=vector2fCreate(1.0, 0.0)/*,.weights={ 1.0, 0.0 }*/},
			{ .position=vector3fCreate(-0.5,  0.5, 0.0), .uv=vector2fCreate(0.0, 0.0)/*,.weights={ 1.0, 0.0 }*/},
			{ .position=vector3fCreate(-0.5, -0.5, 0.0), .uv=vector2fCreate(0.0, 1.0)/*,.weights={ 1.0, 0.0 }*/}
		};
		
		TEXTURE_DATA* textureLink = magicArrayAddItem(&editor->textures);
		textureLink->page = texturePage;
		textureLink->texture = texture;
		textureLink->vertexBuffer = graphicsDeviceCreateVertexBuffer(editor->device, editor->commandQueue, vertices, 6);
		
		sprintf(title, "Texture page #%u", i);
		listItemAddChild(item, LI_FLAG_TEXTURE, textureLink, title);
	}
	
	// Initialize meshes
	item = listItemAddChild(rootItem, 0, NULL, "Meshes");
	length = wadGetNumMeshes(editor->wad);
	
	typedef struct TEMP_SUBMESH
	{
		WK_TEXTURE_PAGE* page;
		TEXTURE2D* texture;
		unsigned int numVertices;
		unsigned int verticesCapacity;
		GR_T_VERTEX* vertices;
	}
	TEMP_SUBMESH;
	unsigned int numTempSubmeshes = 0;
	unsigned int tempSubmeshesCapacity = 1;
	TEMP_SUBMESH* tempSubmeshes = malloc(sizeof(TEMP_SUBMESH) * tempSubmeshesCapacity);
	for (unsigned int k = 0; k < tempSubmeshesCapacity; k++)
	{
		TEMP_SUBMESH* tempSubmesh = &tempSubmeshes[k];
		tempSubmesh->page = NULL;
		tempSubmesh->texture = NULL;
		tempSubmesh->numVertices = 0;
		tempSubmesh->verticesCapacity = 6;
		tempSubmesh->vertices = malloc(sizeof(GR_T_VERTEX) * tempSubmesh->verticesCapacity);
	}
	
	for (unsigned int meshIndex = 0; meshIndex < length; meshIndex++)
	{
		WK_MESH* mesh = wadGetMesh(editor->wad, meshIndex);
		unsigned int numPolygons = meshGetNumPolygons(mesh);
		
		// Clear tempSubmeshes
		for (unsigned int k = 0; k < numTempSubmeshes; k++)
		{
			TEMP_SUBMESH* currentTempSubmesh = &tempSubmeshes[k];
			currentTempSubmesh->numVertices = 0;
		}
		numTempSubmeshes = 0;
		
		for (unsigned int polygonIndex = 0; polygonIndex < numPolygons; polygonIndex++)
		{
			WK_POLYGON* polygon = meshGetPolygon(mesh, polygonIndex);
			WK_TEXTURE_SAMPLE* textureSample = polygonGetTextureSample(polygon);
			
			// Find submesh, to which current textureSample belongs
			WK_TEXTURE_PAGE* texturePage = textureSampleGetTexturePage(textureSample);
			TEXTURE2D* texture = _wadEditor_getTextureForTexturePage(editor, texturePage);
			TEMP_SUBMESH* tempSubmesh = NULL;
			for (unsigned int k = 0; k < numTempSubmeshes; k++)
			{
				TEMP_SUBMESH* currentTempSubmesh = &tempSubmeshes[k];
				if (currentTempSubmesh->page == texturePage)
				{
					tempSubmesh = currentTempSubmesh;
					break;
				}
			}
			
			// If submesh is not found, then add new one
			if (tempSubmesh == NULL)
			{
				numTempSubmeshes++;
				while (numTempSubmeshes > tempSubmeshesCapacity)
				{
					tempSubmeshesCapacity++;
					tempSubmeshes = realloc(tempSubmeshes, sizeof(TEMP_SUBMESH) * tempSubmeshesCapacity);
					assert(tempSubmeshes);
					
					TEMP_SUBMESH* currentTempSubmesh = &tempSubmeshes[tempSubmeshesCapacity - 1];
					currentTempSubmesh->page = NULL;
					currentTempSubmesh->numVertices = 0;
					currentTempSubmesh->verticesCapacity = 6;
					currentTempSubmesh->vertices = malloc(sizeof(GR_T_VERTEX) * currentTempSubmesh->verticesCapacity);
					assert(currentTempSubmesh->vertices);
				}
				
				tempSubmesh = &tempSubmeshes[numTempSubmeshes - 1];
				tempSubmesh->page = texturePage;
				tempSubmesh->texture = texture;
				tempSubmesh->numVertices = 0;
			}
			
			// Reserve 6 free vertices for submesh
			while (tempSubmesh->numVertices + 6 > tempSubmesh->verticesCapacity)
			{
				tempSubmesh->verticesCapacity += 6;
				tempSubmesh->vertices = realloc(tempSubmesh->vertices, sizeof(GR_T_VERTEX) * tempSubmesh->verticesCapacity);
				assert(tempSubmesh->vertices);
			}
			
			vector2f uvLocation = textureSampleGetUVLocation(textureSample);
			vector2f uvSize = textureSampleGetUVSize(textureSample);
			float su = uvLocation.x;
			float sv = uvLocation.y;
			float eu = su + uvSize.x;
			float ev = sv + uvSize.y;
			
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
			
			WK_VERTEX* v1 = polygonGetVertex1(polygon);
			WK_VERTEX* v2 = polygonGetVertex2(polygon);
			WK_VERTEX* v3 = polygonGetVertex3(polygon);
						
			tempSubmesh->vertices[tempSubmesh->numVertices + 0].position = vertexGetPosition(v1);
			tempSubmesh->vertices[tempSubmesh->numVertices + 0].uv = uv1;
			
			tempSubmesh->vertices[tempSubmesh->numVertices + 1].position = vertexGetPosition(v3);
			tempSubmesh->vertices[tempSubmesh->numVertices + 1].uv = uv3;
			
			tempSubmesh->vertices[tempSubmesh->numVertices + 2].position = vertexGetPosition(v2);
			tempSubmesh->vertices[tempSubmesh->numVertices + 2].uv = uv2;
			
			tempSubmesh->numVertices += 3;
			
			if (!polygonIsTriangle(polygon))
			{
				WK_VERTEX* v4 = polygonGetVertex4(polygon);
				
				tempSubmesh->vertices[tempSubmesh->numVertices + 0].position = vertexGetPosition(v1);
				tempSubmesh->vertices[tempSubmesh->numVertices + 0].uv = uv1;
				
				tempSubmesh->vertices[tempSubmesh->numVertices + 1].position = vertexGetPosition(v4);
				tempSubmesh->vertices[tempSubmesh->numVertices + 1].uv = uv4;
				
				tempSubmesh->vertices[tempSubmesh->numVertices + 2].position = vertexGetPosition(v3);
				tempSubmesh->vertices[tempSubmesh->numVertices + 2].uv = uv3;
				
				tempSubmesh->numVertices += 3;
			}
			
			assert(tempSubmesh->numVertices);
		}
		
		MESH_DATA* meshData = magicArrayAddItem(&editor->meshes);
		meshData->mesh = mesh;
		magicArrayInitializeWithAllocator(&meshData->submeshes, editor->submeshAllocator);
		
		for (unsigned int k = 0; k < numTempSubmeshes; k++)
		{
			TEMP_SUBMESH* currentTempSubmesh = &tempSubmeshes[k];
			assert(currentTempSubmesh->numVertices);
			/*for (unsigned int l = 0; l < currentTempSubmesh->numVertices; l++)
			{
				currentTempSubmesh->vertices[l].weights[0] = 1.0f;
				currentTempSubmesh->vertices[l].weights[1] = 0.0f;
			}*/
			
			SUBMESH_DATA* submesh = magicArrayAddItem(&meshData->submeshes);
			submesh->page = currentTempSubmesh->page;
			submesh->texture = currentTempSubmesh->texture;
			submesh->vertexBuffer = graphicsDeviceCreateVertexBuffer(editor->device, editor->commandQueue, currentTempSubmesh->vertices, currentTempSubmesh->numVertices);
			submesh->numVertices = currentTempSubmesh->numVertices;
		}
		
		sprintf(title, "Mesh #%u", meshIndex);
		listItemAddChild(item, LI_FLAG_MESH, meshData, title);
	}
	
	// Release tempSubmeshes
	for (unsigned int k = 0; k < tempSubmeshesCapacity; k++)
	{
		TEMP_SUBMESH* currentTempSubmesh = &tempSubmeshes[k];
		free(currentTempSubmesh->vertices);
	}
	free(tempSubmeshes);
	
	
	// Get placeholder null mesh used by ID_LARA, ID_PISTOLS_ANIM, etc
	WK_MESH* nullMesh = NULL;
	if (wadGetNumMeshes(editor->wad))
	{
		nullMesh = wadGetMesh(editor->wad, 0);
	}
	
	// Initialize movables
	item = listItemAddChild(rootItem, 0, NULL, "Movables");
	length = wadGetNumMovables(editor->wad);
	for (unsigned int i = 0; i < length; i++)
	{
		WK_MOVABLE* movable = wadGetMovableByIndex(editor->wad, i);
		MOVABLE_ID movableId = movableGetId(movable);
		const char* movableName = movableIdGetTerribleName(movableId);
		MESH_DATA* rootMeshData = _wadEditor_getMeshDataForMesh(editor, movableGetRootMesh(movable));
		if (rootMeshData->mesh == nullMesh)
		{
			WK_MOVABLE* laraSkinMovable = wadGetMovableById(editor->wad, MOVABLE_ID_LARA_SKIN);
			if (laraSkinMovable)
			{
				WK_MESH* laraSkinRootMesh = movableGetRootMesh(laraSkinMovable);
				rootMeshData = _wadEditor_getMeshDataForMesh(editor, laraSkinRootMesh);
			}
		}
		
		MOVABLE_DATA* movableData = magicArrayAddItem(&editor->movables);
		movableData->movable = movable;
		movableData->rootMesh = rootMeshData;
		magicArrayInitializeWithAllocator(&movableData->jointedMeshes, editor->movableJointsAllocator);
		
		WE_LIST_ITEM* movableItem = listItemAddChild(item, LI_FLAG_MOVABLE, movableData, movableName);
		
		/*WE_LIST_ITEM* rootMeshItem =*/ listItemAddChild(movableItem, 0, NULL, "Root mesh");
		WE_LIST_ITEM* jointsItem = listItemAddChild(movableItem, 0, NULL, "Jointed meshes");
		unsigned int numJoints = movableGetNumJoints(movable);
		for (unsigned int j = 0; j < numJoints; j++)
		{
			WK_JOINT* joint = movableGetJoint(movable, j);
			WK_MESH* mesh = jointGetMesh(joint);
			if (mesh == nullMesh)
			{
				WK_MOVABLE* laraSkinMovable = wadGetMovableById(editor->wad, MOVABLE_ID_LARA_SKIN);
				if (laraSkinMovable)
				{
					unsigned int fixedJointIndex = j;
					
					if (movableId == MOVABLE_ID_LARA_HOLSTERS ||
						movableId == MOVABLE_ID_LARA_HOLSTERS_PISTOLS ||
						movableId == MOVABLE_ID_LARA_HOLSTERS_UZIS ||
						movableId == MOVABLE_ID_LARA_HOLSTERS_SIXSHOOTER)
					{
						if (fixedJointIndex >= 13 /*&& fixedJointIndex <= 15*/) {
							fixedJointIndex -= 6;
						}
						else if (fixedJointIndex >= 9 /*&& fixedJointIndex <= 12*/) {
							fixedJointIndex++;
						}
						else if (fixedJointIndex == 8) {
							fixedJointIndex = 6;
						}
						else if (fixedJointIndex >= 3/*&& fixedJointIndex <= 7*/) {
							fixedJointIndex--;
						}
					}
					
					if (fixedJointIndex >= movableGetNumJoints(laraSkinMovable))
					{
						fixedJointIndex = 0;
					}
					
					WK_JOINT* laraSkinJoint = movableGetJoint(laraSkinMovable, fixedJointIndex);
					mesh = jointGetMesh(laraSkinJoint);
				}
			}
			
			MOVABLE_JOINT_DATA* jointData = magicArrayAddItem(&movableData->jointedMeshes);
			jointData->joint = joint;
			jointData->mesh = _wadEditor_getMeshDataForMesh(editor, mesh);
			jointData->lataSkinJointMesh = NULL;
			
			/*
			 Fallen so far from where we were before
			 You'll never find what you've been searchin' for
			 Something to fill the void and make up
			 For all those missing pieces of you
			 Like I could only dream do to
			 */
			
			if (movableId == MOVABLE_ID_LARA ||
				movableId == MOVABLE_ID_PISTOLS_ANIM ||
				movableId == MOVABLE_ID_UZI_ANIM ||
				movableId == MOVABLE_ID_SHOTGUN_ANIM ||
				movableId == MOVABLE_ID_CROSSBOW_ANIM ||
				movableId == MOVABLE_ID_GRENADE_GUN_ANIM ||
				movableId == MOVABLE_ID_SIXSHOOTER_ANIM ||
				movableId == MOVABLE_ID_LARA_CROSSBOW_LASER ||
				movableId == MOVABLE_ID_LARA_REVOLVER_LASER ||
				movableId == MOVABLE_ID_CROWBAR_ANIM ||
				movableId == MOVABLE_ID_TORCH_ANIM ||
				movableId == MOVABLE_ID_FLARE_ANIM)
			{
				WK_MOVABLE* laraSkinJoint = wadGetMovableById(editor->wad, MOVABLE_ID_LARA_SKIN_JOINTS);
				if (laraSkinJoint)
				{
					WK_JOINT* skinJoint = movableGetJoint(laraSkinJoint, j);
					WK_MESH* skinJointMesh = jointGetMesh(skinJoint);
					jointData->lataSkinJointMesh = _wadEditor_getMeshDataForMesh(editor, skinJointMesh);
				}
			}
			
			sprintf(title, "Jointed mesh #%u", j);
			listItemAddChild(jointsItem, 0, NULL, title);
		}
		
		WE_LIST_ITEM* animationsItem = listItemAddChild(movableItem, 0, NULL, "Animations");
		unsigned int numAnimations = movableGetNumAnimations(movable);
		for (unsigned int j = 0; j < numAnimations; j++)
		{
			WK_ANIMATION* animation = movableGetAnimation(movable, j);
			ANIMATION_DATA* animationData = magicArrayAddItem(&editor->animations);
			animationData->movable = movableData;
			animationData->animation = animation;
			
			sprintf(title, "Animation #%u", j);
			listItemAddChild(animationsItem, LI_FLAG_ANIMATION, animationData, title);
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
		WK_MESH* staticMesh = staticGetMesh(staticObject);
		
		STATIC_DATA* staticData = magicArrayAddItem(&editor->statics);
		staticData->staticObject = staticObject;
		staticData->mesh = _wadEditor_getMeshDataForMesh(editor, staticMesh);
		
		listItemAddChild(item, LI_FLAG_STATIC, staticData, staticName);
	}
	
	semaphoreLeave(editor->accessSemaphore);
}

static matrix4f _wadEditor_transform(WAD_EDITOR* wadEditor, WK_KEYFRAME* firstKeyframe, WK_KEYFRAME* secondKeyframe, int componentIndex, float keyframeInterpolationCoefficient, int calculateOffset)
{
	matrix4f rotMat = matrix4fIdentity();
	if (wadEditor->useInterpolation)
	{
		if (wadEditor->useQuaternionRotations)
		{
			quaternionf rotQuat1 = keyframeGetQuaternionRotation(firstKeyframe, componentIndex);
			quaternionf rotQuat2 = keyframeGetQuaternionRotation(secondKeyframe, componentIndex);
			quaternionf rotQuat = simd_slerp(rotQuat1, rotQuat2, keyframeInterpolationCoefficient);
			rotMat = matrix4fFromQuaternionf(rotQuat);
		}
		else
		{
			vector3f rotation = keyframeCalculateInterpolatedRotation(firstKeyframe, secondKeyframe, componentIndex, keyframeInterpolationCoefficient);
			matrix4f rotZMat = matrix4fRotationZ(rotation.z);
			matrix4f rotXMat = matrix4fRotationX(rotation.x);
			matrix4f rotYMat = matrix4fRotationY(rotation.y);
			rotMat = matrix4fMul(rotXMat, rotZMat);
			rotMat = matrix4fMul(rotYMat, rotMat);
		}
	}
	else
	{
		if (wadEditor->useQuaternionRotations)
		{
			quaternionf rotQuat = keyframeGetQuaternionRotation(firstKeyframe, componentIndex);
			rotMat = matrix4fFromQuaternionf(rotQuat);
		}
		else
		{
			vector3f rotation = keyframeGetEulerRotation(firstKeyframe, componentIndex);
			matrix4f rotZMat = matrix4fRotationZ(rotation.z);
			matrix4f rotXMat = matrix4fRotationX(rotation.x);
			matrix4f rotYMat = matrix4fRotationY(rotation.y);
			rotMat = matrix4fMul(rotXMat, rotZMat);
			rotMat = matrix4fMul(rotYMat, rotMat);
		}
	}
	
	if (calculateOffset)
	{
		vector3f offset;
		if (wadEditor->useInterpolation)
		{
			offset = keyframeGetInterpolatedRootOffset(firstKeyframe, secondKeyframe, keyframeInterpolationCoefficient);
		}
		else
		{
			offset = keyframeGetRootOffset(firstKeyframe);
		}
		matrix4f translateMat = matrix4fTranslation(offset.x, offset.y, offset.z);
		return matrix4fMul(translateMat, rotMat);
	}
	
	return rotMat;
}


static void _wadEditor_updateState(WAD_EDITOR* wadEditor)
{
	const double currentTime = systemGetCurrentTime(wadEditor->system);
	const double timeElapsed = currentTime - wadEditor->lastUpdateTime;
	
	static float rotation = 0.0f;
	static float rotationSpeed = 0.7f;
	
	rotation += (float)(timeElapsed * rotationSpeed);
	
	const float fovyRadians = 65.0 * (M_PI / 180.0f);
	float aspect = graphicsViewGetWidth(wadEditor->outputView) / graphicsViewGetHeight(wadEditor->outputView);
	
	GR_VIEWPORT_UNIFORMS_DATA viewportUniformsData;
	viewportUniformsData.projection = matrix4fPerspectiveRightHand_MetalNDC(fovyRadians, aspect, 0.01f, 100.0f);
	viewportUniformsBufferSetData(wadEditor->viewportUniforms, &viewportUniformsData);
	
	GR_MESH_UNIFORM_DATA data;
	data.ambient = vector3fCreate(1.0f, 1.0f, 1.0f);
	if (wadEditor->selectedTexture)
	{
		data.modelView = matrix4fMul(matrix4fTranslation(0.0f, 0.0f, -0.8f), matrix4fIdentity());
		
		TRANSFORM_DATA* firstTranslation = &wadEditor->transforms[0];
		meshUniformsBufferSetData(firstTranslation->transformUniforms, &data);
	}
	else if (wadEditor->selectedMesh)
	{
		matrix4f viewMatrix = matrix4fTranslation(0.0f, 0.0f, -0.3f);
		matrix4f rotationMatrix = matrix4fRotationY(rotation);
		data.modelView = matrix4fMul(viewMatrix, rotationMatrix);
		
		TRANSFORM_DATA* firstTranslation = &wadEditor->transforms[0];
		meshUniformsBufferSetData(firstTranslation->transformUniforms, &data);
	}
	else if (wadEditor->selectedMovable)
	{
		// Calculate animation
		WK_KEYFRAME* keyframe = NULL;
		WK_KEYFRAME* nextKeyframe = NULL;
		float keyframeInterpolationCoefficient = 0.0f;
		if (wadEditor->selectedAnimation)
		{
			unsigned int numKeyFrames = animationGetNumKeyframes(wadEditor->selectedAnimation);
			if (numKeyFrames)
			{
				float animationFrameDuration = //5.0f *
									(float)animationGetFrameDuration(wadEditor->selectedAnimation);
				float animationSpeed = animationFrameDuration / 30.0f;
				
				wadEditor->animationTime += timeElapsed;
				float animationDuration = numKeyFrames * animationSpeed;
				while (wadEditor->animationTime > animationDuration) {
					wadEditor->animationTime -= animationDuration;
				}
				
				unsigned int currentKeyframe = floorf(wadEditor->animationTime / animationSpeed);
				keyframe = animationGetKeyframe(wadEditor->selectedAnimation, currentKeyframe);
				
				unsigned int nextKeyframeIndex = (currentKeyframe + 1) % numKeyFrames;
				nextKeyframe = animationGetKeyframe(wadEditor->selectedAnimation, nextKeyframeIndex);
				
				float keyframeTime = wadEditor->animationTime - ((float)currentKeyframe * animationSpeed);
				
				keyframeInterpolationCoefficient = 1.0f / animationSpeed * keyframeTime;
			}
		}
		
		matrix4f viewRotationMatrix = matrix4fRotationY(rotation);
		matrix4f viewTranslationMatrix = matrix4fTranslation(0.0f, 0.0f, -0.8f);
		viewTranslationMatrix = matrix4fTranslation(0.0f, -0.4f, -0.8f);
		matrix4f viewMatrix = matrix4fMul(viewTranslationMatrix, viewRotationMatrix);
		
		TRANSFORM_DATA* transforms = wadEditor->transforms;
		TRANSFORM_DATA* firstTranslation = &transforms[0];
		firstTranslation->mesh = wadEditor->selectedMovable->rootMesh;
		firstTranslation->skinJointMesh = NULL;
		firstTranslation->stackParentIndex = 0;
		firstTranslation->modelMatrix = matrix4fIdentity();
		if (keyframe)
		{
			firstTranslation->modelMatrix = _wadEditor_transform(wadEditor, keyframe, nextKeyframe, 0, keyframeInterpolationCoefficient, 1);
		}
		data.modelView = matrix_multiply(viewMatrix, firstTranslation->modelMatrix);
		meshUniformsBufferSetData(firstTranslation->transformUniforms, &data);
		
		unsigned int transformIndex = 0;
		
		MOVABLE_DATA* selectedMovable = wadEditor->selectedMovable;
		unsigned int numJointedMeshes = selectedMovable->jointedMeshes.length;
		wadEditor->numTranslations = numJointedMeshes + 1;
		for (unsigned int jointIndex = 0; jointIndex < numJointedMeshes; jointIndex++)
		{
			transformIndex++;
			assert(transformIndex < wadEditor->translationsCapacity);
			TRANSFORM_DATA* parentTransform = &transforms[transformIndex - 1];
			TRANSFORM_DATA* currentTransform = &transforms[transformIndex];
			currentTransform->stackParentIndex = parentTransform->stackParentIndex;
			
			MOVABLE_JOINT_DATA* jointData = magicArrayGetItem(&selectedMovable->jointedMeshes, jointIndex);
			WK_JOINT* joint = jointData->joint;
			vector3f offset = jointGetOffset(joint);
			matrix4f translationMatrix = matrix4fTranslation(offset.x, offset.y, offset.z);
			
			JOINT_LOCATION_TYPE jointLinkType = jointGetLocationType(joint);
			if (jointLinkType == JOINT_LOCATION_TYPE_LINK_TO_PREVIOUS_MESH)
			{
				// do nothing lol
			}
			else if (jointLinkType == JOINT_LOCATION_TYPE_PULL_PARENT_FROM_STACK_AND_CONNECT)
			{
				parentTransform = &transforms[currentTransform->stackParentIndex];
				currentTransform->stackParentIndex = parentTransform->stackParentIndex;
			}
			else if (jointLinkType == JOINT_LOCATION_TYPE_PUSH_PARENT_TO_STACK_AND_CONNECT)
			{
				currentTransform->stackParentIndex = transformIndex - 1;
				currentTransform->modelMatrix = transforms[transformIndex - 1].modelMatrix;
			}
			else if (jointLinkType == JOINT_LOCATION_TYPE_LINK_TO_PARENT_IN_STACK)
			{
				parentTransform = &transforms[currentTransform->stackParentIndex];
			}
			
			if (keyframe)
			{
				matrix4f mat = _wadEditor_transform(wadEditor, keyframe, nextKeyframe, jointIndex + 1, keyframeInterpolationCoefficient, 0);
				translationMatrix = matrix4fMul(translationMatrix, mat);
			}
			
			currentTransform->mesh = jointData->mesh;
			currentTransform->skinJointMesh = jointData->lataSkinJointMesh;
			currentTransform->modelMatrix = matrix_multiply(parentTransform->modelMatrix, translationMatrix);
			data.modelView = matrix_multiply(viewMatrix, currentTransform->modelMatrix);
			meshUniformsBufferSetData(currentTransform->transformUniforms, &data);
		}
	}
	else if (wadEditor->selectedStatic)
	{
		matrix4f viewMatrix = matrix4fTranslation(0.0f, -0.7f, -1.7f);
		matrix4f rotationMatrix = matrix4fRotationY(rotation);
		data.modelView = matrix4fMul(viewMatrix, rotationMatrix);
		
		TRANSFORM_DATA* firstTranslation = &wadEditor->transforms[0];
		meshUniformsBufferSetData(firstTranslation->transformUniforms, &data);
	}
	
	wadEditor->lastUpdateTime = currentTime;
}

static void _wadEditor_commandBufferCompleted(COMMAND_BUFFER* commandBuffer, void* userInfo)
{
	GRAPHICS_DRAWABLE* drawable = userInfo;
	
	graphicsDrawableReturn(drawable);
	commandBufferRelease(commandBuffer);
}

static void _wadEditor_render(WAD_EDITOR* wadEditor)
{
	GRAPHICS_VIEW* graphicsView = wadEditor->outputView;
	GRAPHICS_DRAWABLE* drawable = graphicsViewRequestCurrentDrawable(graphicsView);
	if (!drawable) {
		return;
	}
	
	COMMAND_BUFFER* commandBuffer = commandQueueCreateCommandBuffer(wadEditor->commandQueue);
	if (!commandBuffer) {
		printf("Command buffer is NULL, skipping render frame.");
		return;
	}
	commandBufferAddCompletionHandler(commandBuffer, _wadEditor_commandBufferCompleted, drawable);
	
	graphicsDeviceScheduleUpdateBuffers(wadEditor->device, commandBuffer);
	
	RENDER_COMMAND_ENCODER* renderEncoder = commandBufferStartRenderCommandEncoder(commandBuffer, graphicsView);
	
	if (wadEditor->selectedTexture)
	{
		TRANSFORM_DATA* translation = &wadEditor->transforms[0];
		renderCommandEncoderScheduleDrawMesh(renderEncoder, wadEditor->selectedTexture->vertexBuffer, 6, wadEditor->selectedTexture->texture, translation->transformUniforms, wadEditor->viewportUniforms);
	}
	else
	{
		for (unsigned int i = 0; i < wadEditor->numTranslations; i++)
		{
			TRANSFORM_DATA* translation = &wadEditor->transforms[i];
			for (unsigned int j = 0; j < translation->mesh->submeshes.length; j++)
			{
				SUBMESH_DATA* submesh = magicArrayGetItem(&translation->mesh->submeshes, j);
				renderCommandEncoderScheduleDrawMesh(renderEncoder, submesh->vertexBuffer, submesh->numVertices, submesh->texture, translation->transformUniforms, wadEditor->viewportUniforms);
			}
			
			if (translation->skinJointMesh)
			{
				for (unsigned int j = 0; j < translation->skinJointMesh->submeshes.length; j++)
				{
					SUBMESH_DATA* submesh = magicArrayGetItem(&translation->skinJointMesh->submeshes, j);
					renderCommandEncoderScheduleDrawMesh(renderEncoder, submesh->vertexBuffer, submesh->numVertices, submesh->texture, translation->transformUniforms, wadEditor->viewportUniforms);
				}
			}
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
