//
//  UIAlertController.h
//  StorageStarted
//
//  Created by hdh on 2018/11/29.
//  Copyright © 2018 cuiyiran. All rights reserved.
//

#ifndef UIAlertController_h
#define UIAlertController_h


#endif /* UIAlertController_h */
#import <Foundation/Foundation.h>

#import <UIKit/UIKit.h>

@interface YZBAlert : NSObject

+(void)showAlert:(UIViewController *)viewController :(NSString *)msg;//使用加号就是可以直接通过类就可以访问本行为，而不用像 - 号一样要先去创建YZBAlert这个类的行为。

@end

