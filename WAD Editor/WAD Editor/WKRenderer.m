//
//  WKRenderer.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WKRenderer.h"

@implementation WKRenderer
{
	__weak id<MTLDevice> device;
	
	id<MTLRenderPipelineState> drawSubmeshPipelineState;
}

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice
{
	self = [super init];
	if (self)
	{
		device = metalDevice;
		
		_drawCommandQueue = [metalDevice newCommandQueue];
		
		
		// Initialize pipeline states
	}
	return self;
}

@end
