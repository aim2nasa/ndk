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
        System.loadLibrary("glsusb");
    }

    private Button btnCon;
    private static final String TAG = "Tag";
    private static final String ACTION_USB_PERMISSION = "com.example.usbHandle.USB_PERMISSION";
    private UsbManager usbManager;
    private int fileDescriptor;
    public static final int PRODUCT_ID = 0x00f0;    //FX3
    public static final int VENDOR_ID = 0x04b4;     //Cypress

    public native int helloNNDK(int v);
    public native int open(int fileDescriptor);
    public native void close();
    public native int reader();
    public native long count();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnCon = (Button)findViewById(R.id.button_Connect);
        btnCon.setEnabled(true);

        int result = helloNNDK(5);

        ((TextView)findViewById(R.id.tvHello)).setText("result :"+result);

        usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
        PendingIntent permissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        registerReceiver(usbReceiver, filter);

        btnCon.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Log.i(TAG,"Connect button clicked");

                if(usbManager==null) {
                    Log.e(TAG,"usbManager = null");
                    return;
                }

                HashMap<String, UsbDevice> deviceList = usbManager.getDeviceList();
                if(deviceList == null) {
                    Log.e(TAG,"deviceList = null");
                    return;
                }
                Log.i(TAG,"deviceList="+deviceList.size());

                Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
                while(deviceIterator.hasNext()) {
                    UsbDevice device = deviceIterator.next();
                    Log.i(TAG,"deviceName="+device.getDeviceName());

                    if(device.getProductId()==PRODUCT_ID && device.getVendorId()==VENDOR_ID) {
                        Log.i(TAG,"FX3 found (ProductID=0x00f0,VendorID=0x04b4)");

                        UsbDeviceConnection usbDeviceConnection = usbManager.openDevice(device);
                        if(usbDeviceConnection==null) {
                            Log.e(TAG,"usbDeviceConnection = null");
                        }else{
                            fileDescriptor = usbDeviceConnection.getFileDescriptor();
                            Log.i(TAG,"fileDescriptor="+fileDescriptor);

                            usbManager.requestPermission(device,permissionIntent);
                            Log.i(TAG,"requestPermission done");
                        }
                    }
                }
            }
        });
    }

    private final BroadcastReceiver usbReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            Log.i(TAG, "BroadcastReceiver.onReceive called");

            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        if(device != null){
                            //call method to set up device communication
                            Log.i(TAG, "permission granted for device " + device);

                            int r = open(fileDescriptor);
                            ((TextView)findViewById(R.id.tvHello)).setText("open="+r);
                            Toast.makeText(getApplicationContext(), "open="+r, Toast.LENGTH_LONG).show();

                            Log.i(TAG, "reader = " + reader());
                        }else
                            Log.e(TAG, "device = null");
                    }
                    else {
                        Log.e(TAG, "permission denied for device " + device);
                    }
                }
            }
        }
    };
}
