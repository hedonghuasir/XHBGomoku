//
//  UIAlertController.m
//  StorageStarted
//
//  Created by hdh on 2018/11/29.
//  Copyright © 2018 cuiyiran. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CommonAlert.h"

@implementation YZBAlert

+(void)showAlert:(UIViewController *)viewController :(NSString *)msg

{
    
        UIAlertController * uia = [UIAlertController alertControllerWithTitle:@"友情提示" message:msg preferredStyle:UIAlertControllerStyleAlert];
    
        UIAlertAction * okaction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction * action){
        
            }];
    
        [uia addAction:okaction];
    
       [viewController presentViewController:uia animated:YES completion:nil];
    
}

@end

