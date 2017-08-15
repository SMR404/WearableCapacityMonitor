
How the communication script is setup
----------------------------------------
The communication between the android device and the raspberry-pi is enabled
through a bash script that calls two python programs through a virtual terminal. 
This script launches on the start of the desktop environment.

The desktop enviromnent seems necessary because if the bash script is called (through cron or rc.local) if
the pi has booted to command line, the magentic card reader (which is a usb keyboard)
has no interactive window to output to (input() and raw_input() throw an EOF error).

This issue could possibly be solved if the two python programs were consolidated
into one program by using multiprocessing, allowing the pi script to function properly
in CLI.


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
