//
// Created by yxhuang on 2017/9/25.
//

#ifndef NDKPROJECT_STORE_H
#define NDKPROJECT_STORE_H

#include <cstdint>
#include <stdint.h>
#include "jni.h"

#define STORE_MAX_CAPACITY 16

typedef enum {
    StoreType_String
} StoreType;

typedef union {
    char* mString;
} StoreValue;

typedef struct {
    char* mKey;
    StoreType mType;
    StoreValue mValue;
} StoreEntry;

typedef struct {
    StoreEntry mEntities[STORE_MAX_CAPACITY];
    int32_t mLength;
} Store;
#endif //NDKPROJECT_STORE_H
