//
//  TextureReflection.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;

NS_ASSUME_NONNULL_BEGIN

@interface TextureReflection : NSObject

- (instancetype)init;

@property (nonatomic, nullable) id<MTLTexture> texture;
@property (nonatomic) unsigned int index;

@end

NS_ASSUME_NONNULL_END
