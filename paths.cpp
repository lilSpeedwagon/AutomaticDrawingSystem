#include "paths.h"

void Path::operator<<(Point& point)   {
    if (!empty())
        length += back().distantion(point);

    push_back(point);
    if (point.x > maxSize.w)
        maxSize.w = point.x;
    if (point.y > maxSize.h)
        maxSize.h = point.y;
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

float Path::pathLength() const  {
    return length;
}

ImgSize Path::getMaxSize() const    {
    return maxSize;
}

Paths::Paths()
{

}

void Paths::addPath(Path &path) {
    paths.push_back(path);

    ImgSize pathMaxSize = path.getMaxSize();
    if (pathMaxSize.w > imgSize.w)
        imgSize.w = pathMaxSize.w;
    if (pathMaxSize.h > imgSize.h)
        imgSize.h = pathMaxSize.h;
}

void Paths::removePath(int const& index)   {
    paths.remove(index);
}

void Paths::clear() {
    paths.clear();
}

int Paths::size() const {
    return paths.size();
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
    float scaleFactor = (screenSize.w < screenSize.h) ?
                        screenSize.w / imgSize.w :
                        screenSize.h / imgSize.h;
    scale(scaleFactor, scaleFactor);
}

const Path& Paths::at(int index) const  {
    if (index >= paths.size() || index < 0)
        throw OutOfBoundException();
    return paths.at(index);
}
