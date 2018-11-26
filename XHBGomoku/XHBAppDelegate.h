//
//  XHBAppDelegate.h
//  XHBGomoku
//
//  Created by weqia on 14-9-1.
//  Copyright (c) 2014年 xhb. All rights reserved.
//

#import <UIKit/UIKit.h>
static NSString *appKey = @"301cb2641c21071250933bb9";
static NSString *channel = @"App Store";
static BOOL isProduction = FALSE;
@interface XHBAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property(nonatomic,weak)IBOutlet UIImageView * _splashImageView;


@end
