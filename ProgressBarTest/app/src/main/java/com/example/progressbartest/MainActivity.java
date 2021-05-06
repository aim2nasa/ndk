package com.example.progressbartest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.ProgressBar;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ProgressBar pb = (ProgressBar) findViewById(R.id.progressBar2);
        pb.setMax(250);
        pb.setProgress(100);
    }
}