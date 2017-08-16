package com.example.smr.capacityapplicationsmr;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;


public class MainActivity extends Activity {
    //    private final String DEVICE_NAME="MyBTBee";

    private final String DEVICE_ADDRESS = "00:00:00:00:00:00"; //unfortunately you have to change this string(to that of the bluetooth address of the pi)
    private final UUID PORT_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");//Serial Port Service ID, should be able to leave this alone
    private BluetoothDevice device;
    private BluetoothSocket socket;
    private OutputStream outputStream;
    private InputStream inputStream;
    Button connectButton, disconnectButton, powerDownButton;

    TextView textViewCapacityValue, textViewConnectionStatus, staticTextCapacityInfo;


    EditText editText;
    boolean deviceConnected = false;
    byte buffer[];
    boolean stopThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        connectButton = (Button) findViewById(R.id.buttonStart);
        disconnectButton = (Button) findViewById(R.id.buttonStop);
        textViewCapacityValue = (TextView) findViewById(R.id.textViewCapacityValue);
        textViewConnectionStatus = (TextView) findViewById(R.id.textViewConnectionStatus);
        staticTextCapacityInfo = (TextView) findViewById(R.id.textCapacityInfo);

        setUiEnabled(false);

    }


    public void setUiEnabled(boolean bool) {
        connectButton.setEnabled(!bool);

        disconnectButton.setEnabled(bool);
        staticTextCapacityInfo.setEnabled(bool);

        textViewCapacityValue.setEnabled(bool);

        textViewConnectionStatus.setEnabled(bool);
        textViewConnectionStatus.setText("");
        textViewConnectionStatus.setTextColor(Color.RED);
        textViewConnectionStatus.append(" No");

        powerDownButton.setEnabled(bool);


    }

    public boolean BTinit() {
        boolean found = false;
        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            Toast.makeText(getApplicationContext(), "Device doesnt Support Bluetooth", Toast.LENGTH_SHORT).show();
        }
        if (!bluetoothAdapter.isEnabled()) {

            Intent enableAdapter = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableAdapter, 0);
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        Set<BluetoothDevice> bondedDevices = bluetoothAdapter.getBondedDevices();
        if (bondedDevices.isEmpty()) {
            Toast.makeText(getApplicationContext(), "Please Pair the Device first", Toast.LENGTH_SHORT).show();
        } else {
            for (BluetoothDevice iterator : bondedDevices) {
                if (iterator.getAddress().equals(DEVICE_ADDRESS)) {
                    device = iterator;
                    found = true;
                    break;
                }
            }
        }

        return found;
    }

    public boolean BTconnect() {
        boolean connected = true;
        try {
            socket = device.createRfcommSocketToServiceRecord(PORT_UUID);
            socket.connect();
        } catch (IOException e) {
            e.printStackTrace();
            connected = false;
        }
        if (connected) {
            try {
                outputStream = socket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                inputStream = socket.getInputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }

        }

        return connected;
    }


    public void onClickStart(View view) {
        if (BTinit()) {
            if (BTconnect()) {
                setUiEnabled(true);
                deviceConnected = true;
                beginListenForData();
                Toast.makeText(getApplicationContext(), "Connection Opened!", Toast.LENGTH_SHORT).show();
                textViewConnectionStatus.setText("");
                textViewConnectionStatus.setTextColor(Color.GREEN);
                textViewConnectionStatus.append(" Yes");
                textViewCapacityValue.setText("");

            }

        }
    }


    void beginListenForData() {
        final Handler handler = new Handler();
        stopThread = false;
        buffer = new byte[1024];

        Thread thread = new Thread(new Runnable() {
            public void run() {
                int noMessage = 0;
                while (!Thread.currentThread().isInterrupted() && !stopThread) {

                    if (noMessage <= 150) {
                        noMessage = ++noMessage;
                    }

                    try {
                        int byteCount = inputStream.available();
                        if (byteCount > 0) {
                            noMessage = 0;
                            byte[] rawBytes = new byte[byteCount];
                            inputStream.read(rawBytes);             //get data from server

                            try {                                   //sent the data right back to confirm receipt
                                outputStream.write(rawBytes);
                            } catch (IOException e) {
                                e.printStackTrace();
                                Toast.makeText(getApplicationContext(), "IO error", Toast.LENGTH_SHORT).show();

                            }


                            final String string = new String(rawBytes, "UTF-8");
                            handler.post(new Runnable() {
                                public void run() {

                                    //textView.append(string);

                                    if (new String(string).equals("\nNo Capacity Data Received"))    //this means that there was no capacity read by server
                                    {
                                        textViewCapacityValue.setText("");
                                        textViewCapacityValue.append("\nNo Info");
                                    } else {
                                        textViewCapacityValue.setText("");
                                        textViewCapacityValue.append(string);
                                    }

                                }
                            });

                        }
                    } catch (IOException ex) {
                        stopThread = true;
                    }


                    if (noMessage == 150) {
                        handler.post(new Runnable() {
                            public void run() {
                                disconnectButton.performClick();
                                stopThread = true;

                                return;

                            }
                        });
                    } else {
                        SystemClock.sleep(10); //pause for 1/100 second
                    }
                }
            }
        });

        thread.start();
    }

    public void onClickSend(View view) {
        String string = editText.getText().toString();
        string.concat("\n");
        try {
            outputStream.write(string.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void onClickStop(View view) throws IOException {


        try {                                   //tell the server that we're closing connection
            outputStream.write("Closing Connection".getBytes());
        } catch (IOException e) {
            e.printStackTrace();
            Toast.makeText(getApplicationContext(), "IO error", Toast.LENGTH_SHORT).show();

        }
        stopThread = true;
        outputStream.close();
        inputStream.close();
        socket.close();
        setUiEnabled(false);
        deviceConnected = false;
        textViewConnectionStatus.setText("");
        textViewConnectionStatus.setTextColor(Color.RED);
        textViewConnectionStatus.append(" No");
        Toast.makeText(getApplicationContext(), "Connection Closed!", Toast.LENGTH_SHORT).show();
    }
}
