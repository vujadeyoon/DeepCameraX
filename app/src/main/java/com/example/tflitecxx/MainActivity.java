package com.example.tflitecxx;


import androidx.appcompat.app.AppCompatActivity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity {
    private final String MODEL_FILE_1 = "centerface.tflite";
    private final String MODEL_FILE_2 = "super_point/model_SuperPoint_120x160.tflite";
    private final String MODEL_FILE_3 = "super_point/model_SuperPoint_240x320.tflite";
    private final String MODEL_FILE_4 = "super_point/model_SuperPoint_256x256.tflite";
    private final String MODEL_FILE_5 = "super_point/model_SuperPoint_480x640.tflite";
    private final String MODEL_FILE_6 = "super_point/model_SuperPoint_512x512.tflite";
    private final String MODEL_FILE_7 = "super_point/model_SuperPointDepthwiseSeparableConv_120x160.tflite";
    private final String MODEL_FILE_8 = "super_point/model_SuperPointDepthwiseSeparableConv_240x320.tflite";
    private final String MODEL_FILE_9 = "super_point/model_SuperPointDepthwiseSeparableConv_256x256.tflite";
    private final String MODEL_FILE_10 = "super_point/model_SuperPointDepthwiseSeparableConv_480x640.tflite";
    private final String MODEL_FILE_11 = "super_point/model_SuperPointDepthwiseSeparableConv_512x512.tflite";
    private final String MODEL_FILE_SP_1 = "super_point/converted_model.tflite";
    private final String MODEL_FILE_SP_2 = "super_point/spModel_lst.tflite";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(loadModelJNI(this.getAssets(), MODEL_FILE_SP_2));
    }

    /*
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
    */

    // Load model by TF Lite C++ API
    private native String loadModelJNI(AssetManager assetManager, String fileName);
}
