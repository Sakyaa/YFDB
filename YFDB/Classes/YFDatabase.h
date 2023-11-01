//
//  YFDatabase.h
//  YFDBExample
//
//  Created by Sakya on 2023/11/1.
//

#import <Foundation/Foundation.h>
#import "YFResultSet.h"

NS_ASSUME_NONNULL_BEGIN

typedef int(^YFDBExecuteStatementsCallbackBlock)(NSDictionary *resultsDictionary);

/**
 Enumeration used in checkpoint methods.
 */
typedef NS_ENUM(int, YFDBCheckpointMode) {
    YFDBCheckpointModePassive  = 0, // SQLITE_CHECKPOINT_PASSIVE,
    YFDBCheckpointModeFull     = 1, // SQLITE_CHECKPOINT_FULL,
    YFDBCheckpointModeRestart  = 2, // SQLITE_CHECKPOINT_RESTART,
    YFDBCheckpointModeTruncate = 3  // SQLITE_CHECKPOINT_TRUNCATE
};

@interface YFDatabase : NSObject

///-----------------
/// @name Properties
///-----------------

/** Whether should trace execution */

@property (atomic, assign) BOOL traceExecution;

/** Whether checked out or not */

@property (atomic, assign) BOOL checkedOut;

/** Crash on errors */

@property (atomic, assign) BOOL crashOnErrors;

/** Logs errors */

@property (atomic, assign) BOOL logsErrors;

/** Dictionary of cached statements */

@property (atomic, retain, nullable) NSMutableDictionary *cachedStatements;

///---------------------
/// @name Initialization
///---------------------

/** Create a @c YFDatabase  object.
 For example, to open a database in the app's “Application Support” directory:

@code
NSURL *folder  = [[NSFileManager defaultManager] URLForDirectory:NSApplicationSupportDirectory inDomain:NSUserDomainMask appropriateForURL:nil create:true error:&error];
NSURL *fileURL = [folder URLByAppendingPathComponent:@"test.db"];
YFDatabase *db = [YFDatabase databaseWithPath:fileURL.path];
@endcode

 @param inPath Path of database file

 @return @c YFDatabase  object if successful; @c nil  if failure.

 */

+ (instancetype)databaseWithPath:(NSString * _Nullable)inPath;

+ (instancetype)databaseWithURL:(NSURL * _Nullable)url;

- (instancetype)initWithPath:(NSString * _Nullable)path;

- (instancetype)initWithURL:(NSURL * _Nullable)url;

///-----------------------------------
/// @name Opening and closing database
///-----------------------------------

/// Is the database open or not?

@property (nonatomic) BOOL isOpen;

/** Opening a new database connection
 */

- (BOOL)open;

/** Opening a new database connection with flags and an optional virtual file system (VFS)

 @param flags One of the following three values, optionally combined with the @c SQLITE_OPEN_NOMUTEX , @c SQLITE_OPEN_FULLMUTEX , @c SQLITE_OPEN_SHAREDCACHE , @c SQLITE_OPEN_PRIVATECACHE , and/or @c SQLITE_OPEN_URI flags:

 */

- (BOOL)openWithFlags:(int)flags;

/** Opening a new database connection with flags and an optional virtual file system (VFS)
 
 @param flags One of the following three values, optionally combined with the @c SQLITE_OPEN_NOMUTEX , `SQLITE_OPEN_FULLMUTEX`, `SQLITE_OPEN_SHAREDCACHE`, @c SQLITE_OPEN_PRIVATECACHE , and/or @c SQLITE_OPEN_URI flags:
 
 @param vfsName   If vfs is given the value is passed to the vfs parameter of sqlite3_open_v2.
 
 @return @c YES if successful, @c NO on error.
 */

- (BOOL)openWithFlags:(int)flags vfs:(NSString * _Nullable)vfsName;

/** Closing a database connection
 */

- (BOOL)close;

/** Test to see if we have a good connection to the database.
 @return @c YES if everything succeeds, @c NO on failure.
 */

@property (nonatomic, readonly) BOOL goodConnection;


///----------------------
/// @name Perform updates
///----------------------

/** Execute single update statement

 @see [`sqlite3_bind`](https://sqlite.org/c3ref/bind_blob.html)
 */

- (BOOL)executeUpdate:(NSString*)sql withErrorAndBindings:(NSError * _Nullable __autoreleasing *)outErr, ...;

/** Execute single update statement
 */

- (BOOL)executeUpdate:(NSString*)sql, ...;

/** Execute single update statement
 */

- (BOOL)executeUpdateWithFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(1,2);

/** Execute single update statement
 */

- (BOOL)executeUpdate:(NSString*)sql withArgumentsInArray:(NSArray *)arguments;

/** Execute single update statement
 
 */

- (BOOL)executeUpdate:(NSString*)sql values:(NSArray * _Nullable)values error:(NSError * _Nullable __autoreleasing *)error;

/** Execute single update statement
 
*/

- (BOOL)executeUpdate:(NSString*)sql withParameterDictionary:(NSDictionary *)arguments;


/** Execute single update statement

 */

- (BOOL)executeUpdate:(NSString*)sql withVAList: (va_list)args;

/** Execute multiple SQL statements

 */

- (BOOL)executeStatements:(NSString *)sql;

/** Execute multiple SQL statements with callback handler
 
 */

- (BOOL)executeStatements:(NSString *)sql withResultBlock:(__attribute__((noescape)) YFDBExecuteStatementsCallbackBlock _Nullable)block;

/** Last insert rowid

 */

@property (nonatomic, readonly) int64_t lastInsertRowId;

/** The number of rows changed by prior SQL statement.
 
 */

@property (nonatomic, readonly) int changes;


///-------------------------
/// @name Retrieving results
///-------------------------

/** Execute select statement

 */

- (YFResultSet * _Nullable)executeQuery:(NSString*)sql, ...;

/** Execute select statement
 
 */

- (YFResultSet * _Nullable)executeQueryWithFormat:(NSString*)format, ... NS_FORMAT_FUNCTION(1,2);

/** Execute select statement

 */

- (YFResultSet * _Nullable)executeQuery:(NSString *)sql withArgumentsInArray:(NSArray *)arguments;

/** Execute select statement

 */

- (YFResultSet * _Nullable)executeQuery:(NSString *)sql values:(NSArray * _Nullable)values error:(NSError * _Nullable __autoreleasing *)error;

/** Execute select statement

 */

- (YFResultSet * _Nullable)executeQuery:(NSString *)sql withParameterDictionary:(NSDictionary * _Nullable)arguments;


// Documentation forthcoming.
- (YFResultSet * _Nullable)executeQuery:(NSString *)sql withVAList:(va_list)args;

/// Prepare SQL statement.
///
/// @param sql SQL statement to prepare, generally with `?` placeholders.

- (YFResultSet *)prepare:(NSString *)sql;

///-------------------
/// @name Transactions
///-------------------

/** Begin a transaction
 
 @return @c YES on success; @c NO on failure. If failed, you can call @c lastError , @c lastErrorCode , or @c lastErrorMessage  for diagnostic information regarding the failure.

 */

- (BOOL)beginTransaction;

/** Begin a deferred transaction
 
 */

- (BOOL)beginDeferredTransaction;

/** Begin an immediate transaction
 
 @return @c YES on success; @c NO on failure. If failed, you can call @c lastError , @c lastErrorCode , or @c lastErrorMessage  for diagnostic information regarding the failure.
 */

- (BOOL)beginImmediateTransaction;

/** Begin an exclusive transaction
 
 @return @c YES on success; @c NO on failure. If failed, you can call @c lastError , @c lastErrorCode , or @c lastErrorMessage  for diagnostic information regarding the failure.
 */

- (BOOL)beginExclusiveTransaction;

/** Commit a transaction
 */

- (BOOL)commit;

/** Rollback a transaction

 */

- (BOOL)rollback;

/** Identify whether currently in a transaction or not
 */

@property (nonatomic, readonly) BOOL isInTransaction;

///----------------------------------------
/// @name Cached statements and result sets
///----------------------------------------

/** Clear cached statements */

- (void)clearCachedStatements;

/** Close all open result sets */

- (void)closeOpenResultSets;

/** Whether database has any open result sets
 
 @return @c YES if there are open result sets; @c NO if not.
 */

@property (nonatomic, readonly) BOOL hasOpenResultSets;

/** Whether should cache statements or not
  */

@property (nonatomic) BOOL shouldCacheStatements;

/** Interupt pending database operation
 
 This method causes any pending database operation to abort and return at its earliest opportunity
 
 @return @c YES on success; @c NO on failure. If failed, you can call @c lastError , @c lastErrorCode , or @c lastErrorMessage  for diagnostic information regarding the failure.
 
 */

- (BOOL)interrupt;

///-------------------------
/// @name Encryption methods
///-------------------------

/** Set encryption key.
 
 */

- (BOOL)setKey:(NSString*)key;

/** Reset encryption key

 @param key The key to be used.

 @return @c YES if success, @c NO on error.
 */

- (BOOL)rekey:(NSString*)key;

/** Set encryption key using `keyData`.
 
 */

- (BOOL)setKeyWithData:(NSData *)keyData;

/** Reset encryption key using `keyData`.

 @param keyData The @c NSData  to be used.

 @return @c YES if success, @c NO on error.
 */

- (BOOL)rekeyWithData:(NSData *)keyData;


///------------------------------
/// @name General inquiry methods
///------------------------------

/** The path of the database file.
 */

@property (nonatomic, readonly, nullable) NSString *databasePath;

/** The file URL of the database file.
 */

@property (nonatomic, readonly, nullable) NSURL *databaseURL;

/** The underlying SQLite handle .
 
 @return The `sqlite3` pointer.
 
 */

@property (nonatomic, readonly) void *sqliteHandle;


///-----------------------------
/// @name Retrieving error codes
///-----------------------------

/** Last error message
 
 Returns the English-language text that describes the most recent failed SQLite API call associated with a database connection. If a prior API call failed but the most recent API call succeeded, this return value is undefined.

 @return @c NSString  of the last error message.
 */

- (NSString*)lastErrorMessage;

/** Last error code
 
 Returns the numeric result code or extended result code for the most recent failed SQLite API call associated with a database connection. If a prior API call failed but the most recent API call succeeded, this return value is undefined.
 
 @return Integer value of the last error code.
 */

- (int)lastErrorCode;

/** Last extended error code
 
 Returns the numeric extended result code for the most recent failed SQLite API call associated with a database connection. If a prior API call failed but the most recent API call succeeded, this return value is undefined.
 
 @return Integer value of the last extended error code.
 */

- (int)lastExtendedErrorCode;

/** Had error

 @return @c YES if there was an error, @c NO if no error.
 */

- (BOOL)hadError;

/** Last error

 @return @c NSError  representing the last error.
 */

- (NSError *)lastError;


// description forthcoming
@property (nonatomic) NSTimeInterval maxBusyRetryTimeInterval;


///------------------
/// @name Save points
///------------------

/** Start save point
 
 @param name Name of save point.
 
 @param outErr A @c NSError  object to receive any error object (if any).
 
 @return @c YES on success; @c NO on failure. If failed, you can call @c lastError , @c lastErrorCode , or @c lastErrorMessage  for diagnostic information regarding the failure.
 */

- (BOOL)startSavePointWithName:(NSString*)name error:(NSError * _Nullable __autoreleasing *)outErr;

/** Release save point

 @param name Name of save point.
 
 @param outErr A @c NSError  object to receive any error object (if any).
 
 @return @c YES on success; @c NO on failure. If failed, you can call @c lastError , @c lastErrorCode , or @c lastErrorMessage  for diagnostic information regarding the failure.
 
 */

- (BOOL)releaseSavePointWithName:(NSString*)name error:(NSError * _Nullable __autoreleasing *)outErr;

/** Roll back to save point

 @param name Name of save point.
 @param outErr A @c NSError  object to receive any error object (if any).
 
 @return @c YES on success; @c NO on failure. If failed, you can call @c lastError , @c lastErrorCode , or @c lastErrorMessage  for diagnostic information regarding the failure.
 
 */

- (BOOL)rollbackToSavePointWithName:(NSString*)name error:(NSError * _Nullable __autoreleasing *)outErr;

/** Start save point

 @param block Block of code to perform from within save point.
 
 @return The NSError corresponding to the error, if any. If no error, returns @c nil .

 */

- (NSError * _Nullable)inSavePoint:(__attribute__((noescape)) void (^)(BOOL *rollback))block;


///-----------------
/// @name Checkpoint
///-----------------

/** Performs a WAL checkpoint
 
 @param checkpointMode The checkpoint mode for @c sqlite3_wal_checkpoint_v2
 @param error The @c NSError corresponding to the error, if any.
 @return @c YES on success, otherwise @c NO .
 */
- (BOOL)checkpoint:(YFDBCheckpointMode)checkpointMode error:(NSError * _Nullable *)error;

/** Performs a WAL checkpoint
 
 @param checkpointMode The checkpoint mode for @c sqlite3_wal_checkpoint_v2
 @param name The db name for @c sqlite3_wal_checkpoint_v2
 @param error The @c NSError corresponding to the error, if any.
 @return @c YES on success, otherwise @c NO .
 */
- (BOOL)checkpoint:(YFDBCheckpointMode)checkpointMode name:(NSString * _Nullable)name error:(NSError * _Nullable *)error;

/** Performs a WAL checkpoint
 
 @param checkpointMode The checkpoint mode for sqlite3_wal_checkpoint_v2
 @param name The db name for sqlite3_wal_checkpoint_v2
 @param error The NSError corresponding to the error, if any.
 @param logFrameCount If not @c NULL , then this is set to the total number of frames in the log file or to -1 if the checkpoint could not run because of an error or because the database is not in WAL mode.
 @param checkpointCount If not @c NULL , then this is set to the total number of checkpointed frames in the log file (including any that were already checkpointed before the function was called) or to -1 if the checkpoint could not run due to an error or because the database is not in WAL mode.
 @return @c YES on success, otherwise @c NO .
 */
- (BOOL)checkpoint:(YFDBCheckpointMode)checkpointMode name:(NSString * _Nullable)name logFrameCount:(int * _Nullable)logFrameCount checkpointCount:(int * _Nullable)checkpointCount error:(NSError * _Nullable *)error;

///----------------------------
/// @name SQLite library status
///----------------------------

/** Test to see if the library is threadsafe

 @return @c NO if and only if SQLite was compiled with mutexing code omitted due to the @c SQLITE_THREADSAFE compile-time option being set to 0.

 @see [sqlite3_threadsafe()](https://sqlite.org/c3ref/threadsafe.html)
 */

+ (BOOL)isSQLiteThreadSafe;

/** Examine/set limits

 @param type The type of limit. See https://sqlite.org/c3ref/c_limit_attached.html
 @param newLimit The new limit value. Use -1 if you don't want to change the limit, but rather only want to check it.

 @return Regardless, returns previous value.

 @see [sqlite3_limit()](https://sqlite.org/c3ref/limit.html)
*/

- (int)limitFor:(int)type value:(int)newLimit;

/** Run-time library version numbers
 
 @return The sqlite library version string.
 
 @see [sqlite3_libversion()](https://sqlite.org/c3ref/libversion.html)
 */

+ (NSString*)sqliteLibVersion;

/// The YFDB version number as a string in the form of @c "2.7.8" .
///
/// If you want to compare version number strings, you can use NSNumericSearch option:
///
/// @code
/// NSComparisonResult result = [[YFDatabase YFDBUserVersion] compare:@"2.11.0" options:NSNumericSearch];
/// @endcode
///
/// @returns The version number string.

+ (NSString*)YFDBUserVersion;

/** The YFDB version

 This returns the YFDB as hexadecimal value, e.g., @c 0x0243 for version 2.4.3.

 @warning This routine will not work if any component of the version number exceeds 15.
       For example, if it is version @c 2.17.3 , this will max out at @c 0x2f3.
       For this reason, we would recommend using @c YFDBUserVersion  and with @c NSNumericSearch  option, e.g.

 @code
 NSComparisonResult result = [[YFDatabase YFDBUserVersion] compare:@"2.11.0" options:NSNumericSearch];
 @endcode

 @returns The version number in hexadecimal, e.g., @c 0x0243 for version 2.4.3. If any component exceeds what can be
       can be represented in four bits, we'll max it out at @c 0xf .
 */

+ (SInt32)YFDBVersion __deprecated_msg("Use YFDBUserVersion instead");

///------------------------
/// @name Make SQL function
///------------------------

/** Adds SQL functions or aggregates or to redefine the behavior of existing SQL functions or aggregates.
 
 For example:

@code
[db makeFunctionNamed:@"RemoveDiacritics" arguments:1 block:^(void *context, int argc, void **argv) {
    SqliteValueType type = [self.db valueType:argv[0]];
    if (type == SqliteValueTypeNull) {
        [self.db resultNullInContext:context];
         return;
    }
    if (type != SqliteValueTypeText) {
        [self.db resultError:@"Expected text" context:context];
        return;
    }
    NSString *string = [self.db valueString:argv[0]];
    NSString *result = [string stringByFoldingWithOptions:NSDiacriticInsensitiveSearch locale:nil];
    [self.db resultString:result context:context];
}];

YFResultSet *rs = [db executeQuery:@"SELECT * FROM employees WHERE RemoveDiacritics(first_name) LIKE 'jose'"];
NSAssert(rs, @"Error %@", [db lastErrorMessage]);
@endcode

 @param name Name of function.

 @param arguments Maximum number of parameters.

 @param block The block of code for the function.

 @see [sqlite3_create_function()](https://sqlite.org/c3ref/create_function.html)
 */

- (void)makeFunctionNamed:(NSString *)name arguments:(int)arguments block:(void (^)(void *context, int argc, void * _Nonnull * _Nonnull argv))block;

- (void)makeFunctionNamed:(NSString *)name maximumArguments:(int)count withBlock:(void (^)(void *context, int argc, void * _Nonnull * _Nonnull argv))block __deprecated_msg("Use makeFunctionNamed:arguments:block:");

//- (SqliteValueType)valueType:(void *)argv;

/**
 Get integer value of parameter in custom function.
 
 @param value The argument whose value to return.
 @return The integer value.
 
 @see makeFunctionNamed:arguments:block:
 */
- (int)valueInt:(void *)value;

/**
 Get long value of parameter in custom function.
 
 @param value The argument whose value to return.
 @return The long value.
 
 @see makeFunctionNamed:arguments:block:
 */
- (long long)valueLong:(void *)value;

/**
 Get double value of parameter in custom function.
 
 @param value The argument whose value to return.
 @return The double value.
 
 @see makeFunctionNamed:arguments:block:
 */
- (double)valueDouble:(void *)value;

/**
 Get @c NSData  value of parameter in custom function.
 
 @param value The argument whose value to return.
 @return The data object.
 
 @see makeFunctionNamed:arguments:block:
 */
- (NSData * _Nullable)valueData:(void *)value;

/**
 Get string value of parameter in custom function.
 
 @param value The argument whose value to return.
 @return The string value.
 
 @see makeFunctionNamed:arguments:block:
 */
- (NSString * _Nullable)valueString:(void *)value;

/**
 Return null value from custom function.
 
 @param context The context to which the null value will be returned.
 
 @see makeFunctionNamed:arguments:block:
 */
- (void)resultNullInContext:(void *)context NS_SWIFT_NAME(resultNull(context:));

/**
 Return integer value from custom function.
 
 @param value The integer value to be returned.
 @param context The context to which the value will be returned.
 
 @see makeFunctionNamed:arguments:block:
 */
- (void)resultInt:(int) value context:(void *)context;

/**
 Return long value from custom function.
 
 @param value The long value to be returned.
 @param context The context to which the value will be returned.
 
 @see makeFunctionNamed:arguments:block:
 */
- (void)resultLong:(long long)value context:(void *)context;

/**
 Return double value from custom function.
 
 @param value The double value to be returned.
 @param context The context to which the value will be returned.
 
 @see makeFunctionNamed:arguments:block:
 */
- (void)resultDouble:(double)value context:(void *)context;

/**
 Return @c NSData  object from custom function.
 
 @param data The @c NSData  object to be returned.
 @param context The context to which the value will be returned.
 
 @see makeFunctionNamed:arguments:block:
 */
- (void)resultData:(NSData *)data context:(void *)context;

/**
 Return string value from custom function.
 
 @param value The string value to be returned.
 @param context The context to which the value will be returned.
 
 @see makeFunctionNamed:arguments:block:
 */
- (void)resultString:(NSString *)value context:(void *)context;

/**
 Return error string from custom function.
 
 @param error The error string to be returned.
 @param context The context to which the error will be returned.
 
 @see makeFunctionNamed:arguments:block:
 */
- (void)resultError:(NSString *)error context:(void *)context;

/**
 Return error code from custom function.
 
 @param errorCode The integer error code to be returned.
 @param context The context to which the error will be returned.
 
 @see makeFunctionNamed:arguments:block:
 */
- (void)resultErrorCode:(int)errorCode context:(void *)context;

/**
 Report memory error in custom function.
 
 @param context The context to which the error will be returned.
 
 @see makeFunctionNamed:arguments:block:
 */
- (void)resultErrorNoMemoryInContext:(void *)context NS_SWIFT_NAME(resultErrorNoMemory(context:));

/**
 Report that string or BLOB is too long to represent in custom function.
 
 @param context The context to which the error will be returned.
 
 @see makeFunctionNamed:arguments:block:
 */
- (void)resultErrorTooBigInContext:(void *)context NS_SWIFT_NAME(resultErrorTooBig(context:));

///---------------------
/// @name Date formatter
///---------------------

/** Generate an @c NSDateFormatter  that won't be broken by permutations of timezones or locales.
 
 Use this method to generate values to set the dateFormat property.
 
 Example:

@code
myDB.dateFormat = [YFDatabase storeableDateFormat:@"yyyy-MM-dd HH:mm:ss"];
@endcode

 @param format A valid NSDateFormatter format string.
 
 @return A @c NSDateFormatter  that can be used for converting dates to strings and vice versa.
 
 @see hasDateFormatter
 @see setDateFormat:
 @see dateFromString:
 @see stringFromDate:
 @see storeableDateFormat:

 @warning Note that @c NSDateFormatter  is not thread-safe, so the formatter generated by this method should be assigned to only one YFDB instance and should not be used for other purposes.

 */

+ (NSDateFormatter *)storeableDateFormat:(NSString *)format;

/** Test whether the database has a date formatter assigned.
 
 @return @c YES if there is a date formatter; @c NO if not.
 
 @see hasDateFormatter
 @see setDateFormat:
 @see dateFromString:
 @see stringFromDate:
 @see storeableDateFormat:
 */

- (BOOL)hasDateFormatter;

/** Set to a date formatter to use string dates with sqlite instead of the default UNIX timestamps.
 
 @param format Set to nil to use UNIX timestamps. Defaults to nil. Should be set using a formatter generated using @c YFDatabase:storeableDateFormat .
 
 @see hasDateFormatter
 @see setDateFormat:
 @see dateFromString:
 @see stringFromDate:
 @see storeableDateFormat:
 
 @warning Note there is no direct getter for the @c NSDateFormatter , and you should not use the formatter you pass to YFDB for other purposes, as @c NSDateFormatter  is not thread-safe.
 */

- (void)setDateFormat:(NSDateFormatter * _Nullable)format;

/** Convert the supplied NSString to NSDate, using the current database formatter.
 
 @param s @c NSString  to convert to @c NSDate .
 
 @return The @c NSDate  object; or @c nil  if no formatter is set.
 
 @see hasDateFormatter
 @see setDateFormat:
 @see dateFromString:
 @see stringFromDate:
 @see storeableDateFormat:
 */

- (NSDate * _Nullable)dateFromString:(NSString *)s;

/** Convert the supplied NSDate to NSString, using the current database formatter.
 
 @param date @c NSDate  of date to convert to @c NSString .

 @return The @c NSString  representation of the date; @c nil  if no formatter is set.
 
 @see hasDateFormatter
 @see setDateFormat:
 @see dateFromString:
 @see stringFromDate:
 @see storeableDateFormat:
 */

- (NSString * _Nullable)stringFromDate:(NSDate *)date;

@end


/** Objective-C wrapper for @c sqlite3_stmt
 
 This is a wrapper for a SQLite @c sqlite3_stmt . Generally when using YFDB you will not need to interact directly with `YFStatement`, but rather with @c YFDatabase  and @c YFResultSet  only.
 
 See also
 
 - @c YFDatabase
 - @c YFResultSet
 - [@c sqlite3_stmt ](https://sqlite.org/c3ref/stmt.html)
 */

@interface YFStatement : NSObject {
    void *_statement;
    NSString *_query;
    long _useCount;
    BOOL _inUse;
}

///-----------------
/// @name Properties
///-----------------

/** Usage count */

@property (atomic, assign) long useCount;

/** SQL statement */

@property (atomic, retain) NSString *query;

/** SQLite sqlite3_stmt
 
 @see [@c sqlite3_stmt ](https://sqlite.org/c3ref/stmt.html)
 */

@property (atomic, assign) void *statement;

/** Indication of whether the statement is in use */

@property (atomic, assign) BOOL inUse;

///----------------------------
/// @name Closing and Resetting
///----------------------------

/** Close statement */

- (void)close;

/** Reset statement */

- (void)reset;

@end

NS_ASSUME_NONNULL_END
