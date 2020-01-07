#ifndef DRAWER_H
#define DRAWER_H

#include <QDataStream>
#include "idrawer.h"
#include "logclient.h"
#include "serialcontroller.h"

class Drawer : public QObject, public IDrawer, LogClient
{
    Q_OBJECT
private:
    SerialController serial;
    static const char BYTE_DRAW;
    static const char BYTE_MOVE;
public:
    Drawer();
    virtual ~Drawer() {}
signals:
    void signalReady();
    void signalError(QString const& err);
public slots:
    virtual void moveTo(float x, float y);
    virtual void drawTo(float x, float y);
    void ready();
private slots:
    void incomingBytes(QByteArray const& bytes);
};

#endif // DRAWER_H
