#include "logclient.h"

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

QString LogClient::getTimeStr() {
    return QTime::currentTime().toString();
}
