//
//  WKOutlineItem.h
//  WAD Editor
//
//  Created by Евгений Лютц on 05.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface WKOutlineItem : NSObject

- (instancetype)initWithSectionIndex:(unsigned int)sectionIndex itemIndex:(unsigned int)itemIndex name:(NSString*)name;

@property (readonly) unsigned int sectionIndex;
@property (readonly) unsigned int itemIndex;
@property (readonly) NSString* name;

@end

NS_ASSUME_NONNULL_END
