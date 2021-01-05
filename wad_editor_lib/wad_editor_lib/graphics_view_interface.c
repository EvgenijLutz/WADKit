//
//  graphics_view_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

GRAPHICS_VIEW* graphicsViewCreate(GRAPHICS_VIEW_GET_CURRENT_DRAWABLE_FUNC getCurrentDrawableFunc, GRAPHICS_DRAWABLE_RETURN_FUNC returnDrawableFunc, GRAPHICS_DRAWABLE_GET_TEXTURE_FUNC drawableGetTextureFunc, GRAPHICS_DRAWABLE_RETURN_TEXTURE_FUNC drawableReturnTexture, void* userInfo, float width, float height, GRAPHICS_DEVICE* graphicsDevice)
{
	assert(graphicsDevice);
	
	GRAPHICS_VIEW* view = malloc(sizeof(GRAPHICS_VIEW));
	view->getCurrentDrawableFunc = getCurrentDrawableFunc;
	view->returnDrawableFunc = returnDrawableFunc;
	view->drawableGetTextureFunc = drawableGetTextureFunc;
	view->drawableReturnTexture = drawableReturnTexture;
	view->width = width;
	view->height = height;
	view->userInfo = userInfo;
	view->device = graphicsDevice;
	arrayInitialize(&view->drawables, sizeof(GRAPHICS_DRAWABLE));
	
	return view;
}

void graphicsViewRelease(GRAPHICS_VIEW* graphicsView)
{
	assert(graphicsView);
	
	assert(graphicsView->drawables.length == 0);
	arrayDeinitialize(&graphicsView->drawables);
	
	debug_memset(graphicsView, 0, sizeof(GRAPHICS_VIEW));
	free(graphicsView);
}


void graphicsViewSetSize(GRAPHICS_VIEW* graphicsView, float width, float height)
{
	assert(graphicsView);
	graphicsView->width = width;
	graphicsView->height = height;
	
	//
}

void graphicsViewDraw(GRAPHICS_VIEW* graphicsView)
{
	//
}


GRAPHICS_DEVICE* graphicsViewGetDevice(GRAPHICS_VIEW* graphicsView)
{
	assert(graphicsView);
	return graphicsView->device;
}

GRAPHICS_DRAWABLE* graphicsViewRequestCurrentDrawable(GRAPHICS_VIEW* graphicsView)
{
	assert(graphicsView);
	
	void* drawableId = graphicsView->getCurrentDrawableFunc(graphicsView);
	if (!drawableId)
	{
		return NULL;
	}
	
	GRAPHICS_DRAWABLE* drawable = arrayAddItem(&graphicsView->drawables);
	drawable->graphicsView = graphicsView;
	drawable->drawableId = drawableId;
	drawable->texture = NULL;
	
	return drawable;
}
