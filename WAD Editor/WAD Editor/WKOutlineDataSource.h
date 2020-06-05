//
//  WKOutlineDataSource.h
//  WAD Editor
//
//  Created by Евгений Лютц on 05.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@class WKOutlineItem;

NS_ASSUME_NONNULL_BEGIN

@interface WKOutlineDataSource : NSObject

- (instancetype)init;
- (void)clear;

- (void)createItemWithSectionIndex:(unsigned int)sectionIndex itemIndex:(unsigned int)itemIndex name:(NSString*)name;
- (WKOutlineItem*)itemAtSectionIndex:(unsigned int)sectionIndex itemIndex:(unsigned int)itemIndex;

@end

NS_ASSUME_NONNULL_END
