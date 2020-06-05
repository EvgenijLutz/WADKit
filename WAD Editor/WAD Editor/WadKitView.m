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

@implementation WadKitView
{
	NSArray* rotatingViewTouches;
}

- (void)setDevice:(id<MTLDevice>)device
{
	[super setDevice:device];
	
	_renderer = [[WKRenderer alloc] initWithMetalDevice:device];
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

- (void)otherMouseDragged:(NSEvent*)event
{
	[super otherMouseDragged:event];
}

- (void)scrollWheel:(NSEvent*)event
{
	const float scrollX = event.scrollingDeltaX / 100.0f;
	const float scrollY = event.scrollingDeltaY / 100.0f;
	simd_float2 scroll = simd_make_float2(scrollY, scrollX);
	
	WadKitViewDelegate* wkDelegate = (WadKitViewDelegate*)self.delegate;
	[wkDelegate rotateView:scroll];
}

#pragma mark - Trackpad? events

- (void)magnifyWithEvent:(NSEvent*)event
{
	const float magnification = event.magnification;
	
	WadKitViewDelegate* wkDelegate = (WadKitViewDelegate*)self.delegate;
	[wkDelegate scaleView:-magnification];
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
