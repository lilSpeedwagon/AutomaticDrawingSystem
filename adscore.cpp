#include "adscore.h"

ADSCore::ADSCore(UIType uitype) : uitype(uitype)
{
    setName("ADSCore");
    log("ADS core initialization...");


    //init components
    controller = new DrawController();
    controller->moveToThread(&controllerThread);

    //signal init
    QObject::connect(&scheduler, SIGNAL(executeTask(Task)), &processor, SLOT(executeTask(Task)));
    QObject::connect(&processor, SIGNAL(finished(Task)), &scheduler, SLOT(taskFinished(Task)));

    QObject::connect(&processor, SIGNAL(draw(Task, PathsPtr)), controller, SLOT(draw(Task, PathsPtr)));
    QObject::connect(controller, SIGNAL(signalFinish(Task)), &scheduler, SLOT(taskFinished(Task)));

    initGUI();

    //run threads
    controllerThread.start();

    log("ADS core ready");
}

ADSCore::~ADSCore() {
    log("ADS core terminated.");
    delete controller;
}

void ADSCore::testProcess() {
    QFile file("C:\\Users\\ded_e\\Desktop\\sketch_180903b\\data\\tiki.svg");
    process(file);
}

void ADSCore::process(QFile &file)  {
    Task task(file);
    scheduler.addTask(task);
    scheduler.start();
}

void ADSCore::initGUI() {
    engine.rootContext()->setContextProperty("ADSCore", this);
    engine.rootContext()->setContextProperty("VirtualDrawer", controller->getVirtualDrawerPtr());
    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
    if (engine.rootObjects().isEmpty()) {
        logError("Error while loading QML GUI engine. Terminating...");
        exit(1);
    }
}
