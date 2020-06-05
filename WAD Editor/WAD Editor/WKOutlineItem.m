//
//  WKOutlineItem.m
//  WAD Editor
//
//  Created by Евгений Лютц on 05.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WKOutlineItem.h"

@implementation WKOutlineItem

- (instancetype)initWithSectionIndex:(unsigned int)sectionIndex itemIndex:(unsigned int)itemIndex name:(NSString*)name
{
	self = [super init];
	if (self)
	{
		_sectionIndex = sectionIndex;
		_itemIndex = itemIndex;
		_name = name;
	}
	return self;
}

@end
