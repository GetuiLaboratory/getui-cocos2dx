/**
* GTPushBridge.cpp
*
* Created on: 2017-03-13
* Author: zhourh
*/

#include "GTPushBridge.h"
#include <jni.h>
#include <platform/android/jni/JniHelper.h>
#include <map>
#include <cocos2d.h>
#include <string>

using namespace cocos2d;

extern "C"{

#define GET_JSTRING(cstr)  ((cstr) ? t.env->NewStringUTF(cstr) : NULL)

#define SAFE_RELEASE_JOBJ(jobj) if(jobj) { t.env->DeleteLocalRef(jobj);}

#define COUNTOF(x) (sizeof((x)) / sizeof(x)[0])


const char* kGTPushHelperClassName = "org/cocos2dx/cpp/org.cocos2dx.org.cocos2dx.cpp.getui/GTPushHelper";

jobject getContext(){
  JniMethodInfo t;
  if (JniHelper::getStaticMethodInfo(t,
                                    kGTPushHelperClassName,
                                    "getContext",
                                    "()Landroid/content/Context;")) {
      jobject ret = t.env->CallStaticObjectMethod(t.classID, t.methodID);
      SAFE_RELEASE_JOBJ(t.classID);
      return ret;
  }
  return NULL;
}

const char* GTPushBridge::getVersion(){

  const char *ret = "";
  JniMethodInfo t;
  if (JniHelper::getStaticMethodInfo(t,
                               kGTPushHelperClassName,
                               "getVersion",
                               "()Ljava/lang/String;")) {
    jstring j_string = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    if (j_string) {
      string c_string = JniHelper::jstring2string(j_string);
      ret = c_string.c_str();
    }
    SAFE_RELEASE_JOBJ(t.classID);
  }
  return ret;
}

const char* GTPushBridge::getClientId(){
  const char *ret = "";
  JniMethodInfo t;
  if (JniHelper::getStaticMethodInfo(t,
                               kGTPushHelperClassName,
                               "getClientId",
                               "()Ljava/lang/String;")) {
    jstring j_string = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    if (j_string) {
      string c_string = JniHelper::jstring2string(j_string);
      ret = c_string.c_str();
    }
    SAFE_RELEASE_JOBJ(t.classID);
  }
  return ret;
}

bool GTPushBridge::isPushTurnOn(){
   JniMethodInfo t;
   jboolean isTurnOn;
   if (JniHelper::getStaticMethodInfo(t,
                                      kGTPushHelperClassName,
                                      "isPushTurnOn",
                                      "()Z")) {
      isTurnOn = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
      SAFE_RELEASE_JOBJ(t.classID);
   }
   return isTurnOn;

}

void GTPushBridge::turnOnPush(){
  JniMethodInfo t;
  if (JniHelper::getStaticMethodInfo(t,
                                     kGTPushHelperClassName,
                                     "turnOnPush",
                                     "()V")) {
      t.env->CallStaticVoidMethod(t.classID, t.methodID);
      SAFE_RELEASE_JOBJ(t.classID);
  }
}

void GTPushBridge::turnOffPush(){
  JniMethodInfo t;
  if (JniHelper::getStaticMethodInfo(t,
                                     kGTPushHelperClassName,
                                     "turnOffPush",
                                     "()V")) {
      t.env->CallStaticVoidMethod(t.classID, t.methodID);
      SAFE_RELEASE_JOBJ(t.classID);
  }
}

void GTPushBridge::stopService(){
  JniMethodInfo t;
  if (JniHelper::getStaticMethodInfo(t,
                                     kGTPushHelperClassName,
                                     "stopService",
                                     "()V")) {
      t.env->CallStaticVoidMethod(t.classID, t.methodID);
      SAFE_RELEASE_JOBJ(t.classID);
  }
}

int GTPushBridge::setTag(string *tags, const char* sn){
  jint ret = 0;
  if (!tags) {
    return ret;
  }
  JniMethodInfo t;
  if (JniHelper::getStaticMethodInfo(t,
                                     kGTPushHelperClassName,
                                     "setTag",
                                     "([Ljava/lang/String;Ljava/lang/String;)I")) {
     jclass stringClass = t.env->FindClass("java/lang/String");
     int tagCount = COUNTOF(tags);
     jobjectArray tagArray = t.env->NewObjectArray(tagCount, stringClass, NULL);
     for( int i = 0; i < tagCount; ++i ){
        t.env->SetObjectArrayElement(tagArray, i, t.env->NewStringUTF(tags[i].c_str()));
     }
     ret = t.env->CallStaticIntMethod(t.classID, t.methodID, tagArray, t.env->NewStringUTF(sn));
     SAFE_RELEASE_JOBJ(t.classID);
  }
  return ret;
}


bool GTPushBridge::setSilentTime(int beginHour, int duration){
  JniMethodInfo t;
  jboolean ret = JNI_FALSE;
  if (JniHelper::getStaticMethodInfo(t,
                                     kGTPushHelperClassName,
                                     "setSilentTime",
                                     "(II)Z")) {
      ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, beginHour, duration);
      SAFE_RELEASE_JOBJ(t.classID);
  }
  return ret;
}


bool GTPushBridge::sendFeedbackMessage(const char* taskId, const char* messageId, int actionId){
   JniMethodInfo t;
   jboolean ret = JNI_FALSE;
   if (JniHelper::getStaticMethodInfo(t,
                                      kGTPushHelperClassName,
                                      "sendFeedbackMessage",
                                      "(Ljava/lang/String;Ljava/lang/String;I)Z")) {

      jstring j_taskId = t.env->NewStringUTF(taskId);
      jstring j_messageId = t.env->NewStringUTF(messageId);
      jint j_actionId = (jint)actionId;
      ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, j_taskId, j_messageId, j_actionId);
      SAFE_RELEASE_JOBJ(t.classID);

   }
   return ret;
}


bool GTPushBridge::bindAlias(const char* alias){
  JniMethodInfo t;
  jboolean ret = JNI_FALSE;
  if (JniHelper::getStaticMethodInfo(t,
                                     kGTPushHelperClassName,
                                     "bindAlias",
                                     "(Ljava/lang/String;)Z")) {
     jstring j_alias = t.env->NewStringUTF(alias);
     ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, j_alias);
     SAFE_RELEASE_JOBJ(t.classID);
  }
  return ret;
}

bool GTPushBridge::unBindAlias(const char* alias, bool isSelf){
  JniMethodInfo t;
  jboolean ret = JNI_FALSE;
  if (JniHelper::getStaticMethodInfo(t,
                                     kGTPushHelperClassName,
                                     "unBindAlias",
                                     "(Ljava/lang/String;Z)Z")) {
     jstring j_alias = t.env->NewStringUTF(alias);
     jboolean j_isSelf = (jboolean)isSelf;
     ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, j_alias, j_isSelf);
     SAFE_RELEASE_JOBJ(t.classID);
  }
  return ret;
}

GTReceiveServicePid_callback g_pid_callback = NULL;
GTReceiveClientId_callback g_clientId_callback = NULL;
GTReceiveMessageData_callback g_messageData_callback = NULL;
GTReceiveOnlineState g_onlineState_callback = NULL;
GTReceiveCommandResult g_commandResult_callback = NULL;
GTReceiveNotificationArrived_callback g_notificationArrived_callback = NULL;
GTReceiveNotificationClicked_callback g_notificationClicked_callback = NULL;
void * notificationHandler = NULL;

void GTPushBridge::registerNotificationCallback(void *p_handle,
    GTReceiveServicePid_callback pid_callback,
    GTReceiveClientId_callback clientId_callback,
    GTReceiveMessageData_callback messageData_callback,
    GTReceiveOnlineState onlineState_callback,
    GTReceiveCommandResult commandResult_callback,
    GTReceiveNotificationArrived_callback notificationArrived_callback,
    GTReceiveNotificationArrived_callback notificationClicked_callback){

      g_pid_callback = pid_callback;
      g_clientId_callback = clientId_callback;
      g_messageData_callback = messageData_callback;
      g_onlineState_callback = onlineState_callback;
      g_commandResult_callback = commandResult_callback;
      g_notificationArrived_callback = notificationArrived_callback;
      g_notificationClicked_callback = notificationClicked_callback;
      notificationHandler = p_handle;

}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_getui_GTPushHelper_onReceiveServicePid__I(JNIEnv *env, jclass, jint pid){
    CCLOG("GeTui OnReciveServicePid = %d", pid);
    if (g_pid_callback != NULL) {
      g_pid_callback(notificationHandler, (int)pid);
    }
}


JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_getui_GTPushHelper_onReceiveClientId__Ljava_lang_String_2(JNIEnv *env, jclass, jstring clientId){
    const char * str_clientId = env->GetStringUTFChars(clientId, JNI_FALSE);
    CCLOG("GeTui OnReciveClientId = %s", str_clientId);
    if (g_clientId_callback != NULL) {
      g_clientId_callback(notificationHandler, str_clientId);
      env->ReleaseStringUTFChars(clientId, str_clientId);
    }

}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_getui_GTPushHelper_onReceiveMessageData__Ljava_lang_String_2(JNIEnv *env, jclass, jstring message){
    const char * notification = NULL;
    if(message != NULL){
        notification = env->GetStringUTFChars(message, JNI_FALSE);
        CCLOG("GeTui onReceiveMessageData, message = %s", notification);
    }
    if(g_messageData_callback != NULL){
        g_messageData_callback(notificationHandler, notification);
    }
    if(message != NULL){
        env->ReleaseStringUTFChars(message, notification);
    }
}

JNIEXPORT void Java_org_cocos2dx_cpp_getui_GTPushHelper_onNotificationMessageArrived__Ljava_lang_String_2(JNIEnv *env, jclass, jstring message){
    const char * notification = NULL;
    if (message != NULL) {
        notification = env->GetStringUTFChars(message,JNI_FALSE);
        CCLOG("GeTui onNotificationArrived, message = %s", notification);
    }
    if(g_notificationArrived_callback != NULL){
        g_notificationArrived_callback(notificationHandler, notification);
    }
    if(message != NULL){
        env->ReleaseStringUTFChars(message, notification);
    }

}

JNIEXPORT void Java_org_cocos2dx_cpp_getui_GTPushHelper_onNotificationMessageClicked__Ljava_lang_String_2(JNIEnv *env, jclass, jstring message){
    const char * notification = NULL;
    if (message != NULL) {
        notification = env->GetStringUTFChars(message,JNI_FALSE);
        CCLOG("GeTui onNotificationClicked, message = %s", notification);
    }
    if(g_notificationClicked_callback != NULL){
        g_notificationClicked_callback(notificationHandler, notification);
    }
    if(message != NULL){
        env->ReleaseStringUTFChars(message, notification);
    }
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_getui_GTPushHelper_onReceiveOnlineState__Z(JNIEnv *env, jclass, jboolean online){
  CCLOG("GeTui onReceiveOnlineState, online = %s", online == JNI_TRUE ? "true" : "false" );
  if (g_onlineState_callback != NULL) {
    g_onlineState_callback(notificationHandler, (bool)online);
  }
}


JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_getui_GTPushHelper_onReceiveCommandResult__Ljava_lang_String_2(JNIEnv *env, jclass, jstring message){
  const char * notification = NULL;
    if(message != NULL){
        notification = env->GetStringUTFChars(message, JNI_FALSE);
        CCLOG("GeTui onReceiveCommandResult, message = %s", notification );
    }
    if(g_commandResult_callback != NULL){
        g_commandResult_callback(notificationHandler, notification);
    }
    if(message != NULL){
        env->ReleaseStringUTFChars(message, notification);
    }
}

}
