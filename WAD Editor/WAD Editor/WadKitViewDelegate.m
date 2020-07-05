//
//  WadKitViewDelegate.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WadKitViewDelegate.h"
#import "WadKitView.h"
#import "WKViewport.h"
#import "WKRenderer.h"

@implementation WadKitViewDelegate
{
	//
}

- (void)mtkView:(nonnull MTKView*)view drawableSizeWillChange:(CGSize)size
{
	WadKitView* wkView = (WadKitView*)view;
	WAD_EDITOR_VIEWPORT* viewport = wkView.viewport;
	
	vector2f viewportSize = vector2fCreate(size.width, size.height);
	wadEditorViewportSetSize(viewport, viewportSize);
}

- (void)drawInMTKView:(nonnull MTKView*)view
{
	WadKitView* wkView = (WadKitView*)view;
	WKRenderer* renderer = wkView.renderer;
	WAD_EDITOR_VIEWPORT* viewport = wkView.viewport;
	
	id<MTLCommandBuffer> commandBuffer = [renderer.drawCommandQueue commandBuffer];
	[commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull buffer) {
		//
	}];
	
	MTLRenderPassDescriptor* renderPassDescriptor = wkView.currentRenderPassDescriptor;
	if (renderPassDescriptor)
	{
		id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
	
		renderer.renderEncoder = renderEncoder;
		//[viewport drawWithRenderer:renderer];
		
		wadEditorViewportDraw(viewport);
		
		[renderEncoder endEncoding];
		[commandBuffer presentDrawable:view.currentDrawable];
	}
	
	[commandBuffer commit];
}

@end
