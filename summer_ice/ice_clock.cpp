//#include <QGuiApplication>
#include <QCoreApplication>
//#include <QQmlApplicationEngine>

#ifdef __ANDROID__

#include "snow/snow.h"
Snow snow_inst(5);

#include "rep_pingpong_source.h"

class PingPong : public PingPongSource {
public slots:
    // PingPongSource interface
    void ping(const QString &msg) override {
        QString qstr_init = "init";
        QString qstr_piyo = "piyo"; //Snow::getOSTime();
        QString msg2(msg);

        qDebug() << "Summer_Ice::ping received <= " << msg;

        if (msg.contains(qstr_init)) {
            snow_inst.init_flag();
        } else {
            snow_inst.wakeup_finish();
            snow_inst.set_alerm(msg2);
        }
        emit pong(qstr_piyo);
    }
};

#endif /* __ANDROID__ */

int main(int argc, char *argv[])
{
    qDebug("Summer_Ice::Ice_clock waking[S]");

    QCoreApplication app(argc, argv);

#ifdef  __ANDROID__
    // wakelock初期化
    snow_inst.init_wakelocks();

    // アラームまでポーリングする
    snow_inst.start();

    // ping受信開始
    QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:replica")));
    PingPong pingPongServer;
    srcNode.enableRemoting(&pingPongServer);

#endif /*  __ANDROID__ */
    qDebug("Summer_Ice::Ice_clock waking[E]");
    return app.exec();
}
