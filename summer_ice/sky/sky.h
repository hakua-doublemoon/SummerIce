#ifndef SKY_H
#define SKY_H

#include <QObject>

#ifdef __ANDROID__
#include "rep_pingpong_replica.h"
#endif /* __ANDROID__ */

class Sky : public QObject
{
    Q_OBJECT
public:
    explicit Sky(QObject *parent = 0);
    Q_INVOKABLE void service_start(void);
    Q_INVOKABLE void ping2server(const QString &str) const;

#ifdef __ANDROID__
    Q_INVOKABLE QSharedPointer<PingPongReplica> rep;
    Q_INVOKABLE QRemoteObjectNode               repNode;
#endif /* __ANDROID__ */

    bool    pingEnable;

signals:

public slots:
};

#endif // SKY_H
