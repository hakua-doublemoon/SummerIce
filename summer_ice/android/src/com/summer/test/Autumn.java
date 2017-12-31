package com.summer.test;

import android.content.Context;
import android.content.Intent;
import android.app.Activity;

import org.qtproject.qt5.android.bindings.QtService;

import android.util.Log;
import android.text.format.Time;

//import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.app.AlarmManager;
import android.app.PendingIntent;
import java.util.Calendar;

public class Autumn {
    public static String j_getOSTime()
    {
        Log.d("SummerIce", "MyService::j_getOSTime()"); 
        Time time = new Time("Asia/Tokyo");
        time.setToNow();
        String date = time.year + "/" + (time.month+1) + "/" + time.monthDay + " " 
                    + time.hour + ":" + time.minute    + ":" + time.second;
        Log.d("SummerIce", "#=>" + date);
        return date;
    }
    
    public static void j_setAlerm(Context ctx)
    {
        Log.d("SummerIce", "Set Alarm[S]");
        // 時間をセットする
        Calendar calendar = Calendar.getInstance();
        // Calendarを使って現在の時間をミリ秒で取得
        calendar.setTimeInMillis(System.currentTimeMillis());
        // 5秒後に設定
        calendar.add(Calendar.SECOND, 3);

Log.d("SummerIce", "Set Alarm[1]");

        //Intent intent = new Intent(getApplicationContext(), AutumnBCasRcv.class);
        Intent intent = new Intent(ctx, AutumnBCasRcv.class);

Log.d("SummerIce", "Set Alarm[1.2]");

        PendingIntent pending = PendingIntent.getBroadcast(ctx, 0, intent, 0);

Log.d("SummerIce", "Set Alarm[2]");

        // アラームをセットする
        AlarmManager am = (AlarmManager)ctx.getSystemService(ctx.ALARM_SERVICE);
        //am.setExact(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), pending);

        //Toast.makeText(getApplicationContext(), "Set Alarm ", Toast.LENGTH_SHORT).show();
        Log.d("SummerIce", "Set Alarm[E]");
        
        return;
    }
}
