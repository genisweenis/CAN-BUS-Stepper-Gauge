#include <SPI.h>
#include <Stepper.h>


Stepper stepper(600, 4, 5, 6, 7); //Adafruit Automotive Gauge Stepper Motor has 600 steps. 4, 5, 6, and 7 are the arduino pins going to the driver board's AIN2, AIN1, BIN1, BIN2 pins respectively.
//Stepper stepper2(600, 0, 1, 2, 3);
#define CAN_2515 //choosing the correct IC for the CAN shield

// For Arduino MCP2515 Hat (I used the UNO one from SEEED):
const int SPI_CS_PIN = 9; //don't change these if using the SEEED hat.
const int CAN_INT_PIN = 2;

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin for the MCP Library.
#endif                          

long previousMillis = 0; //used later in a "non-blocking millis loop" (gooogle this for more info)
long interval = 10; //that loop runs every 10 ms

int position = 0; //make a variable for the stepper gauge's current position each time it changes
//int position2 = 0; //UNCOMMENT TO ADD IN ANOTHER STEPPER

void setup() {

    while (CAN_OK != CAN.begin(CAN_250KBPS)) {// init can bus : baudrate = 250k BE SURE TO CHANGE IF YOUR BUS IS RUNNING FASTER / SLOWER!!!
        delay(100); //give it time to wake up
    }
    stepper.setSpeed(60); //RPM the motor will run at
    //stepper2.setSpeed(60); //UNCOMMENT TO ADD IN ANOTHER STEPPER
    stepper.step(-600); //step CCW all of the steps so that it will always be zeroed on startup.
    //stepper2.step(-600); //UNCOMMENT TO ADD IN ANOTHER STEPPER
    position = 0; //initial set of zeroed position
    //position1 = 0; //UNCOMMENT TO ADD IN ANOTHER STEPPER
}

void loop() {
    uint8_t len = 8; //length of the data expected from the CAN bus; 8 is enough for everything Orion can send.
    uint8_t buf[8]; //this is an array of 8 bytes, where every incoming CAN message is stored during its "turn".
    uint16_t SOCa;
    int SOCb;
    //ADD ANOTHER SET OF A UINT16_T AND AN INT HERE IF YOU ARE ADDING ANOTHER MESSAGE / BYTE OF DATA BELOW.

    if (CAN_MSGAVAIL == CAN.checkReceive()) {  //Check if data is coming
        CAN.readMsgBuf(&len, buf); //Read data

        unsigned long canId = CAN.getCanId(); //store the Identifier in an unsigned long (in case it happens to be an extended ID)

        if(canId == 0x6B0){ //CAN message from Orion that contains DC bus current and State Of Charge data; I think 0x6B0 is the default
         SOCa = (uint16_t) (buf[4]); //Orion sends SOC in Byte 4 of the 6B0 message.
         SOCb = SOCa * 3; //scaling the float to show correctly on the gauge. 100% will be gauge position 600. 0% gauge position 0. It comes from the BMS by default multiplied by 2 for some reason.
        }
        //ADD ANOTHER "IF" HERE, THE SAME AS ABOVE, BUT WITH THE ID, BYTE(S), AND SCALING ADJUSTED APPROPRIATELY FOR THE MESSAGE YOU WANT TO DISPLAY DATA FROM.
    }
   unsigned long currentMillis = millis();
   if(currentMillis - previousMillis > interval) { //how I trigger this non-blocking loop that I want to be happening every 10 ms. without stopping the above loop from executing.
   //this is where you'd have your arduino send the data via spi or whatever, to your Raspberry Pi.

    if((SOCa != 0)){ //this is a primitive "filter" for the all-zero data frames that seem to be coming in. Should work long term, just ugly
       previousMillis = currentMillis;  
       if(SOCb > position){
         stepper.step(1);
         position = position + 1;
       }
       if(SOCb < position){
         stepper.step(-1);
         position = position - 1;
       }
    }
  }
} 