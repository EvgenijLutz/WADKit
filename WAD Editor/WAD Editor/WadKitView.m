//
//  WadKitView.m
//  WAD Editor
//
//  Created by Евгений Лютц on 26.05.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WadKitView.h"
#import "WKRenderer.h"

@implementation WadKitView

- (void)setDevice:(id<MTLDevice>)device
{
	[super setDevice:device];
	
	_renderer = [[WKRenderer alloc] initWithMetalDevice:device];
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

@end
