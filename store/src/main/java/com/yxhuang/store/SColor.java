package com.yxhuang.store;

import android.graphics.Color;
import android.text.TextUtils;

/**
 * Created by yxhuang
 * Date: 2017/10/6
 * Description:
 */

public class SColor {
    private int mColor;

    public SColor(String color){
        if (TextUtils.isEmpty(color)){
            throw new IllegalArgumentException();
        }
        mColor = Color.parseColor(color);
    }

    @Override
    public String toString() {
        return String.format("#%06X", mColor);
    }
}
