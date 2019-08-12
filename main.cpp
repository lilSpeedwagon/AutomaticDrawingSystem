#include <QCoreApplication>
#include <QApplication>
#include "adscore.h"
#include <QDebug>
#include <QTime>

void initMetaTypes()    {

    int taskClassId = qRegisterMetaType<Task>();
    int pathsPtrClassId = qRegisterMetaType<PathsPtr>();
}

int main(int argc, char *argv[])
{
    qDebug() << "Application initializating...";
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    ADSCore* core = new ADSCore();   

    return a.exec();
}
