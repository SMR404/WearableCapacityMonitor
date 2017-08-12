
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
