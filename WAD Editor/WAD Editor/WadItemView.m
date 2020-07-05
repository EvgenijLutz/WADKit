//
//  WadItemView.m
//  WAD Editor
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WadItemView.h"

@implementation WadItemView
{
	NSTextField* customText;
}

- (instancetype)init
{
	self = [super init];
	if (self)
	{
		customText = [[NSTextField alloc] init];
		customText.drawsBackground = NO;
		customText.selectable = NO;
		customText.editable = NO;
		customText.bezeled = NO;
		customText.lineBreakMode = NSLineBreakByTruncatingMiddle;
		
		[self addSubview:customText];
		
		customText.translatesAutoresizingMaskIntoConstraints = NO;
		[customText.leadingAnchor constraintEqualToAnchor:customText.superview.leadingAnchor].active = YES;
		[customText.centerYAnchor constraintEqualToAnchor:customText.superview.centerYAnchor].active = YES;
		[customText.trailingAnchor constraintEqualToAnchor:customText.superview.trailingAnchor].active = YES;
	}
	return self;
}

- (void)setText:(NSString*)text
{
	customText.stringValue = text;
	[customText sizeToFit];
}

@end
