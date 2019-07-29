#ifndef IDRAWER_H
#define IDRAWER_H

#include <QObject>
#include "task.h"
#include "paths.h"
#include "utils.h"

interface IDrawer
{
    virtual ~IDrawer() {}
signals:
    virtual void signalReady() = 0;
public slots:
    virtual void moveTo(float x, float y) = 0;
    virtual void drawTo(float x, float y) = 0;

};

#endif // IDRAWER_H
