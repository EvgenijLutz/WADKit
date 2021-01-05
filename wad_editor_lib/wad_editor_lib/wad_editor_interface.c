//
//  wad_editor_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Viewport delegate implementations

static WAD_EDITOR* _wadEditorFromUserInfo(void* userInfo)
{
	return (WAD_EDITOR*)userInfo;
}

static void _viewportMouseDown(WE_MOUSE_KEY mouseKey, void* userInfo)
{
	//WAD_EDITOR* wadEditor = _wadEditorFromUserInfo(userInfo);
}

static void _viewportMouseUp(WE_MOUSE_KEY mouseKey, void* userInfo)
{
	//WAD_EDITOR* wadEditor = _wadEditorFromUserInfo(userInfo);
}

static void _viewportMouseMove(vector2f position, void* userInfo)
{
	//WAD_EDITOR* wadEditor = _wadEditorFromUserInfo(userInfo);
}

static void _viewportDrawMesh(RENDERER* renderer, void* userInfo)
{
	WAD_EDITOR* wadEditor = _wadEditorFromUserInfo(userInfo);
	if (!wadEditor->selectedItem)
	{
		return;
	}
	
	WAD_EDITOR_VIEWPORT* viewport = wadEditor->mainViewport;
	
	if (wadEditorItemGetType(wadEditor->selectedItem) == WAD_EDITOR_ITEM_TYPE_MESH)
	{
		MESH* mesh = wadEditorItemGetData(wadEditor->selectedItem).mesh;
		
		matrix4f model = matrix4f_identity;
		matrix4f view = wadEditorViewportGetViewMatrix(viewport);
		matrix4f projection = wadEditorViewportGetProjectionMatrix(viewport);
		
		rendererRenderMesh(renderer, mesh, &model, &view, &projection);
	}
}

// MARK: - Editor interface implementation

WAD_EDITOR* wadEditorCreate(RESOURCE_STORAGE* resourceStorage)
{
	assert(resourceStorage);
	
	WAD_EDITOR* wadEditor = malloc(sizeof(WAD_EDITOR));
	wadEditor->resourceStorage = resourceStorage;
	wadEditor->filePath = NULL;
	wadEditor->wad = wadCreate();
	
	WAD_EDITOR_ITEM_DATA itemData;
	itemData.texturePage = NULL;
	WAD_EDITOR_ITEM* root = wadEditorItemCreate(WAD_EDITOR_ITEM_TYPE_ROOT, itemData, "root");
	wadEditor->rootItem = root;
	wadEditor->texturePagesItem = wadEditorItemAddChild(root, WAD_EDITOR_ITEM_TYPE_SECTION, itemData, "Texture pages");
	wadEditor->meshesItem = wadEditorItemAddChild(root, WAD_EDITOR_ITEM_TYPE_SECTION, itemData, "Meshes");
	wadEditor->skeletonsItem = wadEditorItemAddChild(root, WAD_EDITOR_ITEM_TYPE_SECTION, itemData, "Skeletons");
	wadEditor->movablesItem = wadEditorItemAddChild(root, WAD_EDITOR_ITEM_TYPE_SECTION, itemData, "Movables");
	wadEditor->staticsItem = wadEditorItemAddChild(root, WAD_EDITOR_ITEM_TYPE_SECTION, itemData, "Statics");
	
	wadEditor->meshViewportDelegate = wadEditorViewportDelegateCreate(_viewportMouseDown, _viewportMouseUp, _viewportMouseMove, _viewportDrawMesh, wadEditor);
	wadEditor->mainViewport = wadEditorViewportCreate(wadEditor);
	
	return wadEditor;
}

void wadEditorRelease(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	
	wadEditorViewportRelease(wadEditor->mainViewport);
	wadEditorViewportDelegateRelease(wadEditor->meshViewportDelegate);
	
	wadEditorItemRelease(wadEditor->rootItem);
	
	if (wadEditor->wad)
	{
		wadRelease(wadEditor->wad);
	}
	
	free(wadEditor->filePath);
	free(wadEditor);
}

void wadEditorNewWad(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	
	assert(0);
}

void wadEditorLoadWad(WAD_EDITOR* wadEditor, const char* filePath)
{
	assert(wadEditor);
	assert(filePath);
	
	FILE* file = fopen(filePath, "rb");
	if (!file)
	{
		printf("Cannot read file\n");
		return;
	}
	
	fseek(file, 0, SEEK_END);
	long dataSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char* data = malloc(dataSize);
	fread(data, dataSize, 1, file);
	fclose(file);
	
	EXECUTE_RESULT executeResult;
	// TODO: implement
	assert(0);
	WK_WAD* wad = NULL;//wadLoadFromWadData(data, dataSize, &executeResult);
	if (!executeResult.succeeded)
	{
		printf("%s", executeResult.message);
		free(data);
		return;
	}
	
	if (wadEditor->wad)
	{
		wadRelease(wadEditor->wad);
	}
	wadEditor->wad = wad;
	
	free(data);
	
	resourceStorageBeginUploadData(wadEditor->resourceStorage);
	
	wadEditorItemRemoveAllChildren(wadEditor->texturePagesItem);
	int numItems = wadGetNumTexturePages(wad);
	WAD_EDITOR_ITEM_TYPE itemType = WAD_EDITOR_ITEM_TYPE_TEXTURE;
	WAD_EDITOR_ITEM_DATA itemData;
	char text[1024];
	for (unsigned int i = 0; i < numItems; i++)
	{
		itemData.texturePage = wadGetTexturePage(wad, i);
		
		sprintf(text, "Texture page #%03d", i);
		resourceStorageLoadTexturePage(wadEditor->resourceStorage, itemData.texturePage, text);
		wadEditorItemAddChild(wadEditor->texturePagesItem, itemType, itemData, text);
	}
	
	wadEditorItemRemoveAllChildren(wadEditor->meshesItem);
	numItems = wadGetNumMeshes(wad);
	itemType = WAD_EDITOR_ITEM_TYPE_MESH;
	for (unsigned int i = 0; i < numItems; i++)
	{
		itemData.mesh = wadGetMesh(wad, i);
		
		sprintf(text, "Mesh #%03d", i);
		resourceStorageLoadMesh(wadEditor->resourceStorage, itemData.mesh);
		wadEditorItemAddChild(wadEditor->meshesItem, itemType, itemData, text);
	}
	
	wadEditorItemRemoveAllChildren(wadEditor->skeletonsItem);
	numItems = 0;//wadGetNumSkeletons(wad);
	itemType = WAD_EDITOR_ITEM_TYPE_SKELETON;
//	for (unsigned int i = 0; i < numItems; i++)
//	{
//		SKELETON* skeleton = NULL;//wadGetSkeleton(wad, i);
//		itemData.skeleton = skeleton;
//		sprintf(text, "Skeleton #%03d", i);
//		WAD_EDITOR_ITEM* skeletonItem = wadEditorItemAddChild(wadEditor->skeletonsItem, itemType, itemData, text);
//
//		const unsigned int numSkeletonJoints = skeletonGetNumJoints(skeleton);
//		for (unsigned int jointIndex = 0; jointIndex < numSkeletonJoints; jointIndex++)
//		{
//			WAD_EDITOR_ITEM_TYPE jointItemType = WAD_EDITOR_ITEM_TYPE_SKELETON_JOINT;
//			WAD_EDITOR_ITEM_DATA jointItemData;
//			jointItemData.skeletonJoint.skeleton = skeleton;
//			jointItemData.skeletonJoint.joint = skeletonGetJoint(skeleton, jointIndex);
//
//			sprintf(text, "Joint #%03d", jointIndex);
//			wadEditorItemAddChild(skeletonItem, jointItemType, jointItemData, text);
//		}
//	}
	
	wadEditorItemRemoveAllChildren(wadEditor->movablesItem);
	numItems = wadGetNumMovables(wad);
	itemType = WAD_EDITOR_ITEM_TYPE_MOVABLE;
	for (unsigned int i = 0; i < numItems; i++)
	{
		// Add movable info record to list of movables
		MOVABLE* movable = wadGetMovableByIndex(wad, i);
		itemData.movable = movable;
		MOVABLE_ID movableId = movableGetId(itemData.movable);
		const char* movableName = movableIdGetName(movableId);
		WAD_EDITOR_ITEM* item = wadEditorItemAddChild(wadEditor->movablesItem, itemType, itemData, movableName);
		
		// Root mesh
		WAD_EDITOR_ITEM_TYPE movableItemType = WAD_EDITOR_ITEM_TYPE_MOVABLE_ROOT_MESH;
		WAD_EDITOR_ITEM_DATA movableItemData;
		movableItemData.movableRootMesh.movable = movable;
		movableItemData.movableRootMesh.movableMeshIndex = 0;
		//movableItemData.movableRootMesh.mesh = movableGetMesh(movable, 0, wad);
		wadEditorItemAddChild(item, movableItemType, movableItemData, "Root mesh");
		
		// Assigned skeleton
//		movableItemType = WAD_EDITOR_ITEM_TYPE_MOVABLE_SKELETON;
//		movableItemData.movableSkeleton.movable = movable;
//		const unsigned int movableSkeletonIndex = 0;//movableGetSkeletonIndex(movable);
//		SKELETON* skeleton = NULL;//wadGetSkeleton(wad, movableSkeletonIndex);
//		movableItemData.movableSkeleton.skeleton = skeleton;
//		WAD_EDITOR_ITEM* movableSkeletonItem = wadEditorItemAddChild(item, movableItemType, movableItemData, "Skeleton");
//		const unsigned int numSkeletonJoints = skeletonGetNumJoints(skeleton);
//		for (unsigned int jointIndex = 0; jointIndex < numSkeletonJoints; jointIndex++)
//		{
//			WAD_EDITOR_ITEM_TYPE jointItemType = WAD_EDITOR_ITEM_TYPE_MOVABLE_MESHES;
//			WAD_EDITOR_ITEM_DATA jointItemData;
//			jointItemData.movableSkeletonJoint.movable = movable;
//			jointItemData.movableSkeletonJoint.skeleton = skeleton;
//			jointItemData.movableSkeletonJoint.joint = skeletonGetJoint(skeleton, jointIndex);
//			
//			sprintf(text, "Joint #%03d", jointIndex);
//			wadEditorItemAddChild(movableSkeletonItem, jointItemType, jointItemData, text);
//		}
		
		// Meshes assigned to skeleton
		const unsigned int numMovableMeshes = 0;//movableGetNumMeshes(movable);
		if (numMovableMeshes > 0)
		{
			WAD_EDITOR_ITEM_TYPE itemType = WAD_EDITOR_ITEM_TYPE_MOVABLE_MESHES;
			WAD_EDITOR_ITEM_DATA itemData;
			itemData.movable = movable;
			WAD_EDITOR_ITEM* movableMeshsesItem = wadEditorItemAddChild(item, itemType, itemData, "Skeleton meshes");
			
			for (unsigned int j = 1; j < numMovableMeshes; j++)
			{
				WAD_EDITOR_ITEM_TYPE movableItemType = WAD_EDITOR_ITEM_TYPE_MOVABLE_MESHES;
				WAD_EDITOR_ITEM_DATA movableItemData;
				movableItemData.movableRootMesh.movable = movable;
				movableItemData.movableRootMesh.movableMeshIndex = j;
				//movableItemData.movableRootMesh.mesh = movableGetMesh(movable, j, wad);
				
				unsigned int meshIndex = 0;//movableGetMeshIndex(movable, j);
				sprintf(text, "Mesh #%03d", meshIndex);
				wadEditorItemAddChild(movableMeshsesItem, movableItemType, movableItemData, text);
			}
		}
		
		const unsigned int numMovableAnimations = movableGetNumAnimations(movable);
		if (numMovableAnimations > 0)
		{
			WAD_EDITOR_ITEM_TYPE itemType = WAD_EDITOR_ITEM_TYPE_MOVABLE_ANIMATIONS;
			WAD_EDITOR_ITEM_DATA itemData;
			itemData.movable = movable;
			WAD_EDITOR_ITEM* movableAnimationsItem = wadEditorItemAddChild(item, itemType, itemData, "Animations");
			
			for (unsigned int j = 0; j < numMovableAnimations; j++)
			{
				WAD_EDITOR_ITEM_TYPE movableAnimationItemType = WAD_EDITOR_ITEM_TYPE_MOVABLE_ANIMATION;
				WAD_EDITOR_ITEM_DATA movableAnimationItemData;
				movableAnimationItemData.movableAnimation.movable = movable;
				movableAnimationItemData.movableAnimation.animation = movableGetAnimation(movable, j);
				
				sprintf(text, "Animation %03d", j);
				wadEditorItemAddChild(movableAnimationsItem, movableAnimationItemType, movableAnimationItemData, text);
			}
		}
	}
	
	wadEditorItemRemoveAllChildren(wadEditor->staticsItem);
	numItems = wadGetNumStatics(wad);
	itemType = WAD_EDITOR_ITEM_TYPE_STATIC;
	for (unsigned int i = 0; i < numItems; i++)
	{
		itemData.staticObject = wadGetStaticByIndex(wad, i);
		STATIC_ID staticId = staticGetId(itemData.staticObject);
		const char* staticName = staticIdGetName(staticId);
		wadEditorItemAddChild(wadEditor->staticsItem, itemType, itemData, staticName);
	}
	
	resourceStorageFinishUploadData(wadEditor->resourceStorage);
}

void wadEditorUpdateState(WAD_EDITOR* wadEditor, double timeElapsed)
{
	//
}

// MARK: - UI abstraction interface

static const char* _sectionNames[] =
{
	"Textures",
	"Meshes",
	"Skeletons",
	"Movables",
	"Statics"
};

static const unsigned int _numSections = (unsigned int)(sizeof(_sectionNames) / sizeof(char*));

WAD_EDITOR_ITEM* wadEditorGetRootItem(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	return wadEditor->rootItem;
}

unsigned int wadEditorGetNumChildrenInItem(WAD_EDITOR* wadEditor, WAD_EDITOR_ITEM_TYPE type, unsigned int itemIndex)
{
	assert(wadEditor);
	assert(wadEditor->wad);
	
	if (type == WAD_EDITOR_ITEM_TYPE_ROOT)
	{
		assert(itemIndex == 0);
		return _numSections;
	}
	else if (type == WAD_EDITOR_ITEM_TYPE_SECTION)
	{
		switch (itemIndex)
		{
			case 0: return wadGetNumTexturePages(wadEditor->wad);
			case 1: return wadGetNumMeshes(wadEditor->wad);
			//case 2: return wadGetNumSkeletons(wadEditor->wad);
			case 3: return wadGetNumMovables(wadEditor->wad);
			case 4: return wadGetNumStatics(wadEditor->wad);
				
			default: break;
		}
	}
	else if (type == WAD_EDITOR_ITEM_TYPE_TEXTURE)
	{
		return 0;
	}
	
	return 0;
}

const char* wadEditorGetItemName(WAD_EDITOR* wadEditor, WAD_EDITOR_ITEM_TYPE type, unsigned int itemIndex)
{
	assert(wadEditor);
	assert(wadEditor->wad);
	
	if (type == WAD_EDITOR_ITEM_TYPE_ROOT)
	{
		return "WAD";
	}
	else if (type == WAD_EDITOR_ITEM_TYPE_SECTION)
	{
		assert(itemIndex < _numSections);
		return _sectionNames[itemIndex];
	}
	
	return "item";
}

void wadEditorSelectItem(WAD_EDITOR* editor, WAD_EDITOR_ITEM* item)
{
	assert(editor);
	assert(item);
	
	editor->selectedItem = item;
	if (wadEditorItemGetType(item) == WAD_EDITOR_ITEM_TYPE_MESH)
	{
		wadEditorViewportSetDelegate(editor->mainViewport, editor->meshViewportDelegate);
	}
	else
	{
		wadEditorViewportSetDelegate(editor->mainViewport, NULL);
	}
}

// MARK: - Properties

WK_WAD* wadEditorGetWad(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	return wadEditor->wad;
}

WAD_EDITOR_VIEWPORT* wadEditorGetMainViewport(WAD_EDITOR* wadEditor)
{
	assert(wadEditor);
	return wadEditor->mainViewport;
}
