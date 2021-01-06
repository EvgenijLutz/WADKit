//
//  graphics_view_interface.h
//  graphics
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_view_interface_h
#define graphics_view_interface_h

#include "graphics.h"

GRAPHICS_VIEW* graphicsViewCreate(GRAPHICS_VIEW_GET_CURRENT_DRAWABLE_FUNC getCurrentDrawableFunc, GRAPHICS_DRAWABLE_RETURN_FUNC returnDrawableFunc, GRAPHICS_DRAWABLE_GET_TEXTURE_FUNC drawableGetTextureFunc, GRAPHICS_DRAWABLE_RETURN_TEXTURE_FUNC drawableReturnTexture, void* userInfo, float width, float height, GRAPHICS_DEVICE* graphicsDevice);
void graphicsViewRelease(GRAPHICS_VIEW* graphicsView);

void graphicsViewSetSize(GRAPHICS_VIEW* graphicsView, float width, float height);
void graphicsViewDraw(GRAPHICS_VIEW* graphicsView);

void graphicsViewSubscribe(GRAPHICS_VIEW* graphicsView, GRAPHICS_VIEW_SUBSCRIBER* subscriber);

GRAPHICS_DEVICE* graphicsViewGetDevice(GRAPHICS_VIEW* graphicsView);
/// Result can be @b null if there is no currently available drawable to render to. You should return the drawable ( function @b graphicsDrawableReturn ) when you will not need it, typically when rendering to it is complete.
GRAPHICS_DRAWABLE* graphicsViewRequestCurrentDrawable(GRAPHICS_VIEW* graphicsView);

#endif /* graphics_view_interface_h */
