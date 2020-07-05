//
//  resource_storage_interface.h
//  wad_editor_lib_macos
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef resource_storage_interface_h
#define resource_storage_interface_h

#include "wad_kit_link.h"
#include "resource_storage_functions.h"

typedef struct RESOURCE_STORAGE RESOURCE_STORAGE;

RESOURCE_STORAGE* resourceStorageCreate(RESOURCE_STORAGE_DELEGATE_BEGIN_UPLOAD_DATA_FUNC beginUploadDataFunc, RESOURCE_STORAGE_DELEGATE_FINISH_UPLOAD_DATA_FUNC finishUploadDataFunc, RESOURCE_STORAGE_DELEGATE_CREATE_TEXTURE_FUNC createTextureFunc, RESOURCE_STORAGE_DELEGATE_CREATE_MESH_FUNC createMeshFunc, void* userInfo);
void resourceStorageRelease(RESOURCE_STORAGE* resourceStorage);

void resourceStorageBeginUploadData(RESOURCE_STORAGE* resourceStorage);
void resourceStorageFinishUploadData(RESOURCE_STORAGE* resourceStorage);

void resourceStorageLoadTexturePage(RESOURCE_STORAGE* resourceStorage, TEXTURE_PAGE* texturePage, const char* name);
void resourceStorageLoadMesh(RESOURCE_STORAGE* resourceStorage, MESH* mesh);

#endif /* resource_storage_interface_h */
