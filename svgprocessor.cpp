#include "svgprocessor.h"

PointsThread::PointsThread(QQueue<QString> in &pathStringsQueue, QMutex &pathStringsQueueMutex,
                           PathsPtr out dataStorage, QMutex &storageMutex, QObject *parent)
    : pathStringsQueue(pathStringsQueue), pathStringsQueueMutex(pathStringsQueueMutex),
      dataStorage(dataStorage), storageMutex(storageMutex)
{
    setParent(parent);
    setName("Unknown thread");
}

/*
    lowerBound chars mark coords as relative
    upperBound chars mark coors as absolyte
*/
void PointsThread::extractPoints(const QString &str, Path &path) const  {
    int currentPos = 0;
    Point beginPoint, lastPoint;
    while (currentPos < str.length() && currentPos != NOT_FOUND)   {
        if (Utils::equals(str[currentPos], POINTS_M))    {
            //qDebug() << "M at " << currentPos;
            float nums[COORDS_POINT];
            int newPos;

            try {
                newPos = Utils::stringToFloats<COORDS_POINT>(str, currentPos, nums);
                //qDebug() << "newPos: " << newPos;
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            if (str[currentPos].isLower())  {
                nums[0] += lastPoint.x;
                nums[1] += lastPoint.y;
            }
            //qDebug() << "new point " << nums[0] << nums[1];
            beginPoint = lastPoint = { nums[0], nums[1], false };
            path << beginPoint;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_C))    {
            //qDebug() << "C at " << currentPos;
            float nums[COORDS_CURVE];
            int newPos;
            try {
                newPos = Utils::stringToFloats<COORDS_CURVE>(str, currentPos, nums);
                //qDebug() << "newPos: " << newPos;
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            if (str[currentPos].isLower())  {
                nums[0] += lastPoint.x;
                nums[1] += lastPoint.y;
                nums[2] += lastPoint.x;
                nums[3] += lastPoint.y;
                nums[4] += lastPoint.x;
                nums[5] += lastPoint.y;
            }
           // qDebug() << "new curve " << nums[0] << nums[1] << nums[2] << nums[3] << nums[4] << nums[5];
            Curve c = { lastPoint, { nums[0], nums[1] },
               { nums[2], nums[3] }, { nums[4], nums[5] } };
            path << c;
            lastPoint = c.end;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_L))   {
            //qDebug() << "V at " << currentPos;
            float nums[COORDS_POINT];
            int newPos;

            try {
                newPos = Utils::stringToFloats<COORDS_POINT>(str, currentPos, nums);
                //qDebug() << "newPos: " << newPos;
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            if (str[currentPos].isLower())  {
                nums[0] += lastPoint.x;
                nums[1] += lastPoint.y;
            }

            //qDebug() << "new point " << nums[0] << nums[1];
            lastPoint = { nums[0], nums[1], true };
            path << lastPoint;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_V))   {
            //qDebug() << "V at " << currentPos;
            float nums[COORDS_LINE];
            int newPos;

            try {
                newPos = Utils::stringToFloats<COORDS_LINE>(str, currentPos, nums);
                //qDebug() << "newPos: " << newPos;
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            if (str[currentPos].isLower())  {
                nums[0] += lastPoint.y;
            }

            //qDebug() << "new point " << lastPoint.x << nums[0];
            lastPoint = { lastPoint.x, nums[0], true };
            path << lastPoint;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_H))   {
            //qDebug() << "H at " << currentPos;
            float nums[COORDS_LINE];
            int newPos;

            try {
                newPos = Utils::stringToFloats<COORDS_LINE>(str, currentPos, nums);
                //qDebug() << "newPos: " << newPos;
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            if (str[currentPos].isLower())  {
                nums[0] += lastPoint.x;
            }

            //qDebug() << "new point " << lastPoint.x << nums[0];
            lastPoint = { nums[0], lastPoint.y, true };
            path << lastPoint;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_Z))   {
            path << beginPoint;
            lastPoint = beginPoint;
            currentPos++;
            continue;
        }

        currentPos++;
    }
}

void PointsThread::run()    {    
    void* threadPtr = QThread::currentThreadId();
    setName("Thread " + Utils::toStr((qint64) threadPtr));

    log("thread start");

    for (;;)    {
        pathStringsQueueMutex.lock();
        if (pathStringsQueue.empty())   {
            pathStringsQueueMutex.unlock();
            break;
        }
        QString pointsStr = pathStringsQueue.dequeue();        
        pathStringsQueueMutex.unlock();


        try {
            //qDebug() << QThread::currentThreadId() << ": processing " << pointsStr;
            Path path;
            extractPoints(pointsStr, path);           
            //qDebug() << QThread::currentThreadId() << ": processing finished" << pointsStr;

            storageMutex.lock();
            dataStorage->addPath(path);
            storageMutex.unlock();
        } catch (InvalidPathException&) {
            log("invalid path string");
        }


    }
    log("finish thread");
    emit finished();
}

void SVGProcessor::test()   {
    QString s = "M703.3,483.6c0,3,0,6.1,0,9.1c-0.7,0.1-1.4,0.3-2.1,0.6c-1.3,0.4-2.5,0.9-3.8,1.3c-2.9,0.9-5.8,1.8-8.7,2.7"
            "c-7.3,2.2-14.7,4.4-22.1,6.4c-6.2,1.6-12.3,3.1-18.6,4.6c-4.5,1-9,2.2-13.4,3.7c-2.5,0.8-4.9,1.7-7.3,2.6c-5.5,2.1-11,4.4-16.3,7"
            "c-4.7,2.3-9.3,4.6-13.9,7c-2.1,1-4.1,2.2-6.1,3.3c-3,1.6-6,3.3-9,5c-2.9,1.6-5.8,3.2-8.8,4.8c-3.1,1.7-6.2,3.3-9.2,4.9"
            "c-1.1,0.6-2.2,1.2-3.3,1.7c-0.9,0.4-1.7,0.9-2.6,1.3c-2.2,1-4.5,2-6.8,3.1c-2.8,1.4-5.7,2.7-8.6,3.9c-2.7,1.2-5.5,2.3-8.4,3.2"
            "c-4.2,1.4-8.5,2.7-12.7,4.1c-0.2,0.1-0.3,0.1-0.5,0.1c-2.9,0.9-5.8,1.8-8.7,2.7c-2.6,0.8-5.2,1.6-7.7,2.4c-4.8,1.5-9.5,3.1-14.1,5.1"
            "c-1.5,0.7-3.1,1.3-4.7,2c-2.5,1-4.9,2-7.4,3c-0.6,0.3-1.2,0.6-1.8,0.9c-41.5,0-83,0-124.5,0c0.7-0.4,1.4-0.7,2.2-0.9"
            "c4.1-1.2,8.2-2.5,12.3-3.6c5.2-1.4,10.5-2.7,15.7-4c7.9-2,15.9-4,23.8-6c6.3-1.6,12.7-3.3,19-4.9c3.4-0.9,6.8-1.7,10.2-2.3"
            "c4.9-0.9,9.8-1.8,14.6-2.6c4.4-0.8,8.8-1.5,13.2-2.4c4.8-0.9,9.6-2,14.4-3c7-1.5,14-3.1,21-4.4c5.6-1.1,11.3-2.1,16.9-3.4"
            "c3-0.7,5.9-1.3,8.9-2.2c4.2-1.2,8.3-2.6,12.4-3.9c6.4-2.1,12.6-4.7,18.6-7.7c2-1,4-2,6-3.1c1.8-1,3.7-2,5.5-3"
            "c3.6-2,7.2-4.2,10.7-6.4c4.7-3,9.3-5.9,14-8.9c5.5-3.6,10.9-7.2,16.4-10.8c1.6-1.1,3.3-2.2,5-3.2c1.1-0.7,2.2-1.3,3.4-2"
            "c0.6-0.1,1.1-0.4,1.5-0.7c0.7-0.4,1.5-0.8,2.2-1.2c2.6-1.3,5.1-2.6,7.7-3.9c0.6-0.3,1.2-0.6,1.8-0.9c1-0.5,2-0.9,3.1-1.4"
            "c2.6-1.3,5.3-2.5,7.9-3.8c1.4-0.6,2.8-1.3,4.2-1.9c2.5-1.1,5-2.2,7.6-3.3c3.5-1.5,6.9-3,10.4-4.5c2.6-1.1,5.2-2.2,7.8-3.3"
            "c3.6-1.5,7.1-3.1,10.7-4.6c2.9-1.3,5.9-2.5,8.8-3.9c0.5-0.2,0.9-0.4,1.4-0.6c1.6-0.8,3.3-1.5,4.8-2.4c1.9-1,3.8-2.2,5.7-3.3"
            "c0.4-0.3,0.9-0.5,1.3-0.7c0,6.4,0,12.9,0,19.3c-1.4,0.9-2.8,1.8-4.4,2.5c-0.8,0.4-1.6,0.7-2.5,1.1c-1.9,0.8-3.8,1.6-5.8,2.4"
            "c-2.6,1-5.2,1.9-7.8,2.8c-3.4,1.2-6.9,2.4-10.4,3.7c-2.4,0.9-4.9,1.7-7.3,2.8c-1.1,0.5-2.2,1-3.2,1.4c-3.2,1.3-6.3,2.7-9.5,4"
            "c-2.2,0.9-4.3,1.9-6.5,2.8c-1.8,0.8-3.5,1.6-5.3,2.3c-0.7,0.3-1.4,0.6-2.1,1.2l-0.1,0l0.1-0.1c0.8,0,1.6-0.2,2.3-0.4"
            "c3.5-1,7-1.9,10.5-2.7c5-1.2,10.1-2.3,15.2-3.4c6-1.2,12-2.3,17.9-3.5c5-1,10-2,14.9-3C700.8,484.3,702.1,483.9,703.3,483.6z"
            "M686.9,470c0-0.1,0-0.3,0-0.4c-0.3-1.9-2.2-3.3-4.1-2.9c-1.1,0.2-1.9,0.9-2.3,2c-0.5,1.5,0.1,3.1,1.6,4c1.3,0.7,2.9,0.5,4-0.7"
            "C686.7,471.5,686.9,470.8,686.9,470z M433,569.1c0-0.4,0-0.5,0-0.6c-0.1-1.3-0.8-2.2-2-2.7c-1.1-0.5-2.6-0.2-3.4,0.7"
            "c-1.4,1.5-1.2,4,0.6,5.1c1.9,1.2,4.4,0.1,4.7-2.2C433,569.3,433,569,433,569.1z M625.5,504.7c-0.8,0-1.5,0.7-1.5,1.5"
            "c0,0.7,0.6,1.4,1.3,1.5c0.8,0.1,1.6-0.6,1.6-1.4C627,505.5,626.3,504.8,625.5,504.7z";
    QQueue<QString> sq;
    sq.enqueue(s);
    QMutex strQueueMutex, storageMutex;
    PathsPtr paths = std::make_shared<Paths>();
    PointsThread t(sq, strQueueMutex, paths, storageMutex, this);
    t.run();
}

SVGProcessor::SVGProcessor()
{
    setName("SVGProcessor");
}

void SVGProcessor::runInThreads(QQueue<QString> &strQueue, PathsPtr& out paths)   {
    log("running points extracting in " + Utils::toStr(THREAD_COUNT) + " threads");

    QMutex strQueueMutex, storageMutex;
    PointsThread **threads = new PointsThread*[THREAD_COUNT];

    for (unsigned i = 0; i < THREAD_COUNT; i++) {
        threads[i] = new PointsThread(strQueue, strQueueMutex, paths, storageMutex, this);
        connect(threads[i], &QThread::finished, threads[i], &QObject::deleteLater);
        threads[i]->start();
    }

    for (unsigned i = 0; i < THREAD_COUNT; i++) {
        threads[i]->wait();
    }
    delete[] threads;
    log("multi thread processing finished.");
}

void SVGProcessor::extractPaths(QFile in &file, QQueue<QString> out &strQueue)  {
    QDomDocument doc;
    if (doc.setContent(&file))  {

        QString tags[TAG_COUNTER] = { PATH_TAG, POLYGON_TAG, POLYLINE_TAG };        

        for (int t = 0; t < TAG_COUNTER; t++)   {
            QDomNodeList pathNodes = doc.elementsByTagName(tags[t]);

            for (int i = 0; i < pathNodes.size(); i++)    {
                QDomElement path = pathNodes.at(i).toElement();
                QString pointsStr = path.attribute(POINTS_ATTRIBUTE);
                strQueue.enqueue(pointsStr);
            }
        }
    }   else {
        throw InvalidFileException();
    }
}

void SVGProcessor::sortPaths(PathsPtr &pPaths)  {
    log("paths sorting...");
    qSort(pPaths->begin(), pPaths->end(), [] (Path A, Path B) { return A.pathLength() < B.pathLength(); });
    log("sorting done");
}

bool SVGProcessor::process(QFile in &file, PathsPtr out &pPaths)   {
    bool result = true;
    log("processing file \'" + file.fileName() + "\'...");

    if (file.open(QIODevice::ReadOnly)) {
        log("file opened");
        QQueue<QString> pointStrings;
        try {
            extractPaths(file, pointStrings);
        } catch(InvalidFileException &)   {
            log("Invalid file. Fault");
            file.close();
            return false;
        }
        file.close();

        int pathStrCount = pointStrings.size();
        runInThreads(pointStrings, pPaths);

        sortPaths(pPaths);

        log(Utils::toStr(pPaths->size()) +  "/" + Utils::toStr(pathStrCount) + " paths processed");

        log("processing done");

    }   else {
        log("file not opened. Fault");
        result = false;
    }

    return result;
}

void SVGProcessor::executeTask(Task& task)  {
    if (task.isStarted() && !task.isFinished())  {
        task.start();

        PathsPtr pPaths = std::make_shared<Paths>();

        bool processingResult = process(task.getFile(), pPaths);
        if (!processingResult)   {
            task.finish(Task::FAULT);
            log("task " + task.getFile().fileName() + " fault");
            emit finished(task);
        }   else {
            log("drawing " + task.getFile().fileName());
            emit draw(task, std::move(pPaths));
        }

    }   else {
        log("invalid task. Fault");
    }
}
