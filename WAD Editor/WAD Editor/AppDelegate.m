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

static void _commandBuffer_completion(COMMAND_BUFFER* commandBuffer, void* userInfo)
{
	NSLog(@"Completion handler called");
}

@interface AppDelegate ()
@property (weak) IBOutlet MainWindow* window;
@end

@implementation AppDelegate
{
	WK_SYSTEM* _system;
	GRAPHICS_DEVICE* _graphicsDevice;
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
	
	_graphicsDevice = graphicsDeviceCreateDefaultMetalDevice();
	if (!_graphicsDevice)
	{
		[self _showErrorAndDieWithMessage:@"Cannot initialize graphics device to render WAD data 😢"];
		return;
	}
	
	// Some test code, remove in future
	
	EXECUTE_RESULT executeResult;
	WK_WAD* wad = wadCreateFromContentsOfResourceFile(_system, "tut1", &executeResult);
	if (executeResultIsFailed(&executeResult))
	{
		systemRelease(_system);
		NSString* message = [NSString stringWithFormat:@"Could not load WAD file: %s", executeResult.message];
		[self _showErrorAndDieWithMessage:message];
		return;
	}
	
	if (wadGetNumTexturePages(wad) > 2)
	{
		TEXTURE_PAGE* page = wadGetTexturePage(wad, 2);
		const void* data = texturePageGetData(page);
		TEXTURE2D* texture = graphicsDeviceCreateTexture2d(_graphicsDevice, WK_TEXTURE_PAGE_WIDTH, WK_TEXTURE_PAGE_HEIGHT, WK_TEXTURE_PAGE_NUM_COMPONENTS, TEXTURE_USAGE_SHADER_READ, data);
		
		texture2dRelease(texture);
		texture = NULL;
	}
	
	const unsigned long numElements = 1024;
	const unsigned long size = sizeof(vector3f) * numElements;
	
	GRAPHICS_BUFFER* sourceBuffer = graphicsDeviceCreateBuffer(_graphicsDevice, size, GRAPHICS_BUFFER_OPTION_CPU_READ_WRITE);
	vector3f* data = graphicsBufferGetDataToCPUWrite(sourceBuffer);
	for (unsigned long i = 0; i < numElements; i++)
	{
		data[i].x = (float)numElements * 0.005 * sinf((float)i / (float)numElements);
		data[i].y = (float)numElements * 0.005 * cosf((float)i / (float)numElements);
		data[i].z = sinf(data[i].x) * cosf(data[i].y);
	}
	
	GRAPHICS_BUFFER* destinationBuffer = graphicsDeviceCreateBuffer(_graphicsDevice, size, GRAPHICS_BUFFER_OPTION_GPU_ONLY);
	
	COMMAND_QUEUE* commandQueue = graphicsDeviceCreateCommandQueue(_graphicsDevice);
	
	COMMAND_BUFFER* commandBuffer = commandQueueCreateCommandBuffer(commandQueue);
	commandBufferAddCompletionHandler(commandBuffer, _commandBuffer_completion, commandBuffer);
	
	
	BLIT_COMMAND_ENCODER* blitCommandEncoder = commandBufferStartBlitCommandEncoder(commandBuffer);
	blitCommandEncoderScheduleCopyFromBufferToBuffer(blitCommandEncoder, sourceBuffer, 0, destinationBuffer, 0, size);
	blitCommandEncoderEndEncoding(blitCommandEncoder);
	
	
	commandBufferCommit(commandBuffer);
	NSLog(@"Scheduled");
	commandBufferWaitUntilCompleted(commandBuffer);
	NSLog(@"Finished");
	commandBufferRelease(commandBuffer);
	
	commandQueueRelease(commandQueue);
	
	graphicsBufferRelease(destinationBuffer);
	graphicsBufferRelease(sourceBuffer);
	
	wadRelease(wad);
	
	
	
	[_window initializeInterface];
}

+ (WK_SYSTEM*)system
{
	AppDelegate* appDelegate = (AppDelegate*)NSApplication.sharedApplication.delegate;
	return appDelegate->_system;
}

+ (GRAPHICS_DEVICE*)graphicsDevice
{
	AppDelegate* appDelegate = (AppDelegate*)NSApplication.sharedApplication.delegate;
	return appDelegate->_graphicsDevice;
}


- (void)applicationWillTerminate:(NSNotification*)aNotification
{
	NSLog(@"Bye 👋");
	
	if (_graphicsDevice)
	{
		graphicsDeviceReleaseDefaultMetalDevice(_graphicsDevice);
	}
	
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
