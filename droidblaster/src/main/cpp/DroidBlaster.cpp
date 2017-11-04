//
// Created by yxhuang on 2017/11/4.
//

#include "DroidBlaster.hpp"
#include "Log.hpp"

#include <unistd.h>

static const int32_t SHIP_SIZE = 64;

DroidBlaster::DroidBlaster(android_app *pApplication) :
    mEventLoop(pApplication, *this),
    mGraphicsManager(pApplication),
    mShip(pApplication, mGraphicsManager){

    Log::info("Creating DroidBlaster");

    GraphicsElement *shipGraphics = mGraphicsManager.registerElement(SHIP_SIZE, SHIP_SIZE);
    mShip.registerShip(shipGraphics);
}

void DroidBlaster::run() {
    mEventLoop.run();
}

status DroidBlaster::onActivate() {
    Log::info("Activating onActivate");

    if (mGraphicsManager.start() != STATUS_OK) {
        return STATUS_KO;
    }

    mShip.initialize();

    return STATUS_OK;
}

void DroidBlaster ::onDeactivate() {
    Log::info("Activating DroidBlaster");
}

status DroidBlaster::onStep() {
//    Log::info("Starting step");
//    usleep(30000);
//    Log::info("Stepping done");
//    return STATUS_OK;
    return mGraphicsManager.update();
}

void DroidBlaster::onStart(){
    Log::info("onStart");
}

void DroidBlaster::onResume(){
    Log::info("onResume");
}

void DroidBlaster::onPause(){
    Log::info("onPause");
}

void DroidBlaster::onStop(){
    Log::info("onStop");
}

void DroidBlaster::onDestroy(){
    Log::info("onDestroy");
}

void DroidBlaster::onSaveInstanceState(void **pData, size_t *pSize){
    Log::info("onSaveInstanceState");
}

void DroidBlaster::onConfigurationChanged(){
    Log::info("onConfigurationChanged");
}

void DroidBlaster::onLowMemory(){
    Log::info("onLowMemory");
}

void DroidBlaster::onCreateWindow(){
    Log::info("onCreateWindow");
}

void DroidBlaster::onDestroyWindow(){
    Log::info("onDestroyWindow");
}

void DroidBlaster::onGainFocus(){
    Log::info("onGainFocus");
}

void DroidBlaster::onLostFocus(){
    Log::info("onLostFocus");
}
