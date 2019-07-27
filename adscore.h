#ifndef ADSCORE_H
#define ADSCORE_H

#include <QObject>
#include <QDebug>
#include "scheduler.h"
#include "svgprocessor.h"
#include "logclient.h"

class ADSCore : public QObject, LogClient
{
    Q_OBJECT

public:
    enum UIType { Console, GUI, Dashboard };

private:
    UIType uitype;
    Scheduler scheduler;
    SVGProcessor processor;

private slots:


public slots:
    void process(QFile &file);

public:
    ADSCore(UIType uitype = Console);
    virtual ~ADSCore();
    void setUI(UIType uitype);
};

#endif // ADSCORE_H
