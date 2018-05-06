#include "sky.h"

#include <QtGui/QGuiApplication>
#include <QtQml>

#ifdef    __ANDROID__
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#endif /* __ANDROID__ */

Sky::Sky(QObject *parent)
   : QObject(parent)
{
    qDebug() << "SummerIce::Sky > construction";
    this->pingEnable = false;
}

//virtual void Sky::vtable_dummy()
//{
//}

// サービスを開始する
void Sky::service_start() {
#ifdef    __ANDROID__
    QAndroidJniObject::callStaticMethod<void>(
                "com/summer/test/Spring",
                "startMyService",
                "(Landroid/content/Context;)V",
                QtAndroid::androidActivity().object()
                );

    qDebug("[TRC]::service_start(S)");

    bool ret = repNode.connectToNode(QUrl(QStringLiteral("local:replica_ping")));
    qDebug() << "ret = " << ret << endl;
    rep.reset(repNode.acquire<PingPongReplica>());
    bool res = rep->waitForSource(3000); //ms
    Q_ASSERT(res);
    //QObject::connect(rep.data(), &PingPongReplica::pong, [](const QString &msg){
    //    // pingしたときはいつでもここに戻ってくるっぽい
    //    qDebug() << msg;
    //});
    this->pingEnable = true;

#endif /* __ANDROID__ */
    qDebug("[TRC]::service_start(E)");

    return;
}

// ping
void Sky::ping2server(const QString &str) const
{
    qDebug() << "[TRC]::ping2server <- " << str;
#ifdef __ANDROID__
    if (this->pingEnable) {
        rep->ping(str);
    }
#endif /* __ANDROID__ */
}

//===========================================
//#include "Sky.moc"
