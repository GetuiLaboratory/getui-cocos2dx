# getui-cocos2dx

个推官方提供的 Cocos2d-x插件 

## Android Studio 自动集成

目前最新版本jar包为4.3.5.9, 已更新至libso3

目前自动集成的方式仅支持Android Studio

- 将`getui-cocos2dx`文件夹放置于 `YourProjectName/cocos2d/plugin/glugins/` 目录下

- 使用命令行工具，进入插件目录 `getui-cocos2dx/Android/` 执行自动安装脚本

  ```
	./install_android_studio.py -project PROJECT_NAME -package PACKAGE_NAME -appId APP_ID -appKey APP_KEY -appSecret APP_SECRET
	
  ```
  显示 `GeTui SDK installed successfullfy!` 则安装成功！


- 由于cocos2dx的版本不同，可能导致工程目录不同，若因为工程目录不同导致自动安装失败时，可尝试手动集成，自动集成需要的目录如下


			  ┬ YourProjectName/
			  ├── /Classes/
			  ├─┬ /proj.android/
			  │ ├── /jni/
			  │ ├── /libs/
			  │ └── /src/
			  ├─┬ /proj.android-studio/app/
			  | ├── /jni/
			  | ├── /libs/
			  | └── /src/
			  └─┬ /cocos2d
			  	├── plugin/plugins/getui-cocos2dx/Android/


## Android Studio 手动集成

- 添加GeTui SDK maven地址，在`/YourProjectName/proj.android-studio/build.gradle`中添加maven地址，添加后为

	```
	allprojects {
    	repositories {
        	jcenter()
        	maven {
            	url "http://mvn.gt.igexin.com/nexus/content/repositories/releases/"
        	}
    	}
	}
	
	```

-  添加GeTui SDK gradle依赖，在`/YourProjectName/proj.android-studio/app/build.gradle`中添加`compile 'com.getui:sdk-for-google-play:2.10.3.5'`

   ```
   dependencies {
    	compile fileTree(dir: 'libs', include: ['*.jar'])
    	compile project(':libcocos2dx')
    	compile 'com.getui:sdk-for-google-play:4.3.5.9'
	}
	
   ```
	
- 在你的主包下`/YourProjectName/proj.android-studio/app/src/YourPackage/`新建一个包，命名为`getui`

- 复制`getui-cocos2dx/Android/GTPushService.java``getui-cocos2dx/Android/GTPushIntentService.java`
	`getui-cocos2dx/Android/GTPushBridge.java`到上一步新建的`getui`包中

- 复制`getui-cocos2dx/Android/GTPushBridge.h`到`/YourProjectName/Classes/`中 

- 复制`getui-cocos2dx/Android/GTPushBridge.cpp`到`/YourProjectName/proj.android-studio/app/jni/getui`中

- 在`/YourProjectName/proj.android-studio/app/jni/Android.mk`添加如下代码     
   
   ```
	$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/platform/android/jni)

	LOCAL_SRC_FILES := getui/GTPushBridge.cpp

	LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes
	
   ```
            
- 在`/YourProjectName/proj.androd-studio/app/AndroidManifest.xml`中注册Service
	
   ```
   <!-- Getui Config-->
   <service android:name="org.cocos2dx.cpp.getui.GTPushService"
       android:exported="true"
       android:label="PushService"
       android:process=":pushservice"/>

    <service android:name="org.cocos2dx.cpp.getui.GTPushIntentService"/>
    
   ```
   
- 在`/YourProjectName/proj.android-studio/app/YourPackage/AppActivity`中初始化个推SDK
   
   ```
   package org.cocos2dx.cpp;

	import org.cocos2dx.cpp.getui.GTPushHelper;
	import org.cocos2dx.lib.Cocos2dxActivity;
	import android.os.Bundle;

	public class AppActivity extends Cocos2dxActivity {
    	@Override
    	protected void onCreate(Bundle savedInstanceState) {
        	super.onCreate(savedInstanceState);
        	GTPushHelper.init(getApplicationContext());
        	GTPushHelper.initPush();
    	}
	}
	
   ```

## Android Usage

- 注册推送监听

  	```
	void handlerReceiveServicePid(void *p_handle, int pid){
		 CCLOG("Test handlerReceiveServicePid = %d", pid);
	}
	void handlerReceiveClientId(void *p_handle, const char* clientId){
		 CCLOG("Test handlerReceiveClientId = %s", clientId);
	}
		
	void handlerReceiveMessageData(void *p_handle, const char* message){
		 CCLOG("Test handlerReceiveMessageData message = %s", message );
	}
		
	void handlerReceiveOnlineState(void *p_handle, bool online){
		 CCLOG("Test handlerReceiveOnlineState online = %s", online ? "true" : "false");
	}
		
	void handlerReceiveCommandResult(void *p_handle, const char* message){
		 CCLOG("Test handlerReceiveCommandResult message = %s", message);
	}
		
	void test_registerNotification(){
		GTPushBridge::registerNotificationCallback(
		   NULL,
		   &handlerReceiveServicePid,
		   &handlerReceiveClientId,
		   &handlerReceiveMessageData,
		   &handlerReceiveOnlineState,
		   &handlerReceiveCommandResult
		 );
	}

  	```
- 其他接口(具体接口说明请查看`GTPushBridge.h`)
  
  ```
   void test_getVersion(){
	  CCLOG("Test GeTui SDK version = %s", GTPushBridge::getVersion());
   }
	
   void test_getClientId(){
	  CCLOG("Test GeTui clientId = %s", GTPushBridge::getClientId());
	}
	
	void test_isPushTurnOn(){
	  CCLOG("Test GeTui isPushTurnOn, turnOn = %s", GTPushBridge::isPushTurnOn() ? "true" : "false" );
	}
	
	void test_turnOnPush(){
	  CCLOG("Test GeTui turnOnPush");
	  GTPushBridge::turnOnPush();
	}
	
	void test_turnOffPush(){
	  CCLOG("Test GeTui turnOffPush");
	  GTPushBridge::turnOffPush();
	}
	
	void test_stopService(){
	  CCLOG("Test GeTui stopService");
	  GTPushBridge::stopService();
	}
	
	void test_setTag(){
	  std::string *tag = new std::string("tag-test");
	  const char* sn = "test_setTag";
	  CCLOG("Test GeTui setTag, result = %d", GTPushBridge::setTag(tag, sn));
	  delete tag;
	}
	
	void test_setSilentTime(int beginHours, int durationHours){
	  CCLOG("Test GeTui setSilentTime, result = %s", GTPushBridge::setSilentTime(beginHours, durationHours) ? "true" : "false");
	}
	
	void test_sendFeedbackMessage(const char* messageId, const char* taskId){
	  CCLOG("Test GeTui sendFeedbackMessage, result = %s", GTPushBridge::sendFeedbackMessage(taskId, messageId, 100) ? "true" : "false");
	}
	
	void test_bindAlias(){
	  CCLOG("Test GeTui bindAlias, result = %s", GTPushBridge::bindAlias("alias-test") ? "true" : "false" );
	}
	
	void test_unBindAlias(){
	  CCLOG("Test GeTui unBindAlias, result = %s", GTPushBridge::unBindAlias("alias-test", true) ? "true" : "false" );
	}
	
  ```

## iOS Xcode 集成

可参照HelloWorld目录下工程进行集成

目前iOS版本采用手动集成，其中GTSDK推荐使用cocoapos集成；

如需要手动集成,请参照官方集成指南： [iOS SDK 集成指南](https://docs.getui.com/getui/mobile/ios/xcode/)

#### 集成步骤

* 使用 Cocos2d-x 生成 iOS 工程

* 在.xcodeproj文件同目录下，新建Podfile文件，可参照插件目录`iOS`下Podfile文件

* 使用 `pod install` 安装GTSDK依赖

* 将插件目录`iOS`下，`GTPushBridge.h`、`GTPushOCBridge.h`、`GTPushBridge.mm`三个文件拖入xcode工程

* 如果运行过程中显示swift依赖文件，建议新建一个swift文件

* 开启工程推送权限：在buildSetting下Signing&Capabilities下加入Push Notifications权限

**注意：** 

Apple 在 iOS 10 中新增了Notification Service Extension机制，可在消息送达时进行业务处理。为精确统计消息送达率，在集成个推SDK时，可以添加 Notification Service Extension，并在 Extensions 中添加 GTExtensionSDK 的统计接口，实现消息展示回执统计功能。具体可参考[个推集成文档](https://docs.getui.com/getui/mobile/ios/xcode/)。

#### 添加代码

* 从个推官网获取appid、appkey及appSecret

* 在xcode工程中找到iOS的‘AppController.mm’文件

* 在‘AppController.mm’文件头部加入代码：

  ```
  #import "GTPushOCBridge.h" //引入SDK回调承接类
  // GTSDK 配置信息
  #define kGtAppId @"您的AppId"
  #define kGtAppKey @"您的AppKey"
  #define kGtAppSecret @"您的AppSecret"
  ```

* 在 `- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions` 方法中加入GTSDK启动方法,建议写在方法末尾 

  ```
  //承接类一定要使用[GTPushOCBridge sharedInstance]
  [GeTuiSdk startSdkWithAppId:kGtAppId appKey:kGtAppKey appSecret:kGtAppSecret delegate:[GTPushOCBridge sharedInstance] launchingOptions:launchOptions];
  //注册通知类型 参考代码 可根据需求修改
  [GeTuiSdk registerRemoteNotification: (UNAuthorizationOptionSound | UNAuthorizationOptionAlert | UNAuthorizationOptionBadge)];
  
  ```
  
* 在cocos2dx代码中使用推送桥接类
  
  ```
  #include "GTPushBridge.h" //引入桥接类
  ```
  ```
  //回调函数
  static void gtClientCallBack(const char* clientId) {
    printf("GTSDK clientId: %s\n", clientId);
  }
  ```
  ```
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //注册回调函数
    GTPushBridge::registerNotificationCallback(gtClientCallBack, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    #endif
  ```
  如果成功在控制台输出clientId，则完成了集成；


## iOS Usage

iOS提供的方法如下，如有需要，可参考下述方法自行增删方法


    ```
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
    ```
