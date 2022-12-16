//
//  GTPushBridge.m
//  HelloWorld-mobile
//
//  Created by  joy on 2022/12/15.
//

#import "GTPushBridge.h"
#import "GTPushOCBridge.h"
#include <string>
#include <cocos2d.h>
#include <map>

using namespace cocos2d;

const char* GTPushBridge::getVersion(){
    NSString *version = [GeTuiSdk version];
    const char *usedStr = [version cStringUsingEncoding:NSUTF8StringEncoding];
    return usedStr;
}

const char* GTPushBridge::getClientId(){
    NSString *version = [GeTuiSdk clientId];
    const char *usedStr = [version cStringUsingEncoding:NSUTF8StringEncoding];
    return usedStr;
}

int GTPushBridge::getStatus() {
    return (int)[GeTuiSdk status];
}

void GTPushBridge::setPushModeForOff(bool isValue) {
    [GeTuiSdk setPushModeForOff:isValue];
}

void GTPushBridge::destroy(){
    [GeTuiSdk destroy];
}

bool GTPushBridge::setTags(vector<string> tags, const char* sn){
    int ret = NO;
    if(tags.size() == 0) {
        return ret;
    }
    NSMutableArray *usedTags = [NSMutableArray array];
    for(NSInteger i = 0; i < tags.size(); i++) {
        string tmp = tags[i];
        NSString *tmpStr = [NSString stringWithUTF8String:tmp.c_str()];
        [usedTags addObject:tmpStr];
    }
    NSString *usedSn = [NSString stringWithUTF8String:sn];
    ret = [GeTuiSdk setTags:usedTags andSequenceNum:usedSn];
    return ret;
}

void GTPushBridge::setBadge(int badge) {
    [GeTuiSdk setBadge:badge];
}

void GTPushBridge::resetBadge() {
    [GeTuiSdk resetBadge];
}

bool GTPushBridge::sendFeedbackMessage(const char *taskId, const char *messageId, int actionId) {
    NSString *tid = [NSString stringWithUTF8String:taskId];
    NSString *mid = [NSString stringWithUTF8String:messageId];
    return [GeTuiSdk sendFeedbackMessage:actionId andTaskId:tid andMsgId:mid];
}

void GTPushBridge::bindAlias(const char *alias, const char *aSn) {
    NSString *aliasStr = [NSString stringWithUTF8String:alias];
    NSString *aSnStr = [NSString stringWithUTF8String:aSn];
    [GeTuiSdk bindAlias:aliasStr andSequenceNum:aSnStr];
}

void GTPushBridge::unBindAlias(const char* alias, const char* aSn, bool isSelf) {
    NSString *aliasStr = [NSString stringWithUTF8String:alias];
    NSString *aSnStr = [NSString stringWithUTF8String:aSn];
    [GeTuiSdk unbindAlias:aliasStr andSequenceNum:aSnStr andIsSelf:isSelf];
}


@interface GTPushOCBridge () {
    @public
    GeTuiSdkDidRegisterClient_callback clientCallBack;
    GeTuiSDkDidNotifySdkState_callback stateCallback;
    GeTuiSdkWillPresentNotification_callback willPresentCallback;
    GeTuiSdkDidReceiveNotification_callback didReceiveNotiCallback;
    GeTuiSdkDidReceiveSlience_callback didReceiveSlienceCallBack;
    GeTuiSdkDidSendMessage_callback didSendMessageCallback;
    GeTuiSdkDidSetPushMode_callback didSetPushModeCallback;
    GeTuiSdkDidAliasAction_callback aliasActionCallback;
    GeTuiSdkDidSetTagsAction_callback tagsActionCallback;
    GetuiSdkDidQueryTag_callback queryTagCallback;
}

@end

void GTPushBridge::registerNotificationCallback(GeTuiSdkDidRegisterClient_callback clientCallBack, GeTuiSDkDidNotifySdkState_callback stateCallback, GeTuiSdkWillPresentNotification_callback willPresentCallback, GeTuiSdkDidReceiveNotification_callback didReceiveNotiCallback, GeTuiSdkDidReceiveSlience_callback didReceiveSlienceCallBack, GeTuiSdkDidSendMessage_callback didSendMessageCallback, GeTuiSdkDidSetPushMode_callback didSetPushModeCallback, GeTuiSdkDidAliasAction_callback aliasActionCallback, GeTuiSdkDidSetTagsAction_callback tagsActionCallback,
    GetuiSdkDidQueryTag_callback queryTagCallback) {
    GTPushOCBridge *bridge = [GTPushOCBridge sharedInstance];
    if(clientCallBack != NULL) {
        bridge->clientCallBack = clientCallBack;
    }
    if(stateCallback != NULL) {
        bridge->stateCallback = stateCallback;
    }
    if(willPresentCallback != NULL) {
        bridge->willPresentCallback = willPresentCallback;
    }
    if(didReceiveNotiCallback != NULL) {
        bridge->didReceiveNotiCallback = didReceiveNotiCallback;
    }
    if(didReceiveSlienceCallBack != NULL) {
        bridge->didReceiveSlienceCallBack = didReceiveSlienceCallBack;
    }
    if(didSendMessageCallback != NULL) {
        bridge->didSendMessageCallback = didSendMessageCallback;
    }
    if(didSetPushModeCallback != NULL) {
        bridge->didSetPushModeCallback = didSetPushModeCallback;
    }
    if(aliasActionCallback != NULL) {
        bridge->aliasActionCallback = aliasActionCallback;
    }
    if(tagsActionCallback != NULL) {
        bridge->tagsActionCallback = tagsActionCallback;
    }
    if(queryTagCallback != NULL) {
        bridge->queryTagCallback = queryTagCallback;
    }
}

@implementation GTPushOCBridge

+ (instancetype)sharedInstance {
    static GTPushOCBridge *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [GTPushOCBridge new];
    });
    return instance;
}

//采用json转化为c字符串
- (const char *)convertToCStrWithJSONObject:(id)obj {
    const char *result;
    NSError *error = nil;
    NSData *jsonData =
        [NSJSONSerialization dataWithJSONObject:obj
                                        options:NSJSONWritingPrettyPrinted
                                          error:&error];
    if ([jsonData length] > 0 && error == nil) {
      NSString *jsonString =
          [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
      result = [jsonString UTF8String];
    } else {
      result = NULL;
    }
    return result;
}

#pragma mark -- GeTuiSdkDelegate
/**
 *  SDK登入成功返回clientId
 *
 *  @param clientId 标识用户的clientId
 *  说明:启动GeTuiSdk后，SDK会自动向个推服务器注册SDK，当成功注册时，SDK通知应用注册成功。
 *  注意: 注册成功仅表示推送通道建立，如果appid/appkey/appSecret等验证不通过，依然无法接收到推送消息，请确保验证信息正确。
 */
- (void)GeTuiSdkDidRegisterClient:(NSString *)clientId {
    if(clientCallBack == NULL) {
        return;
    }
    clientCallBack(clientId.UTF8String);
}

/**
 *  SDK运行状态通知
 *
 *  @param status 返回SDK运行状态
 */
- (void)GeTuiSDkDidNotifySdkState:(SdkStatus)status {
    if(stateCallback == NULL) {
        return;
    }
    stateCallback((int)status);
}

/**
 *  SDK遇到错误消息返回error
 *
 *  @param error SDK内部发生错误，通知第三方，返回错误
 */
- (void)GeTuiSdkDidOccurError:(NSError *)error {
    
}


//MARK: - 通知回调

/// 通知授权结果（iOS10及以上版本）
/// @param granted 用户是否允许通知
/// @param error 错误信息
- (void)GetuiSdkGrantAuthorization:(BOOL)granted error:(nullable NSError*)error {
    
}

/// 通知展示（iOS10及以上版本）
/// @param center center
/// @param notification notification
/// @param completionHandler completionHandler
- (void)GeTuiSdkNotificationCenter:(UNUserNotificationCenter *)center
           willPresentNotification:(UNNotification * )notification
                 completionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler {
    if(willPresentCallback != NULL) {
        const char *result = [self convertToCStrWithJSONObject:notification.request.content.userInfo];
        willPresentCallback(result);
    }
    
    if(completionHandler) {
        // [ 参考代码，开发者注意根据实际需求自行修改 ] 根据APP需要自行修改参数值
        completionHandler(UNNotificationPresentationOptionBadge | UNNotificationPresentationOptionSound | UNNotificationPresentationOptionAlert);
    }
}

 
/// 收到通知信息
/// @param userInfo apns通知内容
/// @param center UNUserNotificationCenter（iOS10及以上版本）
/// @param response UNNotificationResponse（iOS10及以上版本）
/// @param completionHandler 用来在后台状态下进行操作（iOS10以下版本）
- (void)GeTuiSdkDidReceiveNotification:(NSDictionary *)userInfo
                    notificationCenter:(nullable UNUserNotificationCenter *)center
                              response:(nullable UNNotificationResponse *)response
                fetchCompletionHandler:(nullable void (^)(UIBackgroundFetchResult))completionHandler {
    if(didReceiveNotiCallback != NULL) {
        const char *result = [self convertToCStrWithJSONObject:userInfo];
        didReceiveNotiCallback(result);
    }
    
    if(completionHandler) {
        // [ 参考代码，开发者注意根据实际需求自行修改 ] 根据APP需要自行修改参数值
        completionHandler(UIBackgroundFetchResultNoData);
    }
}


/// 收到透传消息
/// @param userInfo    推送消息内容,  {"payload": 消息内容}
/// @param fromGetui   YES: 个推通道  NO：苹果apns通道
/// @param offLine     是否是离线消息，YES.是离线消息
/// @param appId       应用的appId
/// @param taskId      推送消息的任务id
/// @param msgId       推送消息的messageid
/// @param completionHandler 用来在后台状态下进行操作（通过苹果apns通道的消息 才有此参数值）
- (void)GeTuiSdkDidReceiveSlience:(NSDictionary *)userInfo
                        fromGetui:(BOOL)fromGetui
                          offLine:(BOOL)offLine
                            appId:(nullable NSString *)appId
                           taskId:(nullable NSString *)taskId
                            msgId:(nullable NSString *)msgId
           fetchCompletionHandler:(nullable void (^)(UIBackgroundFetchResult))completionHandler {
    NSMutableDictionary *dic = [NSMutableDictionary dictionary];
    dic[@"fromGetui"] = @(fromGetui);
    dic[@"offLine"] = @(offLine);
    dic[@"appId"] = appId;
    dic[@"taskId"] = taskId;
    dic[@"msgId"] = msgId;
    if(didReceiveSlienceCallBack != NULL) {
        const char *result = [self convertToCStrWithJSONObject:dic];
        didReceiveSlienceCallBack(result);
    }
    if(completionHandler) {
        // [ 参考代码，开发者注意根据实际需求自行修改 ] 根据APP需要自行修改参数值
        completionHandler(UIBackgroundFetchResultNoData);
    }
}


- (void)GeTuiSdkNotificationCenter:(UNUserNotificationCenter *)center
       openSettingsForNotification:(nullable UNNotification *)notification {
    
}


//MARK: - 发送上行消息
/**
 *  SDK通知发送上行消息结果，收到sendMessage消息回调
 *
 *  @param messageId “sendMessage:error:”返回的id
 *  @param isSuccess    成功返回 YES, 失败返回 NO
 *  @param aError       成功返回nil, 错误返回相应error信息
 *  说明: 当调用sendMessage:error:接口时，消息推送到个推服务器，服务器通过该接口通知sdk到达结果，isSuccess为 YES 说明消息发送成功
 *  注意: 需第三方服务器接入个推,SendMessage 到达第三方服务器后返回 1
 */
- (void)GeTuiSdkDidSendMessage:(NSString *)messageId result:(BOOL)isSuccess error:(nullable NSError *)aError {
    if(didSendMessageCallback != NULL) {
        didSendMessageCallback(messageId.UTF8String, isSuccess);
    }
}


//MARK: - 开关设置

/**
 *  SDK设置关闭推送模式回调
 *
 *  @param isModeOff 关闭模式，YES.服务器关闭推送功能 NO.服务器开启推送功能
 *  @param error     错误回调，返回设置时的错误信息
 */
- (void)GeTuiSdkDidSetPushMode:(BOOL)isModeOff error:(nullable NSError *)error {
    if(didSetPushModeCallback != NULL) {
        didSetPushModeCallback(isModeOff);
    }
}


//MARK: - 别名设置
/**
 *  SDK绑定、解绑回调
 *
 *  @param action       回调动作类型 kGtResponseBindType 或 kGtResponseUnBindType
 *  @param isSuccess    成功返回 YES, 失败返回 NO
 *  @param aSn          返回请求的序列码
 *  @param aError       成功返回nil, 错误返回相应error信息
 */
- (void)GeTuiSdkDidAliasAction:(NSString *)action result:(BOOL)isSuccess sequenceNum:(NSString *)aSn error:(nullable NSError *)aError {
    if(aliasActionCallback != NULL) {
        const char *errorStr = [self convertToCStrWithJSONObject:aError.userInfo];
        aliasActionCallback(action.UTF8String, isSuccess, aSn.UTF8String , errorStr);
    }
}


//MARK: - 标签设置
/**
 *  设置标签回调
 *
 *  @param sequenceNum  请求的序列码
 *  @param isSuccess    成功返回 YES, 失败返回 NO
 *  @param aError       成功返回 nil, 错误返回相应error信息
 */
- (void)GeTuiSdkDidSetTagsAction:(NSString *)sequenceNum result:(BOOL)isSuccess error:(nullable NSError *)aError {
    if(tagsActionCallback != NULL) {
        const char *errorStr = [self convertToCStrWithJSONObject:aError.userInfo];
        tagsActionCallback(isSuccess, sequenceNum.UTF8String, errorStr);
    }
}

/**
 * 查询当前绑定tag结果返回
 * @param aTags   当前绑定的 tag 信息
 * @param aSn     返回 queryTag 接口中携带的请求序列码，标识请求对应的结果返回
 * @param aError  成功返回nil,错误返回相应error信息
 */
- (void)GetuiSdkDidQueryTag:(NSArray *)aTags sequenceNum:(NSString *)aSn error:(nullable NSError *)aError {
    if(queryTagCallback != NULL) {
        const char *tagsStr = [self convertToCStrWithJSONObject:aTags];
        const char *errorStr = [self convertToCStrWithJSONObject:aError.userInfo];
        queryTagCallback(tagsStr, aSn.UTF8String, errorStr);
    }
}


@end
