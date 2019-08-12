#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class Scene : public QGraphicsScene
{
private:
    float lastX, lastY;
public:
    Scene();

public slots:
    void moveTo(float x, float y);
    void drawTo(float x, float y);
};

#endif // SCENE_H
