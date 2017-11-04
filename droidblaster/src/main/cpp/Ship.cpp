//
// Created by yxhuang on 2017/11/4.
//

#include "Log.hpp"
#include "Ship.hpp"
#include "Types.hpp"

static const float INITAL_X = 0.5f;
static const float INITAL_Y = 0.25f;

Ship::Ship(android_app *pApplication, GraphicsManager &pGraphicsManager) :
        mGraphicsManager(pGraphicsManager),
        mGraphics(NULL){
}

void Ship::registerShip(GraphicsElement *pGraphics) {
    mGraphics = pGraphics;
}

void Ship::initialize() {
    mGraphics->location.x = INITAL_X * mGraphicsManager.getRenderWith();
    mGraphics->location.y = INITAL_Y * mGraphicsManager.getRenderHeight();

}

