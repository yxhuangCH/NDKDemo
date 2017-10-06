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


    public native int getInteger(String pKey);

    public native void setInteger(String pkey, int pInt);

    // Store part 7
    public native void setColor(String key, SColor color);
    public native SColor getSColor(String key);
}
