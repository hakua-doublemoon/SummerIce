#ifdef __ANDROID__

#include "star.h"

#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
//#include <QAndroidJniObject>
#include <QQuickItem>
#include <QAndroidJniEnvironment>
#include <QThread>

Star::Star(QObject *parent)
    : QObject(parent)
    , m_mediaPlayer("android/media/MediaPlayer")
    //, inst_winter("com/summer/test/Winter")
{
    qDebug() << "SummerIce::Star > construction";
}

Star::~Star()
{
    qDebug() << "SummerIce::Star > destruction";

    //QAndroidJniEnvironment env;
    m_mediaPlayer.callMethod<void>("stop");
    m_mediaPlayer.callMethod<void>("reset");
    m_mediaPlayer.callMethod<void>("release");
}

void Star::playFile(const QString &file)
{
    //QAndroidJniEnvironment env;
    qDebug() << "SummerIce::Star > stop";
    m_mediaPlayer.callMethod<void>("stop"); //     engine.rootContext()->setContextProperty("Ice", &ice_inst);.

    qDebug() << "SummerIce::Star > reset";
    m_mediaPlayer.callMethod<void>("reset"); // try to reset the media player.

    // http://developer.android.com/reference/android/media/MediaPlayer.html#setDataSource(java.lang.String) - the path of the file, or the http/rtsp URL of the stream you want to play.
    //QAndroidJniObject j_path = QAndroidJniObject::fromString(file);
    //QAndroidJniObject j_uri  = QAndroidJniObject("android/net/Uri","(Ljava/lang/String;)V",j_path.object<jstring>());
    qDebug() << "SummerIce::Star > set:" << file;
    /*
    m_mediaPlayer.callMethod<void>("setDataSource"
                                  ,"(Landroid/content/Context;Landroid/net/Uri;)V"
                                  ,QtAndroid::androidActivity().object()
                                  ,j_uri.object<jobject>()
                                  );
                                  */
    m_mediaPlayer.callMethod<void>("setDataSource"
                                  ,"(Ljava/lang/String;)V"
                                  ,QAndroidJniObject::fromString(file).object()
                                  );

    // prepare media player
    qDebug() << "SummerIce::Star > prepare";
    m_mediaPlayer.callMethod<void>("prepare");

    // start playing
    qDebug() << "SummerIce::Star > start";
    m_mediaPlayer.callMethod<void>("start");
}

#endif /* __ANDROID__ */
