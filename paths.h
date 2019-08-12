#ifndef PATHS_H
#define PATHS_H

#include <QVector>
#include <QDebug>
#include <math.h>

const float precisionFactor = 1;

struct ImgSize   {
    float w, h;
};
Q_DECLARE_METATYPE(ImgSize)

struct Point {
    float x = 0;
    float y = 0;
    bool draw = false;
    Point() : x(0), y(0), draw(false) {}
    Point(float x, float y, bool draw = true) :
        x(x), y(y), draw(draw) {}

    void reflect(Point const& base) {
        x = 2 * base.x - x;
        y = 2 * base.y - y;
    }

    Point operator*(float const& f)    {
        return { x * f, y * f, draw };
    }

    Point operator*(Point const& p) {
        return { x*p.x, y*p.y, true };      // <- new point will be drawn (by default)
    }

    Point operator+(Point const& p) {
        return { x + p.x, y + p.y, true };
    }

    void operator+=(Point const& p)  {
        x += p.x;
        y += p.y;
    }

    float distantion(Point const& p) const   {
        return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
    }
};
Q_DECLARE_METATYPE(Point)

struct Curve    {
    Point begin;
    Point p1;
    Point p2;
    Point end;

    float baseLength() const {
        return begin.distantion(p1) + p1.distantion(p2) + p2.distantion(end);
    }
};
Q_DECLARE_METATYPE(Curve)

class EmptyPathsException : public std::exception   {};

class Path : public QVector<Point> {
public:
    void operator<<(Point& point);
    void operator<<(Curve& curve);

    float pathLength() const;
    ImgSize getMaxSize() const;
private:
    float length;
    ImgSize maxSize;
};
Q_DECLARE_METATYPE(Path)

/*
 *Hint: use at() to read-only access (better performance than operator[])
 */
class Paths
{
private:
    QVector<Path> paths;
    ImgSize imgSize;
public:
    Paths();
    void addPath(Path &points);
    void removePath(int const& index);
    void clear();
    int size() const;
    void scale(float scaleFactor);
    void scale(float wScaleFactor, float hScaleFactor);
    void scaleForScreen(ImgSize const& screenSize);
    const Path& at(int index) const;

    auto begin() -> decltype (paths.begin()) { return paths.begin(); }
    auto end() -> decltype (paths.end()) { return paths.end(); }

    class OutOfBoundException : public std::exception {};
};
Q_DECLARE_METATYPE(Paths)

typedef std::shared_ptr<Paths> PathsPtr;
Q_DECLARE_METATYPE(PathsPtr)

#endif // PATHS_H
