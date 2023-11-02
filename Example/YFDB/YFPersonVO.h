//
//  YFPersonVO.h
//  YFDB_Example
//
//  Created by Sakya on 2023/11/2.
//  Copyright © 2023 PYFer. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface YFPersonVO : NSObject

@property (nonatomic, assign) int ID;
@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *phone;
@property (nonatomic, copy) NSString *address;
@property (nonatomic, assign) int score;

@end

NS_ASSUME_NONNULL_END
