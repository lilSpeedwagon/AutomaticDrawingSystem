#ifndef PATHS_H
#define PATHS_H

#include <QVector>
#include <QDebug>
#include <math.h>

const float precisionFactor = 1;
const float scaleBordersFactor = 0.8f;

struct ImgSize   {
    float w, h;
    ImgSize operator*(float const& f) const {
        return { w * f, h *f };
    }
};
Q_DECLARE_METATYPE(ImgSize)

struct ImgBorders   {
    float minW, minH, maxW, maxH;
    ImgBorders() : minW(0.0f), minH(0.0f), maxW(0.0f), maxH(0.0f) {}
    ImgBorders(ImgBorders const& b) : minW(b.minW), minH(b.minH), maxW(b.maxW), maxH(b.maxH) {}
    void operator=(ImgBorders const& b) {
        minW = b.minW; minH = b.minH;
        maxW = b.maxW; maxH = b.maxH;
    }
};
Q_DECLARE_METATYPE(ImgBorders)

struct Point {
    float x = 0;
    float y = 0;
    bool draw = false;
    Point() : x(0), y(0), draw(false) {}
    Point(float x, float y, bool draw = true) :
        x(x), y(y), draw(draw) {}

    Point reflect(Point const& base) {
        return { 2 * base.x - x, 2 * base.y - y, true };
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

    void operator*=(float const& f) {
        x *= f;
        y *= f;
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

struct BCurve    {
    Point begin;
    Point p;
    Point end;

    float baseLength() const {
        return begin.distantion(p) + p.distantion(end);
    }
};
Q_DECLARE_METATYPE(BCurve)

class EmptyPathsException : public std::exception   {};

class Path : public QVector<Point> {
public:
    void operator<<(Point& point);
    void operator<<(Curve& curve);
    void operator<<(BCurve& curve);

    float pathLength() const;
    ImgBorders getBorders() const;
private:
    float length;
    ImgBorders borders;
};
Q_DECLARE_METATYPE(Path)

/*
 *Hint: use at() to read-only access (better performance than operator[])
 */
class Paths
{
private:
    QVector<Path> paths;
    ImgBorders imgBorders;
public:
    Paths();
    void addPath(Path &points);
    void removePath(int const& index);
    void clear();
    int size() const;
    void moveToZero();
    void scale(float scaleFactor);
    void scale(float wScaleFactor, float hScaleFactor);
    void scaleForScreen(ImgSize const& screenSize);
    void sort();
    std::pair<float, float> getImgSize() const;
    const Path& at(int index) const;

    auto begin() -> decltype (paths.begin()) { return paths.begin(); }
    auto end() -> decltype (paths.end()) { return paths.end(); }

    class OutOfBoundException : public std::exception {};
};
Q_DECLARE_METATYPE(Paths)

typedef std::shared_ptr<Paths> PathsPtr;
Q_DECLARE_METATYPE(PathsPtr)

#endif // PATHS_H
