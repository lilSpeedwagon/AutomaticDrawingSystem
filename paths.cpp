#include "paths.h"

void Path::operator<<(Point& point)   {
    if (!empty())
        length += back().distantion(point);

    push_back(point);
    if (point.x > borders.maxW)
        borders.maxW = point.x;
    if (point.y > borders.maxH)
        borders.maxH = point.y;
    if (point.x < borders.minW)
        borders.minW = point.x;
    if (point.y < borders.minH)
        borders.minH = point.y;
    /*if (point.x > maxSize.w)
        maxSize.w = point.x;
    if (point.y > maxSize.h)
        maxSize.h = point.y;*/
}

void Path::operator<<(Curve& curve)   {
    int pointCount = static_cast<int>(curve.baseLength() * precisionFactor);
    float t_step = 1.0f / pointCount;
    float t = t_step;

    //we skip first point because it's already contained in prev point (M) or curve (C)
    for (int i = 1; i < pointCount; i++)    {
        Point p = curve.begin * pow(1 - t, 3) + curve.p1 * 3 * pow(1 - t, 2) * t + curve.p2 * 3 * (1 - t) * pow(t, 2) + curve.end * pow(t, 3);
        t += t_step;
        operator<<(p);
    }
}

void Path::operator<<(BCurve& curve)    {
    int pointCount = static_cast<int>(curve.baseLength() * precisionFactor);
    float t_step = 1.0f / pointCount;
    float t = t_step;

    //we skip first point because it's already contained in prev point (M) or curve (C)
    for (int i = 1; i < pointCount; i++)    {
        Point p = curve.begin * pow(1 - t, 2) + curve.p * 2 *(1 - t) * t + curve.end * pow(t, 2);
        t += t_step;
        operator<<(p);
    }
}

float Path::pathLength() const  {
    return length;
}

ImgBorders Path::getBorders() const    {
    return borders;
}

Paths::Paths()
{

}

void Paths::addPath(Path &path) {
    paths.push_back(path);

    ImgBorders pathBorders = path.getBorders();
    if (pathBorders.maxW > imgBorders.maxW)
        imgBorders.maxW = pathBorders.maxW;
    if (pathBorders.maxH > imgBorders.maxH)
        imgBorders.maxH = pathBorders.maxH;
    if (pathBorders.minW < imgBorders.minW)
        imgBorders.minW = pathBorders.minW;
    if (pathBorders.minH < imgBorders.minH)
        imgBorders.minH = pathBorders.minH;
}

void Paths::removePath(int const& index)   {
    paths.remove(index);
}

void Paths::clear() {
    paths.clear();
    imgBorders = ImgBorders();
}

int Paths::size() const {
    return paths.size();
}

//moves left side of img to x=0 and top side to y=0
void Paths::moveToZero()  {
    qDebug() << "To zero: " << imgBorders.minW << imgBorders.minH << imgBorders.maxW << imgBorders.maxH;
    Point delta = { -imgBorders.minW, -imgBorders.minH };
    for (Path &path : paths)    {
        for (Point &point : path)   {
            point += delta;
        }
    }
    imgBorders.maxW -= imgBorders.minW;
    imgBorders.maxH -= imgBorders.minH;
    imgBorders.minW = 0.0f;
    imgBorders.minH = 0.0f;
    qDebug() << "Result: " << imgBorders.minW << imgBorders.minH << imgBorders.maxW << imgBorders.maxH;
}

std::pair<float, float> Paths::getImgSize() const   {
    return { imgBorders.maxW, imgBorders.maxH };
}

void Paths::sort()  {
    qSort(begin(), end(), [] (Path A, Path B) { return A.pathLength() < B.pathLength(); });
}

void Paths::scale(float scaleFactor)    {
    scale(scaleFactor, scaleFactor);
}

void Paths::scale(float wScaleFactor, float hScaleFactor)   {
    for (Path &path : paths)    {
        for (Point &point : path)   {
            point.x *= wScaleFactor;
            point.y *= hScaleFactor;
        }
    }
}

void Paths::scaleForScreen(const ImgSize &screenSize)   {
    ImgSize boundedSize = screenSize * scaleBordersFactor;
    float scaleFactor = (boundedSize.w < boundedSize.h) ?
                        boundedSize.w / imgBorders.maxW :
                        boundedSize.h / imgBorders.maxH;
    qDebug() << "Scalling: " << imgBorders.minW << imgBorders.minH << imgBorders.maxW << imgBorders.maxH;
    qDebug() << scaleFactor;

    float borderScale = (1 - scaleBordersFactor) / 2;
    Point borderPoint = { borderScale * screenSize.w, borderScale * screenSize.h };
    for (Path &path : paths)    {
        for (Point &point : path)   {
            point *= scaleFactor;
            point += borderPoint;
        }
    }
}

const Path& Paths::at(int index) const  {
    if (index >= paths.size() || index < 0)
        throw OutOfBoundException();
    return paths.at(index);
}
