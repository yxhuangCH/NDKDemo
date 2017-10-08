//
// Created by yxhuang on 2017/9/25.
//
#include <cstdlib>
#include <string.h>
#include "com_yxhuang_store_Store.h"
#include "Store.h"
#include <cstdint>

static Store gStore;
static jclass StringClass;
static jclass ColorClass;

static jmethodID MethodOnSuccessInt;
static jmethodID MethodOnSuccessString;
static jmethodID MethodOnSuccessColor;

JNIEXPORT jint JNI_OnLoad(JavaVM* pVM, void* reserved){
    JNIEnv *env;
    if (pVM->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK){
        abort();
    }
    jclass StringClassTmp = env->FindClass("java/lang/String");
    if (StringClassTmp == NULL) {
        abort();
    }
    StringClass = (jclass) env->NewGlobalRef(StringClassTmp);
    // 获取 Java 类
    jclass ColorClassTmp = env->FindClass("com/yxhuang/store/SColor");
    if (ColorClassTmp == NULL){
        abort();
    }
    ColorClass = (jclass) env->NewGlobalRef(ColorClassTmp);
    env->DeleteLocalRef(ColorClassTmp);

    // Caches Methods
    jclass StoreClass = env->FindClass("com/yxhuang/store/Store");
    if (StoreClass == NULL){
        abort();
    }
    MethodOnSuccessInt = env->GetMethodID(StoreClass, "onSuccess", "(I)V");
    if (MethodOnSuccessInt == NULL){
        abort();
    }
    MethodOnSuccessString = env->GetMethodID(StoreClass, "onSuccess", "(Ljava/lang/String;)V");
    if (MethodOnSuccessString == NULL){
        abort();
    }
    MethodOnSuccessColor = env->GetMethodID(StoreClass, "onSuccess", "(Lcom/yxhuang/store/SColor;)V");
    if (MethodOnSuccessColor == NULL){
        abort();
    }
    env->DeleteLocalRef(StoreClass);

    // Store initialization
    gStore.mLength = 0;
    return JNI_VERSION_1_6;
}

bool isEntryValid(JNIEnv* pEnv, StoreEntry *pEntry, StoreType pType){
    if (pEntry == NULL){
        throwNotExistingKeyException(pEnv);
    } else if (pEntry->mType != pType){
        throwInvalidTypeException(pEnv);
    }
    return !pEnv->ExceptionCheck();
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
        case StoreType_IntegerArray:
            delete[] pEntry->mValue.mIntegerArray;
            break;
        case StoreType_StringArray:
            // Destroys every C string pointed by the String array before releasing it.
            for (int32_t i = 0; i < pEntry->mLength; ++i) {
                delete pEntry->mValue.mStringArray[i];
            }
            delete[] pEntry->mValue.mStringArray;
            break;
        case StoreType_ColorArray:
            // Unreferences every Id before releasing the Id array.
            for (int32_t i = 0; i < pEntry->mLength; ++i) {
                pEnv->DeleteGlobalRef(pEntry->mValue.mSColorArray[i]);
            }
            delete[] pEntry->mValue.mSColorArray;
            break;
    }
}

StoreEntry* allocateEntry(JNIEnv* pEnv, Store* pStore, jstring pKey){
    // If entry already exists in the store, release its content and keep its key.
    StoreEntry* entry = findEntry(pEnv, pStore, pKey);
    if (entry != NULL){
        releaseEntryValue(pEnv, entry);
    } else{
        // Checks store can accept a new entry
        if (pStore->mLength >= STORE_MAX_CAPACITY){
            throwStoreFullException(pEnv);
            return NULL;
        }
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

        pEnv->CallVoidMethod(pThis, MethodOnSuccessString, pEnv->NewStringUTF(entry->mValue.mString));
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

        pEnv->CallVoidMethod(pThis, MethodOnSuccessInt, entry->mValue.mInteger);
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

        pEnv->CallVoidMethod(pThis, MethodOnSuccessColor, entry->mValue.mSColor);
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


JNIEXPORT void JNICALL
Java_com_yxhuang_store_Store_setIntegerArray(JNIEnv* pEnv, jobject pThis, jstring pKey, jintArray pIntegerArray){
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL){
        jsize length = pEnv->GetArrayLength(pIntegerArray);
        int32_t* array = new int32_t[length];
        pEnv->GetIntArrayRegion(pIntegerArray, 0, length, array);

        entry->mType = StoreType_IntegerArray;
        entry->mLength = length;
        entry->mValue.mIntegerArray = array;
    }
}


JNIEXPORT jintArray JNICALL
Java_com_yxhuang_store_Store_getIntegerArray(JNIEnv* pEnv, jobject pThis, jstring pKey){
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_IntegerArray)){
        jintArray javaArray = pEnv->NewIntArray(entry->mLength);
        pEnv->SetIntArrayRegion(javaArray, 0, entry->mLength, entry->mValue.mIntegerArray);
        return javaArray;
    } else{
        return NULL;
    }
}

JNIEXPORT void JNICALL
Java_com_yxhuang_store_Store_setStringArray(JNIEnv* pEnv, jobject pThis, jstring pKey, jobjectArray pStringArray){
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL){
        jsize length = pEnv->GetArrayLength(pStringArray);
        char** array = new char*[length];
        for (int32_t i = 0; i < length; ++i) {
            jstring string = (jstring) pEnv->GetObjectArrayElement(pStringArray, i);
            jsize stringLength = pEnv->GetStringUTFLength(string);
            array[i] = new char[stringLength + 1];
            // Directly copies  the Java string into our new C buffer.
            pEnv->GetStringUTFRegion(string, 0, stringLength, array[i]);
            array[i][stringLength] = '\0';
            pEnv->DeleteLocalRef(string);
        }
        entry->mType = StoreType_StringArray;
        entry->mLength = length;
        entry->mValue.mStringArray = array;
    }
}


JNIEXPORT jobjectArray JNICALL Java_com_yxhuang_store_Store_getStringArray(JNIEnv* pEnv, jobject pThis, jstring pKey){
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_StringArray)){
        // An array of String in Java si in fact an array of object.
        jobjectArray javaArray = pEnv->NewObjectArray(entry->mLength, StringClass, NULL);
        for (int32_t i = 0; i < entry->mLength; ++i) {
            jstring string = pEnv->NewStringUTF(entry->mValue.mStringArray[i]);
            pEnv->SetObjectArrayElement(javaArray, i, string);
            pEnv->DeleteLocalRef(string);
        }
        return javaArray;
    } else{
        return NULL;
    }
}


JNIEXPORT void JNICALL
Java_com_yxhuang_store_Store_setColorArray(JNIEnv* pEnv, jobject pThis, jstring pKey, jobjectArray pColorArray){
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL){
        // 获取 array 的长度
        jsize length = pEnv->GetArrayLength(pColorArray);
        jobject* array = new jobject[length];
        // File the C array with a copy of each input Java Color
        for (int32_t i = 0; i < length; ++i) {
            // GetObjectArrayElement puts one single object reference from a java array.
            // The returned references is local.
            jobject localColor = pEnv->GetObjectArrayElement(pColorArray, i);
            array[i] = pEnv->NewGlobalRef(localColor);
            pEnv->DeleteLocalRef(localColor);
        }
        entry->mType = StoreType_ColorArray;
        entry->mLength = length;
        entry->mValue.mSColorArray = array;
    }
}


JNIEXPORT jobjectArray JNICALL
Java_com_yxhuang_store_Store_getColorArray(JNIEnv* pEnv, jobject pThis, jstring pKey){
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_ColorArray)){
        // 创建新的 object array
        jobjectArray javaArray = pEnv->NewObjectArray(entry->mLength, ColorClass, NULL);
        for (int32_t i = 0; i < entry->mLength; ++i) {
            // SetObjectArrayElement() put one single object reference into a java array.
            // A Global reference is created implicitly.
            pEnv->SetObjectArrayElement(javaArray, i, entry->mValue.mSColorArray[i]);
        }
        return javaArray;
    } else{
        return NULL;
    }

}