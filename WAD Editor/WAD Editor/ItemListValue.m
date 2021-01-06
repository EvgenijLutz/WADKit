//
//  ItemListValue.m
//  WAD Editor
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#import "ItemListValue.h"

@implementation ItemListValue

- (instancetype)initWithItem:(WE_LIST_ITEM*)item parent:(ItemListValue*)parent
{
	self = [super init];
	if (self)
	{
		_parent = parent;
		_item = item;
		
		_children = [[NSMutableArray alloc] init];
		unsigned int numChildren = listItemGetNumberOfChildren(item);
		for (unsigned int i = 0; i < numChildren; i++)
		{
			WE_LIST_ITEM* currentItem = listItemGetChild(item, i);
			ItemListValue* value = [[ItemListValue alloc] initWithItem:currentItem parent:self];
			[_children addObject:value];
		}
	}
	return self;
}

- (ItemListValue*)insertedItem:(WE_LIST_ITEM*)item
{
	WE_LIST_ITEM* parent = listItemGetParent(item);
	
	if (parent == _item)
	{
		unsigned int itemIndex = listItemGetIndex(item);
		ItemListValue* value = [[ItemListValue alloc] initWithItem:item parent:self];
		[_children insertObject:value atIndex:itemIndex];
		return value;
	}
	
	for (ItemListValue* itemValue in _children)
	{
		ItemListValue* value = [itemValue insertedItem:item];
		if (value)
		{
			return value;
		}
	}
	
	return nil;
}

- (ItemListValue*)removeItem:(WE_LIST_ITEM*)item returnIndex:(NSUInteger*)removedIndex
{
	ItemListValue* valueToDelete = nil;
	for (ItemListValue* itemValue in _children)
	{
		if (itemValue.item == item)
		{
			valueToDelete = itemValue;
			break;
		}
		
		ItemListValue* deletedValue = [itemValue removeItem:item returnIndex:removedIndex];
		if (deletedValue)
		{
			return deletedValue;
		}
	}
	if (valueToDelete)
	{
		*removedIndex = [self.children indexOfObject:valueToDelete];
		[self.children removeObject:valueToDelete];
	}
	
	return valueToDelete;
}

- (void)clearedItems
{
	[self.children removeAllObjects];
}

@end
