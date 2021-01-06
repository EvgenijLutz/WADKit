//
//  ItemList.m
//  WAD Editor
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#import "ItemList.h"
#import "ItemListCell.h"
#import "ItemListValue.h"

static void _itemList_itemAdded(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo)
{
	ItemList* itemList = (__bridge ItemList*)userInfo;
	[itemList itemAdded:item];
}

static void _itemList_itemWillBeRemoved(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo)
{
	ItemList* itemList = (__bridge ItemList*)userInfo;
	[itemList itemWillBeRemoved:item];
}

@implementation ItemList
{
	ItemListValue* _rootItem;
	NSOutlineView* _outline;
}

- (instancetype)initWithFrame:(NSRect)frameRect
{
	self = [super initWithFrame:frameRect];
	if (self)
	{
		WE_LIST_DELEGATE listDelegate;
		listDelegate.itemAddedFunc = _itemList_itemAdded;
		listDelegate.itemWillBeRemovedFunc = _itemList_itemWillBeRemoved;
		listDelegate.userInfo = (__bridge void*)self;
		_list = listCreate(&listDelegate);
		
		WE_LIST_ITEM* root = listGetRootItem(_list);
		_rootItem = [[ItemListValue alloc] initWithItem:root parent:nil];
		
		NSScrollView* scrollView = [[NSScrollView alloc] initWithFrame:frameRect];
		scrollView.hasVerticalScroller = YES;
		scrollView.hasHorizontalScroller = YES;
		scrollView.drawsBackground = NO;
		[self addSubview:scrollView];
		scrollView.translatesAutoresizingMaskIntoConstraints = NO;
		[scrollView.leadingAnchor constraintEqualToAnchor:self.leadingAnchor].active = YES;
		[scrollView.topAnchor constraintEqualToAnchor:self.topAnchor].active = YES;
		[scrollView.trailingAnchor constraintEqualToAnchor:self.trailingAnchor].active = YES;
		[scrollView.bottomAnchor constraintEqualToAnchor:self.bottomAnchor].active = YES;
		
		NSClipView* clipView = [[NSClipView alloc] initWithFrame:frameRect];
		clipView.autoresizesSubviews = YES;
		clipView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
		clipView.drawsBackground = NO;
		scrollView.contentView = clipView;
		
		_outline = [[NSOutlineView alloc] initWithFrame:frameRect];
		_outline.floatsGroupRows = NO;
		_outline.indentationMarkerFollowsCell = YES;
		_outline.headerView = nil;
		_outline.focusRingType = NSFocusRingTypeNone;
		_outline.selectionHighlightStyle = NSTableViewSelectionHighlightStyleSourceList;
		
		clipView.documentView = _outline;
		
		NSTableColumn* column = [[NSTableColumn alloc] initWithIdentifier:@"OutlineColumnIdentifier"];
		column.title = @"Column";
		/*
		 Without the division by two, every cell has fixed width, which does not resize even while resizing the outline view. As workaround, we divide the width of column by two. I have no idea why this works.
		 */
		column.width = frameRect.size.width / 2.0;
		column.headerCell.stringValue = @"Some string value";
		column.headerCell.font = [NSFont systemFontOfSize:NSFont.smallSystemFontSize weight:NSFontWeightBold];
		
		[_outline addTableColumn:column];
		[_outline setOutlineTableColumn:column];
		
		_outline.dataSource = self;
		_outline.delegate = self;
	}
	return self;
}

- (void)dealloc
{
	listRelease(_list);
}


- (void)itemAdded:(WE_LIST_ITEM*)item
{
	if (_rootItem == nil)
	{
		return;
	}
	
	ItemListValue* value = [_rootItem insertedItem:item];
	assert(value);
	NSUInteger index = [value.parent.children indexOfObject:value];
	NSIndexSet* indexSet = [NSIndexSet indexSetWithIndex:index];
	ItemListValue* parent = value.parent == _rootItem ? nil : value.parent;
	[_outline insertItemsAtIndexes:indexSet inParent:parent withAnimation:NSTableViewAnimationSlideLeft];
}

- (void)itemWillBeRemoved:(WE_LIST_ITEM*)item
{
	if (_rootItem == nil)
	{
		return;
	}
	
	NSUInteger index = 0;
	ItemListValue* value = [_rootItem removeItem:item returnIndex:&index];
	assert(value);
	NSIndexSet* indexSet = [NSIndexSet indexSetWithIndex:index];
	ItemListValue* parent = value.parent == _rootItem ? nil : value.parent;
	[_outline removeItemsAtIndexes:indexSet inParent:parent withAnimation:NSTableViewAnimationSlideRight];
}


// MARK: Data source delegate implementation

// Number of children
- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
{
	if (item == nil)
	{
		return _rootItem.children.count;
	}

	assert([item isKindOfClass:ItemListValue.class]);
	ItemListValue* value = (ItemListValue*)item;

	return value.children.count;
}

// Item for child
- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item
{
	if (item == nil)
	{
		return [_rootItem.children objectAtIndex:index];
	}
	
	assert([item isKindOfClass:ItemListValue.class]);
	ItemListValue* value = (ItemListValue*)item;
	return [value.children objectAtIndex:index];
}

// Is expandable
- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item
{
	if (item == nil)
	{
		return YES;
	}

	assert([item isKindOfClass:ItemListValue.class]);
	ItemListValue* value = (ItemListValue*)item;
	if (value.children.count > 0)
	{
		return YES;
	}
	
	return NO;
}

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item
{
	assert([item isKindOfClass:ItemListValue.class]);

	NSUserInterfaceItemIdentifier cellId = @"wk_cell";
	ItemListCell* view = [outlineView makeViewWithIdentifier:cellId owner:nil];
	if (view == nil)
	{
		view = [[ItemListCell alloc] init];
		view.identifier = cellId;
	}

	ItemListValue* value = (ItemListValue*)item;
	const char* title = listItemGetTitle(value.item);
	[view setText:[NSString stringWithUTF8String:title]];

	return view;
}

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
	// 🤨
	return item;
}

- (CGFloat)outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item
{
	return 22.0f;
}

- (void)outlineViewSelectionDidChange:(NSNotification*)notification
{
//	if (![notification.object isKindOfClass:NSOutlineView.class])
//	{
//		return;
//	}
//
//	NSOutlineView* outlineView = (NSOutlineView*)notification.object;
//	NSInteger selectedRowIndex = outlineView.selectedRow;
//	id outlineItem = [outlineView itemAtRow:selectedRowIndex];
//	if (![outlineItem isKindOfClass:WadItem.class])
//	{
//		return;
//	}
//
//	WadItem* wadItem = (WadItem*)outlineItem;
//	WAD_EDITOR_ITEM* wadEditorItem = wadItem.item;
//	wadEditorSelectItem(editor, wadEditorItem);
}

/*- (void)outlineView:(NSOutlineView*)outlineView didRemoveRowView:(NSTableRowView*)rowView forRow:(NSInteger)row
{
	//
}*/

/*- (void)outlineViewSelectionDidChange:(NSNotification *)notification
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

@end
