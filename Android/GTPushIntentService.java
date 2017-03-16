package org.cocos2dx.cpp.getui;

import android.content.Context;
import android.util.Log;

import com.igexin.sdk.GTIntentService;
import com.igexin.sdk.message.GTCmdMessage;
import com.igexin.sdk.message.GTTransmitMessage;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by zhourh on 2017/3/13.
 * 继承 GTIntentService 接收来自个推的消息, 所有消息在线程中回调, 如果注册了该服务, 则务必要在 AndroidManifest中声明, 否则无法接受消息<br>
 * onReceiveMessageData 处理透传消息<br>
 * onReceiveClientId 接收 cid <br>
 * onReceiveOnlineState cid 离线上线通知 <br>
 * onReceiveCommandResult 各种事件处理回执 <br>
 */

public class GTPushIntentService extends GTIntentService {




    @Override
    public void onReceiveServicePid(Context context, int pid) {
        GTPushHelper.onReceiveServicePid(pid);
    }

    @Override
    public void onReceiveClientId(Context context, String clientId) {
        GTPushHelper.onReceiveClientId(clientId);
    }

    @Override
    public void onReceiveMessageData(Context context, GTTransmitMessage gtTransmitMessage) {
        try {
            JSONObject jsonObject = new JSONObject();
            jsonObject.put("messageId", gtTransmitMessage.getMessageId());
            jsonObject.put("payload", new String(gtTransmitMessage.getPayload()));
            jsonObject.put("payloadId", gtTransmitMessage.getPayloadId());
            jsonObject.put("taskId", gtTransmitMessage.getTaskId());
            GTPushHelper.onReceiveMessageData(jsonObject.toString());
        } catch (JSONException e) {
            e.printStackTrace();
        }

    }

    @Override
    public void onReceiveOnlineState(Context context, boolean online) {
        GTPushHelper.onReceiveOnlineState(online);
    }

    @Override
    public void onReceiveCommandResult(Context context, GTCmdMessage cmdMessage) {
        try {
            JSONObject jsonObject = new JSONObject();
            jsonObject.put("action", cmdMessage.getAction());
            GTPushHelper.onReceiveCommandResult(jsonObject.toString());
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}
