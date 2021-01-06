//
//  ItemListCell.m
//  WAD Editor
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "ItemListCell.h"

@implementation ItemListCell
{
	NSTextField* _customText;
}

- (instancetype)init
{
	self = [super init];
	if (self)
	{
		_customText = [[NSTextField alloc] init];
		_customText.drawsBackground = NO;
		_customText.selectable = NO;
		_customText.editable = NO;
		_customText.bezeled = NO;
		_customText.lineBreakMode = NSLineBreakByTruncatingMiddle;
		_customText.backgroundColor = NSColor.redColor;
		
		[self addSubview:_customText];
		
		_customText.translatesAutoresizingMaskIntoConstraints = NO;
		[_customText.leadingAnchor constraintEqualToAnchor:_customText.superview.leadingAnchor].active = YES;
		[_customText.centerYAnchor constraintEqualToAnchor:_customText.superview.centerYAnchor].active = YES;
		[_customText.trailingAnchor constraintEqualToAnchor:_customText.superview.trailingAnchor].active = YES;
	}
	return self;
}

- (void)setText:(NSString*)text
{
	_customText.stringValue = text;
	[_customText sizeToFit];
}

@end
