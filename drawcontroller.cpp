#include "drawcontroller.h"

DrawController::DrawController()
{
    setName("DrawController");
    log("draw controller init");
    QObject::connect(this, SIGNAL(signalMoveTo(float, float)), &virtualDrawer, SLOT(moveTo(float, float)));
    QObject::connect(this, SIGNAL(signalDrawTo(float, float)), &virtualDrawer, SLOT(drawTo(float, float)));
    QObject::connect(&virtualDrawer, SIGNAL(signalReady()), this, SLOT(ready()));
}

void DrawController::draw(Task& task, PathsPtr pPaths)   {
    if (task.isStarted() && !task.isFinished())   {
        log("start drawing");

        current = { &task, std::move(pPaths), 0, 0 };
        drawing = true;

        try {
            //search first not empry path
            while (!current.pPaths->at(current.pathIndex).size())    {
                current.pathIndex++;
            }
            log("first not-empty path found");

            ready();

        } catch (Paths::OutOfBoundException& ) {
            log("empty paths. Cancel");
            task.finish(Task::SUCCESS);
        }

    }   else {
        log("invalid task. Fault");
        task.finish(Task::FAULT);
    }
}

VirtualDrawer* DrawController::getVirtualDrawerPtr()   {
    return &virtualDrawer;
}

void DrawController::ready()    {
    if (drawing)    {
        try {
            const Point &p = current.pPaths->at(current.pathIndex).at(current.pointIndex);
            if (p.draw) {
                signalDrawTo(p.x, p.y);
            }   else    {
                signalMoveTo(p.x, p.y);
            }

            current.pointIndex++;

            while (current.pointIndex >= current.pPaths->at(current.pathIndex).size())   {
                current.pathIndex++;
            }
        } catch (Paths::OutOfBoundException&) {
            log("Paths end reached. Finish drawing.");
            drawing = false;
            current.task->finish(Task::SUCCESS);
            emit signalFinish(*current.task);
        }
    }
}
