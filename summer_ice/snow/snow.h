#ifndef SNOW_H
#define SNOW_H

#include <QObject>

#include <QThread>
#include <QString>
#include <QAndroidJniObject>

//#include "rep_pingpong_replica.h"

#include "ice/ice.h"

class Snow : public QThread //QObject
{
    Q_OBJECT
public:
    Snow(int left_count = 5, QObject *parent = nullptr);
    ~Snow();
    //staticをつけると静的メンバ関数になる
    //static QString  getOSTime(); //getOSTime(void)ではないらしい //
    void wakeup(void);
    void wakeup_finish(void);
    void init_wakelocks(void);
    //void prepare_ping(void);
    //void ping2server(const QString &) const;
    void init_flag(void);
    void set_alerm(QString &qstr);

private:
    QAndroidJniObject inst_winter;
    int  wake_count;
    bool isWaked_s;
    bool isWaked_c;
    int  hour;
    int  min;
    //bool isWakelock;

protected:
    void run();

signals:

//public slots:

private slots:
    void timerHit(void);
};

#endif // SNOW_H
