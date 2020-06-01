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
	id<MTLDevice> device;
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification
{
	// Insert code here to initialize your application
	
	device = MTLCreateSystemDefaultDevice();
	[_window initializeWithMetalDevice:device];
}


- (void)applicationWillTerminate:(NSNotification*)aNotification
{
	// Insert code here to tear down your application
}


-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender
{
	return YES;
}


@end
