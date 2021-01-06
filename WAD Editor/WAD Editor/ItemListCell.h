//
//  ItemListCell.h
//  WAD Editor
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Cocoa;

NS_ASSUME_NONNULL_BEGIN

@interface ItemListCell : NSTableCellView//NSView

- (void)setText:(NSString*)text;

@end

NS_ASSUME_NONNULL_END
