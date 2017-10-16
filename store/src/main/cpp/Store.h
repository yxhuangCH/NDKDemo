//
// Created by yxhuang on 2017/9/25.
//

#ifndef NDKPROJECT_STORE_H
#define NDKPROJECT_STORE_H

#include <cstdint>
#include <cstdint>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include "jni.h"

#define STORE_MAX_CAPACITY 16

typedef enum {
    StoreType_String,
    StoreType_Integer,
    StoreType_Color,
    StoreType_IntegerArray,
    StoreType_StringArray,
    StoreType_ColorArray
} StoreType;

typedef union {
    int32_t mInteger;
    char* mString;
    jobject mSColor;
    int32_t* mIntegerArray;
    char** mStringArray;
    jobject* mSColorArray;
} StoreValue;

typedef struct {
    char* mKey;
    StoreType mType;
    StoreValue mValue;
    int32_t mLength;
} StoreEntry;

typedef struct {
    StoreEntry mEntities[STORE_MAX_CAPACITY];
    int32_t mLength;
} Store;

typedef struct {
    Store* mStore;
    JavaVM* mJavaVm;
    jobject mLock;
    pthread_t mThread;
    int32_t mRunning;
} StoreWatcher;

void throwInvalidTypeException(JNIEnv* pEnv){
    jclass clazz = pEnv->FindClass("com/yxhuang/store/exception/InvalidTypeException");
    if (clazz != NULL){
        pEnv->ThrowNew(clazz, "Invalid type.");
    }
    pEnv->DeleteLocalRef(clazz);
}

void throwNotExistingKeyException(JNIEnv* pEnv){
    jclass clazz = pEnv->FindClass("com/yxhuang/store/exception/NotExistingKeyException");
    if (clazz != NULL){
        pEnv->ThrowNew(clazz, "Key does not exist.");
    }
    pEnv->DeleteLocalRef(clazz);
}

void throwStoreFullException(JNIEnv* pEnv){
    jclass clazz = pEnv->FindClass("com/yxhuang/store/exception/StoreFullException");
    if (clazz != NULL){
        pEnv->ThrowNew(clazz, "Store full.");
    }
    pEnv->DeleteLocalRef(clazz);
}


void processEntry(StoreEntry* pEntry){
    switch (pEntry->mType){
        case StoreType_Integer:
            if (pEntry->mValue.mInteger > 10000){
                pEntry->mValue.mInteger = 10000;
            } else if (pEntry->mValue.mInteger < -10000){
                pEntry->mValue.mInteger = -10000;
            }
            break;
    }
}

void* runWatcher(void* pArgs){
    StoreWatcher* watcher = (StoreWatcher *) pArgs;
    Store* store = watcher->mStore;

    JavaVM* javaVM = watcher->mJavaVm;
    JavaVMAttachArgs javaVMAttachArgs;
    javaVMAttachArgs.version = JNI_VERSION_1_6;
    javaVMAttachArgs.name = "NativeThread";
    javaVMAttachArgs.group = NULL;

    JNIEnv* env;
    if (javaVM->AttachCurrentThreadAsDaemon(&env, &javaVMAttachArgs) != JNI_OK){
        abort();
    }
    // Runs the thread loop.
    while (true){
        sleep(5);  // 5 秒
        env->MonitorEnter(watcher->mLock);
        if (!watcher->mRunning) break;
        StoreEntry* entry = watcher->mStore->mEntities;
        StoreEntry* entryEnd = entry + watcher->mStore->mLength;
        while (entry < entryEnd){
            processEntry(entry);
            ++entry;
        }
        // Critical section end.
        env->MonitorExit(watcher->mLock);
    }

    // Terminate the thread
    javaVM->DetachCurrentThread();
    delete watcher;
    pthread_exit(NULL);
}

StoreWatcher* startWatcher(JavaVM* pJavaVM, Store* pStore, jobject pLock){
    StoreWatcher* watcher = new StoreWatcher();
    watcher->mJavaVm = pJavaVM;
    watcher->mStore = pStore;
    watcher->mLock = pLock;
    watcher->mRunning = true;

    pthread_attr_t lAttributes;
    if (pthread_attr_init(&lAttributes)){
        abort();
    }
    if (pthread_create(&watcher->mThread, &lAttributes, runWatcher, watcher)){
        abort();
    }
    return watcher;
}

void storepWatcher(StoreWatcher* pWatcher){
    pWatcher->mRunning = false;
}




#endif //NDKPROJECT_STORE_H
