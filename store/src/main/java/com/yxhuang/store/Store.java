package com.yxhuang.store;

/**
 * Created by yxhuang
 * Date: 2017/9/21
 * Description:
 */

public class Store {

    static {
        System.loadLibrary("com_yxhuang_Store");
    }

    public native int getCount();

    public native String getString(String pKey);

    public native void setString(String pKey, String pString);
}
