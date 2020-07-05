//
//  MainWindow.m
//  WAD Editor
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#import "MainWindow.h"
#import "ResourceStorage.h"
#import "WKEditor.h"
#import "WadKitViewDelegate.h"
#import "WadKitView.h"
#import "WKOutlineCellView.h"
#import "WKOutlineItem.h"
#import "WKOutlineDataSource.h"


#import "WadItemsDataSource.h"
#include "wad_editor_lib_link.h"

#define SECTION_INDEX_TEXTURE_PAGE	(0)
#define SECTION_INDEX_MESH			(1)
#define SECTION_INDEX_MOVABLE		(3)
#define SECTION_INDEX_STATIC		(4)


#define MAIN_SPLIT_VIEW_ITEM_0_MIN_WIDTH	(100.0f)
#define MAIN_SPLIT_VIEW_ITEM_0_MAX_WIDTH	(230.0f)

#define MAIN_SPLIT_VIEW_ITEM_1_MIN_WIDTH	(50.0f)

#define MAIN_SPLIT_VIEW_ITEM_2_MIN_WIDTH	(150.0f)
#define MAIN_SPLIT_VIEW_ITEM_2_MAX_WIDTH	(250.0f)


#define CENTER_SPLIT_VIEW_ITEM_0_MIN_HEIGHT	(100.0f)

#define CENTER_SPLIT_VIEW_ITEM_1_MIN_HEIGHT	(150.0f)
#define CENTER_SPLIT_VIEW_ITEM_1_MAX_HEIGHT	(250.0f)

#define LEFT_TOOLBAR_WIDTH	(40.0f)

#define MAIN_SPLIT_VIEW_MIN_WIDTH (MAIN_SPLIT_VIEW_ITEM_0_MAX_WIDTH + LEFT_TOOLBAR_WIDTH + MAIN_SPLIT_VIEW_ITEM_1_MIN_WIDTH + MAIN_SPLIT_VIEW_ITEM_2_MAX_WIDTH)


@interface MainWindow()
@property (weak) IBOutlet NSOutlineView* outlineView;
@property (weak) IBOutlet WadKitView* wadKitView;
@end

@implementation MainWindow
{
	id<MTLDevice> device;
	//WKEditor* editor;
	WadKitViewDelegate* wkViewDelegate;
	
	NSArray<NSString*>* outlineViewSections;
	WKOutlineDataSource* outlineDataSource;
	
	
	ResourceStorage* resourceStorage;
	
	// New data
	WAD_EDITOR* wadEditor;
	
	NSSplitView* mainSplitView;
	
	
	// Left sidebar
	NSView* leftSidebarView;
	WadItemsDataSource* wadItemsDataSource;
	NSOutlineView* outline;
	
	
	// Center view
	NSSplitView* centerSplitView;
	
	// Top center view
	NSSplitView* topCenterSplitView;
	NSView* topCenterLeftSidebarView;
	WadKitView* wadKitViewportView;
	
	// Bottom center view
	NSView* bottomCenterView;
	
	
	// Right sidebar
	NSView* rightSidebarView;
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
	
	resourceStorage = [[ResourceStorage alloc] initWithMetalDevice:metalDevice];
	
	wadEditor = wadEditorCreate(resourceStorage.resourceStorage);
	
	[self _setupDynamicUserInterface];
	
	NSString* path = [NSBundle.mainBundle pathForResource:@"tut1" ofType:@"WAD"];
	const char* filePath = path.UTF8String;
	wadEditorLoadWad(wadEditor, filePath);
	//[outline reloadData];
}

- (void)dealloc
{
	wadEditorRelease(wadEditor);
}

- (void)nextObject:(id)sender
{
	/*
	[editor selectNextObject];
	*/
}

- (void)previousObject:(id)sender
{
	/*
	[editor selectPreviousObject];
	*/
}

#pragma mark - Wad Kit editor delegate implementation

- (void)editor:(WKEditor*)editor wadLoaded:(WAD*)wad
{
	[outlineDataSource clear];
	
	// Texture pages
	unsigned int numTexturePages = wadGetNumTexturePages(editor.wad);
	for (unsigned int i = 0; i < numTexturePages; i++)
	{
		[outlineDataSource createItemWithSectionIndex:SECTION_INDEX_TEXTURE_PAGE itemIndex:i name:[NSString stringWithFormat:@"Texture page #%d", i]];
	}
	
	// Meshes
	unsigned int numMeshes = wadGetNumMeshes(editor.wad);
	for (unsigned int i = 0; i < numMeshes; i++)
	{
		[outlineDataSource createItemWithSectionIndex:SECTION_INDEX_MESH itemIndex:i name:[NSString stringWithFormat:@"Mesh #%d", i]];
	}
	
	// Skeletons
	unsigned numItems = wadGetNumSkeletons(editor.wad);
	for (unsigned int i = 0; i < numItems; i++)
	{
		[outlineDataSource createItemWithSectionIndex:2 itemIndex:i name:[NSString stringWithFormat:@"Skeleton #%d", i]];
	}
	
	// Movables
	numItems = wadGetNumMovables(editor.wad);
	for (unsigned int movableIndex = 0; movableIndex < numItems; movableIndex++)
	{
		MOVABLE* movable = wadGetMovableByIndex(wad, movableIndex);
		MOVABLE_ID movableId = movableGetId(movable);
		const char* movableName = movableIdGetName(movableId);
		NSString* movableNameString = [NSString stringWithUTF8String:movableName];
		
		[outlineDataSource createItemWithSectionIndex:SECTION_INDEX_MOVABLE itemIndex:movableIndex name:movableNameString];
	}
	
	// Statics
	numItems = wadGetNumStatics(editor.wad);
	for (unsigned int staticIndex = 0; staticIndex < numItems; staticIndex++)
	{
		STATIC* staticObject = wadGetStaticByIndex(wad, staticIndex);
		STATIC_ID staticId = staticGetId(staticObject);
		const char* staticName = staticIdGetName(staticId);
		NSString* staticNameString = [NSString stringWithUTF8String:staticName];
		
		[outlineDataSource createItemWithSectionIndex:SECTION_INDEX_STATIC itemIndex:staticIndex name:staticNameString];
	}
	
	[_outlineView reloadData];
}

#pragma mark - Outline view data source and delegate implementation

/*- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
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
			case SECTION_INDEX_MOVABLE: return wadGetNumMovables(editor.wad);
			case SECTION_INDEX_STATIC: return wadGetNumStatics(editor.wad);
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
		else if (item.sectionIndex == SECTION_INDEX_MOVABLE)
		{
			[editor selectMovableAtIndex:item.itemIndex];
		}
		else if (item.sectionIndex == SECTION_INDEX_STATIC)
		{
			[editor selectStaticAtIndex:item.itemIndex];
		}
	}
	
}*/

#pragma mark - New code

- (void)_setupDynamicUserInterface
{
	// 1. Clear garbage from main window
	NSArray<__kindof NSView*>* subviews = self.contentView.subviews.copy;
	for (NSView* view in subviews)
	{
		[view removeFromSuperview];
	}
	
	// 2. Create main split view, it horizontally divides window view into three columns
	[self _initializeMainSplitView];
}

- (void)_initializeMainSplitView
{
	NSRect mainRect = NSMakeRect(0.0f, 0.0f, 400.0f, 300.0f);
	mainSplitView = [[NSSplitView alloc] initWithFrame:mainRect];
	mainSplitView.dividerStyle = NSSplitViewDividerStyleThin;
	mainSplitView.vertical = YES;
	mainSplitView.delegate = self;
	[self.contentView addSubview:mainSplitView];
	mainSplitView.translatesAutoresizingMaskIntoConstraints = NO;
	[mainSplitView.leadingAnchor constraintEqualToAnchor:self.contentView.leadingAnchor].active = YES;
	[mainSplitView.topAnchor constraintEqualToAnchor:self.contentView.topAnchor].active = YES;
	[mainSplitView.trailingAnchor constraintEqualToAnchor:self.contentView.trailingAnchor].active = YES;
	[mainSplitView.bottomAnchor constraintEqualToAnchor:self.contentView.bottomAnchor].active = YES;
	[mainSplitView.widthAnchor constraintGreaterThanOrEqualToConstant:MAIN_SPLIT_VIEW_MIN_WIDTH].active = YES;
	[mainSplitView.heightAnchor constraintGreaterThanOrEqualToConstant:300.0f].active = YES;
	
	[self _initializeLeftSidebar];
	[self _initializeCenterSplitView];
	[self _initializeRightSidebar];
	
	[mainSplitView setHoldingPriority:250 forSubviewAtIndex:0];
	[mainSplitView setHoldingPriority:10 forSubviewAtIndex:1];
	[mainSplitView setHoldingPriority:250 forSubviewAtIndex:2];
}

- (void)_initializeLeftSidebar
{
	NSRect sidebarRect = NSMakeRect(0.0f, 0.0f, 160.0f, 10.0f);
	
	leftSidebarView = [[NSView alloc] initWithFrame:sidebarRect];
	[mainSplitView addArrangedSubview:leftSidebarView];
	
	NSScrollView* scrollView = [[NSScrollView alloc] initWithFrame:sidebarRect];
	scrollView.hasVerticalScroller = YES;
	scrollView.hasHorizontalScroller = YES;
	scrollView.drawsBackground = NO;
	[leftSidebarView addSubview:scrollView];
	scrollView.translatesAutoresizingMaskIntoConstraints = NO;
	[scrollView.leadingAnchor constraintEqualToAnchor:leftSidebarView.leadingAnchor].active = YES;
	[scrollView.topAnchor constraintEqualToAnchor:leftSidebarView.topAnchor].active = YES;
	[scrollView.trailingAnchor constraintEqualToAnchor:leftSidebarView.trailingAnchor].active = YES;
	[scrollView.bottomAnchor constraintEqualToAnchor:leftSidebarView.bottomAnchor].active = YES;
	
	NSClipView* clipView = [[NSClipView alloc] initWithFrame:sidebarRect];
	clipView.autoresizesSubviews = YES;
	clipView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
	clipView.drawsBackground = NO;
	scrollView.contentView = clipView;
	
	outline = [[NSOutlineView alloc] initWithFrame:sidebarRect];
	outline.floatsGroupRows = NO;
	//outline.indentationPerLevel = 20.0f;
	outline.indentationMarkerFollowsCell = YES;
	outline.headerView = nil;
	outline.focusRingType = NSFocusRingTypeNone;
	outline.selectionHighlightStyle = NSTableViewSelectionHighlightStyleSourceList;
	
	wadItemsDataSource = [[WadItemsDataSource alloc] initWithEditor:wadEditor];
	outline.dataSource = wadItemsDataSource;
	outline.delegate = wadItemsDataSource;
	
	clipView.documentView = outline;
	
	NSTableColumn* column = [[NSTableColumn alloc] initWithIdentifier:@"OutlineColumnIdentifier"];
	column.title = @"Column";
	column.width = sidebarRect.size.width;
	//column.minWidth = 100.0f;
	column.headerCell.stringValue = @"Some string value";
	column.headerCell.font = [NSFont systemFontOfSize:NSFont.smallSystemFontSize weight:NSFontWeightBold];
	
	[outline addTableColumn:column];
	[outline setOutlineTableColumn:column];
}


- (void)_initializeCenterSplitView
{
	NSRect centerRect = NSMakeRect(0.0f, 0.0f, 150.0f, 150.0f);
	centerSplitView = [[NSSplitView alloc] initWithFrame:centerRect];
	centerSplitView.dividerStyle = NSSplitViewDividerStyleThin;
	centerSplitView.vertical = NO;
	centerSplitView.delegate = self;
	[mainSplitView addArrangedSubview:centerSplitView];
	
	[self _setupTopCenterSplitView];
	[self _setupBottomCenterView];
	
	[centerSplitView setHoldingPriority:10 forSubviewAtIndex:0];
	[centerSplitView setHoldingPriority:250 forSubviewAtIndex:1];
}

- (void)_setupTopCenterSplitView
{
	NSRect topCenterRect = NSMakeRect(0.0f, 0.0f, 70.0f, 70.0f);
	topCenterSplitView = [[NSSplitView alloc] initWithFrame:topCenterRect];
	topCenterSplitView.dividerStyle = NSSplitViewDividerStyleThin;
	topCenterSplitView.vertical = YES;
	topCenterSplitView.delegate = self;
	[centerSplitView addArrangedSubview:topCenterSplitView];
	
	[self _setupTopCenterLeftSidebar];
	[self _setupTopCenterViewport];
	
	[topCenterSplitView setHoldingPriority:250 forSubviewAtIndex:0];
	[topCenterSplitView setHoldingPriority:10 forSubviewAtIndex:1];
}

- (void)_setupTopCenterLeftSidebar
{
	NSRect topCenterLeftRect = NSMakeRect(0.0f, 0.0f, LEFT_TOOLBAR_WIDTH, 10.0f);
	topCenterLeftSidebarView = [[NSView alloc] initWithFrame:topCenterLeftRect];
	[topCenterSplitView addArrangedSubview:topCenterLeftSidebarView];
}

- (void)_setupTopCenterViewport
{
	WAD_EDITOR_VIEWPORT* mainViewport = wadEditorGetMainViewport(wadEditor);
	wkViewDelegate = [[WadKitViewDelegate alloc] init];
	
	NSRect viewportRect = NSMakeRect(0.0f, 0.0f, 10.0f, 10.0f);
	wadKitViewportView = [[WadKitView alloc] initWithFrame:viewportRect resourceStorage:resourceStorage];
	wadKitViewportView.viewport = mainViewport;
	wadKitViewportView.delegate = wkViewDelegate;
	
	wadKitViewportView.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
	wadKitViewportView.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
	wadKitViewportView.colorspace = CGColorSpaceCreateWithName(kCGColorSpaceDisplayP3);
	wadKitViewportView.clearColor = MTLClearColorMake(0.1, 0.1, 0.1, 1.0);
	//if (self.screen)
	//{
	//	wadKitViewportView.colorspace = self.screen.colorSpace.CGColorSpace;
	//	NSLog(@"%@", self.screen.colorSpace);
	//}
	[topCenterSplitView addArrangedSubview:wadKitViewportView];
}

- (void)_setupBottomCenterView
{
	NSRect bottomCenterRect = NSMakeRect(0.0f, 0.0f, 10.0f, 170.0f);
	bottomCenterView = [[NSView alloc] initWithFrame:bottomCenterRect];
	[centerSplitView addArrangedSubview:bottomCenterView];
}


- (void)_initializeRightSidebar
{
	NSRect sidebarRect = NSMakeRect(0.0f, 0.0f, 190.0f, 10.0f);
	
	rightSidebarView = [[NSView alloc] initWithFrame:sidebarRect];
	[mainSplitView addArrangedSubview:rightSidebarView];
}

#pragma mark - Split view settings

- (CGFloat)splitView:(NSSplitView*)splitView constrainMinCoordinate:(CGFloat)proposedMinimumPosition ofSubviewAt:(NSInteger)dividerIndex
{
	if (splitView == mainSplitView)
	{
		switch (dividerIndex)
		{
			case 0: return MAIN_SPLIT_VIEW_ITEM_0_MIN_WIDTH;
			case 1: return mainSplitView.frame.size.width - MAIN_SPLIT_VIEW_ITEM_2_MAX_WIDTH;
			default: break;
		}
	}
	else if (splitView == centerSplitView)
	{
		switch (dividerIndex)
		{
			case 0: return mainSplitView.frame.size.height - CENTER_SPLIT_VIEW_ITEM_1_MAX_HEIGHT;
			default: break;
		}
	}
	else if (splitView == topCenterSplitView)
	{
		switch (dividerIndex)
		{
			case 0: return LEFT_TOOLBAR_WIDTH;
			default: break;
		}
	}
	
	return proposedMinimumPosition;
}

- (CGFloat)splitView:(NSSplitView*)splitView constrainMaxCoordinate:(CGFloat)proposedMaximumPosition ofSubviewAt:(NSInteger)dividerIndex
{
	if (splitView == mainSplitView)
	{
		switch (dividerIndex)
		{
			case 0: return MAIN_SPLIT_VIEW_ITEM_0_MAX_WIDTH;
			case 1: return mainSplitView.frame.size.width - MAIN_SPLIT_VIEW_ITEM_2_MIN_WIDTH;
			default: break;
		}
	}
	else if (splitView == centerSplitView)
	{
		switch (dividerIndex)
		{
			case 0: return mainSplitView.frame.size.height - CENTER_SPLIT_VIEW_ITEM_1_MIN_HEIGHT;
			default: break;
		}
	}
	else if (splitView == topCenterSplitView)
	{
		switch (dividerIndex)
		{
			case 0: return LEFT_TOOLBAR_WIDTH;
			default: break;
		}
	}
	
	return proposedMaximumPosition;
}

- (BOOL)splitView:(NSSplitView*)splitView canCollapseSubview:(NSView*)subview
{
	return subview == leftSidebarView || subview == rightSidebarView || subview == bottomCenterView || subview == topCenterLeftSidebarView;
}

// Root of evil for subview constraints
- (BOOL)splitView:(NSSplitView*)splitView shouldAdjustSizeOfSubview:(NSView*)view
{
	if (view == leftSidebarView || view == rightSidebarView || view == bottomCenterView || view == topCenterLeftSidebarView)
	{
		return NO;
	}
	
	return YES;
}

@end
