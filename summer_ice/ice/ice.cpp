#include "ice.h"

#include <QtGui/QGuiApplication>
#include <QtQml>

#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QTime>

#include <cmath>

#ifdef __ANDROID__
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#endif /* __ANDROID__ */

//=================================================
// prototypes
//=================================================
//static void    exec_AlmMng(int period);
//→  https://www.kdab.com/qt-on-android-how-to-restart-your-application/
static QString cnvDiffSec(int sec);

//=================================================
// methods
//=================================================
Ice::Ice(QObject *parent)
   : QObject(parent)
{
    qDebug() << "SummerIce::Ice > construction";
}

void Ice::set_infos(QString alerm_file,
                    QString voice_file
                   )
{
    this->alermInfoFile = alerm_file;
    this->voiceListFile = voice_file;
}

//virtual void Ice::vtable_dummy()
//{
//}

// alerm
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

    //↑ポーリングすることにしたので不要……
    //exec_AlmMng(period);


    /* セットされた時間を保存する */
    QString qs_alerm = qdt_tom.toString("H:m:s");
    QJsonObject qjhash;
    qjhash["time"] = qs_alerm;
    this->writefile_json(this->alermInfoFile, qjhash);

    return qstr_td;
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
    if (abs(period) < thr) { // とりあえず2分以内なら
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
    //QByteArray qbstr;
    //QJsonDocument qjdoc;
    //QJsonObject qjhash;
    //QString qs_alerm;

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
    //return this->voice_list_obj;

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
