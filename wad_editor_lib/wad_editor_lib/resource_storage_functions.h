//
//  resource_storage_delegate_functions.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef resource_storage_delegate_functions_h
#define resource_storage_delegate_functions_h

#include "wad_editor_lib.h"

typedef void (* RESOURCE_STORAGE_DELEGATE_BEGIN_UPLOAD_DATA_FUNC)(void* userInfo);
typedef void (* RESOURCE_STORAGE_DELEGATE_FINISH_UPLOAD_DATA_FUNC)(void* userInfo);
typedef void (* RESOURCE_STORAGE_DELEGATE_CREATE_TEXTURE_FUNC)(TEXTURE_PAGE* texturePage, const char* name, void* userInfo);
typedef void (* RESOURCE_STORAGE_DELEGATE_CREATE_MESH_FUNC)(MESH* mesh, void* userInfo);

#endif /* resource_storage_delegate_functions_h */
