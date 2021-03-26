package com.example.usbhandle;

import androidx.appcompat.app.AppCompatActivity;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.HashMap;
import java.util.Iterator;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("testLib");
    }

    private Button btnCon;
    private static final String TAG = "Tag";
    private static final String ACTION_USB_PERMISSION = "com.example.usbHandle.USB_PERMISSION";
    private UsbManager usbManager;
    public static final int PRODUCT_ID = 0x00f0;    //FX3
    public static final int VENDOR_ID = 0x04b4;     //Cypress

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

        usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
        PendingIntent permissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        registerReceiver(usbReceiver, filter);

        btnCon.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Log.v(TAG,"Connect button clicked");
                Toast.makeText(getApplicationContext(), "Connect clicked", Toast.LENGTH_SHORT).show();

                if(usbManager==null)
                    Toast.makeText(getApplicationContext(), "usbManager = null", Toast.LENGTH_SHORT).show();
                else
                    Toast.makeText(getApplicationContext(), "usbManager not null", Toast.LENGTH_SHORT).show();

                HashMap<String, UsbDevice> deviceList = usbManager.getDeviceList();
                if(deviceList == null) Toast.makeText(getApplicationContext(), "deviceList = null", Toast.LENGTH_LONG).show();
                Toast.makeText(getApplicationContext(), "deviceList="+deviceList.size(), Toast.LENGTH_LONG).show();

                Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
                while(deviceIterator.hasNext()) {
                    UsbDevice device = deviceIterator.next();
                    Toast.makeText(getApplicationContext(), "deviceList="+device.getDeviceName(), Toast.LENGTH_LONG).show();

                    if(device.getProductId()==PRODUCT_ID && device.getVendorId()==VENDOR_ID) {
                        Toast.makeText(getApplicationContext(), "FX3 found", Toast.LENGTH_LONG).show();

                        UsbDeviceConnection usbDeviceConnection = usbManager.openDevice(device);
                        if(usbDeviceConnection==null) {
                            Toast.makeText(getApplicationContext(), "usbDeviceConnection = null", Toast.LENGTH_LONG).show();
                        }else{
                            Toast.makeText(getApplicationContext(), "usbDeviceConnection != null", Toast.LENGTH_LONG).show();
                        }
                    }
                }
            }
        });
    }

    private final BroadcastReceiver usbReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            Log.d(TAG, "onReceive");
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        if(device != null){
                            //call method to set up device communication
                            Log.d(TAG, "permission granted for device " + device);
                            Toast.makeText(getApplicationContext(), "permission granted for device "+ device, Toast.LENGTH_SHORT).show();
                        }
                    }
                    else {
                        Log.d(TAG, "permission denied for device " + device);
                        Toast.makeText(getApplicationContext(), "permission denied for device " + device, Toast.LENGTH_SHORT).show();
                    }
                }
            }
        }
    };
}