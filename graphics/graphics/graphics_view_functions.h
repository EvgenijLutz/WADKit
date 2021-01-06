//
//  graphics_view_functions.h
//  graphics
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_view_functions_h
#define graphics_view_functions_h

#include "graphics.h"

typedef void* (* GRAPHICS_VIEW_GET_CURRENT_DRAWABLE_FUNC)(GRAPHICS_VIEW* graphicsView);
typedef void (* GRAPHICS_DRAWABLE_RETURN_FUNC)(GRAPHICS_DRAWABLE* graphicsDrawable);

typedef void* (* GRAPHICS_DRAWABLE_GET_TEXTURE_FUNC)(GRAPHICS_DRAWABLE* graphicsDrawable);
typedef void (* GRAPHICS_DRAWABLE_RETURN_TEXTURE_FUNC)(GRAPHICS_DRAWABLE* graphicsDrawable, TEXTURE2D* texture);


typedef void (* GRAPHICS_VIEW_SIZE_CHANGED_FUNC)(GRAPHICS_VIEW* graphicsView, float width, float height, void* userInfo);
typedef void (* GRAPHICS_VIEW_UPDATE_FUNC)(GRAPHICS_VIEW* graphicsView, void* userInfo);
typedef struct GRAPHICS_VIEW_SUBSCRIBER
{
	GRAPHICS_VIEW_SIZE_CHANGED_FUNC sizeChangedFunc;
	GRAPHICS_VIEW_UPDATE_FUNC updateFunc;
	void* userInfo;
}
GRAPHICS_VIEW_SUBSCRIBER;

#endif /* graphics_view_functions_h */
