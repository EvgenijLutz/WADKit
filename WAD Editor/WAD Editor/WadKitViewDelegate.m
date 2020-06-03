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
	id<WKViewport> viewport;
}

- (instancetype)initWithViewport:(id<WKViewport>)wkViewport
{
	self = [super init];
	if (self)
	{
		viewport = wkViewport;
	}
	return self;
}

- (void)mtkView:(nonnull MTKView*)view drawableSizeWillChange:(CGSize)size
{
	simd_float2 viewportSize = simd_make_float2(size.width, size.height);
	[viewport sizeChanged:viewportSize];
}

- (void)drawInMTKView:(nonnull MTKView*)view
{
	WadKitView* wkView = (WadKitView*)view;
	WKRenderer* renderer = wkView.renderer;
	
	id<MTLCommandBuffer> commandBuffer = [renderer.drawCommandQueue commandBuffer];
	[commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull buffer) {
		//
	}];
	
	MTLRenderPassDescriptor* renderPassDescriptor = wkView.currentRenderPassDescriptor;
	if (renderPassDescriptor)
	{
		id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
	
		renderer.renderEncoder = renderEncoder;
		[viewport drawWithRenderer:renderer];
		
		[renderEncoder endEncoding];
		[commandBuffer presentDrawable:view.currentDrawable];
	}
	
	[commandBuffer commit];
}

@end
