#include <QCoreApplication>
#include "adscore.h"
#include <QDebug>
#include <QTime>

void test() {

}

int main(int argc, char *argv[])
{
    qDebug() << "Application initializating...";
    QCoreApplication a(argc, argv);

    //SVGProcessor proc;
    //proc.test();

    QTime t;
    t.start();

    ADSCore core;
    QFile file("C:\\Users\\ded_e\\Desktop\\sketch_180903b\\data\\steinsS.svg");
    core.process(file);

    qDebug() << "4 thread time " << t.elapsed();

    return a.exec();
}
