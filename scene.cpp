#include "scene.h"

Scene::Scene()
{

}

void Scene::moveTo(float x, float y)    {
    lastX = x;
    lastY = y;
}

void Scene::drawTo(float x, float y)    {
    addLine(lastX, lastY, x, y, QPen(Qt::red,5,Qt::SolidLine,Qt::RoundCap));
    lastX = x;
    lastY = y;
}
