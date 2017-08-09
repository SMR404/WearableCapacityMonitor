# BT_CapacityMonitor

AR-15 Magazine Capacity Monitoring System (Proof of Concept)
==============

This is a proof of concept for a method to display the number of rounds in an AR-15's magazine on an Android device or Android-Wear device using bluetooth for data transfer from Raspberry Pi to android and using a modified version of Samy Kamkar's (@samyk) magspoof for data transfer between the magazine and raspberry pi.

**Check out my video for more information, explanations, and pictures/video.**


*Flow of information*
--------------
The system is composed of several components and boards.
Below is the flow of infomation from the magazine to the user.

<br><br><br><br>
- **Component: Magazine: Arduino Pro Mini (or similar) with a Magspoof magnetic Card emulator (magspoof)**
<br>

|<br>
|<br>
|      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Data (ultra-low baud rate) is sent wirelessly from the magazine to the card reader via magspoof <br>
|<br>
|<br>
▼      

<br>

- **Component: Magnetic Card Reader connected as a usb keyboard to the Raspberry-pi (or similar)**

<br>

|<br>
|<br>
|    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     Data is sanitized and sent over a bluetooth serial connection from the Raspberry-pi to android-wear or other
<br>
|<br>
|<br>
▼     
<br>

- **Component: Android Wear or other android device (or similar)**
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|          Data is displayed to user
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

▼ <br>
<br> 
  
- **Component: Eyeballs (or similar /s)**

