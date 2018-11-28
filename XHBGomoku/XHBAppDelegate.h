//
//  XHBAppDelegate.h
//  XHBGomoku
//
//  Created by weqia on 14-9-1.
//  Copyright (c) 2014年 xhb. All rights reserved.
//

#import <UIKit/UIKit.h>
static NSString *appKey = @"66cc47fa08157cb4628bf71e";
static NSString *channel = @"App Store";
static BOOL isProduction = YES;
@interface XHBAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property(nonatomic,weak)IBOutlet UIImageView * _splashImageView;


@end
