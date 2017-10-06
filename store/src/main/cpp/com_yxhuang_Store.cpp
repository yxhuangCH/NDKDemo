//
// Created by yxhuang on 2017/9/25.
//
#include <cstdlib>
#include <string.h>
#include "com_yxhuang_store_Store.h"
#include "Store.h"

static Store gStore;

JNIEXPORT jint JNI_OnLoad(JavaVM* pVM, void* reserved){
    // Store initialization
    gStore.mLength = 0;
    return JNI_VERSION_1_6;
}

bool isEntryValid(JNIEnv *pEnv, StoreEntry *pEntry, StoreType pType){
    return ((pEntry != NULL) && (pEntry->mType == pType));
}

StoreEntry* findEntry(JNIEnv* pEnv, Store* pStore, jstring pKey){
    StoreEntry* entry = pStore->mEntities;
    StoreEntry* entryEnd = entry + pStore->mLength;

    const char* tmpKey = pEnv->GetStringUTFChars(pKey, NULL);
    while (entry < entryEnd && strcmp(entry->mKey, tmpKey) != 0){
        ++entry;
    }
    pEnv->ReleaseStringUTFChars(pKey, tmpKey);

    return entry == entryEnd ? NULL : entry;
}


void releaseEntryValue(JNIEnv* pEnv, StoreEntry* pEntry){
    switch (pEntry->mType){
        case StoreType_String:
            delete pEntry->mValue.mString;
            break;
        case StoreType_Color:
            pEnv->DeleteGlobalRef(pEntry->mValue.mSColor);
            break;
    }
}

StoreEntry* allocateEntry(JNIEnv* pEnv, Store* pStore, jstring pKey){
    // If entry already exists in the store, release its content and keep its key.
    StoreEntry* entry = findEntry(pEnv, pStore, pKey);
    if (entry != NULL){
        releaseEntryValue(pEnv, entry);
    } else{
        entry = pStore->mEntities + pStore->mLength;
        // Copies the new key into its final C string buffer.
        const char* tmpKey = pEnv->GetStringUTFChars(pKey, NULL);
        entry->mKey = new char[strlen(tmpKey) + 1];
        strcpy(entry->mKey, tmpKey);
        pEnv->ReleaseStringUTFChars(pKey, tmpKey);

        ++pStore->mLength;
    }
    return entry;
}

JNIEXPORT jint JNICALL
Java_com_yxhuang_store_Store_getCount(JNIEnv *env, jobject /* this */) {
    return gStore.mLength;
}

JNIEXPORT jstring JNICALL
Java_com_yxhuang_store_Store_getString(JNIEnv* pEnv, jobject pThis, jstring pKey){
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_String)){
        // Converts a C string into a Java String.
        return pEnv->NewStringUTF(entry->mValue.mString);
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL
Java_com_yxhuang_store_Store_setString(JNIEnv* pEnv, jobject pThis, jstring pKey, jstring pString){
    // Turns the java String into a temporary C string.
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL){
        entry->mType = StoreType_String;
        // Copy the temporary C string into its dynamically allocated final location.
        jsize stringLength = pEnv->GetStringLength(pString);
        entry->mValue.mString = new char[stringLength + 1];
        pEnv->GetStringUTFRegion(pString, 0, stringLength, entry->mValue.mString);
        // Append the null character for  string termination.
        entry->mValue.mString[stringLength] = '\0';
    }
}

JNIEXPORT jint JNICALL
Java_com_yxhuang_store_Store_getInteger(JNIEnv* pEnv, jobject pThis, jstring pKey){
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if(isEntryValid(pEnv, entry, StoreType_Integer)){
        return entry->mValue.mInteger;
    } else{
        return 0;
    }
};


JNIEXPORT void JNICALL
Java_com_yxhuang_store_Store_setInteger(JNIEnv* pEnv, jobject pThis, jstring pKey, jint pInteger){
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL){
        entry->mType = StoreType_Integer;
        entry->mValue.mInteger = pInteger;
    }
};


JNIEXPORT void JNICALL
Java_com_yxhuang_store_Store_setColor(JNIEnv* pEnv, jobject pThis, jstring pKey, jobject pColor){
    // Save the SColor reference in the store
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL){
        entry->mType = StoreType_Color;
        // 防止被回收，需要一个全局引用, 但是主要要释放
        entry->mValue.mSColor = pEnv->NewGlobalRef(pColor);
    }
}


JNIEXPORT jobject JNICALL
Java_com_yxhuang_store_Store_getSColor(JNIEnv* pEnv, jobject pThis, jstring pKey){
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_Color)){
        return entry->mValue.mSColor;
    } else{
        return NULL;
    }

}