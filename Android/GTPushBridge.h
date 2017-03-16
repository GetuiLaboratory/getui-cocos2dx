#ifndef GTPUSHBRIDGE_H
#define GTPUSHBRIDGE_H

#include <string>
#include <map>
#include <set>
#include <jni.h>

using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
typedef void (*GTReceiveServicePid_callback)(void *p_handle, int pid);
typedef void (*GTReceiveClientId_callback)(void *p_handle, const char* clientId);
typedef void (*GTReceiveMessageData_callback)(void *p_handle, const char* message);
typedef void (*GTReceiveOnlineState)(void *p_handle,  bool online);
typedef void (*GTReceiveCommandResult)(void *p_handle, const char* message);

#endif

#define COCOS2D_DEBUG 1

class GTPushBridge {
  public:

  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

  //获取个推版本号
  static const char* getVersion();

  //获取clientId
  static const char* getClientId();

  //获取推送服务是否已经开启
  static bool isPushTurnOn();

  //开启推送
  static void turnOnPush();

  //关闭推送
  static void turnOffPush();

  //关闭推送服务
  static void stopService();

  /**
  * 设置标签
  * @param tags 用户标签集合
  * @param sn 用户自定义的序列号，用来唯一标识该动作，会在回调中回执该结果
  * @return 执行结果
  * 0：成功
  * 20001：tag 数量过大(单次设置的tag数量不超过100)
  * 20002：设置频率过快(频率限制每小时一次)
  * 20003：标签重复
  * 20004：服务初始化失败
  * 20005：setTag 异常
  * 20006：tag 为空
  * 20007：sn为空
  * 20008：离线,还未登陆成功
  * 20009：该 appid 已经在黑名单列表
  *  20010：已存 tag 数目超限
  */
  static int setTag(string *tags, const char* sn);

  /**
  * 设置静默时间
  * @param beginHour 开始时间，设置范围在0-23小时之间，单位为h
  * @param duration 持续时间，设置范围在0-23小时之间。持续时间为0则不静默，单位h
  * @return 设置结果
  */
  static bool setSilentTime(int beginHour, int duration);

  /**
  * 发送自定义回执
  * @param taskId 下发任务的任务ID
  * @param messageId 下发任务的消息ID
  * @param actionId 用户自定义的actionId， int类型，取值90001-909999
  * @return 上行结果
  */
  static bool sendFeedbackMessage(const char* taskId, const char* messageId, int actionId);

  /**
  * 绑定别名
  * 同一个别名最多绑定10个 ClientID (适用于允许多设备同时登陆的应用)，当已绑定10个 ClientID 时，再次调用此接口会自动解绑最早绑定的记录，
  * 当ClientID已绑定了别名A，若调用此接口绑定别名B，则与别名A的绑定关系会自动解除。
  * 此接口与 unBindAlias 一天内最多调用100次，两次调用的间隔需大于5s。
  * @param alias 别名名称，长度40字节，支持中、英文（区分大小写）、数字以及下划线
  * @return 绑定结果
  */
  static bool bindAlias(const char* alias);

  /**
  * 解绑别名
  * 只能解绑当前手机 ClientID 与别名的关系，不能解绑其他手机上 ClientID 与别名的关系。
  * @param alias 别名名称
  * @param isSelf 是否只对当前cid有效，如果是 true，只对当前cid做解绑；如果是 false，对所有绑定该别名的cid列表做解绑
  * @return 解绑结果
  */
  static bool unBindAlias(const char* alias, bool isSelf);

  static void registerNotificationCallback(
    void *p_handle,
    GTReceiveServicePid_callback pid_callback,
    GTReceiveClientId_callback clientId_callback,
    GTReceiveMessageData_callback messageData_callback,
    GTReceiveOnlineState onlineState_callback,
    GTReceiveCommandResult commandResult_callback
  );

  #endif


};

#endif
