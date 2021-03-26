package com.example.usbhandle;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("testLib");
    }

    private Button btnCon;
    private static final String TAG = "Tag";

    public native int helloNNDK(int v);
    public native int open();
    public native void close();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnCon = (Button)findViewById(R.id.button_Connect);
        btnCon.setEnabled(true);

        int result = helloNNDK(5);

        ((TextView)findViewById(R.id.tvHello)).setText("result :"+result);
        ((TextView)findViewById(R.id.tvHello)).setText("open :"+open());

        btnCon.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Log.v(TAG,"Connect button clicked");
                Toast.makeText(getApplicationContext(), "Connect clicked", Toast.LENGTH_SHORT).show();
            }
        });
    }
}