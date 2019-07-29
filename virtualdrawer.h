#ifndef VIRTUALDRAWER_H
#define VIRTUALDRAWER_H

#include <QObject>
#include "idrawer.h"
#include "logclient.h"

class VirtualDrawer : public QObject, public IDrawer, LogClient
{
    Q_OBJECT

signals:
    void signalMoveTo(float x, float y);
    void signalDrawTo(float x, float y);

    void signalReady();
public:
    VirtualDrawer();    
    virtual ~VirtualDrawer();

public slots:
    void ready();
    virtual void moveTo(float x, float y);
    virtual void drawTo(float x, float y);
};

#endif // VIRTUALDRAWER_H
