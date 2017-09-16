#### Step 1.
Download [Raspbian Jessie with Pixel](https://www.raspberrypi.org/downloads/raspbian/), the most current operating system for the Pi.  If the opening the zip file on your Mac results in cpgz file rather than an img file then try opening in a third party unarchiving app like [this one](https://itunes.apple.com/us/app/the-unarchiver/id425424353?mt=12).

#### Step 2.
Copy the OS to an SD card.  If you're doing this on a Mac I recommend using the [Pi Filler](http://ivanx.com/raspberrypi/) app for this.

#### Step 3.  
Boot your Pi.  Log in as user:```pi```, pass:```raspberry```.  If you wanna put your Pi on CMU WiFi you'll need to register it at [https://netreg.net.cmu.edu](https://netreg.net.cmu.edu).  Register a new machine on the Legacy Wireless Network (the one that shows up as "CMU").  The HWAddr can be found be entering the following command in the terminal:

    ifconfig

Look for the wlan0 section and copy the HWaddr (it will look something like b8:27:eb:3e:cf:b9)

It will take 20 to 45 minutes for you machine to be registered and able to connect to the net on the CMU network.

#### Step 4.  
Configurate the Pi:

You may wish to configure your keyboard in Preferences/keyboard.   You may also wish to change your password using Preferences/Raspi configuration (I'm using ```3.14ispi```).  Enable SSH on the Pi under Preferences/RaspberryPi configuration.

#### Step 5.
Install PD vanilla using the terminal:

```sudo apt-get update```
```sudo apt-get install puredata```

#### Step 6.  
Install TightVNCserver:

```sudo apt-get install tightvncserver```

#### Step 7.
To copy files to your Pi from a Mac using an ethernet cable set up both machine with static IP addresses [here](https://www.modmypi.com/blog/how-to-give-your-raspberry-pi-a-static-ip-address-update)'s how to do it on a Pi. Give every Pi a unique IP address.

 Then you can SSH in from mac terminal: ```ssh pi@10.0.0.2``` where you can start vncserver ```vncserver``` which will allow you to screenshare (on the Mac this is done by "connect to server" [Apple K] and entering vnc://10.0.0.2:5901).  

 To copy files from the Mac to the Pi in mac terminal: scp -r /Users/jessestiles/Documents/Github/CHIPD pi@10.0.0.2:Documents

#### Step 8.  
Set up PD preferences to add necessary directories.

#### Step 9.  
Make a script to open your pd patch with all the right configs.

1) create an executable script that opens PD:

2) Create a /bin folder in your home/pi folder by typing this :

```mkdir ./bin``` (hit enter)

3) Move to this folder by typing :

```cd ./bin```

4) Create a file in this folder named “script_pd” (or whatever you want to call this) :

```sudo nano script_pd``` (this opens a blank text file in the nano editor)

5) In the nano editor, type this script :

```
#!/bin/bash
# This script will start Pd
echo "Starting Pd..."
pd -rt -r 44100 -audiodev 5 -mididev 1 -inchannels 1 -outchannels 2 /home/pi/Documents/PD/launch.pd &
```

6) Save your script by pressing Ctrl+X, hitting y, and Enter.
7) This script is still nothing but a text file. We need to make it executable by typing this :

```sudo chmod 755 script_pd```

Step 10.  Make pi run the script on startup: add the executable script to /etc/profile

Once your Pi is all set up you can copy the entire machine using [Pi Copier](http://ivanx.com/raspberrypi/) and duplicate it onto new SD cards using Pi Filler.

Step 11.  Making the Pi join automatically join a network that you create.

```sudo nano /etc/network/interfaces```

Add new first line:

```auto wlan0```

then do ```sudo nano /etc/wpa_supplicant/wpa_supplicant.conf```

and add the following:

    network={
    ssid="YOUR_NETWORK_NAME"
    psk="YOUR_NETWORK_PASSWORD"
    proto=RSN
    key_mgmt=WPA-PSK
    pairwise=CCMP
    auth_alg=OPEN
    }
