#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickView>

#include <QQmlContext>
#include <QThread>

#ifdef    __ANDROID__
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#endif /* __ANDROID__ */

#include "ice/ice.h"
#include "sky/sky.h"

#ifdef    __ANDROID__
#include "star/star.h"
#else
#include "star/fakestar.h"
#endif /* __ANDROID__ */

#ifdef __ANDROID__
#include "rep_pingpong_replica.h"
//#include "rep_pingpong_source.h"
#endif /* __ANDROID__ */

/*------------*/
/* ProtoTypes */
/*------------*/
//void connect_service(QQmlApplicationEngine *engine);
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
#define ANDROID_VOICE_DIR "qrc:/images/"

//---------------------------------------------------------------------
// メイン関数
int main(int argc, char *argv[])
{
    qDebug("Summer_Ice::Summer_clock waking");

    Ice  ice_inst;
    Sky  sky_inst;

#ifdef    __ANDROID__
    Star star_inst;
#else
    FakeStar star_inst;
#endif /* __ANDROID__ */

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

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

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

