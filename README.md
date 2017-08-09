# BT_CapacityMonitor

AR-15 Magazine Capacity Monitoring System (Proof of Concept)
==============

This is a proof of concept for a method to display the number of rounds in an AR-15's magazine on an Android device or Android-Wear device using bluetooth for data transfer from Raspberry Pi to android and using a modified version of Samy Kamkar's (@samyk) magspoof for data transfer between the magazine and raspberry pi.

Check out my video for more information, explanations, and pictures/video.


Flow of information
--------------
The system is composed of several components and boards.
Below is the flow of infomation from the magazine to the user.

<br><br><br><br>
**Magazine: Arduino Pro Mini (or similar) with a Magspoof magnetic Card emulator (magspoof)**
<br><br>
|<br>
|<br>
↓        Data (ultra-low baud rate) is sent wirelessly from the magazine to the card reader via magspoof
 <br>   <br>            
**Magnetic Card Reader connected as a usb keyboard to the Raspberry-pi (or similar)**
<br><br>

↓         Data is sanitized and sent over a bluetooth serial connection from the Raspberry-pi to android-wear or other
<br>    <br>            
                
**Android Wear or other android device**
<br><br>

↓         Data is displayed to user
<br> <br>
  
**Eyeballs (or similar)**

