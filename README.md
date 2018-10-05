# rfm69-elster
Elster EngergyAxis Receiver using PlatformIO/Arduino with the Adafruit Feather M0 RFM69

## History
I've been using this code to monitor my water meter readings for about a year. I had been sitting on the code, planning to clean it up before publishing to Github, but rather than continuing to procrastinate, I've decided to publish it as-is in the hopes that it might help someone else.

This project came after I had worked on ea_receiver for a while, and decided that it would be better to use a single low powered device than multiple rtlsdr dongles and lots of CPU power. The Adafruit Feather series devices are great, and I've found the RFM69 easy to work with.

Note: In my municipality, the EA_LAN between electric meters is now encrypted. However, the water meter messages (even those relayed by an electric meter) are in the clear.

## Building
- Install PlatformIO (https://docs.platformio.org/en/latest/installation.html)
- Run `platformio run`

Alternatively, you can copy the code into an Arduino sketch.

## Installing
To upload the resulting binary to your device:

`platformio run -t upload`

## Running
Once the binary has been uploaded to your device, it will start running. The messages it receives will be sent on the serial port at 921600 baud. To monitor for messages, you can use PlatformIO:

`platformio device monitor`

Example:

```
$ platformio device monitor

--- Miniterm on /dev/ttyACM0  921600,8,N,1 ---
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
[CH11] 0052098066e893000000001c00003b42ee251500010056030000ffff030500c019e34591f9e5b10382b1d5826814d27ec80a7dbc095d7e53820ce3823baca73b69242724b5774690f972eb9e2baf64ad9c32b2ad (RSSI: -79) CRC OK
```
