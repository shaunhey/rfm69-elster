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

## Hardware
A 3" piece of wire soldered to the antenna pin works great on this board.

**Important Note** this code makes use of an interrupt handler from the rfm69 to the m0 to indicate when a message has been received.  This isn't wired up by default on the board, so you need to solder a jumper wire between A1 and IO1

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

## Message Format
### Example:
This example is a water meter reading from meter ID "028-0006326883" (Network ID = 1C, LAN ID = 608A63), which was relayed by meter ID "028-0012589224" (C018A8) to the hub, which is meter ID "028-0006744211" (66E893).
```
2018-10-05 05:12:22.145984 [CH15] 004c0100c018a88066e8931c280000005600000040000700330827000001020000000000011200608a63016200141d000000cebc0300040000020500000000080501000503010000000000008883 (RSSI: -74) CRC OK
                                  AAAA  BBBBBBBBCCCCCCCCDD                                                    EEEEEEEE            FFFFFFFFGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGHHHH

A - Message Length
B - Source LAN ID
C - Destination LAN ID
D - Network ID
E - Original Source LAN ID
F - Current Water Meter Reading Value (in 10's of gallons)
G - Water Usage by Hour (Each byte represents the usage for that hour, in 10's of gallons)
H - CRC

Current value: 52924

Usage history:
Date                 Usage (gallons)
2018-10-05 04:00:00: 0
2018-10-05 03:00:00: 0
2018-10-05 02:00:00: 0
2018-10-05 01:00:00: 0
2018-10-05 00:00:00: 0
2018-10-04 23:00:00: 0
2018-10-04 22:00:00: 10
2018-10-04 21:00:00: 30
2018-10-04 20:00:00: 50
2018-10-04 19:00:00: 0
2018-10-04 18:00:00: 10
2018-10-04 17:00:00: 50
2018-10-04 16:00:00: 80
2018-10-04 15:00:00: 0
2018-10-04 14:00:00: 0
2018-10-04 13:00:00: 0
2018-10-04 12:00:00: 0
2018-10-04 11:00:00: 50
2018-10-04 10:00:00: 20
2018-10-04 09:00:00: 0
2018-10-04 08:00:00: 0
2018-10-04 07:00:00: 40
2018-10-04 06:00:00: 0
2018-10-04 05:00:00: 30
```
