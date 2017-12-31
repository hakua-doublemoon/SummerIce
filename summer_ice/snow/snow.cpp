#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>

#include <QDebug>
#include <QTimer>

#include "snow.h"

#define polling_interval (30)
#define monitor_mergin   (polling_interval/4)

Snow::Snow(int count_left, QObject *parent)
    : QThread(parent) /*QObject(parent)*/
    , inst_winter("com/summer/test/Winter")
{
    qDebug() << "SummerIce::Snow - construct/" << count_left;
    wake_count = count_left;
    isWaked_s = false;
    isWaked_c = false;
    // 一応の時間取得
    hour = 99;
    min  = 99;
}

// wakelockの初期化
void Snow::init_wakelocks()
{
    qDebug() << "SummerIce::Snow::init_wakelock";
    auto activity = QtAndroid::androidService();
    inst_winter.callMethod<void>("j_init_wakelocks"
                                ,"(Landroid/content/Context;)V"
                                ,activity.object()
                                );
    qDebug() << "SummerIce::Snow::init_wakelock - finish";
}

void Snow::set_alerm(QString &qstr)
{
    QStringList ql = qstr.split(":");
    QString tmp;
    tmp = ql.at(1);
    this->hour = tmp.toInt();
    tmp = ql.at(2);
    this->min  = tmp.toInt();
    qDebug() << "SummerIce::Snow::set_alerm()";
    qDebug() << qstr << " -> " << this->hour << ":" << this->min;
    // CPUを起こしっぱなしにする
    auto activity = QtAndroid::androidService();
    inst_winter.callMethod<void>("j_wakelock"
                                ,"(Landroid/content/Context;)V"
                                ,activity.object()
                                );
}

Snow::~Snow()
{
    qDebug() << "SummerIce::Snow - destruct";
    this->quit();
    this->wait();
}

void Snow::run()
{
    qDebug() << "SummerIce::Snow - run";

    //this->prepare_ping();

    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);

    timer.setInterval(polling_interval * 1000);
    timer.start();
    exec();
    qDebug() << "SummerIce::Snow - stop";
    timer.stop();
}

void Snow::timerHit()
{
    qDebug() << "SummerIce::Snow::timer/" << wake_count;

    // アラームを確認する
    bool isWake = false;
    isWake = Ice::chkAlerm_s(this->hour, this->min, polling_interval+monitor_mergin);

    // 時間だったら画面を起こす
    if (isWake) {
        //this->ping2server("hoge");
        this->wakeup();
        isWaked_s = true;
        isWaked_c = false;
        qDebug() << "SummerIce::Snow::wake up!";
    } else {
        if (isWaked_c) {
            // しばらくして起きた場合はこっちに入る
            isWaked_s = false;
            isWaked_c = false;
        }
    }

    return;
}

// wakeup
void Snow::wakeup()
{
    if (isWaked_s) {
        // もう起こし始めてたら何もしない
        // 起こし始めてから放置されていた場合の処理
        qDebug() << "SummerIce::wakeup[WAKED]";
        return;
    }
    if (isWaked_c) {
        // 即座に起こした場合こっちに引っかかる
        qDebug() << "SummerIce::wakeup[WAKED II]";
        isWaked_s = false;
        isWaked_c = false;
        return;
    }
    qDebug() << "SummerIce::wakeup[S]";
#ifdef __ANDROID__
    auto activity = QtAndroid::androidService();

    inst_winter.callMethod<void>("j_wakeup"
                                ,"(Landroid/content/Context;)V"
                                ,activity.object()
                                );
#endif /* __ANDROID__ */
    qDebug() << "SummerIce::wakeup[E]";
}

void Snow::wakeup_finish()
{
    if (!isWaked_s) {
        // 起こしてない時に呼ばれたら何もしない
        qDebug() << "SummerIce::wakeup_finish[NOP]";
        return;
    }
    if (isWaked_c) {
        // もう起きたことは知ってる
        qDebug() << "SummerIce::wakeup_finish[NOP2]";
        return;
    }
    qDebug() << "SummerIce::wakeup_finish[S]";
#ifdef __ANDROID__
    auto activity = QtAndroid::androidService();
    inst_winter.callMethod<void>("j_wakeup_finish"
                                ,"(Landroid/content/Context;)V"
                                ,activity.object()
                                );
#endif /* __ANDROID__ */
    isWaked_c = true;
    qDebug() << "SummerIce::wakeup_finish[E]";
}

// フラグを初期化する
void Snow::init_flag()
{
    qDebug() << "SummerIce::Snow::init_flag";
    isWaked_s = false;
    isWaked_c = false;
}

/*
QString Snow::getOSTime()
{
    QAndroidJniObject jni_str
        = QAndroidJniObject::callStaticObjectMethod<jstring>("com/summer/test/Autumn"
                                                            ,"j_getOSTime"
                                                            );


    //QAndroidJniObject::callStaticMethod<void>(
    //            "com/summer/test/Autumn",
    //            "j_setAlerm",
    //            "(Landroid/content/Context;)V",
    //            QtAndroid::androidActivity().object()
    //            );

    return jni_str.toString(); // Convert to QString
}
*/
