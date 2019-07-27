#ifndef SVGPROCESSOR_H
#define SVGPROCESSOR_H

#include <QObject>
#include "utils.h"
#include "task.h"
#include "paths.h"
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QDebug>
#include <QDomDocument>
#include "logclient.h"

#define SVG_BEGIN "<svg"
#define PATH_BEGIN "<path"
#define POLYGON_BEGIN "<polygon"
#define POINTS_BEGIN " d=\""
#define POINTS_END '\"'
#define END '>'

#define TAG_COUNTER 3
#define PATH_TAG "path"
#define POLYGON_TAG "polygon"
#define POLYLINE_TAG "polyline"
#define POINTS_ATTRIBUTE "d"

#define POINTS_M 'M'
#define POINTS_m 'm'
#define POINTS_C 'C'
#define POINTS_c 'c'
#define POINTS_L 'L'
#define POINTS_H 'H'
#define POINTS_V 'V'
#define POINTS_Z 'Z'

#define COORDS_LINE 1
#define COORDS_POINT 2
#define COORDS_CURVE 6

class InvalidPathException : public std::exception  {};
class InvalidFileException : public std::exception  {};

class PointsThread : public QThread, LogClient   {
    Q_OBJECT
private:
    QQueue<QString> &pathStringsQueue;
    QMutex &pathStringsQueueMutex;
    PathsPtr dataStorage;
    QMutex &storageMutex;

    void extractPoints(QString const& in str, Path out &points) const;
public:
    void run() override;
    explicit PointsThread(QQueue<QString> in &pathStringsQueue, QMutex &pathStringsQueueMutex,
                                      PathsPtr out dataStorage, QMutex &storageMutex, QObject *parent = nullptr);
signals:
    void finished();
};

class SVGProcessor : public QObject, LogClient
{
    Q_OBJECT
private:
    static const unsigned THREAD_COUNT = 4;

    bool process(QFile in &file, PathsPtr out &pPaths);
    void extractPaths(QFile in &file, QQueue<QString> out &strQueue);
    void sortPaths(PathsPtr &pPaths);

    void runInThreads(QQueue<QString> in &strQueue, PathsPtr& out pPaths);        

public:
    SVGProcessor();
    void test();

signals:
    void finished(Task& task);
    void draw(Task& task, PathsPtr paths);

private slots:

public slots:
    void startTask(Task& task);
};

#endif // SVGPROCESSOR_H
