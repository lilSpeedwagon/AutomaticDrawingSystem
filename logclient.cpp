#include "logclient.h"

const QString LogClient::errorStr = "Error. ";
const QString LogClient::warnStr = "Warning. ";

LogClient::LogClient(QString logName) : logName(logName), logNameWithColon(' ' + logName + ": ")
{
}

void LogClient::setName(const QString &name)    {
    logName = name;
    logNameWithColon = ' ' + name + ": ";
}

QString LogClient::name() const {
    return logName;
}

void LogClient::log(QString const& message) const {
    qDebug().noquote() << getTimeStr() + logNameWithColon + message;
}

void LogClient::logError(const QString &message) const {
    qDebug().noquote() << getTimeStr() + logNameWithColon + errorStr + message;
}

void LogClient::logWarn(const QString &message) const {
    qDebug().noquote() << getTimeStr() + logNameWithColon + warnStr + message;
}

QString LogClient::getTimeStr() {
    return QTime::currentTime().toString();
}
