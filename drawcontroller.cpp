#include "drawcontroller.h"

DrawController::DrawController()
{
    setName("DrawController");
    log("draw controller init");

    QObject::connect(this, SIGNAL(signalMoveTo(float, float)), &realDrawer, SLOT(moveTo(float, float)));
    QObject::connect(this, SIGNAL(signalDrawTo(float, float)), &realDrawer, SLOT(drawTo(float, float)));
    QObject::connect(&realDrawer, SIGNAL(signalReady()), this, SLOT(ready()));
    QObject::connect(&realDrawer, SIGNAL(signalError(QString const&)), this, SLOT(error(QString const&)));

    QObject::connect(this, SIGNAL(signalMoveTo(float, float)), &virtualDrawer, SLOT(moveTo(float, float)));
    QObject::connect(this, SIGNAL(signalDrawTo(float, float)), &virtualDrawer, SLOT(drawTo(float, float)));
}

void DrawController::draw(Task task, PathsPtr pPaths)   {
    if (task.isStarted() && !task.isFinished())   {
        log("start drawing");

        current = { &task, std::move(pPaths), 0, 0 };
        drawing = true;

        try {
            //search first not empty path
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
        logError("invalid task. Fault");
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
            log("drawing " + Utils::toStr(p));
            if (p.draw) {
                emit signalDrawTo(p.x, p.y);
            }   else    {
                emit signalMoveTo(p.x, p.y);
            }                        

            current.pointIndex++;

            if (current.pointIndex >= current.pPaths->at(current.pathIndex).size())   {
                current.pointIndex = 0;
                do {
                    current.pathIndex++;
                } while(current.pPaths->at(current.pathIndex).empty());
            }
        } catch (Paths::OutOfBoundException&) {
            log("Paths end reached. Finish drawing.");
            drawing = false;
            current.task->finish(Task::SUCCESS);
            emit signalFinish(*current.task);
        }
    }
}

void DrawController::cancel()   {
    logError("Drawing was cancelled");
    drawing = false;
    current.task->finish(Task::CANCEL);
    emit signalFinish(*current.task);
}

void DrawController::error(QString const& err)  {
    logError("Drawing was stopped due an error. Error details: " + err);
    drawing = false;
    current.task->finish(Task::FAULT);
    emit signalFinish(*current.task);
}
