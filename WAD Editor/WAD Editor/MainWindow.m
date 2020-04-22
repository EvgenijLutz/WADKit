//
//  MainWindow.m
//  WAD Editor
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#import "MainWindow.h"

@implementation MainWindow
{
	NSArray<NSString*>* list;
}

// This method is called when application creates window
- (instancetype)initWithContentRect:(NSRect)contentRect styleMask:(NSWindowStyleMask)style backing:(NSBackingStoreType)backingStoreType defer:(BOOL)flag
{
	self = [super initWithContentRect:contentRect styleMask:style backing:backingStoreType defer:flag];
	if (self)
	{
		//[self setupUI];
	}
	return self;
}

// or this...
- (instancetype)initWithContentRect:(NSRect)contentRect styleMask:(NSWindowStyleMask)style backing:(NSBackingStoreType)backingStoreType defer:(BOOL)flag screen:(NSScreen *)screen
{
	self = [super initWithContentRect:contentRect styleMask:style backing:backingStoreType defer:flag screen:screen];
	if (self)
	{
		//[self setupUI];
	}
	return self;
}

- (void)setupUI
{
	list = [NSArray arrayWithObjects:@"Lara", @"Animating 1", @"Hero", @"Spaceship", @"Asteroid", @"Bluster", @"Laser", nil];
	
	self.contentView.wantsLayer = YES;
	
	NSVisualEffectView* vibrant = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(0, 0, 100, 100)];
	vibrant.blendingMode = NSVisualEffectBlendingModeBehindWindow;
	//vibrant.blendingMode = NSVisualEffectBlendingModeWithinWindow;
	//[clipView addSubview:vibrant positioned:NSWindowBelow relativeTo:nil];
	[self.contentView addSubview:vibrant];
	vibrant.translatesAutoresizingMaskIntoConstraints = NO;
	[vibrant.leadingAnchor constraintEqualToAnchor:self.contentView.leadingAnchor].active = YES;
	[vibrant.topAnchor constraintEqualToAnchor:self.contentView.topAnchor].active = YES;
	[vibrant.widthAnchor constraintEqualToConstant:180.0f].active = YES;
	[vibrant.trailingAnchor constraintLessThanOrEqualToAnchor:self.contentView.trailingAnchor constant:-10.0f].active = YES;
	[vibrant.bottomAnchor constraintEqualToAnchor:self.contentView.bottomAnchor].active = YES;
	//[vibrant.heightAnchor constraintGreaterThanOrEqualToConstant:100.0f].active = YES;
	//vibrant.wantsLayer = YES;
	
	NSScrollView* scrollView = [[NSScrollView alloc] initWithFrame:vibrant.bounds];
	//vibrant.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
	[vibrant addSubview:scrollView];
	//scrollView.backgroundColor = NSColor.darkGrayColor;
	
	scrollView.translatesAutoresizingMaskIntoConstraints = NO;
	[scrollView.leadingAnchor constraintEqualToAnchor:vibrant.leadingAnchor].active = YES;
	[scrollView.topAnchor constraintEqualToAnchor:vibrant.topAnchor].active = YES;
	[scrollView.rightAnchor constraintEqualToAnchor:vibrant.rightAnchor].active = YES;
	[scrollView.bottomAnchor constraintEqualToAnchor:vibrant.bottomAnchor].active = YES;
	scrollView.hasVerticalScroller = YES;
	scrollView.hasHorizontalScroller = YES;
	scrollView.identifier = @"scrollView";
	scrollView.drawsBackground = NO;
	
	
	NSClipView* clipView = [[NSClipView alloc] init];
	clipView.autoresizesSubviews = YES;
	clipView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
	scrollView.contentView = clipView;
	
	NSOutlineView* outline = [[NSOutlineView alloc] init];
	outline.floatsGroupRows = NO;
	outline.indentationPerLevel = 2.0f;
	outline.indentationMarkerFollowsCell = NO;
	outline.headerView = nil;
	outline.dataSource = self;
	outline.delegate = self;
	clipView.documentView = outline;
	
	NSTableColumn* column = [[NSTableColumn alloc] init];
	column.title = @"Column";
	column.minWidth = 100.0f;
	column.headerCell.stringValue = @"Some string value";
	[outline addTableColumn:column];
	[outline setOutlineTableColumn:column];
	
	
	
	//[scrollView removeFromSuperview];
}

- (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item
{
	if (item == nil)
	{
		return list.count;
	}
	
	return 0;
}

/*- (NSView *)outlineView:(NSOutlineView *)outlineView viewForTableColumn:(NSTableColumn *)tableColumn item:(id)item
{
	NSRect r = NSMakeRect(0, 0, 256, 30);
	NSView* rv = [[NSView alloc] initWithFrame:r];
	return rv;
}

- (NSTableRowView *)outlineView:(NSOutlineView *)outlineView rowViewForItem:(id)item
{
	NSRect r = NSMakeRect(0, 0, 256, 30);
	NSTableRowView* rv = [[NSTableRowView alloc] initWithFrame:r];
	return rv;
}*/

- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item
{
	return item? NO : YES;
}

- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item
{
	return item? nil : [list objectAtIndex:index];
}

- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
	return item;
}

@end
