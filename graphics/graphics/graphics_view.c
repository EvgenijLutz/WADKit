//
//  graphics_view.c
//  graphics
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
	magicArrayInitialize(&view->drawables, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(GRAPHICS_DRAWABLE), 8);
	view->hasSubscriber = 0;
	debug_memset(&view->subscriber, 0, sizeof(GRAPHICS_VIEW_SUBSCRIBER));
	
	return view;
}

void graphicsViewRelease(GRAPHICS_VIEW* graphicsView)
{
	assert(graphicsView);
	
	assert(graphicsView->drawables.length == 0);
	magicArrayDeinitialize(&graphicsView->drawables);
	
	debug_memset(graphicsView, 0, sizeof(GRAPHICS_VIEW));
	free(graphicsView);
}


void graphicsViewSetSize(GRAPHICS_VIEW* graphicsView, float width, float height)
{
	assert(graphicsView);
	graphicsView->width = width;
	graphicsView->height = height;
	
	if (graphicsView->hasSubscriber)
	{
		graphicsView->subscriber.sizeChangedFunc(graphicsView, width, height, graphicsView->subscriber.userInfo);
	}
}

void graphicsViewDraw(GRAPHICS_VIEW* graphicsView)
{
	if (graphicsView->hasSubscriber)
	{
		graphicsView->subscriber.updateFunc(graphicsView, graphicsView->subscriber.userInfo);
	}
}


void graphicsViewSubscribe(GRAPHICS_VIEW* graphicsView, GRAPHICS_VIEW_SUBSCRIBER* subscriber)
{
	assert(graphicsView);
	assert(subscriber);
	
	graphicsView->hasSubscriber = 1;
	graphicsView->subscriber = *subscriber;
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
	
	GRAPHICS_DRAWABLE* drawable = magicArrayAddItem(&graphicsView->drawables);
	drawable->graphicsView = graphicsView;
	drawable->drawableId = drawableId;
	drawable->texture = NULL;
	
	return drawable;
}


float graphicsViewGetWidth(GRAPHICS_VIEW* graphicsView)
{
	assert(graphicsView);
	return graphicsView->width;
}

float graphicsViewGetHeight(GRAPHICS_VIEW* graphicsView)
{
	assert(graphicsView);
	return graphicsView->height;
}
