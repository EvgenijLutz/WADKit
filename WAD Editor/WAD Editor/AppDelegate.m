//
//  AppDelegate.m
//  WAD Editor
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#import "AppDelegate.h"
#import "MainWindow.h"
@import Metal;

@interface AppDelegate ()
@property (weak) IBOutlet MainWindow* window;
@end

@implementation AppDelegate
{
	WK_SYSTEM* _system;
	id<MTLDevice> _device;
}

- (void)_showErrorAndDieWithMessage:(NSString*)message
{
	NSAlert* alert = [[NSAlert alloc] init];
	alert.messageText = message;
	[alert beginSheetModalForWindow:_window completionHandler:^(NSModalResponse returnCode) {
		[NSApplication.sharedApplication terminate:self];
	}];
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification
{
	_system = systemCreateDefaultIfAvailable();
	if (!_system)
	{
		[self _showErrorAndDieWithMessage:@"Cannot initialize WAD Kit's essential components to work 😢"];
		return;
	}
	
	EXECUTE_RESULT executeResult;
	GRAPHICS_DEVICE* device = graphicsDeviceCreateDefault();
	WK_WAD* wad = wadCreateFromContentsOfResourceFile(_system, "tut1", &executeResult);
	if (executeResultIsFailed(&executeResult)) {
		systemRelease(_system);
		NSString* message = [NSString stringWithFormat:@"Could not load WAD file: %s", executeResult.message];
		[self _showErrorAndDieWithMessage:message];
		return;
	}
	
	if (wadGetNumTexturePages(wad) > 1)
	{
		TEXTURE_PAGE* page = wadGetTexturePage(wad, 1);
		const void* data = texturePageGetData(page);
		TEXTURE2D* texture = graphicsDeviceCreateTexture2d(device, WK_TEXTURE_PAGE_WIDTH, WK_TEXTURE_PAGE_HEIGHT, WK_TEXTURE_PAGE_NUM_COMPONENTS, data);
		
		texture2dRelease(texture);
	}
	
	wadRelease(wad);
	graphicsDeviceRelease(device);
	
	_device = MTLCreateSystemDefaultDevice();
	[_window initializeWithMetalDevice:_device];
}

+ (WK_SYSTEM*)system
{
	AppDelegate* appDelegate = (AppDelegate*)NSApplication.sharedApplication.delegate;
	return appDelegate->_system;
}


- (void)applicationWillTerminate:(NSNotification*)aNotification
{
	NSLog(@"Bye 👋");
	
	if (_system) {
		systemRelease(_system);
	}
	
	_device = nil;
}


-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
	return YES;
}


@end
