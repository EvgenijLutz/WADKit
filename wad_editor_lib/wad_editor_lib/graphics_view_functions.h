//
//  graphics_view_functions.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_view_functions_h
#define graphics_view_functions_h

#include "wad_editor_lib.h"

typedef void* (* GRAPHICS_VIEW_GET_CURRENT_DRAWABLE_FUNC)(GRAPHICS_VIEW* graphicsView);
typedef void (* GRAPHICS_DRAWABLE_RETURN_FUNC)(GRAPHICS_DRAWABLE* graphicsDrawable);

typedef void* (* GRAPHICS_DRAWABLE_GET_TEXTURE_FUNC)(GRAPHICS_DRAWABLE* graphicsDrawable);
typedef void (* GRAPHICS_DRAWABLE_RETURN_TEXTURE_FUNC)(GRAPHICS_DRAWABLE* graphicsDrawable, TEXTURE2D* texture);

#endif /* graphics_view_functions_h */
