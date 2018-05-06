#include "ice.h"

#include <QtGui/QGuiApplication>
#include <QtQml>

#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QTime>
#include <QWindow>

#include <cmath>

#ifdef __ANDROID__
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#endif /* __ANDROID__ */

//=================================================
// static
//=================================================
QObject* Ice::main_obj = NULL;
bool     Ice::main_ready = false;

//=================================================
// prototypes
//=================================================
static QString cnvDiffSec(int sec);
static bool chk_list_version(QString fn, QString ext_fn);
static void restart_app();

//=================================================
// methods
//=================================================
Ice::Ice(QObject *parent)
   : QObject(parent)
{
    this->snow_ready = false;
    qDebug() << "SummerIce::Ice > construction";
}

void Ice::set_infos(QString alerm_file,
                    QString voice_file,
                    QString voice_file_ext
                    )
{
    this->alermInfoFile = alerm_file;
    this->voiceListFile = voice_file;
    // アプリの領域にファイルをコピーする
    if (!chk_list_version(voice_file, voice_file_ext)) {
        qDebug() << "SummerIce::set_infos > update";
        if (QFile::exists(voice_file)) {
            qDebug() << "SummerIce::set_infos > delete old file";
            QFile::remove(voice_file);
        }
        QFile ext_qf(voice_file_ext);
        ext_qf.copy(voice_file);
    }
}

bool chk_list_version(QString fn, QString ext_fn)
{
    qDebug() << "SummerIce::chk_list_version()";
    // アプリの領域にファイルがなかったら終わり
    if (!QFile::exists(fn)) {
        return false;
    }
    // バージョン情報を抜き出す
    int version1 = 0;
    int version2 = 0;
    QFile file;
    QByteArray qbstr;
    QJsonDocument qjdoc;
    QJsonObject qjhash;
    // - fn
    file.setFileName(fn);
    file.open(QIODevice::ReadOnly);
    qbstr  = file.readAll();
    qjdoc  = QJsonDocument::fromJson(qbstr);
    qjhash = qjdoc.object();
    if (qjhash.contains("version")) {
        version1 = qjhash["version"].toInt();
        qDebug() << "version1 = " << version1 << " : " << qjhash["version"];
    }
    file.close();
    // - ext
    file.setFileName(ext_fn);
    file.open(QIODevice::ReadOnly);
    qbstr  = file.readAll();
    qjdoc  = QJsonDocument::fromJson(qbstr);
    qjhash = qjdoc.object();
    //qDebug() << qjhash;
    if (qjhash.contains("version")) {
        version2 = qjhash["version"].toInt();
        qDebug() << "version2 = " << version2 << " : " << qjhash["version"];
    }
    file.close();
    // 比較
    if (version1 == version2) {
        qDebug() << "SummerIce::chk_list_version() #=> true";
        return true;
    }
    return false;
}

//virtual void Ice::vtable_dummy()
//{
//}

// アラームを設定する
QString Ice::setAlerm(int hour, int min)
{
    /* 現在時刻との差を計算する */
    // 現在の日時を取得
    QTime qdt_now = QTime::currentTime();
    // 入力からQTimeを作成
    QTime qdt_tom = QTime(hour, min);
    // 現在時刻との差を計算する
    int period = qdt_now.secsTo(qdt_tom);
    
    qDebug() << "period[1] = " << period;
    
    if (period < 0) {
        period += 60*60*24;
    }
    qDebug() << "period[2] = " << period;
    QString qstr_td = cnvDiffSec(period);
    qDebug() << "=> " << qstr_td;

    /* セットされた時間を保存する */
    QString qs_alerm = qdt_tom.toString("H:m:s");
    QJsonObject qjhash;
    qjhash["time"] = qs_alerm;
    this->writefile_json(this->alermInfoFile, qjhash);

    this->set_alerm_monitor(hour, min);

    Ice::set_window_visi(false);

    // アプリが死んだ時のために雪見スレッドの周期でアプリを起動できるようにしておく
    Ice::restart_app_main(period-SNOWDEF_POLLING_INTERVAL, false);

    return qstr_td;
}

// アラーム監視スレッドに設定を伝える
void Ice::set_alerm_monitor(int hour, int min)
{
    if (this->snow_ready) {
        QString arg_qstr = QString("hoge:%1:%2").arg(hour).arg(min);
        qDebug() << "invoke[send] <- " << arg_qstr;
        QMetaObject::invokeMethod(this->snow_p, "sig_set_alerm", Qt::QueuedConnection, Q_ARG(QString, arg_qstr));
    }
}

// - おめざまでの時間
static QString cnvDiffSec(int sec)
{
    int hour = sec / (60*60);
    int min  = (sec-hour*60*60) / 60;
    int dsec = sec - hour*60*60 - min*60;
    QString qstr = QString("%1:%2:%3").arg(hour).arg(min).arg(dsec);
    return qstr;
}

// いまアラームの時間か？
bool Ice::chkAlerm(int hour, int min, int thr) {
    return this->chkAlerm_s(hour, min, thr);
}
bool Ice::chkAlerm_s(int hour, int min, int thr)
{
    if (hour == 99) {
        return false;
    }
    /* 現在時刻との差を計算する */
    // 現在の日時を取得
    QTime qdt_now = QTime::currentTime();
    qDebug() << "qdt_now = " << qdt_now;
    // 入力からQTimeを作成
    QTime qdt_tom = QTime(hour, min);
    qDebug() << "qdt_tom = " << qdt_tom;
    // 現在時刻との差を計算する
    int period = qdt_now.secsTo(qdt_tom);
    qDebug() << "D/|period| = " << abs(period);
    if (abs(period) < thr) {
        return true;
    }
    return false;
}

// read alerm file
QJsonObject Ice::readfile_alerm() {
    return this->readfile_alerm_s(this->alermInfoFile);
}
QJsonObject Ice::readfile_alerm_s(QString &file_name_str)
{
    QFile file(file_name_str);

    // ROで開く
    if (!file.open(QIODevice::ReadOnly))
    {
        QJsonObject tmp;
        tmp["isOK"] = false;
        qDebug() << "E/SummerIce::read_file => Open Error!!";
        return tmp;
    }
    
    // ファイルの中身をJSONオブジェクトに変換する
    QByteArray qbstr = file.readAll();
    QJsonDocument qjdoc(QJsonDocument::fromJson(qbstr));
    QJsonObject qjhash(qjdoc.object());
    QString qs_alerm = qjhash["time"].toString();

    //qDebug() << "SummerIce::read_file <- " << qbstr;
    //qDebug() << "SummerIce::read_file <- " << qs_alerm;

    QTime qtime = QTime::fromString(qs_alerm, "H:m:s");
    qDebug() << "SummerIce::read_file/time = " << qtime.toString();

    qjhash["isOK"] = true;
    qjhash["hour"] = qtime.hour();
    qjhash["min"]  = qtime.minute();

    file.close();

    return qjhash;
}

// アラームのリストを取得する
void Ice::load_voice_list()
{
    QFile file(this->voiceListFile);

    qDebug() << "SummerIce::Ice::readfile_voice_list <- " << this->voiceListFile;

    // ROで開く
    if (!file.open(QIODevice::ReadOnly))
    {
        QJsonObject tmp;
        tmp["isOK"] = false;
        //return tmp;
        return;
    }

    // ファイルの中身をJSONオブジェクトに変換する
    QByteArray qbstr = file.readAll();
    QJsonDocument qjdoc(QJsonDocument::fromJson(qbstr));
    QJsonObject qjhash(qjdoc.object());
    this->voiceListObj = qjhash;

    file.close();

    return;
}

// アラームのリストを更新する
void Ice::update_voice_select(int select)
{
    this->voiceListObj["selecting"] = select;
    qDebug() << "SummerIce::update_voice_list[1] / " << this->voiceListObj["selecting"].toInt();
    this->writefile_json(this->voiceListFile, this->voiceListObj);
}
void Ice::update_voice_selectChara(int select)
{
    this->voiceListObj["selectChara"] = select;
    qDebug() << "SummerIce::update_voice_list[2] / " << this->voiceListObj["selectChara"].toInt();
    this->writefile_json(this->voiceListFile, this->voiceListObj);
}

// jsonファイルに書き込む
bool Ice::writefile_json(const QString &file_name_qstr, const QJsonObject &qjhash) const
{
    QFile file(file_name_qstr);

    // WOで開く
    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    // JSONオブジェクトから文字列に変換する
    QJsonDocument qjdoc(qjhash);
    QByteArray qbstr = qjdoc.toJson();
    QString qstr(qbstr);

    // 書き込む
    QTextStream out(&file);
    out << qstr << endl;

    qDebug() << "SummerIce::write_file <- "; // << qstr;

    file.close();

    return true;

}

// Windowのvisibilityを変える [static]
void Ice::set_window_visi(bool flag)
{
    if (!Ice::main_ready) {
        qDebug() << "SummerIce::Ice::set_window_visi @ not Ready";
        return;
    }
    qDebug() << "SummerIce::Ice::set_window_visi <- " << flag;
    //Ice::visi_ctx_sp->setContextProperty(ICEDEF_NAME_VISI, flag);

    int visi = QQmlProperty::read(Ice::main_obj, ICEDEF_NAME_VISI).toInt();
    qDebug() << "SummerIce::Ice::set_window_visi[1] : " << ICEDEF_NAME_VISI << "=" << QQmlProperty::read(Ice::main_obj, ICEDEF_NAME_VISI).toInt();

    if (flag) {
        if (visi == (int)QWindow::Minimized) {
            qDebug() << "SummerIce !restart!";
            restart_app(); // 別に再起動じゃない……こうすると画面が起動する
        }
        QQmlProperty::write(Ice::main_obj, ICEDEF_NAME_VISI, (int)QWindow::Maximized);
    } else {
        QQmlProperty::write(Ice::main_obj, ICEDEF_NAME_VISI, (int)QWindow::Minimized);
    }

    qDebug() << "SummerIce::Ice::set_window_visi[2] : " << ICEDEF_NAME_VISI << "=" << QQmlProperty::read(Ice::main_obj, ICEDEF_NAME_VISI).toInt();
}

// QMLが起動したらセットさせるもの。
void Ice::set_main_ready(bool flag)
{
    Ice::main_ready = flag;
}

// アプリ再起動/アラームマネージャー制御
static void restart_app()
{
    Ice::restart_app_main(1, false);
}
void Ice::restart_app_main(int interval_sec, bool isService)
{   //https://www.kdab.com/qt-on-android-how-to-restart-your-application/
    qDebug() << "[TRC]::restart_app <- " << interval_sec;

    auto activity = isService ? QtAndroid::androidService() : QtAndroid::androidActivity();
    auto packageManager = activity.callObjectMethod("getPackageManager"
                                                   ,"()Landroid/content/pm/PackageManager;"
                                                   );

    auto activityIntent = packageManager.callObjectMethod("getLaunchIntentForPackage"
                                                         ,"(Ljava/lang/String;)Landroid/content/Intent;"
                                                         ,activity.callObjectMethod("getPackageName","()Ljava/lang/String;").object()
                                                         );

    auto pendingIntent = QAndroidJniObject::callStaticObjectMethod("android/app/PendingIntent"
                                                                  , (isService ? "getService" : "getActivity")
                                                                  ,"(Landroid/content/Context;ILandroid/content/Intent;I)Landroid/app/PendingIntent;"
                                                                  ,activity.object()
                                                                  ,jint(0)
                                                                  ,activityIntent.object()
                                                                  ,QAndroidJniObject::getStaticField<jint>("android/content/Intent","FLAG_ACTIVITY_CLEAR_TOP")
                                                                  );

   auto alarmManager = activity.callObjectMethod("getSystemService"
                                                ,"(Ljava/lang/String;)Ljava/lang/Object;"
                                                ,QAndroidJniObject::getStaticObjectField("android/content/Context"
                                                                                        ,"ALARM_SERVICE"
                                                                                        ,"Ljava/lang/String;").object()
                                                );

   alarmManager.callMethod<void>("set"
                                ,"(IJLandroid/app/PendingIntent;)V"
                                ,QAndroidJniObject::getStaticField<jint>("android/app/AlarmManager", "RTC_WAKEUP")
                                ,jlong(QDateTime::currentMSecsSinceEpoch() + (interval_sec*1000))
                                ,pendingIntent.object()
                                );

   //QCoreApplication::exit();
   //qApp->quit();
}

// // フラグをセットする
//void Ice::set_window_flag() {
//    QAndroidJniObject activity = QtAndroid::androidActivity();

//    QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

//    //getWindow().addFlags(WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED
//	//                   | WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD);
//    // Constant Value:  524288 (0x00080000)
//    // Constant Value: 4194304 (0x00400000)
//    window.callMethod<void>("setFlags"
//                           ,"(II)V"
//                           , 0x00080000 | 0x00400000
//                           , 0x00080000 | 0x00400000
//                           );
//    qDebug() << "SummerIce::set_window_flag";
//}
