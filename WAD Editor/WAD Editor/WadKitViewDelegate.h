//
//  WadKitViewDelegate.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import MetalKit;

@protocol WKViewport;

NS_ASSUME_NONNULL_BEGIN

@interface WadKitViewDelegate : NSObject<MTKViewDelegate>

//- (instancetype)initWithEditor:(WKEditor*)editor;	// TODO: id<WKViewport>(WKPreviewViewport, WKAnimationViewport)?
- (instancetype)initWithViewport:(id<WKViewport>)wkViewport;

@end

NS_ASSUME_NONNULL_END
