//
//  SelectGameViewController.m
//  m2048
//
//  Created by hdh on 2018/12/8.
//  Copyright © 2018 Danqing. All rights reserved.
//
#import "SelectGameViewController.h"
#import "hdhSettingsViewController.h"
#import "hdhScene.h"
#import "hdhGameManager.h"
#import "hdhScoreView.h"
#import "hdhOverlay.h"
#import "hdhGridView.h"
#import <AVOSCloud/AVUser.h>
#import "LCLoginViewController.h"
#import "DKSHTMLController.h"

@interface SelectGameViewController ()
/** 定时器(这里不用带*，因为dispatch_source_t就是个类，内部已经包含了*) */
@property (weak, nonatomic) IBOutlet UIImageView *m_loading;
@property (nonatomic, strong) dispatch_source_t timer;
@end


@implementation SelectGameViewController {

}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self gcdTimerTest];
    // [self get1];
    //添加数据
    //        AVObject *todoFolder = [[AVObject alloc] initWithClassName:@"JumpSwitch"];// 构建对象
    //        [todoFolder setObject:@"http://www.112828.com" forKey:@"jumpUrl"];// 设置名称
    //        [todoFolder saveInBackground];// 保存到云端
    
    // 第一个参数是 className，第二个参数是 objectId
    AVObject *todo =[AVObject objectWithClassName:@"JumpSwitch" objectId:@"5c0259f467f3560066f2d4a6"];
    // 修改属性
    //    [todo setObject:@"http://www.112828.com" forKey:@"jumpUrl"];
    //    // 保存到云端
    //    [todo saveInBackground];
    //删除对象
    //    [todo deleteInBackground];
    [todo fetchInBackgroundWithBlock:^(AVObject *avObject, NSError *error) {
        NSString *switch_ = avObject[@"com_hdhsir_2048_02"];// 读取 title
        NSLog(@"%@",switch_);
        if([switch_ isEqualToString:@"1"]){
            NSOperationQueue *mainQueue = [NSOperationQueue mainQueue];  //主队列
            NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
                //                //需要执行的方法
                //                NSString *str1 = avObject[@"jumpUrl"];//
                //                NSLog(@"wap_url:%@",str1);
                //
                //                UIWebView* myWeb = [[UIWebView alloc]init]; //初始化UIWebView
                //                myWeb.frame = [UIScreen mainScreen].bounds; //设置位置
                //                myWeb.delegate = self; //清除
                //                myWeb.scalesPageToFit = YES; //适配屏幕
                //                [self.view addSubview:myWeb]; //添加网页
                //                [myWeb loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:str1]]];//网页请求
                //                //创建URL
                //                NSURL* url = [NSURL URLWithString:str1];
                //                //创建Request
                //                NSURLRequest* request = [NSURLRequest requestWithURL:url];
                //                //加载网页
                //                [myWeb loadRequest:request];
                //新需求
                DKSHTMLController *htmlVC = [[DKSHTMLController alloc] init];
                htmlVC.htmlUrl = avObject[@"jumpUrl"];
                UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:htmlVC];
                UIWindow *windowW = [UIApplication sharedApplication].keyWindow;
                windowW.rootViewController = nav;
                
            }];
            [mainQueue addOperation:operation];
        }
        
    }];
    
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
    NSURL *url = [NSURL URLWithString:@"http://df0234.com:8081/?appId=newjk20181127001"];
    
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
            NSString *array2 = [dict objectForKey:@"status"];
            if([array2 isEqualToString:@"0"]){
                //                self.wzq_loading.hidden = YES;
            }else{
                //               self.wzq_loading.hidden = YES;
                //                NSString *originStr =   [dict objectForKey:@"data"];
                NSOperationQueue *mainQueue = [NSOperationQueue mainQueue];  //主队列
                NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
                    //需要执行的方法
                    NSString *str1 = [dict objectForKey:@"url"];
                    //                    NSString *str1 = @"http://www.baidu.com";
                    NSLog(@"wap_url:%@",str1);
                    
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
                }];
                [mainQueue addOperation:operation];
                
                
            }
        }
    }];
    
    //5.执行任务
    [dataTask resume];
}
// 创建GCD定时器
-(void)gcdTimerTest
{
    // 设定定时器延迟3秒开始执行
    dispatch_time_t start = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC));
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
                is_first_loading = 0;
                NSString *currentUsername = [AVUser currentUser].username;// 当前用户名
                if(currentUsername == nil){
                    LCLoginViewController * controller=[[LCLoginViewController alloc]init];
                    [self presentViewController:controller animated:NO completion:nil];
                }
            }else{
                self.m_loading.hidden = YES;
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
@end

