//
//  ItemList.h
//  WAD Editor
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

@import Cocoa;
#include "wad_editor_lib_link.h"

NS_ASSUME_NONNULL_BEGIN

@interface ItemList : NSView<NSOutlineViewDataSource, NSOutlineViewDelegate>

- (instancetype)initWithFrame:(NSRect)frameRect;

- (void)itemAdded:(WE_LIST_ITEM*)item;
- (void)itemWillBeRemoved:(WE_LIST_ITEM*)item;

@property (nonatomic, readonly) WE_LIST* list;

@end

NS_ASSUME_NONNULL_END
