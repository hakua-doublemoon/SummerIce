#ifndef STAR_H
#define STAR_H

#ifdef __ANDROID__

#include <QAndroidJniObject>

#include <QObject>

class Star : public QObject
{
    Q_OBJECT

public:
    Star(QObject *parent = nullptr);
    ~Star();

    Q_INVOKABLE void playFile(const QString &file);

signals:

private:
    QAndroidJniObject m_mediaPlayer;
};

#endif /* __ANDROID__ */
#endif // STAR_H
