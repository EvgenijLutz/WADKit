//
//  WadItemsDataSource.m
//  WAD Editor
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WadItemsDataSource.h"
#import "WadItemView.h"
#import "WadItem.h"

@implementation WadItemsDataSource
{
	WAD_EDITOR* editor;
	WAD_EDITOR_ITEM* rootItem;
	
	NSMutableArray<WadItem*>* items;
}

- (instancetype)initWithEditor:(WAD_EDITOR*)wadEditor
{
	self = [super init];
	if (self)
	{
		editor = wadEditor;
		rootItem = wadEditorGetRootItem(editor);
		
		items = [[NSMutableArray alloc] initWithCapacity:1024];
	}
	return self;
}

// Number of children
- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
{
	if (item == nil)
	{
		return (NSInteger)wadEditorGetNumChildrenInItem(editor, WAD_EDITOR_ITEM_TYPE_ROOT, 0);
	}
	
	assert([item isKindOfClass:WadItem.class]);
	WadItem* wadItem = (WadItem*)item;
	
	return wadEditorItemGetNumChildren(wadItem.item);
}

// Item for child
- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item
{
	unsigned int childIndex= (unsigned int)index;
	
	if (item == nil)
	{
		WAD_EDITOR_ITEM* wadEditorItem = wadEditorItemGetChild(rootItem, childIndex);
		
		const char* sectionName = wadEditorItemGetName(wadEditorItem);
		NSString* title = [NSString stringWithUTF8String:sectionName];
		
		WadItem* item = [[WadItem alloc] init];
		item.item = wadEditorItem;
		item.title = title;
		
		return item;
	}
	
	assert([item isKindOfClass:WadItem.class]);
	WadItem* wadItem = (WadItem*)item;

	WAD_EDITOR_ITEM* child = wadEditorItemGetChild(wadItem.item, childIndex);
	const char* name = wadEditorItemGetName(child);
	
	WadItem* childItem = [[WadItem alloc] init];
	childItem.item = child;
	childItem.title = [NSString stringWithUTF8String:name];
	return childItem;
}

// Is expandable
- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item
{
	if (item == nil)
	{
		return YES;
	}
	
	assert([item isKindOfClass:WadItem.class]);
	WadItem* wadItem = (WadItem*)item;
	if (wadEditorItemGetNumChildren(wadItem.item) > 0)
	{
		return YES;
	}
	
	return NO;
}

- (NSView*)outlineView:(NSOutlineView*)outlineView viewForTableColumn:(NSTableColumn*)tableColumn item:(id)item
{
	assert([item isKindOfClass:WadItem.class]);
	
	NSUserInterfaceItemIdentifier cellId = @"wk_cell";
	WadItemView* view = [outlineView makeViewWithIdentifier:cellId owner:nil];
	if (view == nil)
	{
		view = [[WadItemView alloc] init];
		view.identifier = cellId;
	}
	
	WadItem* wadItem = (WadItem*)item;
	[view setText:wadItem.title];
	
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
	if (![notification.object isKindOfClass:NSOutlineView.class])
	{
		return;
	}
	
	NSOutlineView* outlineView = (NSOutlineView*)notification.object;
	NSInteger selectedRowIndex = outlineView.selectedRow;
	id outlineItem = [outlineView itemAtRow:selectedRowIndex];
	if (![outlineItem isKindOfClass:WadItem.class])
	{
		return;
	}
	
	WadItem* wadItem = (WadItem*)outlineItem;
	WAD_EDITOR_ITEM* wadEditorItem = wadItem.item;
	wadEditorSelectItem(editor, wadEditorItem);
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
