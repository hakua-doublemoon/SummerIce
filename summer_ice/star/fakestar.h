#ifndef STAR_H
#define STAR_H

#include <QObject>

//#include <QMediaPlayer>

class FakeStar : public QObject
{
    Q_OBJECT

public:
    FakeStar(QObject *parent = nullptr);
    ~FakeStar();

    Q_INVOKABLE void playFile(const QString &file);

    //QMediaPlayer player;

signals:

private:
};

#endif // STAR_H
