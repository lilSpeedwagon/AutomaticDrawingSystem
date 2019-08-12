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
    upperBound chars mark coords as absolyte
*/
void PointsThread::extractPoints(const QString &str, Path &path) const  {
    int currentPos = 0;
    Point beginPoint, lastPoint, lastBasePoint;
    while (currentPos < str.length() && currentPos != NOT_FOUND)   {
        if (Utils::equals(str[currentPos], POINTS_M))    {
            float nums[COORDS_POINT];
            int newPos;

            try {
                newPos = Utils::stringToFloats<COORDS_POINT>(str, currentPos, nums);
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            Point p = { nums[0], nums[1], false };

            if (str[currentPos].isLower())  {
                p += lastPoint;
            }

            beginPoint = lastPoint = p;
            path << p;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_C))    {
            float nums[COORDS_CURVE];
            int newPos;
            try {
                newPos = Utils::stringToFloats<COORDS_CURVE>(str, currentPos, nums);
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            Point base1 = { nums[0], nums[1] };
            Point base2 = { nums[2], nums[3] };
            Point p = { nums[4], nums[5] };

            if (str[currentPos].isLower())  {
                base1 += lastPoint;
                base2 += lastPoint;
                p += lastPoint;
            }

            Curve c = { lastPoint, base1, base2, p };

            path << c;
            lastPoint = p;
            lastBasePoint = base2;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_S))    {
            float nums[COORDS_CURVE_S];
            int newPos;
            try {
                newPos = Utils::stringToFloats<COORDS_CURVE_S>(str, currentPos, nums);
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            lastBasePoint.reflect(lastPoint);
            Point base2 = { nums[0], nums[1] };
            Point p = { nums[2], nums[3] };

            if (str[currentPos].isLower())  {
                base2 += lastPoint;
                p += lastPoint;
            }

            Curve c = { lastPoint, lastBasePoint, base2, p };

            path << c;
            lastPoint = p;
            lastBasePoint = base2;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_L))   {
            float nums[COORDS_POINT];
            int newPos;

            try {
                newPos = Utils::stringToFloats<COORDS_POINT>(str, currentPos, nums);
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            Point p = { nums[0], nums[1], true };
            if (str[currentPos].isLower())  {
                p += lastPoint;
            }

            lastPoint = p;
            path << p;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_V))   {
            float nums[COORDS_LINE];
            int newPos;

            try {
                newPos = Utils::stringToFloats<COORDS_LINE>(str, currentPos, nums);
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            Point p = { lastPoint.x, nums[0], true };
            if (str[currentPos].isLower())  {
                p.y += lastPoint.y;
            }

            lastPoint = p;
            path << lastPoint;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_H))   {
            float nums[COORDS_LINE];
            int newPos;

            try {
                newPos = Utils::stringToFloats<COORDS_LINE>(str, currentPos, nums);
            } catch (Utils::InvalidFloatException&) {
                throw InvalidPathException();
            }

            Point p = { nums[0], lastPoint.y, true };
            if (str[currentPos].isLower())  {
                p.x += lastPoint.x;
            }

            lastPoint = p;
            path << lastPoint;
            currentPos = newPos;
            continue;
        }

        if (Utils::equals(str[currentPos], POINTS_Z))   {
            beginPoint.draw = true;
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
            Path path;
            extractPoints(pointsStr, path);

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

void SVGProcessor::extractPaths(QDomDocument in &doc, QQueue<QString> out &strQueue)  {
    QString tags[TAG_COUNTER] = { PATH_TAG, POLYGON_TAG, POLYLINE_TAG };

    for (int t = 0; t < TAG_COUNTER; t++)   {
        QDomNodeList pathNodes = doc.elementsByTagName(tags[t]);

        for (int i = 0; i < pathNodes.size(); i++)    {
            QDomElement path = pathNodes.at(i).toElement();
            QString pointsStr = path.attribute(POINTS_ATTRIBUTE);
            strQueue.enqueue(pointsStr);
        }
    }
}

void SVGProcessor::extractImgSize(QDomDocument in &doc, ImgSize out &size)   {
    QDomNodeList svgNodes = doc.elementsByTagName(SVG_TAG);
    if (!svgNodes.isEmpty())    {
        QString viewBoxStr = svgNodes.at(0).toElement().attribute(VIEWBOX_ATTRIBUTE);
        if (!viewBoxStr.isEmpty())  {
            QStringList sizeList = viewBoxStr.split(' ');

            if (sizeList.size() == COORDS_VIEWBOX)  {
                float nums[COORDS_VIEWBOX];
                for (int i = 0; i < COORDS_VIEWBOX; i++)    {
                    bool result = false;
                    nums[i] = sizeList.at(i).toFloat(&result);
                    if (!result){
                        throw InvalidFileException();
                    }
                }

                size.w = nums[2] - nums[0];
                size.h = nums[3] - nums[1];

                return;
            }   else {
                throw InvalidFileException();
            }
        }
    }
    throw InvalidFileException();
}

void SVGProcessor::sortPaths(PathsPtr &pPaths)  {
    log("paths sorting...");
    qSort(pPaths->begin(), pPaths->end(), [] (Path A, Path B) { return A.pathLength() < B.pathLength(); });
    log("sorting done");
}

void SVGProcessor::scalePaths(PathsPtr &pPaths, ImgSize const& size) {
    // test value
    ImgSize testSize = {800.0, 600.0};
    /////////////
    float scaleFactorX = testSize.w / size.w;
    float scaleFactorY = testSize.h / size.h;

    for (Path& path : *pPaths)   {
        for (Point &point : path)   {
            point.x *= scaleFactorX;
            point.y *= scaleFactorY;
        }
    }
}

bool SVGProcessor::process(QFile in &file, PathsPtr out &pPaths)   {
    bool result = true;
    log("processing file \'" + file.fileName() + "\'...");

    if (file.open(QIODevice::ReadOnly)) {
        log("file opened");
        QDomDocument doc;
        QQueue<QString> pointStrings;


        try {
            bool validDOM = doc.setContent(&file);
            file.close();

            if (!validDOM) {
                throw InvalidFileException();
            }

            extractPaths(doc, pointStrings);
        } catch (InvalidFileException&) {
            log("Invalid file. Fault");
            file.close();
            return false;
        }

        int pathStrCount = pointStrings.size();
        runInThreads(pointStrings, pPaths);

        sortPaths(pPaths);
        pPaths->scaleForScreen( { 800.0f, 600.0f } );

        log(Utils::toStr(pPaths->size()) +  "/" + Utils::toStr(pathStrCount) + " paths processed");

        log("processing done");

    }   else {
        log("file not opened. Fault");
        result = false;
    }

    return result;
}

void SVGProcessor::executeTask(Task task)  {
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
            qRegisterMetaType<Task>();
            qRegisterMetaType<PathsPtr>();
            emit draw(task, std::move(pPaths));
        }

    }   else {
        log("invalid task. Fault");
    }
}
