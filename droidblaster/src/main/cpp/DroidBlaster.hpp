#ifndef _PACKT_DROIDBLASTER_HPP_
#define _PACKT_DROIDBLASTER_HPP_

#include "ActivityHandler.hpp"
#include "EventLoop.hpp"
#include "Types.hpp"
#include "GraphicsManager.hpp"
#include "Ship.hpp"

class DroidBlaster : public ActivityHandler{
public:
    DroidBlaster(android_app *pApplication);
    void run();

protected:
    status onActivate();
    void onDeactivate();
    status onStep();

     void onStart();
     void onResume();
     void onPause();
     void onStop();
     void onDestroy();

     void onSaveInstanceState(void **pData, size_t *pSize);
     void onConfigurationChanged();
     void onLowMemory();

     void onCreateWindow();
     void onDestroyWindow();
     void onGainFocus();
     void onLostFocus();

private:
    EventLoop mEventLoop;

    GraphicsManager mGraphicsManager;
    Ship mShip;
};

#endif