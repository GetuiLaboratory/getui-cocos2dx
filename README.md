# getui-cocos2dx

个推官方提供的 Cocos2d-x插件 （目前仅支持Android，iOS版本近期将会发布）。

## Android Studio 自动集成
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

-  添加GeTui SDK gradle依赖，在`/YourProjectName/proj.android-studio/app/build.gradle`中添加`compile 'com.getui:sdk:2.10.2.0'`

   ```
   dependencies {
    	compile fileTree(dir: 'libs', include: ['*.jar'])
    	compile project(':libcocos2dx')
    	compile 'com.getui:sdk:2.10.2.0'
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
