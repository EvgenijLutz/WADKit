//
//  graphics_view.h
//  graphics
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_view_h
#define graphics_view_h

#include "private_interface.h"

typedef struct GRAPHICS_VIEW
{
	GRAPHICS_VIEW_GET_CURRENT_DRAWABLE_FUNC getCurrentDrawableFunc;
	GRAPHICS_DRAWABLE_RETURN_FUNC returnDrawableFunc;
	
	GRAPHICS_DRAWABLE_GET_TEXTURE_FUNC drawableGetTextureFunc;
	GRAPHICS_DRAWABLE_RETURN_TEXTURE_FUNC drawableReturnTexture;
	
	void* userInfo;
	
	float width;
	float height;
	
	GRAPHICS_DEVICE* device;
	
	/// Array of @b GRAPHICS_DRAWABLE objects.
	WK_ARRAY drawables;
	
	// TODO: INPUT_RESPONDER
	
	int hasSubscriber;
	GRAPHICS_VIEW_SUBSCRIBER subscriber;
}
GRAPHICS_VIEW;

#endif /* graphics_view_h */
