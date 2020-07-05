//
//  WadKitView.m
//  WAD Editor
//
//  Created by Евгений Лютц on 26.05.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WadKitView.h"
#import "WKRenderer.h"
#import "WadKitViewDelegate.h"
#import "ResourceStorage.h"

@implementation WadKitView
{
	NSArray* rotatingViewTouches;
}


- (instancetype)initWithFrame:(NSRect)frame resourceStorage:(ResourceStorage*)resourceStorage
{
	self = [super initWithFrame:frame device:resourceStorage.device];
	if (self)
	{
		_resourceStorage = resourceStorage;
		_renderer = [[WKRenderer alloc] initWithResourceStorage:resourceStorage];
	}
	return self;
}

// MARK: - Properties

- (void)setViewport:(WAD_EDITOR_VIEWPORT*)viewport
{
	_viewport = viewport;
	wadEditorViewportConnectRenderer(_viewport, _renderer.renderer);
}

- (void)setResourceStorage:(ResourceStorage*)resourceStorage
{
	self.device = resourceStorage.device;
	_resourceStorage = resourceStorage;
	_renderer = [[WKRenderer alloc] initWithResourceStorage:resourceStorage];
}

// MARK: - Overloading stuff

- (void)setDevice:(id<MTLDevice>)device
{
	assert(device == _resourceStorage.device);
	[super setDevice:device];
}

- (void)setDelegate:(id<MTKViewDelegate>)delegate
{
	[super setDelegate:delegate];
	
	[delegate mtkView:self drawableSizeWillChange:self.frame.size];
	self.allowedTouchTypes = NSTouchTypeMaskDirect | NSTouchTypeMaskIndirect;
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

#pragma mark - Mouse events

- (void)_viewportMouseDown:(WE_MOUSE_KEY)key
{
	if (_viewport)
	{
		wadEditorViewportMouseDown(_viewport, key);
	}
}

- (void)_viewportMouseUp:(WE_MOUSE_KEY)key
{
	if (_viewport)
	{
		wadEditorViewportMouseUp(_viewport, key);
	}
}

- (void)_viewportMouseMove:(NSEvent*)event
{
	if (_viewport)
	{
		//wadEditorViewportMouseUp(_viewport, key);
	}
}

- (void)mouseDown:(NSEvent*)event {
	[self _viewportMouseDown:WE_MOUSE_KEY_LEFT];
}

- (void)otherMouseDown:(NSEvent*)event {
	[self _viewportMouseDown:WE_MOUSE_KEY_OTHER];
}

- (void)rightMouseDown:(NSEvent*)event {
	[self _viewportMouseDown:WE_MOUSE_KEY_RIGHT];
}


- (void)mouseUp:(NSEvent*)event {
	[self _viewportMouseUp:WE_MOUSE_KEY_LEFT];
}

- (void)otherMouseUp:(NSEvent*)event {
	[self _viewportMouseUp:WE_MOUSE_KEY_OTHER];
}

- (void)rightMouseUp:(NSEvent*)event {
	[self _viewportMouseUp:WE_MOUSE_KEY_RIGHT];
}


- (void)mouseDragged:(NSEvent*)event {
	[self _viewportMouseMove:event];
}

- (void)otherMouseDragged:(NSEvent*)event {
	[self _viewportMouseMove:event];
}

- (void)rightMouseDragged:(NSEvent*)event {
	[self _viewportMouseMove:event];
}


- (void)mouseMoved:(NSEvent*)event {
	[self _viewportMouseMove:event];
}

- (void)otherMouseMoved:(NSEvent*)event {
	[self _viewportMouseMove:event];
}

- (void)rightMouseMoved:(NSEvent*)event {
	[self _viewportMouseMove:event];
}

- (void)scrollWheel:(NSEvent*)event
{
	/*const float scrollX = event.scrollingDeltaX / 100.0f;
	const float scrollY = event.scrollingDeltaY / 100.0f;
	simd_float2 scroll = simd_make_float2(scrollY, scrollX);
	
	WadKitViewDelegate* wkDelegate = (WadKitViewDelegate*)self.delegate;
	[wkDelegate rotateView:scroll];*/
}

#pragma mark - Trackpad? events

- (void)magnifyWithEvent:(NSEvent*)event
{
	/*const float magnification = event.magnification;
	
	WadKitViewDelegate* wkDelegate = (WadKitViewDelegate*)self.delegate;
	[wkDelegate scaleView:-magnification];*/
}

- (void)rotateWithEvent:(NSEvent*)event
{
	[super rotateWithEvent:event];
}

// Touch events

- (void)touchesBeganWithEvent:(NSEvent*)event
{
	//[super touchesBeganWithEvent:event];
}

- (void)touchesMovedWithEvent:(NSEvent*)event
{
	NSSet<NSTouch*>* touchesSet = [event touchesMatchingPhase:NSTouchPhaseTouching inView:self];
	if (touchesSet.count == 2)
	{
		/*NSArray<NSTouch*>* touches = [touchesSet allObjects];
		
		for (NSTouch* touch in touches)
		{
			//
		}*/
	}
}

- (void)touchesEndedWithEvent:(NSEvent*)event
{
	//[super touchesEndedWithEvent:event];
}

- (void)touchesCancelledWithEvent:(NSEvent*)event
{
	//[super touchesCancelledWithEvent:event];
}

@end
