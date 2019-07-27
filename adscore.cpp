#include "adscore.h"

ADSCore::ADSCore(UIType uitype) : uitype(uitype)
{
    qDebug() << "ADS core initialization...";
    QObject::connect(&scheduler, SIGNAL(executeTask(Task&)), &processor, SLOT(startTask(Task&)));
}

void ADSCore::process(QFile &file)  {
    Task task(file);
    scheduler.addTask(task);
    scheduler.start();
}
