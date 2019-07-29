#ifndef PATHS_H
#define PATHS_H

#include <QVector>
#include <QDebug>
#include <math.h>

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

class EmptyPathsException : public std::exception   {};

class Path : public QVector<Point> {
public:
    void operator<<(Point& point);
    void operator<<(Curve& curve);

    float pathLength() const;
private:
    float length;
};


/*
 *Hint: use at() to read-only access (better performance than operator[])
 */
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
    void scale(float scaleFactor);
    const Path& at(int index) const;

    auto begin() -> decltype (paths.begin()) { return paths.begin(); }
    auto end() -> decltype (paths.end()) { return paths.end(); }

    class OutOfBoundException : public std::exception {};
};

typedef std::shared_ptr<Paths> PathsPtr;

#endif // PATHS_H
