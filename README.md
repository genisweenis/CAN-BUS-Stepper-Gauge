# CAN-BUS-Stepper-Gauge
Controlling Adafruit stepper motor automotive gauges using an arduino, CAN shield, and adafruit stepper motor drivers.
This is all the information you should need to display CAN-bus data on 1 or more of these adafruit gauges https://www.adafruit.com/product/2424 .

![IMG_20240919_140325566_HDR](https://github.com/user-attachments/assets/73b66a32-cdda-4a28-8dad-419e28657caa)


Shopping list:

Adafruit automotive gauge stepper motor (x1 or 2) https://www.adafruit.com/product/2424

Adafruit stepper motor driver breakout board (x1 or 2): https://www.adafruit.com/product/2448

Arduino Uno R3 (x1. this is the only Arduino I have tested this code on)

USB-CAN V8.00 (x1): https://www.seeedstudio.com/USB-CAN-Analyzer-p-2888.html?srsltid=AfmBOorTRwHyM56Y-_f6Wj41nrf5IJagPchUQ1w5svigmSBI2gMsgNLiS6Y

Solderless breadboard (x1): https://www.adafruit.com/product/64 or similar

Jumper wires

SEEED Studios CAN-bus shield V2.0 (x1): https://www.seeedstudio.com/CAN-BUS-Shield-V2.html?srsltid=AfmBOop6YOp9PTkRAy3E6Ld7hsMVZV870k-GRIPxLgrNTdYjfJAHpglQ

A power supply capable of 5V output

Arduino UNO programming cable


Software / Libraries:

USB-CAN V8.00 port driver and software: https://github.com/SeeedDocument/USB-CAN-Analyzer (downloads for both are in here)

Arduino IDE: https://www.arduino.cc/en/software

Once Arduino IDE is open, install "Stepper.h" library

Also install "SPI.h" in case you need to use it for troubleshooting


Wiring:

![IMG_20240919_134455409](https://github.com/user-attachments/assets/3fb9342c-3325-4330-a01d-5a4191fff1aa)

Please excuse the messy schematic. I will add a better one soon!


Coding: 
Download the Arduino sketch from this repository and open in in your Arduino IDE. It is thoroughly commented, and I tried to condense it minimally so you can tell what's going on. This sketch uses the example of an Orion BMS 2's broadcast of a battery state-of-charge (SOC) percent.

After you have made sure the system works using my code, please find the CAN .db file for the device whose data you are hoping to display with your gauge. You will need to find the message ID, what byte(s) the data to be displayed are sent in, and whether there is a scalar you must adjust for in your code.


Testing:

Upload the code to your Uno. Unplug the programming cable.

Plug the USB-CAN V8.00 into a serial port and run the USB-CAN software
. 
![usbcan](https://github.com/user-attachments/assets/c911e63f-1156-435f-ada7-694827c0ba28)
Choose the correct com port.

Choose "normal mode", "standard frame", and "250kbps".

Click "open".

Click "add" to the right of the big "more frames to send" window, then in the message template that appears, type in the ID 6B0. 

Type some random data (make sure to put in real HEX values). 

Check "sequence" ("cycle" doesn't need to be checked).

Turn on your power supply -AFTER SETTING IT TO 5 VOLTS WHILE DISCONNECTED-

Then back in the USB-CAN software, go back over to the left of the window and click "set and start".

Type different stuff into byte 4 of the message you constructed and watch the stepper needle move!

Notes:

If you want to use more than 1 stepper gauge, you need more than 1 stepper motor driver breakout board. You also need to hook "VM" (motor voltage) directly up to the power supply, otherwise you may brown out your arduino or SEEED CAN shield!!!

If your CAN wires between the SEEED shield and the USB-CAN V8.00 are longer than a few inches, please twist CANH and CANL tightly together to prevent noise from your power supply garbling the messages or knocking the bus offline. CAN is a delicate signal until the proper physical measures are taken to protect it.

If you need your gauge to respond faster, like for a tachometer, change these 3 things: 

1. find the variable "interval" in the Arduino sketch and decrease it.
2. find the function "stepper.setSpeed" in the Arduino sketch and increase it.
3. in the USB-CAN V8.00 software, decrease "Interval (ms)" (top of the "more messages to send" window).
4. Keep adjusting these things until the gauge response is as fast as you require.
