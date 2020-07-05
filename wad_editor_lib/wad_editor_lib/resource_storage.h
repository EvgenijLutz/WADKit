//
//  resource_storage.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__resource_storage_h
#define wad_editor_lib__resource_storage_h

#include "resource_storage_functions.h"

typedef struct RESOURCE_STORAGE
{
	RESOURCE_STORAGE_DELEGATE_BEGIN_UPLOAD_DATA_FUNC beginUploadDataFunc;
	RESOURCE_STORAGE_DELEGATE_FINISH_UPLOAD_DATA_FUNC finishUploadDataFunc;
	RESOURCE_STORAGE_DELEGATE_CREATE_TEXTURE_FUNC createTextureFunc;
	RESOURCE_STORAGE_DELEGATE_CREATE_MESH_FUNC createMeshFunc;
	
	void* userInfo;
}
RESOURCE_STORAGE;

#endif /* wad_editor_lib__resource_storage_h */
