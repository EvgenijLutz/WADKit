//
//  resource_storage_interface.c
//  wad_editor_lib_macos
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

RESOURCE_STORAGE* resourceStorageCreate(RESOURCE_STORAGE_DELEGATE_BEGIN_UPLOAD_DATA_FUNC beginUploadDataFunc, RESOURCE_STORAGE_DELEGATE_FINISH_UPLOAD_DATA_FUNC finishUploadDataFunc, RESOURCE_STORAGE_DELEGATE_CREATE_TEXTURE_FUNC createTextureFunc, RESOURCE_STORAGE_DELEGATE_CREATE_MESH_FUNC createMeshFunc, void* userInfo)
{
	assert(beginUploadDataFunc);
	assert(finishUploadDataFunc);
	assert(createTextureFunc);
	assert(createMeshFunc);
	
	RESOURCE_STORAGE* resourceStorage = malloc(sizeof(RESOURCE_STORAGE));
	resourceStorage->beginUploadDataFunc = beginUploadDataFunc;
	resourceStorage->finishUploadDataFunc = finishUploadDataFunc;
	resourceStorage->createTextureFunc = createTextureFunc;
	resourceStorage->createMeshFunc = createMeshFunc;
	resourceStorage->userInfo = userInfo;
	
	return resourceStorage;
}

void resourceStorageRelease(RESOURCE_STORAGE* resourceStorage)
{
	assert(resourceStorage);
	free(resourceStorage);
}

void resourceStorageBeginUploadData(RESOURCE_STORAGE* resourceStorage)
{
	assert(resourceStorage);
	resourceStorage->beginUploadDataFunc(resourceStorage->userInfo);
}

void resourceStorageFinishUploadData(RESOURCE_STORAGE* resourceStorage)
{
	assert(resourceStorage);
	resourceStorage->finishUploadDataFunc(resourceStorage->userInfo);
}

void resourceStorageLoadTexturePage(RESOURCE_STORAGE* resourceStorage, TEXTURE_PAGE* texturePage, const char* name)
{
	assert(resourceStorage);
	assert(texturePage);
	resourceStorage->createTextureFunc(texturePage, name, resourceStorage->userInfo);
}

void resourceStorageLoadMesh(RESOURCE_STORAGE* resourceStorage, MESH* mesh)
{
	assert(resourceStorage);
	assert(mesh);
	resourceStorage->createMeshFunc(mesh, resourceStorage->userInfo);
}
