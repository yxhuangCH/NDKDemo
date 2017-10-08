//
// Created by yxhuang on 2017/9/25.
//

#ifndef NDKPROJECT_STORE_H
#define NDKPROJECT_STORE_H

#include <cstdint>
#include <cstdint>
#include <stdint.h>
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

#endif //NDKPROJECT_STORE_H
