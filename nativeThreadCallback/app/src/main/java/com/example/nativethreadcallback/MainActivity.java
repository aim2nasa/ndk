package com.example.nativethreadcallback;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public native int startThread();
    public native int endThread();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        Log.i("NTC","Java onCreate(), startThread : "+ startThread());
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.i("NTC","Java onStop(), endThread : "+ endThread());
    }

    public static void callback(int a){
        Log.i("NTC", "Java callback function: " + a);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}