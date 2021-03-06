package com.example.usbhandle;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("glsusb");
    }

    private Button btnCon;
    private Button btnSnd;
    private Button btnRcv;
    private static final String TAG = "Tag";
    private static final String ACTION_USB_PERMISSION = "com.example.usbHandle.USB_PERMISSION";
    private UsbManager usbManager;
    private int fileDescriptor;
    public static final int PRODUCT_ID = 0x00f0;    //FX3
    public static final int VENDOR_ID = 0x04b4;     //Cypress
    private static Handler handler;
    private ListView list;
    List<String> listData;
    ArrayAdapter<String> listAdapter;

    public native int helloNNDK(int v);
    public native int open(int fileDescriptor);
    public native void close();
    public native int reader();
    public native long count();
    public native int writer();

    private void createList()
    {
        list = (ListView)findViewById(R.id.list);
        listData = new ArrayList<>();
        listAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1,listData);
        list.setAdapter(listAdapter);
    }

    private void createButtons()
    {
        btnCon = (Button)findViewById(R.id.button_Connect);
        btnSnd = (Button)findViewById(R.id.button_Send);
        btnRcv = (Button)findViewById(R.id.button_Recv);
        btnCon.setEnabled(true);
        btnSnd.setEnabled(false);
        btnRcv.setEnabled(false);
    }

    private boolean listDataAdd(String msg)
    {
        if(listData.add(msg)) {
            listAdapter.notifyDataSetChanged();
            return true;
        }
        return false;
    }

    private int LI(String tag,String msg)
    {
        listDataAdd(msg);
        return Log.i(tag,msg);
    }

    private int LE(String tag,String msg)
    {
        listDataAdd(msg);
        return Log.e(tag,msg);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        createButtons();
        createList();

        handler = new Handler(){
            @Override
            public void handleMessage(Message msg){
                ((TextView)findViewById(R.id.tvHello)).setText("count:"+msg.obj);
            }
        };

        class NewRunnerable implements Runnable{
            @Override
            public void run() {
                while(true){
                    try{
                        Thread.sleep(100);
                    }catch(Exception e){
                        e.printStackTrace();
                    }
                    Message msg = new Message();
                    msg.obj = Long.toString(count());
                    handler.sendMessage(msg);
                }
            }
        };

        usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
        PendingIntent permissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
        IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
        registerReceiver(usbReceiver, filter);

        btnCon.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                LI(TAG,"Connect button clicked");

                if(usbManager==null) {
                    LE(TAG,"usbManager = null");
                    return;
                }

                HashMap<String, UsbDevice> deviceList = usbManager.getDeviceList();
                if(deviceList == null) {
                    LE(TAG,"deviceList = null");
                    return;
                }
                LI(TAG,"deviceList="+deviceList.size());

                Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
                while(deviceIterator.hasNext()) {
                    UsbDevice device = deviceIterator.next();
                    LI(TAG,"deviceName="+device.getDeviceName());

                    if(device.getProductId()==PRODUCT_ID && device.getVendorId()==VENDOR_ID) {
                        LI(TAG,"FX3 found (ProductID=0x00f0,VendorID=0x04b4)");

                        UsbDeviceConnection usbDeviceConnection = usbManager.openDevice(device);
                        if(usbDeviceConnection==null) {
                            LE(TAG,"usbDeviceConnection = null");
                        }else{
                            fileDescriptor = usbDeviceConnection.getFileDescriptor();
                            LI(TAG,"fileDescriptor="+fileDescriptor);

                            usbManager.requestPermission(device,permissionIntent);
                            LI(TAG,"requestPermission done");

                            NewRunnerable nr= new NewRunnerable();
                            (new Thread(nr)).start();
                        }
                    }
                }
            }
        });

        btnRcv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                btnCon.setEnabled(false);
                btnSnd.setEnabled(false);
                btnRcv.setEnabled(false);
                LI(TAG, "Receive button clicked");

                int r = reader();
                if(r==0) {
                    LI(TAG, "Reader starts successfully");
                }else{
                    LI(TAG, "Reader failed to start, error=" + r);
                }
            }
        });

        btnSnd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                btnCon.setEnabled(false);
                btnSnd.setEnabled(false);
                btnRcv.setEnabled(false);
                LI(TAG, "Send button clicked");

                int r = writer();
                if(r==0) {
                    LI(TAG, "Writer starts successfully");
                }else{
                    LI(TAG, "Writer failed to start, error=" + r);
                }
            }
        });

        isStoragePermissionGranted();
    }

    private final BroadcastReceiver usbReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            LI(TAG, "BroadcastReceiver.onReceive called");

            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        if(device != null){
                            //call method to set up device communication
                            LI(TAG, "permission granted for device ");

                            int r = open(fileDescriptor);
                            if(r==0) {
                                LI(TAG,"Device open successful");

                                btnCon.setEnabled(false);
                                btnSnd.setEnabled(true);
                                btnRcv.setEnabled(true);
                            }else{
                                LE(TAG, "Device open failure, error=" + r);
                            }
                        }else
                            LE(TAG, "device = null");
                    }
                    else {
                        LE(TAG, "permission denied for device " + device);
                    }
                }
            }
        }
    };

    public boolean isStoragePermissionGranted()
    {
        if (Build.VERSION.SDK_INT >= 23)
        {
            if (checkSelfPermission(android.Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED)
            {
                LI(TAG,"Permission is granted");
                return true;
            } else {
                LI(TAG,"Permission is revoked");
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
                return false;
            }
        } else {
            //permission is automatically granted on sdk<23 upon installation
            LI(TAG,"Permission is granted");
            return true;
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults)
    {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if(grantResults[0]== PackageManager.PERMISSION_GRANTED)
        {
            LI(TAG,"Permission: "+permissions[0]+ " was "+grantResults[0]);
            Toast.makeText(this, "File access permission granted.", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(this, "need file access permission.", Toast.LENGTH_LONG).show();
        }
    }
}