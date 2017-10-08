package com.yxhuang.store;

import android.app.Fragment;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.google.common.base.Function;
import com.google.common.base.Joiner;
import com.google.common.collect.Lists;
import com.google.common.primitives.Ints;
import com.yxhuang.store.exception.InvalidTypeException;
import com.yxhuang.store.exception.NotExistingKeyException;

import java.util.Arrays;
import java.util.List;
import java.util.regex.Pattern;

/**
 * Created by yxhuang
 * Date: 2017/9/21
 * Description:
 */

public class PlaceholderFragment extends Fragment {
    private Store mStore = new Store();

    private View mRootView;
    private TextView mTvContent;

    private EditText mUIKeyEdit, mUIValueEdit;
    private Spinner mUITypeSpinner;
    private Button mUIGetButton, mUISetButton;
    private Pattern mKeyPattern;

    public PlaceholderFragment() {

    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        mRootView= inflater.inflate(R.layout.fragment_store, container, false);
        initView();
        updateContent();
        return mRootView;
    }

    private void initView(){
        mTvContent = (TextView) mRootView.findViewById(R.id.tv_content);

        mKeyPattern = Pattern.compile("\\p{Alnum}+");
        mUIKeyEdit = (EditText) mRootView.findViewById(R.id.uiKeyEdit);
        mUIValueEdit = (EditText) mRootView.findViewById(R.id.uiValueEdit);

        ArrayAdapter<StoreType> adapter = new ArrayAdapter<>(getActivity(),
                                                android.R.layout.simple_spinner_item,
                                                StoreType.values());
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mUITypeSpinner = (Spinner) mRootView.findViewById(R.id.uiTypeSpinner);
        mUITypeSpinner.setAdapter(adapter);

        mUIGetButton = (Button) mRootView.findViewById(R.id.uiGetValueButton);
        mUIGetButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onGetValue();
            }
        });
        mUISetButton = (Button) mRootView.findViewById(R.id.uiSetValueButton);
        mUISetButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onSetValue();
            }
        });

    }

    private void onGetValue(){
        String key = mUIKeyEdit.getText().toString();
        StoreType type = (StoreType) mUITypeSpinner.getSelectedItem();
        if (!mKeyPattern.matcher(key).matches()){
            displayMessage("Incorrect key.");
            return;
        }

        try {
            switch (type){
                case String:
                    mUIValueEdit.setText(mStore.getString(key));
                    break;
                case Integer:
                    mUIValueEdit.setText(Integer.toString(mStore.getInteger(key)));
                    break;
                case SColor:
                    mUIValueEdit.setText(mStore.getSColor(key).toString());
                    break;
                case IntegerArray:
                    mUIValueEdit.setText(Ints.join(";", mStore.getIntegerArray(key)));
                    break;
                case StringArray:
                    mUIValueEdit.setText(Joiner.on(";").join(mStore.getStringArray(key)));
                    break;
                case ColorArray:
                    mUIValueEdit.setText(Joiner.on(";").join(mStore.getColorArray(key)));
                    break;
            }
        } catch (NotExistingKeyException e) {
            e.printStackTrace();
        } catch (InvalidTypeException e) {
            e.printStackTrace();
        }
    }

    private void onSetValue(){
        String key = mUIKeyEdit.getText().toString();
        String value = mUIValueEdit.getText().toString();
        StoreType type = (StoreType) mUITypeSpinner.getSelectedItem();

        if (!mKeyPattern.matcher(key).matches()){
            displayMessage("Incorrect key.");
            return;
        }

        try {
            switch (type) {
                case String:
                    mStore.setString(key, value);
                    break;
                case Integer:
                    mStore.setInteger(key, Integer.parseInt(value));
                case SColor:
                    mStore.setColor(key, new SColor(value));
                    break;
                case IntegerArray:
                    mStore.setIntegerArray(key, Ints.toArray(stringToList(new Function<String, Integer>() {
                        @javax.annotation.Nullable
                        @Override
                        public Integer apply(@javax.annotation.Nullable String input) {
                            return Integer.parseInt(input);
                        }
                    }, value)));
                    break;
                case StringArray:
                    String[] stringArray = value.split(";");
                    mStore.setStringArray(key, stringArray);
                    break;
                case ColorArray:
                    List<SColor> idList = stringToList(new Function<String, SColor>() {
                        @javax.annotation.Nullable
                        @Override
                        public SColor apply(@javax.annotation.Nullable String input) {
                            return new SColor(input);
                        }
                    }, value);
                    mStore.setColorArray(key, idList.toArray(new SColor[idList.size()]));
                    break;
            }
        } catch (NumberFormatException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
            displayMessage("Incorrect value");
        }
        updateContent();
    }

    private void displayMessage(String msg){
        Toast.makeText(getActivity(), msg, Toast.LENGTH_LONG).show();
    }

    private void updateContent(){
        int numEntries = mStore.getCount();
        mTvContent.setText(String.format("Store  %S", numEntries));
    }

    private <TType>List<TType> stringToList(Function<String, TType> pConversion, String pValue){
        String[] splitArray = pValue.split(";");
        List<String> splitList = Arrays.asList(splitArray);
        return Lists.transform(splitList, pConversion);
    }
}
