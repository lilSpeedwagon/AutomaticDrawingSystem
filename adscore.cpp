#include "adscore.h"

ADSCore::ADSCore(UIType uitype) : uitype(uitype)
{
    setName("ADSCore");
    log("ADS core initialization...");
    QObject::connect(&scheduler, SIGNAL(executeTask(Task&)), &processor, SLOT(executeTask(Task&)));
    QObject::connect(&processor, SIGNAL(finished(Task&)), &scheduler, SLOT(taskFinished(Task&)));
}

ADSCore::~ADSCore() {
    log("ADS core terminated.");
}

void ADSCore::process(QFile &file)  {
    Task task(file);
    scheduler.addTask(task);
    scheduler.start();
}
