# 变量表示源文件在开发树中的位置,  my-dir 将返回当前目录
LOCAL_PATH := $(call my-dir)

# CLEAR_VARS 变量指向特殊 GNU Makefile
include $(CLEAR_VARS)

# 构建的模块的名称
LOCAL_MODULE := droidblaster

#枚举源文件，以空格分隔多个文件
LOCAL_SRC_FILES := Main.cpp EventLoop.cpp Log.cpp

LOCAL_LDLIBS := -landroid -llog
LOCAL_STATIC_LIBRARIES := android_native_app_glue

#将所有内容连接到一起：
include $(BUILD_SHARED_LIBRARY)

$(call import-module, android/native_app_glue)