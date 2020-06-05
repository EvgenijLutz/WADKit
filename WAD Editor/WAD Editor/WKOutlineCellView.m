//
//  WKOutlineCellView.m
//  WAD Editor
//
//  Created by Евгений Лютц on 05.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WKOutlineCellView.h"

@implementation WKOutlineCellView
{
	//NSTextField* strongTextField;
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
		
		[self addSubview:_customText];
		
		_customText.translatesAutoresizingMaskIntoConstraints = NO;
		[_customText.leadingAnchor constraintEqualToAnchor:_customText.superview.leadingAnchor].active = YES;
		[_customText.centerYAnchor constraintEqualToAnchor:_customText.superview.centerYAnchor].active = YES;
		[_customText.trailingAnchor constraintEqualToAnchor:_customText.superview.trailingAnchor].active = YES;
	}
	return self;
}

@end
