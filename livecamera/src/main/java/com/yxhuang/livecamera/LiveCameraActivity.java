package com.yxhuang.livecamera;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.os.Bundle;
import android.view.TextureView;
import android.widget.ImageView;

import java.io.IOException;
import java.util.List;

public class LiveCameraActivity extends Activity implements TextureView.SurfaceTextureListener, Camera.PreviewCallback{

    private Camera mCamera;
    private TextureView mTextureView;
    private byte[] mVideoSource;
    private ImageView mImageViewR, mImageViewG, mImageViewB;
    private Bitmap mBitmapR, mBitmapG, mBitmapB;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mTextureView = findViewById(R.id.preview);
        mImageViewR = findViewById(R.id.imageViewR);
        mImageViewG = findViewById(R.id.imageViewG);
        mImageViewB = findViewById(R.id.imageViewB);

        mTextureView.setSurfaceTextureListener(this);


    }

    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
        if (camera != null){
            decode(mBitmapR, bytes, 0xFFFF0000);
            decode(mBitmapG, bytes, 0xFF00FF00);
            decode(mBitmapB, bytes, 0xFF0000FF);
            mImageViewR.invalidate();
            mImageViewG.invalidate();
            mImageViewB.invalidate();

            mCamera.addCallbackBuffer(mVideoSource);
        }

    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surfaceTexture, int pWith, int pHeight) {
        mCamera = Camera.open();
        try {
            mCamera.setPreviewTexture(surfaceTexture);
            mCamera.setPreviewCallbackWithBuffer(this);
            mCamera.setDisplayOrientation(0);

            Camera.Size size = findBestResolution(pWith, pHeight);
            PixelFormat pixelFormat = new PixelFormat();
            PixelFormat.getPixelFormatInfo(mCamera.getParameters().getPreviewFormat(), pixelFormat);
            int sourceSize = size.width * size.height * pixelFormat.bitsPerPixel / 8;
            // Set up camera size and video format
            // Should be the default on Android anyway
            Camera.Parameters parameters = mCamera.getParameters();
            parameters.setPictureSize(pWith, pHeight);
            parameters.setPreviewFormat(PixelFormat.YCbCr_422_SP);
            mCamera.setParameters(parameters);
            // Set up video
            mVideoSource = new byte[sourceSize];
            mBitmapR = Bitmap.createBitmap(size.width, size.height, Bitmap.Config.ARGB_8888);
            mBitmapG = Bitmap.createBitmap(size.width, size.height, Bitmap.Config.ARGB_8888);
            mBitmapB = Bitmap.createBitmap(size.width, size.height, Bitmap.Config.ARGB_8888);
            mImageViewR.setImageBitmap(mBitmapR);
            mImageViewG.setImageBitmap(mBitmapG);
            mImageViewB.setImageBitmap(mBitmapB);

            mCamera.addCallbackBuffer(mVideoSource);
            mCamera.startPreview();

        } catch (IOException e) {
            e.printStackTrace();
            mCamera.release();
            mCamera = null;
            throw new IllegalArgumentException();
        }
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surfaceTexture, int i, int i1) {

    }

    // 释放
    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surfaceTexture) {
        if (mCamera != null){
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
            mVideoSource = null;
            mBitmapR.recycle();
            mBitmapR = null;
            mBitmapG.recycle();
            mBitmapG = null;
            mBitmapB.recycle();
            mBitmapB = null;
        }
        return true;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surfaceTexture) {


    }

    private Camera.Size findBestResolution(int pWith, int pHeight) {
        List<Camera.Size> sizes = mCamera.getParameters().getSupportedPreviewSizes();
        Camera.Size selectedSize = mCamera.new Size(0, 0);
        // finds the biggest resolution which fits screen;
        for (Camera.Size size : sizes) {
            if (size.width <= pWith && size.height <= pHeight &&
                    size.width >= selectedSize.width && size.height >= selectedSize.height) {
                selectedSize = size;
            }
        }
        if (selectedSize.width == 0 && selectedSize.height == 0) {
            selectedSize = sizes.get(0);
        }
        return selectedSize;
    }

    public native void decode(Bitmap pTartger, byte[] pSource, int pFilter);
}
