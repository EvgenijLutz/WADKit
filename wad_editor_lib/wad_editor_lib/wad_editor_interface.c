//
//  wad_editor_interface.c
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
	TEXTURE_PAGE* page;
	TEXTURE2D* texture;
	GRAPHICS_MESH* mesh;
}
TEXTURE_DATA;

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

typedef struct MOVABLE_JOINT_DATA
{
	JOINT* joint;
	MESH_DATA* mesh;
}
MOVABLE_JOINT_DATA;

typedef struct MOVABLE_DATA
{
	MOVABLE* movable;
	MESH_DATA* rootMesh;
	WK_ARRAY jointedMeshes;
}
MOVABLE_DATA;

typedef struct ANIMATION_DATA
{
	MOVABLE_DATA* movable;
	ANIMATION* animation;
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
	}
	else if (item->flags == LI_FLAG_MOVABLE)
	{
		editor->selectedMovable = item->data;
		editor->numTranslations = 0;
		
		MOVABLE* movable = editor->selectedMovable->movable;
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

static TEXTURE2D* _wadEditor_getTextureForTexturePage(WAD_EDITOR* editor, TEXTURE_PAGE* page)
{
	for (unsigned int i = 0; i < editor->textures.length; i++)
	{
		TEXTURE_DATA* link = arrayGetItem(&editor->textures, i);
		if (link->page == page)
		{
			return link->texture;
		}
	}
	
	// This should never happen
	assert(0);
	return NULL;
}


static MESH_DATA* _wadEditor_getMeshDataForMesh(WAD_EDITOR* editor, MESH* mesh)
{
	for (unsigned int i = 0; i < editor->meshes.length; i++)
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
}


// MARK: - WAD editor interface

WAD_EDITOR* wadEditorCreate(WK_SYSTEM* system, WE_LIST* wadContentsList, GRAPHICS_VIEW* outputView)
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
	
	arrayInitialize(&wadEditor->textures, sizeof(TEXTURE_DATA));
	
	wadEditor->submeshAllocator = dataAllocatorCreate(sizeof(SUBMESH_DATA), 256);
	arrayInitializeWithCapacityIncrement(&wadEditor->meshes, sizeof(MESH_DATA), 128);
	
	wadEditor->movableJointsAllocator = dataAllocatorCreate(sizeof(MOVABLE_JOINT_DATA), 256);
	arrayInitializeWithCapacityIncrement(&wadEditor->movables, sizeof(MOVABLE_DATA), 16);
	
	arrayInitializeWithCapacityIncrement(&wadEditor->statics, sizeof(STATIC_DATA), 16);
	
	arrayInitializeWithCapacityIncrement(&wadEditor->animations, sizeof(ANIMATION_DATA), 128);
	
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
		translation->modelMatrix = matrix4f_identity;
		translation->uniforms = graphicsDeviceCreateMeshUniforms(wadEditor->device);
	}
	
	wadEditor->selectedAnimation = NULL;
	wadEditor->animationTime = 0.0f;
	
	wadEditor->useInterpolation = 1;
	wadEditor->useQuaternionRotations = 1;
	
	return wadEditor;
}

void wadEditorRelease(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	
	for (unsigned int i = 0; i < wadEditor->translationsCapacity; i++)
	{
		TRANSFORM_DATA* translation = &wadEditor->transforms[i];
		graphicsMeshUniformsRelease(translation->uniforms);
	}
	free(wadEditor->transforms);
	
	assert(wadEditor->statics.length == 0);
	arrayDeinitialize(&wadEditor->statics);
	
	assert(wadEditor->animations.length == 0);
	arrayDeinitialize(&wadEditor->animations);
	
	assert(wadEditor->meshes.length == 0);
	arrayDeinitialize(&wadEditor->meshes);
	dataAllocatorRelease(wadEditor->submeshAllocator);
	
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
		editor->selectedTexture = NULL;
		editor->selectedMesh = NULL;
		editor->selectedMovable = NULL;
		editor->selectedStatic = NULL;
		editor->selectedAnimation = NULL;
		
		editor->numTranslations = 0;
		
		// Clear static
		arrayClear(&editor->statics);
		
		// Clear animations
		arrayClear(&editor->animations);
		
		// Clear movables
		for (unsigned int i = 0; i < editor->movables.length; i++)
		{
			MOVABLE_DATA* movableData = arrayGetItem(&editor->movables, i);
			arrayDeinitialize(&movableData->jointedMeshes);
		}
		arrayClear(&editor->movables);
		
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
			TEXTURE_DATA* link = arrayGetItem(&editor->textures, i);
			graphicsMeshRelease(link->mesh);
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
		
		GRAPHICS_VERTEX vertices[] = {
			{ .position=vector3fCreate(-0.5, -0.5, 0.0), .uv=vector2fCreate(0.0, 1.0), .weights={ 1.0, 0.0 } },
			{ .position=vector3fCreate( 0.5, -0.5, 0.0), .uv=vector2fCreate(1.0, 1.0), .weights={ 1.0, 0.0 } },
			{ .position=vector3fCreate( 0.5,  0.5, 0.0), .uv=vector2fCreate(1.0, 0.0), .weights={ 1.0, 0.0 } },
			
			{ .position=vector3fCreate( 0.5,  0.5, 0.0), .uv=vector2fCreate(1.0, 0.0), .weights={ 1.0, 0.0 } },
			{ .position=vector3fCreate(-0.5,  0.5, 0.0), .uv=vector2fCreate(0.0, 0.0), .weights={ 1.0, 0.0 } },
			{ .position=vector3fCreate(-0.5, -0.5, 0.0), .uv=vector2fCreate(0.0, 1.0), .weights={ 1.0, 0.0 } }
		};
		
		TEXTURE_DATA* textureLink = arrayAddItem(&editor->textures);
		textureLink->page = texturePage;
		textureLink->texture = texture;
		textureLink->mesh = graphicsDeviceCreateMesh(editor->device, vertices, 6, texture);
		
		sprintf(title, "Texture page #%u", i);
		listItemAddChild(item, LI_FLAG_TEXTURE, textureLink, title);
	}
	
	// Initialize meshes
	item = listItemAddChild(rootItem, 0, NULL, "Meshes");
	length = wadGetNumMeshes(editor->wad);
	
	typedef struct TEMP_SUBMESH
	{
		TEXTURE_PAGE* page;
		TEXTURE2D* texture;
		unsigned int numVertices;
		unsigned int verticesCapacity;
		GRAPHICS_VERTEX* vertices;
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
		tempSubmesh->vertices = malloc(sizeof(GRAPHICS_VERTEX) * tempSubmesh->verticesCapacity);
	}
	
	for (unsigned int meshIndex = 0; meshIndex < length; meshIndex++)
	{
		MESH* mesh = wadGetMesh(editor->wad, meshIndex);
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
			POLYGON* polygon = meshGetPolygon(mesh, polygonIndex);
			TEXTURE_SAMPLE* textureSample = polygonGetTextureSample(polygon);
			
			// Find submesh, to which current textureSample belongs
			TEXTURE_PAGE* texturePage = textureSampleGetTexturePage(textureSample);
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
					currentTempSubmesh->vertices = malloc(sizeof(GRAPHICS_VERTEX) * currentTempSubmesh->verticesCapacity);
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
				tempSubmesh->vertices = realloc(tempSubmesh->vertices, sizeof(GRAPHICS_VERTEX) * tempSubmesh->verticesCapacity);
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
			
			VERTEX* v1 = polygonGetVertex1(polygon);
			VERTEX* v2 = polygonGetVertex2(polygon);
			VERTEX* v3 = polygonGetVertex3(polygon);
						
			tempSubmesh->vertices[tempSubmesh->numVertices + 0].position = vertexGetPosition(v1);
			tempSubmesh->vertices[tempSubmesh->numVertices + 0].uv = uv1;
			
			tempSubmesh->vertices[tempSubmesh->numVertices + 1].position = vertexGetPosition(v3);
			tempSubmesh->vertices[tempSubmesh->numVertices + 1].uv = uv3;
			
			tempSubmesh->vertices[tempSubmesh->numVertices + 2].position = vertexGetPosition(v2);
			tempSubmesh->vertices[tempSubmesh->numVertices + 2].uv = uv2;
			
			tempSubmesh->numVertices += 3;
			
			if (!polygonIsTriangle(polygon))
			{
				VERTEX* v4 = polygonGetVertex4(polygon);
				
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
		
		MESH_DATA* meshData = arrayAddItem(&editor->meshes);
		meshData->mesh = mesh;
		arrayInitializeWithAllocator(&meshData->submeshes, editor->submeshAllocator);
		
		for (unsigned int k = 0; k < numTempSubmeshes; k++)
		{
			TEMP_SUBMESH* currentTempSubmesh = &tempSubmeshes[k];
			assert(currentTempSubmesh->numVertices);
			for (unsigned int l = 0; l < currentTempSubmesh->numVertices; l++)
			{
				currentTempSubmesh->vertices[l].weights[0] = 1.0f;
				currentTempSubmesh->vertices[l].weights[1] = 0.0f;
			}
			
			SUBMESH_DATA* submesh = arrayAddItem(&meshData->submeshes);
			submesh->page = currentTempSubmesh->page;
			submesh->texture = currentTempSubmesh->texture;
			submesh->mesh = graphicsDeviceCreateMesh(editor->device, currentTempSubmesh->vertices, currentTempSubmesh->numVertices, currentTempSubmesh->texture);
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
	MESH* nullMesh = NULL;
	if (wadGetNumMeshes(editor->wad))
	{
		nullMesh = wadGetMesh(editor->wad, 0);
	}
	
	// Initialize movables
	item = listItemAddChild(rootItem, 0, NULL, "Movables");
	length = wadGetNumMovables(editor->wad);
	for (unsigned int i = 0; i < length; i++)
	{
		MOVABLE* movable = wadGetMovableByIndex(editor->wad, i);
		MOVABLE_ID movableId = movableGetId(movable);
		const char* movableName = movableIdGetTerribleName(movableId);
		MESH_DATA* rootMeshData = _wadEditor_getMeshDataForMesh(editor, movableGetRootMesh(movable));
		if (rootMeshData->mesh == nullMesh)
		{
			MOVABLE* laraSkinMovable = wadGetMovableById(editor->wad, MOVABLE_ID_LARA_SKIN);
			if (laraSkinMovable)
			{
				MESH* laraSkinRootMesh = movableGetRootMesh(laraSkinMovable);
				rootMeshData = _wadEditor_getMeshDataForMesh(editor, laraSkinRootMesh);
			}
		}
		
		MOVABLE_DATA* movableData = arrayAddItem(&editor->movables);
		movableData->movable = movable;
		movableData->rootMesh = rootMeshData;
		arrayInitializeWithAllocator(&movableData->jointedMeshes, editor->movableJointsAllocator);
		
		WE_LIST_ITEM* movableItem = listItemAddChild(item, LI_FLAG_MOVABLE, movableData, movableName);
		
		/*WE_LIST_ITEM* rootMeshItem =*/ listItemAddChild(movableItem, 0, NULL, "Root mesh");
		WE_LIST_ITEM* jointsItem = listItemAddChild(movableItem, 0, NULL, "Jointed meshes");
		unsigned int numJoints = movableGetNumJoints(movable);
		for (unsigned int j = 0; j < numJoints; j++)
		{
			JOINT* joint = movableGetJoint(movable, j);
			MESH* mesh = jointGetMesh(joint);
			if (mesh == nullMesh)
			{
				MOVABLE* laraSkinMovable = wadGetMovableById(editor->wad, MOVABLE_ID_LARA_SKIN);
				if (laraSkinMovable)
				{
					unsigned int fixedJointIndex = j;
					if (fixedJointIndex >= movableGetNumJoints(laraSkinMovable))
					{
						fixedJointIndex = 0;
					}
					
					JOINT* laraSkinJoint = movableGetJoint(laraSkinMovable, fixedJointIndex);
					mesh = jointGetMesh(laraSkinJoint);
				}
			}
			
			MOVABLE_JOINT_DATA* jointData = arrayAddItem(&movableData->jointedMeshes);
			jointData->joint = joint;
			jointData->mesh = _wadEditor_getMeshDataForMesh(editor, mesh);
			
			sprintf(title, "Jointed mesh #%u", j);
			listItemAddChild(jointsItem, 0, NULL, title);
		}
		
		WE_LIST_ITEM* animationsItem = listItemAddChild(movableItem, 0, NULL, "Animations");
		unsigned int numAnimations = movableGetNumAnimations(movable);
		for (unsigned int j = 0; j < numAnimations; j++)
		{
			ANIMATION* animation = movableGetAnimation(movable, j);
			ANIMATION_DATA* animationData = arrayAddItem(&editor->animations);
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
		MESH* staticMesh = staticGetMesh(staticObject);
		
		STATIC_DATA* staticData = arrayAddItem(&editor->statics);
		staticData->staticObject = staticObject;
		staticData->mesh = _wadEditor_getMeshDataForMesh(editor, staticMesh);
		
		listItemAddChild(item, LI_FLAG_STATIC, staticData, staticName);
	}
	
	semaphoreLeave(editor->accessSemaphore);
}

static matrix4f _wadEditor_transform(WAD_EDITOR* wadEditor, KEYFRAME* firstKeyframe, KEYFRAME* secondKeyframe, int componentIndex, float keyframeInterpolationCoefficient, int calculateOffset)
{
	matrix4f rotMat = matrix4f_identity;
	if (wadEditor->useInterpolation)
	{
		if (wadEditor->useQuaternionRotations)
		{
			quaternionf rotQuat1 = keyframeGetRotationQuaternion(firstKeyframe, componentIndex);
			quaternionf rotQuat2 = keyframeGetRotationQuaternion(secondKeyframe, componentIndex);
			quaternionf rotQuat = simd_slerp(rotQuat1, rotQuat2, keyframeInterpolationCoefficient);
			rotMat = matrix4fFromQuaternion(rotQuat);
		}
		else
		{
			vector3f rotation = keyframeCalculateInterpolatedRotation(firstKeyframe, secondKeyframe, componentIndex, keyframeInterpolationCoefficient);
			matrix4f rotZMat = matrix4fRotation(rotation.z, vector3fCreate(0.0f, 0.0f, 1.0f));
			matrix4f rotXMat = matrix4fRotation(rotation.x, vector3fCreate(1.0f, 0.0f, 0.0f));
			matrix4f rotYMat = matrix4fRotation(rotation.y, vector3fCreate(0.0f, 1.0f, 0.0f));
			matrix4f rotMat = matrix4fMul(rotXMat, rotZMat);
			rotMat = matrix4fMul(rotYMat, rotMat);
		}
	}
	else
	{
		if (wadEditor->useQuaternionRotations)
		{
			quaternionf rotQuat = keyframeGetRotationQuaternion(firstKeyframe, componentIndex);
			rotMat = matrix4fFromQuaternion(rotQuat);
		}
		else
		{
			vector3f rotation = keyframeCalculateRotation(firstKeyframe, componentIndex);
			matrix4f rotZMat = matrix4fRotation(rotation.z, vector3fCreate(0.0f, 0.0f, 1.0f));
			matrix4f rotXMat = matrix4fRotation(rotation.x, vector3fCreate(1.0f, 0.0f, 0.0f));
			matrix4f rotYMat = matrix4fRotation(rotation.y, vector3fCreate(0.0f, 1.0f, 0.0f));
			matrix4f rotMat = matrix4fMul(rotXMat, rotZMat);
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
	
	GRAPHICS_MESH_UNIFORMS_DATA data;
	data.projection = matrix4fPerspectiveRightHand(fovyRadians, aspect, 0.01f, 100.0f);
	data.ambient = vector3fCreate(1.0f, 1.0f, 1.0f);
	if (wadEditor->selectedTexture)
	{
		data.modelView[0] = matrix4fMul(matrix4fTranslation(0.0f, 0.0f, -0.8f), matrix4f_identity);
		data.modelView[1] = matrix4fMul(matrix4fTranslation(0.0f, 0.0f, -0.8f), matrix4f_identity);
		
		TRANSFORM_DATA* firstTranslation = &wadEditor->transforms[0];
		graphicsMeshUniformsSetData(firstTranslation->uniforms, wadEditor->commandQueue, &data);
	}
	else if (wadEditor->selectedMesh)
	{
		matrix4f viewMatrix = matrix4fTranslation(0.0f, 0.0f, -0.3f);
		matrix4f rotationMatrix = matrix4fRotation(rotation, vector3fCreate(0.0f, 1.0f, 0.0f));
		data.modelView[0] = matrix4fMul(viewMatrix, rotationMatrix);
		data.modelView[1] = matrix4fMul(viewMatrix, rotationMatrix);
		
		TRANSFORM_DATA* firstTranslation = &wadEditor->transforms[0];
		graphicsMeshUniformsSetData(firstTranslation->uniforms, wadEditor->commandQueue, &data);
	}
	else if (wadEditor->selectedMovable)
	{
		// Calculate animation
		KEYFRAME* keyframe = NULL;
		KEYFRAME* nextKeyframe = NULL;
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
		
		matrix4f viewRotationMatrix = matrix4fRotation(rotation, vector3fCreate(0.0f, 1.0f, 0.0f));
		//viewRotationMatrix = matrix4f_identity;
		//viewRotationMatrix = matrix4fRotation(M_PI_2, vector3fCreate(0.0f, 1.0f, 0.0f));
		matrix4f viewTranslationMatrix = matrix4fTranslation(0.0f, 0.0f, -0.8f);
		viewTranslationMatrix = matrix4fTranslation(0.0f, -0.4f, -0.8f);
		matrix4f viewMatrix = matrix4fMul(viewTranslationMatrix, viewRotationMatrix);
		
		TRANSFORM_DATA* transforms = wadEditor->transforms;
		TRANSFORM_DATA* firstTranslation = &transforms[0];
		firstTranslation->mesh = wadEditor->selectedMovable->rootMesh;
		firstTranslation->stackParentIndex = 0;
		firstTranslation->modelMatrix = matrix4f_identity;
		if (keyframe)
		{
			firstTranslation->modelMatrix = _wadEditor_transform(wadEditor, keyframe, nextKeyframe, 0, keyframeInterpolationCoefficient, 1);
		}
		data.modelView[0] = matrix_multiply(viewMatrix, firstTranslation->modelMatrix);
		data.modelView[1] = data.modelView[0];
		graphicsMeshUniformsSetData(firstTranslation->uniforms, wadEditor->commandQueue, &data);
		
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
			
			MOVABLE_JOINT_DATA* jointData = arrayGetItem(&selectedMovable->jointedMeshes, jointIndex);
			JOINT* joint = jointData->joint;
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
			currentTransform->modelMatrix = matrix_multiply(parentTransform->modelMatrix, translationMatrix);
			data.modelView[0] = matrix_multiply(viewMatrix, currentTransform->modelMatrix);
			data.modelView[1] = data.modelView[0];
			
			graphicsMeshUniformsSetData(currentTransform->uniforms, wadEditor->commandQueue, &data);
		}
	}
	else if (wadEditor->selectedStatic)
	{
		matrix4f viewMatrix = matrix4fTranslation(0.0f, -0.7f, -1.7f);
		matrix4f rotationMatrix = matrix4fRotation(rotation, vector3fCreate(0.0f, 1.0f, 0.0f));
		data.modelView[0] = matrix4fMul(viewMatrix, rotationMatrix);
		data.modelView[1] = matrix4fMul(viewMatrix, rotationMatrix);
		
		TRANSFORM_DATA* firstTranslation = &wadEditor->transforms[0];
		graphicsMeshUniformsSetData(firstTranslation->uniforms, wadEditor->commandQueue, &data);
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
	if (!commandBuffer) {
		printf("Command buffer is NULL, skipping render frame.");
		return;
	}
	commandBufferAddCompletionHandler(commandBuffer, _wadEditor_commandBufferCompleted, drawable);
	
	RENDER_COMMAND_ENCODER* renderEncoder = commandBufferStartRenderCommandEncoder(commandBuffer, graphicsView);
	
	if (wadEditor->selectedTexture)
	{
		TRANSFORM_DATA* translation = &wadEditor->transforms[0];
		renderCommandEncoderScheduleDrawMesh(renderEncoder, wadEditor->selectedTexture->mesh, translation->uniforms);
	}
	else
	{
		for (unsigned int i = 0; i < wadEditor->numTranslations; i++)
		{
			TRANSFORM_DATA* translation = &wadEditor->transforms[i];
			for (unsigned int j = 0; j < translation->mesh->submeshes.length; j++)
			{
				SUBMESH_DATA* submesh = arrayGetItem(&translation->mesh->submeshes, j);
				renderCommandEncoderScheduleDrawMesh(renderEncoder, submesh->mesh, translation->uniforms);
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
