#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QObject>
#include "logclient.h"
#include "utils.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class SerialController : public QObject, LogClient
{
    Q_OBJECT
private:
    typedef QList<QSerialPortInfo> InfoList;

    QSerialPort serial;
    QString lastErrorStr;

    static const int waitForAnswerTimeout = 3000;   ///msecs
public:
    SerialController();
    inline bool isOpened() const;
    inline QString lastError() const;

signals:
    void incomingBytes(QByteArray const& bytes);
public slots:
    bool write(QByteArray const& bytes);
    bool writeAndWait(QByteArray const& bytes);
    bool open(QString const& portName = "COM1");
    bool open(QSerialPortInfo const& portInfo);
    void close();

private slots:
    void readyRead();
};

#endif // SERIALCONTROLLER_H
