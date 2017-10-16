package com.yxhuang.store;

import com.yxhuang.store.exception.InvalidTypeException;
import com.yxhuang.store.exception.NotExistingKeyException;
import com.yxhuang.store.listener.StoreListener;

/**
 * Created by yxhuang
 * Date: 2017/10/11
 * Description:
 */

public class StoreThreadSafe extends Store {

    private static Object LOCK;

    public StoreThreadSafe(StoreListener listener) {
        super(listener);
    }

    @Override
    public int getCount() {
        synchronized (LOCK){
            return super.getCount();
        }
    }

    @Override
    public int getInteger(String pKey) throws NotExistingKeyException, InvalidTypeException {
        synchronized (LOCK){
            return super.getInteger(pKey);
        }
    }

    @Override
    public void setInteger(String pkey, int pInt) {
        synchronized (LOCK){
            super.setInteger(pkey, pInt);
        }
    }

    @Override
    public String getString(String pKey) {
        synchronized (LOCK){
            return super.getString(pKey);
        }
    }

    @Override
    public void setString(String pKey, String pString) {
        synchronized (LOCK){
            super.setString(pKey, pString);
        }
    }

    @Override
    public SColor getSColor(String key) throws NotExistingKeyException, InvalidTypeException {
        synchronized (LOCK){
            return super.getSColor(key);
        }
    }

    @Override
    public void setColor(String key, SColor color) {
        synchronized (LOCK){
            super.setColor(key, color);
        }
    }

    @Override
    public void stopWatcher(long pPointer) {
        synchronized (LOCK){
            super.stopWatcher(pPointer);
        }
    }
}
