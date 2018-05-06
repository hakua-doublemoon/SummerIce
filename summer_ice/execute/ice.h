#ifndef ICE_H
#define ICE_H

#include <QObject>
#include <QJsonObject>
#include <QQmlApplicationEngine>
#include <QGuiApplication>

#include <execute/snow.h>

#if 0 //def    __ANDROID__
#include <QAndroidJniObject>
#endif /* __ANDROID__ */

#define ICEDEF_NAME_VISI "visibilityFlag"

class Ice : public QObject
{
    Q_OBJECT
public:
    Ice(QObject *parent = nullptr);
    void set_infos(QString alerm_file,
                   QString voice_file,
                   QString voice_file_ext
                   );
    Q_INVOKABLE bool chkAlerm(int hour, int min, int thr = 120);
    static      bool chkAlerm_s(int hour, int min, int thr);
    Q_INVOKABLE QJsonObject readfile_alerm(void);
    static      QJsonObject readfile_alerm_s(QString &file_name_str);
    Q_INVOKABLE QString setAlerm(int hour, int min);
    Q_INVOKABLE void    set_alerm_monitor(int hour, int min);
    Q_INVOKABLE void load_voice_list(void);
    Q_INVOKABLE void update_voice_select(int select);
    Q_INVOKABLE void update_voice_selectChara(int select);
    Q_INVOKABLE bool writefile_json(const QString &file_name_qstr, const QJsonObject &qjhash) const;

    Q_INVOKABLE void set_main_ready(bool flag);

    QString alermInfoFile;
    QString voiceListFile;
    QJsonObject voiceListObj;

    Snow* snow_p;
    bool  snow_ready;

    // static
    static QObject* main_obj;
    static bool     main_ready;
    static void set_window_visi(bool flag);
    static void restart_app_main(int interval_sec, bool isService);

signals:

public slots:

private:
};

#endif // ICE_H
