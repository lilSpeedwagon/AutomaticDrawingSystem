#ifndef DRAWCONTROLLER_H
#define DRAWCONTROLLER_H

#include <QObject>
#include "paths.h"
#include "task.h"
#include "utils.h"
#include "logclient.h"
#include "virtualdrawer.h"
#include "drawer.h"

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
    Drawer realDrawer;

    bool drawing = false;
    CurrentDrawing current;

public:
    DrawController();
    VirtualDrawer* getVirtualDrawerPtr();

signals:
    void signalMoveTo(float x, float y);
    void signalDrawTo(float x, float y);
    void signalFinish(Task task);

public slots:
    void draw(Task task, PathsPtr pPaths);
    void cancel();
    void error(QString const& err);

private slots:
    void ready();
};

#endif // DRAWCONTROLLER_H
