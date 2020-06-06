//
//  WKEditor.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;
#import "WKViewport.h"
#import "WKEditorDelegate.h"
#include "editor_selected_entity_type.h"

NS_ASSUME_NONNULL_BEGIN

@interface WKEditor : NSObject<WKViewport>

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice;

- (void)selectNextObject;
- (void)selectPreviousObject;

- (void)loadWadByPath:(NSString*)wadPath;
- (WAD*)wad;

- (void)deselectEntity;
- (void)selectMeshAtIndex:(unsigned int)meshIndex;
- (void)selectStaticAtIndex:(unsigned int)staticIndex;

@property (nonatomic) id<WKEditorDelegate> delegate;

@end

NS_ASSUME_NONNULL_END
