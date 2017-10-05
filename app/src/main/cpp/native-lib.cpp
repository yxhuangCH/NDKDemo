#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_yxhuang_ndkproject_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_yxhuang_ndkproject_MainActivity_getKeyFormJNI(JNIEnv *env, jobject){
    std::string app_key = "123456789";
    return env->NewStringUTF(app_key.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yxhuang_ndkproject_MainActivity_accessJavaStringField(JNIEnv *env, jstring javaString){
    // 把java 字符串转成 C 字符串
//    jbyte* str;
//    jboolean isCopy;
//    str = env->GetStringUTFChars(javaString, &isCopy);
//    if ( 0 != str){
//        printf("Java string:  %s", str);
//
//        if (JNI_TRUE == isCopy){
//            printf("C string is a copy of the Java string.");
//        } else{
//            printf("C string points to actual string.");
//        }
//    }

}
