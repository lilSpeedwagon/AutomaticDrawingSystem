#ifndef LOGCLIENT_H
#define LOGCLIENT_H

#include <QObject>
#include <QTime>
#include <QDebug>
#include <type_traits>

class LogClient
{
private:
    QString logName;
    QString logNameWithColon;
    static QString getTimeStr();

public:
    LogClient(QString logName = "Log");
    void setName(QString const& name);
    QString name() const;
    void log(QString const& message) const;
};

#endif // LOGCLIENT_H
