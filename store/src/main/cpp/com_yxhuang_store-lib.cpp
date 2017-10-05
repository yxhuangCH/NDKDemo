//
// Created by yxhuang on 2017/9/21.
//
#include "com_yxhuang_store_Store.h"
#include "Store.h"
#include <string.h>

extern "C"
JNIEXPORT jint JNICALL
Java_com_yxhuang_store_Store_getCount(JNIEnv *env, jobject /* this */) {
    return 11;
}
