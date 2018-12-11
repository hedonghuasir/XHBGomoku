//
//  XHBGomokuGameSencesViewController.h
//  XHBGomoku
//
//  Created by weqia on 14-9-1.
//  Copyright (c) 2014年 xhb. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "XHBGomokuGameEngine.h"
#import<AVFoundation/AVFoundation.h>

@interface XHBGomokuGameSencesViewController : UIViewController<XHBGomokuGameEngineProtocol>{
    AVAudioPlayer *_player;

}

@end
