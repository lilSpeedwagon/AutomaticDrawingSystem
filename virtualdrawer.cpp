#include "virtualdrawer.h"

VirtualDrawer::VirtualDrawer()  {
    setName("VirtualDrawer");
    log("virtual draver init");
}

void VirtualDrawer::drawTo(float x, float y)    {
    emit signalDrawTo(x, y);
}

void VirtualDrawer::moveTo(float x, float y)    {
    emit signalMoveTo(x, y);
}

VirtualDrawer::~VirtualDrawer() {
    log("Virtual draver terminating...");
}

void VirtualDrawer::ready() {
    emit signalReady();
}
