//
//  MainWindow.m
//  WAD Editor
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#import "MainWindow.h"
#import "ResourceReflectionStorage.h"
#import "WKEditor.h"
#import "WadKitViewDelegate.h"
#import "WadKitView.h"
#include "wad_kit_link.h"

@interface MainWindow()
@property (weak) IBOutlet WadKitView* wadKitView;
@end

@implementation MainWindow
{
	id<MTLDevice> device;
	WKEditor* editor;
	WadKitViewDelegate* wkViewDelegate;
}

- (void)initializeWithMetalDevice:(id<MTLDevice>)metalDevice
{
	device = metalDevice;
	
	if (!device)
	{
		NSArray<__kindof NSView*>* subviews = self.contentView.subviews.copy;
		for (NSView* view in subviews)
		{
			[view removeFromSuperview];
		}
		
		NSTextField* text = [[NSTextField alloc] init];
		text.font = [NSFont systemFontOfSize:32.0f weight:NSFontWeightBold];
		text.stringValue = @"Metal is not available\n on this device";
		text.alignment = NSTextAlignmentCenter;
		text.bezeled = NO;
		text.selectable = NO;
		text.editable = NO;
		text.drawsBackground = NO;
		
		[self.contentView addSubview:text];
		text.translatesAutoresizingMaskIntoConstraints = NO;
		[text.centerXAnchor constraintEqualToAnchor:self.contentView.centerXAnchor].active = YES;
		[text.centerYAnchor constraintEqualToAnchor:self.contentView.centerYAnchor].active = YES;
		
		[text.topAnchor constraintGreaterThanOrEqualToAnchor:self.contentView.topAnchor].active = YES;
		[text.bottomAnchor constraintLessThanOrEqualToAnchor:self.contentView.bottomAnchor].active = YES;
		
		[text.leadingAnchor constraintGreaterThanOrEqualToAnchor:self.contentView.leadingAnchor].active = YES;
		[text.trailingAnchor constraintLessThanOrEqualToAnchor:self.contentView.trailingAnchor].active = YES;
		
		return;
	}
	
	editor = [[WKEditor alloc] initWithMetalDevice:device];
	
	wkViewDelegate = [[WadKitViewDelegate alloc] initWithViewport:editor];
	
	WadKitView* wkView = _wadKitView;
	wkView.device = device;
	wkView.delegate = wkViewDelegate;
	
	wkView.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
	wkView.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
	wkView.colorspace = CGColorSpaceCreateWithName(kCGColorSpaceDisplayP3);
	/*if (self.screen)
	{
		wkView.colorspace = self.screen.colorSpace.CGColorSpace;
		NSLog(@"%@", self.screen.colorSpace);
	}*/
	
	NSString* path = [NSBundle.mainBundle pathForResource:@"tut1" ofType:@"WAD"];
	[editor loadWadByPath:path];
}

- (IBAction)loadTestData:(id)sender
{
	NSString* path = [NSBundle.mainBundle pathForResource:@"tut1" ofType:@"WAD"];
	[editor loadWadByPath:path];
	
	// 3. Display something
	[_wadKitView draw];
}

@end
