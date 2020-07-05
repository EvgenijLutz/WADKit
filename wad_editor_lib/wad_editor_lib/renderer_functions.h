//
//  renderer_functions.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef renderer_functions_h
#define renderer_functions_h

typedef struct RENDERER RENDERER;

typedef void (* RENDERER_DELEGATE_DRAW_MESH_FUNC)(MESH* mesh, matrix4f* model, matrix4f* view, matrix4f* projection, void* userInfo);
typedef void (* RENDERER_DELEGATE_DRAW_DUMMY_CUBE_FUNC)(matrix4f* model, matrix4f* view, matrix4f* projection, void* userInfo);

#endif /* renderer_functions_h */
