#include "drawer.h"

const char Drawer::BYTE_DRAW = 'd';
const char Drawer::BYTE_MOVE = 'm';

Drawer::Drawer()
{
    setName("Drawer");
    log("real drawer init");

    QObject::connect(&serial, SIGNAL(incomingBytes(QByteArray const&)), this, SLOT(incomingBytes(QByteArray const&)));
}

void Drawer::drawTo(float x, float y)   {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);

    stream << BYTE_DRAW;
    stream << x;
    stream << y;

    if (!serial.write(bytes))   {
        emit signalError("");
    }
}

void Drawer::moveTo(float x, float y)   {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);

    stream << BYTE_MOVE;
    stream << x;
    stream << y;

    if (!serial.write(bytes))   {
        emit signalError("");
    }
}

void Drawer::ready()    {
    emit signalReady();
}

void Drawer::incomingBytes(QByteArray const& bytes) {
    if (bytes == "done")    {
        ready();
    }
}
