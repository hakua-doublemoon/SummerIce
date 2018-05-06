#include "fakestar.h"
//#include "qsurfacetexture.h"

//#include <QAndroidJniObject>
#include <QQuickItem>

FakeStar::FakeStar(QObject *parent)
        : QObject(parent)
        //, player()
{
    qDebug() << "SummerIce::Star > construction";
}

FakeStar::~FakeStar()
{
    qDebug() << "SummerIce::Star > destruction";
}

void FakeStar::playFile(const QString &file)
{
    //QAndroidJniEnvironment env;
    qDebug() << "SummerIce::Star > fake / " << file;

    //player.setMedia(QUrl::fromLocalFile(file));
    //player.setVolume(50);
    //player.play();
}
