This repo has the source code for an android-wear and regular android device app. It's more convenient to test and debug from a regular android device app in my experience.
<br>
<br>
I ran out of time to really make a competent application for android (First time android developer, here).
The current android-wear program has only been tested on my Moto 360 sport and has no menu to select the Bluetooth address of the raspberry pi
to which it connects.
This means that the bluetooth address of the pi is hardcoded (gasp) into the program and **you need to change it and then build the .apk yourself**. This is pretty much a vital feature that is missing from the app.

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
