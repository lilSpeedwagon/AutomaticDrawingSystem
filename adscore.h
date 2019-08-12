#ifndef ADSCORE_H
#define ADSCORE_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>
#include "scheduler.h"
#include "svgprocessor.h"
#include "logclient.h"
#include "paths.h"
#include "idrawer.h"
#include "virtualdrawer.h"
#include "drawcontroller.h"

class ADSCore : public QObject, LogClient
{
    Q_OBJECT

public:
    enum UIType { Console, GUI, Dashboard };

private:
    QQmlApplicationEngine engine;
    UIType uitype;

    //threads
    QThread controllerThread;

    //components
    Scheduler scheduler;
    SVGProcessor processor;
    DrawController* controller;

signals:
    void signalDraw(Paths& paths);
    void signalLog(QString const& str);

public slots:
    void process(QFile &file);
    void testProcess();

public:
    ADSCore(UIType uitype = Console);
    virtual ~ADSCore();
    void setUI(UIType uitype);
    void initGUI();
};

#endif // ADSCORE_H
