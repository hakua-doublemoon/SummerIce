package com.summer.test;

import android.content.Context;
import android.content.Intent;
import org.qtproject.qt5.android.bindings.QtService;

import android.util.Log;

public class Spring extends QtService
{
    //コンストラクタを入れると起動できなくなる
    //Spring() {
    //    Log.d("SummerIce", "Construct Spring");
    //}
    
    public static void startMyService(Context ctx) {
        Log.d("SummerIce", "startMyService()");
        ctx.startService(new Intent(ctx, Spring.class));
    }
}
