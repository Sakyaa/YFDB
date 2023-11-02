//
//  YFViewController.m
//  YFDB
//
//  Created by PYFer on 11/01/2023.
//  Copyright (c) 2023 PYFer. All rights reserved.
//

#import "YFViewController.h"
#import "YFPersonVO.h"
#import <YFDB/YFDB.h>

@interface YFViewController ()
@property (nonatomic, strong) YFDatabase *db;
@property (weak, nonatomic) IBOutlet UILabel *contentLabel;

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
//      [_db close];
}

/**
 增删改查中 除了查询（executeQuery），其余操作都用（executeUpdate）
 //1.sql语句中跟columnname 绑定的value 用 ？表示，不加‘’，可选参数是对象类型如：NSString，不是基本数据结构类型如：int，方法自动匹配对象类型
 - (BOOL)executeUpdate:(NSString*)sql, ...;
 //2.sql语句中跟columnname 绑定的value 用%@／%d表示，不加‘’
 - (BOOL)executeUpdateWithFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(1,2);
 //3.sql语句中跟columnname 绑定的value 用 ？表示的地方依次用 (NSArray *)arguments 对应的数据替代
 - (BOOL)executeUpdate:(NSString*)sql withArgumentsInArray:(NSArray *)arguments;
 //4.同3 ，区别在于多一个error指针，记录更新失败
 - (BOOL)executeUpdate:(NSString*)sql values:(NSArray * _Nullable)values error:(NSError * _Nullable __autoreleasing *)error;
 //5.同3，区别在于用 ？ 表示的地方依次用(NSDictionary *)arguments中对应的数据替代
 - (BOOL)executeUpdate:(NSString*)sql withParameterDictionary:(NSDictionary *)arguments;
 - (BOOL)executeUpdate:(NSString*)sql withVAList: (va_list)args;
 */
// 1.直接sql语句
//    BOOL result = [db executeUpdate:@"insert into 't_student' (ID,name,phone,score) values(110,'x1','11',83)"];
    // 2.
//    BOOL result = [db executeUpdate:@"insert into 't_student'(ID,name,phone,score) values(?,?,?,?)",@111,@"x2",@"12",@23];
    // 3.
//    BOOL result = [db executeUpdateWithFormat:@"insert into 't_student' (ID,name,phone,score) values(%d,%@,%@,%d)",112,@"x3",@"13",43];
    // 4.
- (IBAction)addAction:(UIButton *)sender {
//    BOOL result = [_db executeUpdate:@"insert into 't_student'(ID,name,phone,score) values(?,?,?,?)" withArgumentsInArray:@[@113,@"李华",@"12233444",@53]];
//    if (result) {
//        NSLog(@"insert into 't_studet' success");
//    } else {
//        
//    }
    for (NSInteger index = 10; index < 120; index ++) {
        NSString *name = [self randomCreatChineseWithNum:3];
        BOOL result = [_db executeUpdate:@"insert into 't_student'(ID,name,phone,score) values(?,?,?,?)" withArgumentsInArray:@[@(index),name,@"12233444",@53]];
        if (result) {
            NSLog(@"insert into 't_studet' success");
        } else {
            
        }
    }
//    [_db close];
}

- (IBAction)deleteAction:(UIButton *)sender {
    // 1.直接sql语句
//    BOOL result = [db executeUpdate:@"delete from 't_student' where ID = 110"];
    // 2.
//    BOOL result = [db executeUpdate:@"delete from 't_student' where ID = ?",@(111)];
    // 3.
//    BOOL result = [db executeUpdateWithFormat:@"delete from 't_student' where ID = %d",112];
    // 4.
    BOOL result = [_db executeUpdate:@"delete from 't_student' where ID = ?" withArgumentsInArray:@[@113]];
    if (result) {
        NSLog(@"delete from 't_student' success");
    } else {

    }
}


- (IBAction)updateAction:(UIButton *)sender {
    // 1.直接sql语句
//    BOOL result = [db executeUpdate:@"update 't_student' set ID = 110 where name = 'x1'"];
    // 2.
//    BOOL result = [db executeUpdate:@"update 't_student' set ID = ? where name = ?",@111,@"x2" ];
    // 3.
//    BOOL result = [db executeUpdateWithFormat:@"update 't_student' set ID = %d where name = %@",113,@"x3" ];
    // 4.
    BOOL result = [_db executeUpdate:@"update 't_student' set ID = ? where name = ?" withArgumentsInArray:@[@113,@"x3"]];
    if (result) {
        NSLog(@"update 't_student' success");
    } else {
    }
}

/**
FMResultSet根据column name获取对应数据的方法
intForColumn：
longForColumn：
longLongIntForColumn：
boolForColumn：
doubleForColumn：
stringForColumn：
dataForColumn：
dataNoCopyForColumn：
UTF8StringForColumnIndex：
objectForColumn：
*/
- (IBAction)queryAction:(UIButton *)sender {
    // 1.直接sql语句
    //    FMResultSet *result = [db executeQuery:@"select * from 't_student' where ID = 110"];
    // 2.
    //    FMResultSet *result = [db executeQuery:@"select *from 't_student' where ID = ?",@111];
    // 3.
    //    FMResultSet *result = [db executeQueryWithFormat:@"select * from 't_student' where ID = %d",112];
    // 4.
//    FMResultSet *result = [db executeQuery:@"select * from 't_student' where ID = ?" withArgumentsInArray:@[@113]];
    //5
    YFResultSet *result = [_db executeQuery:@"select * from 't_sutdent' where ID = ?" withParameterDictionary:@{@"ID": @114}];
    NSMutableArray *arr = [NSMutableArray array];
    while ([result next]) {
        YFPersonVO *person = [[YFPersonVO alloc] init];;
        person.ID = [result intForColumn:@"ID"];
        person.name = [result stringForColumn:@"name"];
        person.phone = [result stringForColumn:@"phone"];
        person.score = [result intForColumn:@"score"];
        [arr addObject:person];
        NSLog(@"从数据库查询到的人员 %@",person.name);
    }
}

/// 随机生成汉字
/// - Parameter num: 汉字个数
- (NSString *)randomCreatChineseWithNum:(NSInteger)num{
    NSStringEncoding gbkEncoding = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);

    NSInteger randomH = 0xA1+arc4random()%(0xFE - 0xA1+1);

    NSInteger randomL = 0xB0+arc4random()%(0xF7 - 0xB0+1);

    NSInteger number = (randomH<<8)+randomL;

    NSData *data = [NSData dataWithBytes:&number length:2];

    NSString *string = @"";

    for (int i = 0; i < num; i ++) {

        if ([string isEqualToString:@""]) {

            string = [[NSString alloc] initWithData:data encoding:gbkEncoding];

        }else{

            string = [NSString stringWithFormat:@"%@%@",string,string];

        }

    }

    return string;

}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
