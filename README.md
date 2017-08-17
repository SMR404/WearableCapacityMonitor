# BT_CapacityMonitor

# AR-15 Magazine Capacity Monitoring System (Proof of Concept)


This is a proof of concept for a method to display the number of rounds in an AR-15's magazine on an Android device or Android-Wear device using bluetooth for data transfer from Raspberry Pi to android and using a modified version of Samy Kamkar's (@samyk) magspoof for data transfer between the magazine and raspberry pi. Credit to @miaoski for his conversion of the magspoof's ATtiny85's code to arduino.

**Check out my video for more information, explanations, and pictures/video.**

General Overview of project + Demonstration: https://vid.me/MMtok

## *Flow of information*

The system is composed of several components and boards.
Below is the flow of infomation from the magazine to the user.

- **Component: Magazine: Arduino Pro Mini (or similar) with a Magspoof magnetic Card emulator (magspoof)**


 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   *Data (ultra-low baud rate) is sent wirelessly from the magazine to the card reader via magspoof* <br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;▼      

- **Component: Magnetic Card Reader connected as a usb keyboard to the Raspberry-pi (or similar)**

 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     *Data is sent over bluetooth serial connection from the Raspberry-pi to android*
 <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;▼   

- **Component: Android Wear or other android device (or similar)**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *Data is displayed to user*
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;▼ <br>

- **Component: Eyeballs (or similar /s)**



## The info from this point on is copied from the README.md files in each immediate subfolder in this project. Navigate to those subfolders to view each topic's information by itself.


# Topic: Android
I ran out of time to really make a competent application for android (First time android developer, here).
The current android-wear program has only been tested on my Moto 360 sport and has no menu to select the Bluetooth address of the raspberry pi
to which it connects.
This means that the mac address of the pi is hardcoded (gasp) into the program and **you need to change it and then build the .apk yourself**. This is pretty much a vital feature that is missing from the app.

Ideas for improvement (on android-wear):
----------------------------------
- 1. The ability to swipe between several screens which would contain some of the below features
- 2. Bluetooth Address selection screen
- 3. Day/Night mode (on an options screen)
- 4. Power down option for rapsberry pi on options screen (requires alteration to the pi's python program)
- 5. Move the connect/disconnect button to it's own screen and auto-navigate to the capacity value display screen on connection

Debugging over Bluetooth
----------------------
If you don't know how to upload an app to an android wear device without a usb port (like the moto 360 sport), you should get familiar with bluetooth debugging.

https://medium.com/@jcdelvalle/android-wear-bluetooth-debugging-quickly-guide-ef279b84169c

Follow this guide and you should be able to upload the built apk to the watch.

**Important Note:** If you can't get adb to recognize your phone, make sure that you have connected your phone as a camera device. If that doesn't work, then try all the other types of devices it can connect as.

# Topic: Arduino
Upload the sketch to your arduino (pro mini). 
You can use @miaoski's code for debugging and making sure your
magspoof circuit works (just be aware that the digital pins used his code are different numbers).

https://github.com/miaoski/magspoof


Use the softpot test file to calibrate your magazine by putting an increasing number of rounds into the magazine
and recording the analog readings and moving them into the 1d array in the arduino code.

Reference the diagram in this repo or in @miaoski's magspoof repo to make sure that your circuit is right.
I used 24 AWG wire to make the coil for data transfer.


# Topic: Raspberry Pi BT Communication Scripts
How the communication script is setup
----------------------------------------
The communication between the android device and the raspberry-pi is enabled
through a bash script that calls two python programs through a virtual terminal. 
This script launches on the start of the desktop environment.

The desktop enviromnent seems necessary because if the bash script is called (through cron or rc.local) if
the pi has booted to command line, the magentic card reader (which is a usb keyboard)
has no interactive window to output to (input() and raw_input() throw an EOF error).

This issue could possibly be solved if the two python programs were consolidated
into one program by using multiprocessing, and then monitoring usb input at a lower level (maybe using evdev or something similar?) allowing the pi script to function properly
in CLI.

Preparing the Pi
--------------------------
Make sure you are fully updated.
Make sure that the proper bluetooth libraries are installed. Follow the first part of this guide I found to make sure you have the right stuff installed. 

   `sudo apt-get dist-upgrade` <br>
   `sudo apt-get update`<br>
   `sudo apt-get upgrade`<br>
   `sudo apt-get install bluez pi-bluetooth python-bluez`<br>


How to get the script to run on boot
-------------------------------------
1. Make sure that the android device and the pi have been paired and trusted with one another (use Raspbian GUI or bluetooth-ctl).
   <br>
   *This is a permanent action (meaning it survives across reboots) that makes it so that the pi and the android device can open a serial    connection without issues.*

2. Move the magCapacityScripts folder (from github) to the /pi/home/ directory
   - run **sudo chmod +x bluetoothCommunicationScript.sh** while in the same directory as it

3. Move launch.sh (from github) to the /pi/home/ directory. 
   - run **sudo chmod +x launch.sh** while in the same directory as it
   
4. Edit the LXDE autostart file so that launch.sh runs on boot:
   
   - add these lines
   
   **cd /home/pi**
    <br>
    **sudo ./launch.sh**
    <br>
    
    - run **sudo chmod +x launch.sh** while in the same directory as it (should not be necessary, only a redundant precaution)


In this setup, the communication script is run on the start of the desktop gui.

**Note:** For some reason, without launch.sh as an intermediary shell script, autostart does not function properly (as in, bluetoothCommunicationScript.sh is called directly from autostart). In the configuration without launch.sh, autostart functions fine if called by the user, but not if it is run on boot. Not sure why.

**Important Info:** If a modified magazine has been placed into the rifle when the pi is powered, the magstripe reader attempts to send data to the pi's usb port as it is booting. The SHIFT key is pressed by the magstripe reader when it sends it's character string through the usb port. Incidentally, the key to boot to recovery is the SHIFT key. This means that, if the 'boot to recovery by pressing shift' option is enabled, the pi will boot to recovery, probably without you noticing it if you don't have it connected to a monitor. The scripts will not run in this scenario. This issue could likely be fixed by removing the option to boot to recovery, which as far as I am aware, is only something that can be done during the raspbian installation, but I have not looked into it.

# Topic: Solid Modeling
Solid Modeling Files
----------------
Note that this assembly simply represents a proof of concept and has not been designed to fulfill any practical purpose.

It seems that the Raspberry Pi model (and only that model) does not want to load properly when the STEP or IGES file is imported. If you want to place it into the assembly again, I have a zip of that model here. I got it from https://grabcad.com/library/raspberry-pi-3-6

I had to modify the lower receiver to fit the Qi Wireless Charging Ciruit and the Magnetic card reader in a semi-acceptable fashion.
In this assembly, the lower receiver is a modified (read: has sections manually and physically cut away with a power tool) version of the New Frontier LW-15 Polymer Lower Receiver (https://newfrontierarmory.com/shop/lw-15-complete-polymer-lower-receiver/). 

**ONE REALLY IMPORTANT NOTE:**
-----------------------------
I chose this receiver because polymer is easier to cut and because it was a relatively cheap receiver, and **ABOVE ALL**, because *the serial number is located away from where the modifcations take place, and none of the 3d Printed attached parts obfuscate it.*

The serial number is near the fire selector lever, and is not on the exterior of the magazine well like you will find on many other AR-15 lower receivers.

It's a really big deal that you don't alter or cover up the serial number in any way at all if you have bought a lower receiver that has such a serial number associated with it. If you have completed an 80% complete polymer lower yourself, then it's not a problem since there is no serial number assigned to it.

This is the responsibility of you as the person who is modifying their rifle, not mine. Fair warning.

# Topic: Diagrams

Hopefully these diagrams, coupled with the videos and photos, are descriptive enough.

## Items for purchase:
- L293 Motor Driver https://www.amazon.com/NDRTJM-MSTL2930-L293D-Stepper-Driver/dp/B008XCGLNM

<br>

- Raspberry Pi 3 or Raspberry Pi Zero W

<br>

- Arduino Pro Mini or other Arduino

<br>

- Android Wear device or other Android Device (with bluetooth)

<br>

- Polymer Lower Receiver (like a New Frontier LW-15, https://newfrontierarmory.com/shop/lw-15-complete-polymer-lower-receiver/)

<br>

- Qi Wireless Charging Transmitter https://www.adafruit.com/product/2162

<br>

- Qi Wireless Charging Receiver https://www.adafruit.com/product/1901

<br>

- Magnetic Card Reader http://a.co/55RirJM

<br>

- 10 kΩ, 1 kΩ and 1Ω resistors

<br>
 
 - Battery Pack with 2 ports, both of which supplying at least 1 amp each at 5v (2 amps at 5v is preferred for the port powering the Qi wireless Charger Transmitter.
 
 <br>
 
  - BreadBoard or permaProto board (preferred)
 
 <br>
 
 - Jumper Cables or other small wires
 
 <br>
 
 - Magnet Wire (24 AWG is what I used)
 
 <br>
 
 - Bolts or Machine screws of approximate outer thread diameter of 0.3 mm (I don't know what I used, they were found leftovers)
 
 <br>
 
 - 3D printer and filament
 
  <br>
  <br>
  <br>
  
 These are just the items that are attached to/used with the gun. There are lots of other logistical items like arduino programmer chips and other things that you will need for this, but they are intuitive and are closely associated with the use of the above items.
 
