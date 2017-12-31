#ifndef STAR_H
#define STAR_H

#ifdef __ANDROID__

#include <QAndroidJniObject>

#include <QObject>
//#include <QPointer>

//class QSurfaceTexture;
class Star : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QSurfaceTexture *videoOut READ videoOut WRITE setVideoOut NOTIFY videoOutChanged)

public:
    Star(QObject *parent = nullptr);
    ~Star();

    //QSurfaceTexture *videoOut() const;
    //void setVideoOut(QSurfaceTexture *videoOut);

    Q_INVOKABLE void playFile(const QString &file);

signals:
    //void videoOutChanged();

private:
    //QPointer<QSurfaceTexture> m_videoOut;
    QAndroidJniObject m_mediaPlayer;
};

#endif /* __ANDROID__ */
#endif // STAR_H
