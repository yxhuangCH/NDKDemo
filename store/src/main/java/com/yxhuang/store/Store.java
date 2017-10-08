package com.yxhuang.store;

import com.yxhuang.store.exception.InvalidTypeException;
import com.yxhuang.store.exception.NotExistingKeyException;
import com.yxhuang.store.listener.StoreListener;

/**
 * Created by yxhuang
 * Date: 2017/9/21
 * Description:
 */

public class Store implements StoreListener {

    static {
        System.loadLibrary("com_yxhuang_Store");
    }

    private StoreListener mListener;

    public Store(StoreListener listener) {
        mListener = listener;
    }

    public native int getCount();

    public native String getString(String pKey);

    public native void setString(String pKey, String pString);


    public native int getInteger(String pKey) throws NotExistingKeyException, InvalidTypeException;

    public native void setInteger(String pkey, int pInt);

    // Store part 7
    public native void setColor(String key, SColor color);
    public native SColor getSColor(String key) throws NotExistingKeyException, InvalidTypeException;

    // Store part 8
    public native void setIntegerArray(String pKey, int[] pIntArray);
    public native int[] getIntegerArray(String pKey) throws NotExistingKeyException, InvalidTypeException;

    public native void setStringArray(String pKey, String[] pStringArray);
    public native String[] getStringArray(String pKey) throws NotExistingKeyException, InvalidTypeException;

    public native void setColorArray(String pKey, SColor[] pColorArray);
    public native SColor[] getColorArray(String pKey) throws NotExistingKeyException, InvalidTypeException;

    @Override
    public void onSuccess(int pValue) {
        mListener.onSuccess(pValue);
    }

    @Override
    public void onSuccess(String pValue) {
        mListener.onSuccess(pValue);
    }

    @Override
    public void onSuccess(SColor pValue) {
        mListener.onSuccess(pValue);
    }
}
