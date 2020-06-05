//
//  WKOutlineDataSource.m
//  WAD Editor
//
//  Created by Евгений Лютц on 05.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WKOutlineDataSource.h"
#import "WKOutlineItem.h"

@implementation WKOutlineDataSource
{
	NSMutableArray<WKOutlineItem*>* items;
}

- (instancetype)init
{
	self = [super init];
	if (self)
	{
		items = [[NSMutableArray alloc] init];
	}
	return self;
}

- (void)clear
{
	[items removeAllObjects];
}

- (void)createItemWithSectionIndex:(unsigned int)sectionIndex itemIndex:(unsigned int)itemIndex name:(NSString*)name
{
	WKOutlineItem* item = [[WKOutlineItem alloc] initWithSectionIndex:sectionIndex itemIndex:itemIndex name:name];
	[items addObject:item];
}

- (WKOutlineItem*)itemAtSectionIndex:(unsigned int)sectionIndex itemIndex:(unsigned int)itemIndex
{
	for (WKOutlineItem* item in items)
	{
		if (item.sectionIndex == sectionIndex && item.itemIndex == itemIndex)
		{
			return item;
		}
	}
	
	// This should never happen!
	return nil;
}

@end
