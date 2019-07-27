#ifndef PATHS_H
#define PATHS_H

#include <QVector>
#include <QDebug>

const float precisionFactor = 5;

struct Point {
    float x;
    float y;
    bool draw;
    Point() : x(0), y(0), draw(false) {}
    Point(float x, float y, bool draw = true) :
        x(x), y(y), draw(draw) {}

    Point operator*(float const& f)    {
        return { x * f, y * f, draw };
    }

    Point operator*(Point const& p) {
        return { x*p.x, y*p.y, true };      // <- new point will be drawn (by default)
    }

    Point operator+(Point const& p) {
        return { x + p.x, y + p.y, true };
    }

    float distantion(Point const& p) const   {
        return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
    }
};

struct Curve    {
    Point begin;
    Point p1;
    Point p2;
    Point end;

    float baseLength() const {
        return begin.distantion(p1) + p1.distantion(p2) + p2.distantion(end);
    }
};

class Path : public QVector<Point> {
public:
    void operator<<(Point& point)   {
        push_back(point);
    }

    void operator<<(Curve& curve)   {
        int pointCount = static_cast<int>(curve.baseLength() * precisionFactor);
        float t_step = 1.0f / pointCount;
        float t = t_step;

        //we skip first point because it's already contained in prev point (M) or curve (C)
        for (int i = 1; i < pointCount; i++)    {
            Point p = curve.begin * pow(1 - t, 3) + curve.p1 * 3 * pow(1 - t, 2) * t + curve.p2 * 3 * (1 - t) * pow(t, 2) + curve.end * pow(t, 3);
            t += t_step;
            push_back(p);
        }
    }
};

class Paths
{
private:
    QVector<Path> paths;
public:
    Paths();
    void addPath(Path &points);
    void removePath(int const& index);
    void clear();
    int size() const;
};

typedef std::shared_ptr<Paths> PathsPtr;

#endif // PATHS_H
