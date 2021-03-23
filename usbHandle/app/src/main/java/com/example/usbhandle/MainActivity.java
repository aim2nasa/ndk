package com.example.usbhandle;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("testLib");
    }

    public native int helloNNDK(int v);
    public native int open();
    public native void close();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        int result = helloNNDK(5);

        ((TextView)findViewById(R.id.tvHello)).setText("result :"+result);
        ((TextView)findViewById(R.id.tvHello)).setText("open :"+open());
    }
}