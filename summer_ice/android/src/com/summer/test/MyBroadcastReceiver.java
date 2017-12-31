package com.summer.test;

import android.content.Context;
import android.content.Intent;

import org.qtproject.qt5.android.bindings.QtService;

import android.util.Log;

public class MyBroadcastReceiver {
    //@Override
    public void onReceive(Context context, Intent intent) 
    {
        Log.d("SummerIce", "MyBroadcastReceiver::onReceive()");
        /*
        Intent startServiceIntent = new Intent(context, MyService.class);
        context.startService(startServiceIntent);
        */
    }
}
