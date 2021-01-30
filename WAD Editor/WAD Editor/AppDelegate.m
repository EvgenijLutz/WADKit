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
	SYSTEM* _system;
	GraphicsDevice* _graphicsDevice;
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
	
	id<MTLDevice> metalDevice = MTLCreateSystemDefaultDevice();
	if (!metalDevice)
	{
		[self _showErrorAndDieWithMessage:@"Cannot initialize graphics device to render WAD data 😢"];
		return;
	}
	_graphicsDevice = [[GraphicsDevice alloc] initWithSystem:_system metalDevice:metalDevice];
	
	[_window initializeInterface];
}

+ (SYSTEM*)system
{
	AppDelegate* appDelegate = (AppDelegate*)NSApplication.sharedApplication.delegate;
	return appDelegate->_system;
}

+ (GraphicsDevice*)graphicsDevice
{
	AppDelegate* appDelegate = (AppDelegate*)NSApplication.sharedApplication.delegate;
	return appDelegate->_graphicsDevice;
}


- (void)applicationWillTerminate:(NSNotification*)aNotification
{
	NSLog(@"Bye 👋");
	
	_graphicsDevice = nil;
	
	if (_system)
	{
		systemRelease(_system);
	}
}


-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
	return YES;
}


@end
