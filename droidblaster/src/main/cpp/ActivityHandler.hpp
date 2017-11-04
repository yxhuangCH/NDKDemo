//
// Created by yxhuang on 2017/11/4.
// define an "interface" to observe native activity events

#ifndef _PACKT_ACTIVITYHANDLER_
#define _PACKT_ACTIVITYHANDLER_

#include "Types.hpp"

class ActivityHandler{
public:
    virtual ~ActivityHandler(){};

    virtual status onActivate() = 0;
    virtual void onDeactivate() = 0;
    virtual status onStep() = 0;

    virtual void onStart() {};
    virtual void onResume() {};
    virtual void onPause() {};
    virtual void onStop() {};
    virtual void onDestroy() {};

    virtual void onSaveInstanceState(void **pData, size_t *pSize) {};
    virtual void onConfigurationChanged() {};
    virtual void onLowMemory() {};

    virtual void onCreateWindow() {};
    virtual void onDestroyWindow() {};
    virtual void onGainFocus() {};
    virtual void onLostFocus() {};

};


#endif //NDKPROJECT_ACTIVITYHANDLER_H
