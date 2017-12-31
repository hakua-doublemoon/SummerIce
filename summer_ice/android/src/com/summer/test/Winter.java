package com.summer.test;

import android.content.Context;
import android.content.Intent;

import android.app.Activity;

import org.qtproject.qt5.android.bindings.QtService;

import android.util.Log;

import android.app.KeyguardManager;
import android.app.KeyguardManager.KeyguardLock;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;

import android.os.Bundle;
import android.app.AlarmManager;
import android.app.PendingIntent;
import java.util.Calendar;

public class Winter extends QtService
{
    private WakeLock wakelock;
    private WakeLock wakelock_keep;
    private KeyguardLock keylock;

    // こんすとらくた
    public Winter() {
        Log.d("SummerIce", "Construct Winter");
    }
    // wakelockの初期化
    public void j_init_wakelocks(Context ctx)
    {
        Log.d("SummerIce", "init_wakelocks");
        PowerManager power = ((PowerManager) ctx.getSystemService(Context.POWER_SERVICE));
        wakelock = power.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK
                                    |PowerManager.ACQUIRE_CAUSES_WAKEUP
                                    |PowerManager.ON_AFTER_RELEASE
                                    ,"SummerIce"
                                    );
        wakelock_keep = power.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK
                                         ,"SummerIce_IceClock"
                                         );
        // ついでにきーがーども
        KeyguardManager keyguard = (KeyguardManager) ctx.getSystemService(Context.KEYGUARD_SERVICE);
        keylock = keyguard.newKeyguardLock("SummerIce");
        Log.d("SummerIce", "init_wakelocks - finish");
    }

    // 画面起動
    public void j_wakeup(Context ctx) {
        Log.d("SummerIce", "j_wakeup - wake");

        PowerManager power = ((PowerManager) ctx.getSystemService(Context.POWER_SERVICE));

        wakelock.acquire();
        keylock.disableKeyguard();
        Log.d("SummerIce", "j_wakeup - unlock");
    }

    // 画面起動したあとロックがほにゃらららしいので、解除する
    public void j_wakeup_finish(Context ctx) {
        Log.d("SummerIce", "j_wakeup - finish");

        if (wakelock.isHeld()) {
            wakelock.release();
        }
        keylock.reenableKeyguard();
    }

    // CPUをおこしっぱなしにしてみる
    public void j_wakelock(Context ctx) {
        Log.d("SummerIce", "j_wakelock - aquire");
        if (wakelock_keep.isHeld()) {
            Log.d("SummerIce", "j_wakelock - isHeld");
            return;
        }
        wakelock_keep.acquire();
        return;
    }

    // アラームセットする奴。没。
    public void j_setAlerm(Context ctx)
    {
        Log.d("SummerIce", "Set Alarm[S]");

        Calendar calendar = Calendar.getInstance();
        calendar.setTimeInMillis(System.currentTimeMillis());
        calendar.add(Calendar.SECOND, 10);

        //Intent intent = new Intent(getApplicationContext(), AutumnBCasRcv.class);
        Intent intent = new Intent(ctx, AutumnBCasRcv.class);
        PendingIntent pending = PendingIntent.getBroadcast(ctx, 0, intent, 0);

        AlarmManager am = (AlarmManager)ctx.getSystemService(ctx.ALARM_SERVICE);
        am.setExact(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), pending);

        Log.d("SummerIce", "Set Alarm[E]");

        return;
    }
    
}
