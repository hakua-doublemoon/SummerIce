package com.summer.test;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
//import android.widget.Toast;
import android.util.Log;

public class AutumnBCasRcv extends BroadcastReceiver { 
    //@Override
    public void onReceive(Context context, Intent intent) {
        // toast で受け取りを確認
        //Toast.makeText(context, "Received ", Toast.LENGTH_LONG).show();
        Log.d("SummerIce", "I heard Alerm!");
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        context.startActivity(intent);
    }
}
