#include <QGuiApplication>
#include <QQmlApplicationEngine>

#ifdef    __ANDROID__
#include <QAndroidService>
#endif /* __ANDROID__ */

#include <QQuickView>

#include <QQmlContext>
#include <QThread>
#include <QQmlComponent>
#include <QTime>

#ifdef    __ANDROID__
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#endif /* __ANDROID__ */

#include "execute/snow.h"
#include "execute/ice.h"
#include "execute/sky.h"

#ifdef    __ANDROID__
#include "execute/star.h"
#else
#include "star/fakestar.h"
#endif /* __ANDROID__ */

#ifdef __ANDROID__
#include "rep_pingpong_source.h"

class PingPong : public PingPongSource {
public slots:
    // PingPongSource interface
    void ping(const QString &msg) override {
        // サービスで画面を起こすために必要かなーと思ってるがまだサービスだけで起動できてないので役に立ってない
        QString qstr_piyo = "piyo"; //Snow::getOSTime();

        QStringList ql = msg.split(":");
        QString tmp;
        tmp = ql.at(1);
        int hour = tmp.toInt();
        tmp = ql.at(2);
        int min  = tmp.toInt();

        /* 現在時刻との差を計算する */
        QTime qdt_now = QTime::currentTime();
        QTime qdt_tom = QTime(hour, min);
        int period = qdt_now.secsTo(qdt_tom);
        if (period < 0) {
            period += 60*60*24;
        }

        qDebug() << "Summer_Ice::ping received <= " << msg << " : " << period;
        qDebug() << msg << " -> " << hour << ":" << min;
        //Ice::restart_app_main(period, true);

        emit pong(qstr_piyo);
    }
};

#endif /* __ANDROID__ */

/*------------*/
/* ProtoTypes */
/*------------*/
void set_voice_resource(QQmlApplicationEngine &engine);

/*------------------*/
/* Global Variables */
/*------------------*/
QString voice_resource = "";

/*---------*/
/* defines */
/*---------*/
#define ALERM_FILE "bluenote.txt"
#define VOICE_FILE "voice_list.json"
//#define ANDROID_VOICE_DIR "/storage/sdcard1/SummerAlerm/"
#define ANDROID_VOICE_DIR ":/images/" // でもここに音声ファイルを置いておいてもこのパスを解析できるのはQtだけだからAndroidのAPIが鳴らそうとすると落ちる

//---------------------------------------------------------------------
// メイン関数
int summer_clock(int argc, char *argv[]);
int ice_clock(int argc, char *argv[]);
int main(int argc, char *argv[])
{
    if(argc > 1 && strcmp(argv[1], "-service") == 0) {
        qInfo() << "Summer_Ice::Service starting...";
        return ice_clock(argc, argv);
    } else {
        qInfo() << "Summer_Ice::Activity starting...";
        return summer_clock(argc, argv);
    }
}

// アクティビティ
int summer_clock(int argc, char *argv[])
{
    qDebug("Summer_Ice::Summer_clock waking");
    QString qstrd;
    qstrd = QString("[currentThreadId] summer_clock() = 0x%1").arg((ulong)QThread::currentThreadId(), 0, 16);
    qDebug() << qstrd;

    Ice  ice_inst;
    Sky  sky_inst;

#ifdef    __ANDROID__
    Star star_inst;
#else
    FakeStar star_inst;
#endif /* __ANDROID__ */

    Snow* snow_inst_p;
    snow_inst_p = new Snow(10, "Summer");
    //QThread* snow_thread = new QThread;
    //snow_inst_p->moveToThread(snow_thread);
    //snow_thread->start();
    snow_inst_p->start(); // なんかよくわからないがスレッドを切り離してstartできない…
    ice_inst.snow_p = snow_inst_p;
    ice_inst.snow_ready = true;

    qDebug("D/Summer_clock waking[1]");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //C++クラスの登録
    engine.rootContext()->setContextProperty("Ice",  &ice_inst);
    engine.rootContext()->setContextProperty("Sky",  &sky_inst);
    engine.rootContext()->setContextProperty("Star", &star_inst);

    //管理リソースの設定
    set_voice_resource(engine);
    ice_inst.set_infos(ALERM_FILE
                      ,VOICE_FILE
                      ,voice_resource.append(VOICE_FILE)
                      );

    //アラームのリスト
    ice_inst.load_voice_list();
    engine.rootContext()->setContextProperty("voice_list", ice_inst.voiceListObj);

    //サービス起床
    sky_inst.service_start();
#ifdef __ANDROID__
    engine.rootContext()->setContextProperty("ping_pong", sky_inst.rep.data());
#else
    engine.rootContext()->setContextProperty("ping_pong", &sky_inst);
#endif

    QQmlComponent component(&engine, QUrl(QLatin1String("qrc:/main.qml")));
    Ice::main_obj = component.create();

    return app.exec();
}

//---------------------------------------------------------------------
// リソースの場所を設定する
void set_voice_resource(QQmlApplicationEngine &engine)
{
#ifdef __ANDROID__
    voice_resource = ANDROID_VOICE_DIR;
#else
    voice_resource = qgetenv("VOICE_RESOURCE");
#endif /* __ANDROID__ */
    engine.rootContext()->setContextProperty("voice_resource", voice_resource);
    qDebug() << "VOICE RESOURCE = " << voice_resource;
}

//---------------------------------------------------------------------
// サービス
int ice_clock(int argc, char *argv[])
{
#ifdef    __ANDROID__
    QAndroidService app(argc, argv);
#else
    QCoreApplication app(argc, argv);
#endif

    qDebug("Summer_Ice::Ice_clock waking[S]");
    QString qstrd;
    qstrd = QString("[currentThreadId] ice_clock() = 0x%1").arg((ulong)QThread::currentThreadId(), 0, 16);
    qDebug() << qstrd;

#ifdef  __ANDROID__
    // wakelock初期化
    //snow_inst.init_wakelocks();

    // アラームまでポーリングする
    //snow_inst.start();

    // ping受信開始
    QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:replica_ping")));
    PingPong pingPongServer;
    srcNode.enableRemoting(&pingPongServer);

#endif /*  __ANDROID__ */
    qDebug("Summer_Ice::Ice_clock waking[E]");
    return app.exec();
}
