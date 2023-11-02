//
//  YFViewController.m
//  YFDB
//
//  Created by PYFer on 11/01/2023.
//  Copyright (c) 2023 PYFer. All rights reserved.
//

#import "YFViewController.h"
#import <YFDB/YFDB.h>

@interface YFViewController ()
@property (nonatomic, strong) YFDatabase *db;

@end

@implementation YFViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.

    //1.创建database路径
    NSString *docuPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)[0];
    NSString *dbPath = [docuPath stringByAppendingPathComponent:@"test.db"];
    NSLog(@"!!!dbPath = %@",dbPath);
    
    //2.创建对应路径下数据库
     _db = [YFDatabase databaseWithPath:dbPath];
    
    //3.在数据库中进行增删改查操作时，需要判断数据库是否open，如果open失败，可能是权限或者资源不足，数据库操作完成通常使用close关闭数据库
    [_db open];
    if (![_db open]) {
        NSLog(@"db open fail");
        return;
    }
    
    //4.数据库中创建表（可创建多张）
    NSString *sql = @"create table if not exists t_student ('ID' INTEGER PRIMARY KEY AUTOINCREMENT,'name' TEXT NOT NULL, 'phone' TEXT NOT NULL,'score' INTEGER NOT NULL)";
      //5.执行更新操作 此处database直接操作，不考虑多线程问题，多线程问题，用FMDatabaseQueue 每次数据库操作之后都会返回bool数值，YES，表示success，NO，表示fail,可以通过 @see lastError @see lastErrorCode @see lastErrorMessage
      BOOL result = [_db executeUpdate:sql];
      if (result) {
          NSLog(@"create table success");
      }
      [_db close];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
