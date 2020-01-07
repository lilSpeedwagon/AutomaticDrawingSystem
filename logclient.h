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
    static const QString errorStr;
    static const QString warnStr;

public:
    LogClient(QString logName = "Log");
    void setName(QString const& name);
    QString name() const;
    void log(QString const& message) const;
    void logWarn(QString const& message) const;
    void logError(QString const& message) const;
};

#endif // LOGCLIENT_H
