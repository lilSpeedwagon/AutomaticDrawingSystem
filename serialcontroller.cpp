#include "serialcontroller.h"

SerialController::SerialController()    {
    setName("SerialController");
    log("serial port init");
    QObject::connect(&serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(error(QSerialPort::SerialPortError)));
    QObject::connect(&serial, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void SerialController::readyRead()  {
    QByteArray bytes = serial.readAll();
    emit incomingBytes(bytes);
    serial.clear();
}

bool SerialController::open(QSerialPortInfo const& info)    {
    serial.setPort(info);
    log("opening serial port \"" + serial.portName() + "\" in read-write mode");

    bool configResult = true;
    configResult &= serial.setBaudRate(QSerialPort::Baud9600);
    configResult &= serial.setParity(QSerialPort::NoParity);
    configResult &= serial.setFlowControl(QSerialPort::SoftwareControl);
    if (!configResult)  {
        logWarn("Cannot configure port");
    }

    bool isOpened = serial.open(QIODevice::ReadWrite);
    if (isOpened)    {
        log("port opened");
    }

    return isOpened;
}

bool SerialController::open(QString const& portName)   {
    InfoList ports = QSerialPortInfo::availablePorts();
    InfoList::const_iterator iter = std::find_if(ports.begin(), ports.end(), [&portName](QSerialPortInfo const& i){ return i.portName() == portName; });
    if (iter == ports.end())    {
        logError("cannot find device with name: " + portName);
        return false;
    }

    return open(*iter);
}

void SerialController::close()  {
    log("closing port...");
    serial.close();
}

bool SerialController::write(QByteArray const& bytes)   {
    bool result = serial.isOpen() ? true : open();

    if (result) {
        serial.write(bytes);
        serial.flush();
    }

    return result;
}

bool SerialController::writeAndWait(QByteArray const& bytes)   {
    bool result = write(bytes);
    result &= serial.waitForReadyRead(waitForAnswerTimeout);
    return result;
}

bool SerialController::isOpened() const {
    return serial.isOpen();
}

QString SerialController::lastError() const {
    return lastErrorStr;
}
