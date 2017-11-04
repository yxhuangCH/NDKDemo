//
// Created by yxhuang on 2017/10/24.
//
#include "EventLoop.hpp"
#include <android_native_app_glue.h>

void android_main(android_app *pApplication){
    EventLoop(pApplication).run();
}
