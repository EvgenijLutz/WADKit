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
#import "WKOutlineCellView.h"
#import "WKOutlineItem.h"
#import "WKOutlineDataSource.h"
#include "wad_kit_link.h"

#define SECTION_INDEX_TEXTURE_PAGE	(0)
#define SECTION_INDEX_MESH			(1)

@interface MainWindow()
@property (weak) IBOutlet NSOutlineView* outlineView;
@property (weak) IBOutlet WadKitView* wadKitView;
@end

@implementation MainWindow
{
	id<MTLDevice> device;
	WKEditor* editor;
	WadKitViewDelegate* wkViewDelegate;
	
	NSArray<NSString*>* outlineViewSections;
	WKOutlineDataSource* outlineDataSource;
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
	editor.delegate = self;
	
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
	
	outlineViewSections = [NSArray arrayWithObjects:@"Texture pages", @"Meshes", @"Skeletons", @"Movables", @"Statics", nil];
	outlineDataSource = [[WKOutlineDataSource alloc] init];
	
	_outlineView.dataSource = self;
	_outlineView.delegate = self;
	
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

- (void)nextObject:(id)sender
{
	[editor selectNextObject];
}

- (void)previousObject:(id)sender
{
	[editor selectPreviousObject];
}

#pragma mark - Wad Kit editor delegate implementation

- (void)editor:(WKEditor*)editor wadLoaded:(WAD*)wad
{
	[outlineDataSource clear];
	
	// Texture pages
	unsigned int numItems = wadGetNumTexturePages(editor.wad);
	for (unsigned int i = 0; i < numItems; i++)
	{
		[outlineDataSource createItemWithSectionIndex:0 itemIndex:i name:[NSString stringWithFormat:@"Texture page #%d", i]];
	}
	
	// Meshes
	numItems = wadGetNumMeshes(editor.wad);
	for (unsigned int i = 0; i < numItems; i++)
	{
		[outlineDataSource createItemWithSectionIndex:1 itemIndex:i name:[NSString stringWithFormat:@"Mesh #%d", i]];
	}
	
	// Skeletons
	numItems = wadGetNumSkeletons(editor.wad);
	for (unsigned int i = 0; i < numItems; i++)
	{
		[outlineDataSource createItemWithSectionIndex:2 itemIndex:i name:[NSString stringWithFormat:@"Skeleton #%d", i]];
	}
	
	// Movables
	numItems = wadGetNumMovables(editor.wad);
	for (unsigned int i = 0; i < numItems; i++)
	{
		// TODO: give them their own names
		[outlineDataSource createItemWithSectionIndex:3 itemIndex:i name:[NSString stringWithFormat:@"Movable #%d", i]];
	}
	
	// Statics
	numItems = wadGetNumStatics(editor.wad);
	for (unsigned int i = 0; i < numItems; i++)
	{
		// TODO: give them their own names
		[outlineDataSource createItemWithSectionIndex:4 itemIndex:i name:[NSString stringWithFormat:@"Static #%d", i]];
	}
	
	[_outlineView reloadData];
}

#pragma mark - Outline view data source and delegate implementation

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
{
	if (item == nil)
	{
		return outlineViewSections.count;
	}
	
	if ([outlineViewSections containsObject:item])
	{
		NSUInteger childIndex = [outlineViewSections indexOfObject:item];
		switch (childIndex) {
			case SECTION_INDEX_TEXTURE_PAGE: return wadGetNumTexturePages(editor.wad);
			case SECTION_INDEX_MESH: return wadGetNumMeshes(editor.wad);
			case 2: return wadGetNumSkeletons(editor.wad);
			case 3: return wadGetNumMovables(editor.wad);
			case 4: return wadGetNumStatics(editor.wad);
			default: return 0;
		}
		
		// This should never happen!
		return 10;
	}
	
	return 0;
}

- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item
{
	if (item == nil)
	{
		return YES;
	}
	
	return [outlineViewSections containsObject:item];
}

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item
{
	NSUserInterfaceItemIdentifier cellId = @"wk_cell";
	WKOutlineCellView* view = [outlineView makeViewWithIdentifier:cellId owner:nil];
	if (view == nil)
	{
		view = [[WKOutlineCellView alloc] init];
		view.identifier = cellId;
	}
	else
	{
		NSLog(@"Yay!");
	}
	
	if ([item isKindOfClass:WKOutlineItem.class])
	{
		WKOutlineItem* itemData = (WKOutlineItem*)item;
		view.customText.stringValue = itemData.name;
	}
	else
	{
		view.customText.stringValue = item;
	}
	
	return view;
}

- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item
{
	if (item == nil)
	{
		return outlineViewSections[index];
	}
	
	if ([outlineViewSections containsObject:item])
	{
		unsigned int sectionIndex = (unsigned int)[outlineViewSections indexOfObject:item];
		unsigned int itemIndex = (unsigned int)index;
		return [outlineDataSource itemAtSectionIndex:sectionIndex itemIndex:itemIndex];
	}
	
	// This should never happen!
	//return [NSString stringWithFormat:@"%@ child #%ld", item, index];
	return nil;
}

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
	// 🤨
	return item;
}

- (void)outlineViewSelectionDidChange:(NSNotification *)notification
{
	NSInteger selectedRowIndex = _outlineView.selectedRow;
	id outlineItem = [_outlineView itemAtRow:selectedRowIndex];
	
	if ([outlineItem isKindOfClass:WKOutlineItem.class])
	{
		WKOutlineItem* item = (WKOutlineItem*)outlineItem;
		if (item.sectionIndex == SECTION_INDEX_MESH)
		{
			[editor selectMeshAtIndex:item.itemIndex];
		}
	}
	
}

@end
