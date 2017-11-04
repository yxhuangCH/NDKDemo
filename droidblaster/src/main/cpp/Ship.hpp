//
// Created by yxhuang on 2017/11/4.
// represents spaceship.

#ifndef NDKPROJECT_SHIP_H
#define NDKPROJECT_SHIP_H

#include "GraphicsManager.hpp"

class Ship {
public:
    Ship(android_app *pApplication, GraphicsManager &pGraphicsManager);

    void registerShip(GraphicsElement *pGraphics);

    void initialize();

private:
    GraphicsManager &mGraphicsManager;
    GraphicsElement *mGraphics;
};

#endif //NDKPROJECT_SHIP_H
