#ifndef DRAWCONTROLLER_H
#define DRAWCONTROLLER_H

#include <QObject>
#include "paths.h"
#include "task.h"
#include "utils.h"
#include "logclient.h"
#include "virtualdrawer.h"

class DrawController : public QObject, LogClient
{
    Q_OBJECT
private:
    struct CurrentDrawing   {
        Task* task;
        PathsPtr pPaths;        
        int pathIndex, pointIndex;
    };

    VirtualDrawer virtualDrawer;
    //IDrawer realDriwer;

    bool drawing = false;
    CurrentDrawing current;
    void drawNextPoint();

public:
    DrawController();
    VirtualDrawer* getVirtualDrawerPtr();

signals:
    void signalMoveTo(float x, float y);
    void signalDrawTo(float x, float y);
    void signalFinish(Task task);

public slots:
    void draw(Task task, PathsPtr pPaths);

private slots:
    void ready();
};

#endif // DRAWCONTROLLER_H
