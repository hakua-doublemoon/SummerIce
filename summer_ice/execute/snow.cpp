#include "snow.h"

#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>

#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>

#include "ice.h"

#define monitor_mergin   (SNOWDEF_POLLING_INTERVAL/4)
#define ALERM_FILE "bluenote.txt"

// 注記： 2018/05/04現在、こいつはアクティビティ側から使用。サービス側からは多分使えない

Snow::Snow(int count_left, QString id, QObject *parent)
    : QThread(parent) /*QObject(parent)*/
    , inst_winter("com/summer/test/Winter")
    //, inst_air()
{
    qDebug() << "SummerIce::Snow - construct=" << count_left << " id=" << id;
    wake_count = count_left;
    isWaked_s = false;
    isWaked_c = false;
    hour = 99;
    min  = 99;
    id_name = id;
    isInit = false;
}

// wakelockの初期化
void Snow::init_wakelocks()
{
    if (this->isInit) {
        qDebug() << "SummerIce::Snow::init_wakelock > be ready";
        return;
    }
    qDebug() << "SummerIce::Snow::init_wakelock";
    //auto activity = QtAndroid::androidService();
    auto activity = QtAndroid::androidActivity();
    inst_winter.callMethod<void>("j_init_wakelocks"
                                ,"(Landroid/content/Context;)V"
                                ,activity.object()
                                );
    this->isInit = true;
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
    this->init_wakelocks();
    // CPUを起こしっぱなしにする
    //auto activity = QtAndroid::androidService();
    auto activity = QtAndroid::androidActivity();
    inst_winter.callMethod<void>("j_wakelock"
                                ,"(Landroid/content/Context;)V"
                                ,activity.object()
                                );

    // ↓これのイベントは今のところ使ってない
    inst_winter.callMethod<void>("j_setAlerm"
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

    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHit()), Qt::DirectConnection);

    timer.setInterval(SNOWDEF_POLLING_INTERVAL * 1000);
    timer.start();
    exec();
    qDebug() << "SummerIce::Snow - stop";
    timer.stop();
}

// 雪見スレッドの間隔ごとにこれを実行
void Snow::timerHit()
{
    qDebug() << "SummerIce::Snow::timer/" << wake_count << " id=" << id_name;
    QString qstrd;
    qstrd = QString("[currentThreadId] Snow::timerHit() = 0x%1").arg((ulong)QThread::currentThreadId(), 0, 16);
    qDebug() << qstrd;

    // アラームを確認する
    bool isWake = false;
    isWake = Ice::chkAlerm_s(this->hour, this->min, SNOWDEF_POLLING_INTERVAL+monitor_mergin);

    // 時間だったら画面を起こす
    if (isWake) {
        //this->ping2server("hoge");
        this->wakeup();
        isWaked_s = true;
        isWaked_c = false;
        qDebug() << "SummerIce::Snow::wake up!";
        Ice::set_window_visi(true);
    } else {
        if (isWaked_c) {
            // しばらくして起きた場合はこっちに入る
            isWaked_s = false;
            isWaked_c = false;
        }
        if (!Ice::main_ready) {
            // 雪見スレッドが起きたのにメイン画面が起動してない→端末の画面が起動してない
            // →端末を省エネ復帰させる
            qDebug() << "SummerIce::Snow::timerHit > try to wake up!";
            this->init_wakelocks();
            this->wakeup();
            return;
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
    //auto activity = QtAndroid::androidService();
    auto activity = QtAndroid::androidActivity();

    inst_winter.callMethod<void>("j_wakeup"
                                ,"(Landroid/content/Context;)V"
                                ,activity.object()
                                );
#endif /* __ANDROID__ */
    qDebug() << "SummerIce::wakeup[E]";
}

// 起こし終わった時の処理。やりもらすと画面が真っ黒になったりする
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
    //auto activity = QtAndroid::androidService();
    auto activity = QtAndroid::androidActivity();
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

// アラームの情報を受け取るシグナル
void Snow::sig_set_alerm(QString info_qstr)
{
    qDebug() << "invoke[recv] <- " << info_qstr;
    this->set_alerm(info_qstr);
    this->wakeup_finish();
}
