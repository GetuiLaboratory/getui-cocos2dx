//
//  GTPushBridge.h
//  HelloWorld-mobile
//
//  Created by  joy on 2022/12/15.
//

#include <string>
#include <map>
#include <set>

using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
typedef void (*GeTuiSdkDidRegisterClient_callback)(const char* clientId);
typedef void (*GeTuiSDkDidNotifySdkState_callback)(int status);
typedef void (*GeTuiSdkWillPresentNotification_callback)(const char* message);
typedef void (*GeTuiSdkDidReceiveNotification_callback)(const char* message);
typedef void (*GeTuiSdkDidReceiveSlience_callback)(const char* message);
typedef void (*GeTuiSdkDidSendMessage_callback)(const char* messageid,  bool isSuccess);
typedef void (*GeTuiSdkDidSetPushMode_callback)(bool isModeOff);
typedef void (*GeTuiSdkDidAliasAction_callback)(const char* action, bool isSuccess, const char* aSn, const char* error);
typedef void (*GeTuiSdkDidSetTagsAction_callback)(bool isSuccess, const char* aSn, const char* error);
typedef void (*GetuiSdkDidQueryTag_callback)(const char* tags, const char* aSn, const char* error);
#endif

#define COCOS2D_DEBUG 1

class GTPushBridge {
    public:

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    //获取个推版本号
    static const char* getVersion();

    //获取clientId
    static const char* getClientId();

    //获取运行状态 0正在启动 1启动、在线 2停止 3离线
    static int getStatus();

    /**
     *  设置关闭推送模式（默认值：NO）
     *  需要SDK在线才能调用
     *
     *  @param isValue 消息推送开发，YES.关闭消息推送 NO.开启消息推送
     *
     *  SDK-1.2.1+
     *
     */
    static void setPushModeForOff(bool isValue);

    //销毁SDK，并且释放资源
    static void destroy();

    /**
    *  给用户打标签, 后台可以根据标签进行推送
    *
    *  @param tags 别名数组
    *  tag: 只能包含中文字符、英文字母、0-9、+-*_.的组合（不支持空格）
    *  @param aSn  绑定序列码, 不为nil
    *  @return 提交结果，YES表示尝试提交成功，NO表示尝试提交失败
    */
    static bool setTags(vector<string> tags, const char* sn);

    /**
     *  同步角标值到个推服务器
     *  该方法只是同步角标值到个推服务器，本地仍须调用setApplicationIconBadgeNumber函数
     *
     *  SDK-1.4.0+
     *
     *  @param badge 角标数值
     */
    static void setBadge(int badge);

    /**
     *  复位角标，等同于"setBadge:0"
     *
     *  SDK-1.4.0+
     *
     */
    static void resetBadge();
    
    /**
     *  上行第三方自定义回执actionid
     *
     *  @param actionId 用户自定义的actionid，int类型，取值90001-90999。
     *  @param taskId   下发任务的任务ID
     *  @param msgId    下发任务的消息ID
     *
     *  @return BOOL，YES表示尝试提交成功，NO表示尝试提交失败。注：该结果不代表服务器收到该条数据
     *  该方法需要在回调方法“GeTuiSdkDidReceivePayload:andTaskId:andMessageId:andOffLine:fromApplication:”使用
     */
    static bool sendFeedbackMessage(const char* taskId, const char* messageId, int actionId);
    
    

    /**
     *  绑定别名功能:后台可以根据别名进行推送
     *  需要SDK在线才能调用
     *
     *  @param alias 别名字符串
     *  @param aSn   绑定序列码, 不为nil
     */
    static void bindAlias(const char* alias, const char* aSn);

    /**
     *  取消绑定别名功能
     *  需要SDK在线才能调用
     *
     *  @param alias   别名字符串
     *  @param aSn     绑定序列码, 不为nil
     *  @param isSelf  是否只对当前cid有效，如果是true，只对当前cid做解绑；如果是false，对所有绑定该别名的cid列表做解绑
     */
    static void unBindAlias(const char* alias, const char* aSn, bool isSelf);

    //注册回调
    static void registerNotificationCallback(
        GeTuiSdkDidRegisterClient_callback clientCallBack,
        GeTuiSDkDidNotifySdkState_callback stateCallback,
        GeTuiSdkWillPresentNotification_callback willPresentCallback,
        GeTuiSdkDidReceiveNotification_callback didReceiveNotiCallback,
        GeTuiSdkDidReceiveSlience_callback didReceiveSlienceCallBack,
        GeTuiSdkDidSendMessage_callback didSendMessageCallback,
        GeTuiSdkDidSetPushMode_callback didSetPushModeCallback,
        GeTuiSdkDidAliasAction_callback aliasActionCallback,
        GeTuiSdkDidSetTagsAction_callback tagsActionCallback,
        GetuiSdkDidQueryTag_callback queryTagCallback
    );
#endif

};

