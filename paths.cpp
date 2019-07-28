#include "paths.h"

void Path::operator<<(Point& point)   {
    if (!empty())
        length += back().distantion(point);

    push_back(point);
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

Paths::Paths()
{

}

void Paths::addPath(Path &path) {
    paths.push_back(path);
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
