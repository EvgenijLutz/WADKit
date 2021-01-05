//
//  GraphicsView.m
//  wad_editor_lib
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#import "GraphicsView.h"
#import "private_interface.h"

// MARK: - Private interface - GRAPHICS_VIEW callbacks

static void* _metalGraphicsView_getCurrentDrawable(GRAPHICS_VIEW* graphicsView)
{
	GraphicsView* view = (__bridge GraphicsView*)graphicsView->userInfo;
	id<CAMetalDrawable> drawable = view.currentDrawable;
	if (!drawable)
	{
		return NULL;
	}
	
	return (__bridge_retained void*)drawable;
}

static void _metalGraphicsView_returnDrawable(GRAPHICS_DRAWABLE* graphicsDrawable)
{
	id<CAMetalDrawable> drawable = (__bridge_transfer id<CAMetalDrawable>)graphicsDrawable->drawableId;
	drawable = nil;
}

static void* _metalGraphicsView_getTexture(GRAPHICS_DRAWABLE* graphicsDrawable)
{
	id<CAMetalDrawable> drawable = (__bridge id<CAMetalDrawable>)graphicsDrawable->drawableId;
	id<MTLTexture> texture = drawable.texture;
	void* textureId = (__bridge_retained void*)texture;
	return textureId;
}

static void _metalGraphicsView_returnTexture(GRAPHICS_DRAWABLE* graphicsDrawable, TEXTURE2D* texture)
{
	id<MTLTexture> metalTexture = (__bridge_transfer id<MTLTexture>)texture->textureId;
	metalTexture = nil;
}


// MARK: - GraphicsView interface

@implementation GraphicsView
{
	id<MTLDevice> _metalDevice;
}

- (instancetype)initWithFrame:(NSRect)frame graphicsDevice:(GRAPHICS_DEVICE*)graphicsDevice
{
	self = [super init];
	if (self)
	{
		_metalDevice = (__bridge id<MTLDevice>)graphicsDevice->userInfo;
		_graphicsDevice = graphicsDevice;
		_graphicsView = graphicsViewCreate(_metalGraphicsView_getCurrentDrawable, _metalGraphicsView_returnDrawable, _metalGraphicsView_getTexture, _metalGraphicsView_returnTexture, (__bridge void*)self, (float)frame.size.width, (float)frame.size.height, graphicsDevice);
		
		self.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
		self.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
		self.colorspace = CGColorSpaceCreateWithName(kCGColorSpaceDisplayP3);
		self.clearColor = MTLClearColorMake(0.1, 0.1, 0.1, 1.0);
		self.delegate = self;
	}
	return self;
}

- (void)dealloc
{
	graphicsViewRelease(_graphicsView);
}


- (void)mtkView:(MTKView*)view drawableSizeWillChange:(CGSize)size
{
	graphicsViewSetSize(_graphicsView, (float)size.width, (float)size.height);
}

- (void)drawInMTKView:(MTKView*)view
{
	graphicsViewDraw(_graphicsView);
}

@end
