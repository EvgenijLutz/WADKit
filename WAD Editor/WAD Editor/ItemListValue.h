//
//  ItemListValue.h
//  WAD Editor
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

@import Foundation;
#include "wad_editor_lib_link.h"

NS_ASSUME_NONNULL_BEGIN

@interface ItemListValue : NSObject

- (instancetype)initWithItem:(WE_LIST_ITEM*)item parent:(nullable ItemListValue*)parent;

- (nullable ItemListValue*)insertedItem:(WE_LIST_ITEM*)item;
- (nullable ItemListValue*)removeItem:(WE_LIST_ITEM*)item returnIndex:(NSUInteger*)removedIndex;
- (void)clearedItems;

@property (nonatomic, readonly, weak) ItemListValue* parent;
@property (nonatomic, readonly) WE_LIST_ITEM* item;
@property (nonatomic, readonly) NSMutableArray<ItemListValue*>* children;

@end

NS_ASSUME_NONNULL_END
