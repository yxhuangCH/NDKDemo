package com.yxhuang.store.listener;

import com.yxhuang.store.SColor;

/**
 * Created by yxhuang
 * Date: 2017/10/8
 * Description:
 */

public interface StoreListener {

    void onSuccess(int pValue);

    void onSuccess(String pValue);

    void onSuccess(SColor pValue);
}
