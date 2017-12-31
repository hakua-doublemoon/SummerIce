#ifndef ICE_H
#define ICE_H

#include <QObject>
#include <QJsonObject>

#if 0 //def    __ANDROID__
#include <QAndroidJniObject>
#endif /* __ANDROID__ */

class Ice : public QObject
{
    Q_OBJECT
public:
    Ice(QObject *parent = nullptr);
    void set_infos(QString alerm_file,
                   QString voice_file
                  );
    Q_INVOKABLE bool chkAlerm(int hour, int min, int thr = 120);
    static      bool chkAlerm_s(int hour, int min, int thr);
    Q_INVOKABLE QJsonObject readfile_alerm(void);
    static      QJsonObject readfile_alerm_s(QString &file_name_str);
    Q_INVOKABLE QString setAlerm(int hour, int min);
    //Q_INVOKABLE void rangAlerm(void);
    //Q_INVOKABLE void wakeup(void);
    Q_INVOKABLE void load_voice_list(void);
    Q_INVOKABLE void update_voice_select(int select);
    Q_INVOKABLE void update_voice_selectChara(int select);
    Q_INVOKABLE bool writefile_json(const QString &file_name_qstr, const QJsonObject &qjhash) const;
    //Q_INVOKABLE void set_window_flag(void);

    QString alermInfoFile;
    QString voiceListFile;
    QJsonObject voiceListObj;

signals:

public slots:

private:
};

#endif // ICE_H
