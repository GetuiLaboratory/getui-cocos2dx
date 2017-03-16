package org.cocos2dx.cpp.getui;

import android.content.Context;
import android.content.res.TypedArray;

import com.igexin.sdk.PushManager;
import com.igexin.sdk.Tag;

/**
 * Created by zhourh on 2017/3/13.
 */

@SuppressWarnings("JniMissingFunction")
public class GTPushHelper {

    private static Context mContext;

    public static native void onReceiveServicePid(int pid);

    public static native void onReceiveClientId(String clientId);

    public static native void onReceiveMessageData(String message);

    public static native void onReceiveOnlineState(boolean online);

    public static native void onReceiveCommandResult(String message);

    public static void init(Context context){
        mContext = context;
    }

    public static Context getContext(){
        return mContext;
    }

    public static void initPush(){
        PushManager.getInstance().initialize(mContext, GTPushService.class);
        PushManager.getInstance().registerPushIntentService(mContext, GTPushIntentService.class);
    }

    public static String getVersion(){
        return PushManager.getInstance().getVersion(mContext);
    }

    public static String getClientId(){
        return PushManager.getInstance().getClientid(mContext);
    }

    public static boolean isPushTurnOn(){
        return PushManager.getInstance().isPushTurnedOn(mContext);
    }

    public static void turnOnPush(){
        PushManager.getInstance().turnOnPush(mContext);
    }

    public static void turnOffPush(){
        PushManager.getInstance().turnOffPush(mContext);
    }

    public static void stopService(){
        PushManager.getInstance().stopService(mContext);
    }

    public static int setTag(String[] tagNames, String sn){
        Tag[] tags;
        if (tagNames == null || tagNames.length <= 0){
            tags = null;
        }
        else{
            tags = new Tag[tagNames.length];
            for (int i = 0; i < tagNames.length; i++){
                Tag tag = new Tag();
                tag.setName(tagNames[i]);
                tags[i] = tag;
            }
        }

        return PushManager.getInstance().setTag(mContext, tags, sn);
    }

    public static boolean setSilentTime(int beginHour, int duration){
        return PushManager.getInstance().setSilentTime(mContext, beginHour, duration);
    }

    public static boolean sendFeedbackMessage(String taskId, String messageId, int actionId){
        return PushManager.getInstance().sendFeedbackMessage(mContext, taskId, messageId, actionId);
    }

    public static boolean bindAlias(String alias){
        return PushManager.getInstance().bindAlias(mContext, alias);
    }

    public static boolean unBindAlias(String alias, boolean isSelf){
        return PushManager.getInstance().unBindAlias(mContext, alias, isSelf);
    }


}
