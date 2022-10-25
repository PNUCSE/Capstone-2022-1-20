/*
  Reading multiple RFID tags, simultaneously!
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 3rd, 2016
  https://github.com/sparkfun/Simultaneous_RFID_Tag_Reader

  Constantly reads and outputs any tags heard

  If using the Simultaneous RFID Tag Reader (SRTR) shield, make sure the serial slide
  switch is in the 'SW-UART' position
*/

#include <SoftwareSerial.h> //Used for transmitting to the device

SoftwareSerial softSerial(2, 3); //RX, TX

#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module
RFID nano; //Create instance

// Bluetooth setup port and baudrate
#define BT_TXD (7) // Bluetooth RX is 7
#define BT_RXD (8) // Bluetooth TX is 8
#define BAUDRATE 9600
SoftwareSerial BTSerial(BT_RXD, BT_TXD); // RX TX

// Limit reading duplicate key value in timeout period.
#define LIMIT_TIME 5000 // 5 second
unsigned long curTime, preTime;

// preKey container
String preKey = "";

void setup()
{
  Serial.begin(115200);
  while (!Serial); //Wait for the serial port to come online
  BTSerial.begin(BAUDRATE); //Bluetooth begin

  if (setupNano(38400) == false) //Configure nano to run at 38400bps
  {
    Serial.println(F("Module failed to respond. Please check wiring."));
    while (1); //Freeze!
  }

  nano.setRegion(REGION_KOREA); //Set to Korea

  nano.setReadPower(2000); //5.00 dBm. Higher values may caues USB port to brown out
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling
  //Over 20.00 dBm didn't operate arduino, i don't know why occur this problem.

  preTime = millis();
  curTime = millis();
  nano.startReading(); //Begin scanning for tags
}

void loop()
{
  if (nano.check() == true) //Check to see if any new data has come in from module
  {
    byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp

    if (responseType == RESPONSE_IS_KEEPALIVE)
    {
      Serial.println(F("Scanning"));
    }
    else if (responseType == RESPONSE_IS_TAGFOUND)
    {
      //If we have a full record we can pull out the fun bits
      int rssi = nano.getTagRSSI(); //Get the RSSI for this tag read
      long freq = nano.getTagFreq(); //Get the frequency this tag was detected at
      long timeStamp = nano.getTagTimestamp(); //Get the time this was read, (ms) since last keep-alive message
      byte tagEPCBytes = nano.getTagEPCBytes(); //Get the number of bytes of EPC from response

      // Convert nano.msg value to string
      String epcID = "";
      for (byte x = 0; x < tagEPCBytes; x++) {
        epcID += String(nano.msg[31 + x]);
        epcID += ",";
      }

      // Remove last char ",";
      if (epcID.length() > 0) {
        int idx = epcID.length() - 1;
        epcID.remove(idx);
      }

      if (epcID.length() > 0 && preKey == epcID) { // reading the same Key doesn't send key to bluetooth
        // after 5 sec 
        curTime = millis();
        if (curTime - preTime >= LIMIT_TIME) {
          Serial.println("Clear");
          BTSerial.print("Clear\n");
          preTime = curTime;
          preKey = "";
        }
      } else {
        // Update PreString (Pre key value) and Send key to bluetooth
        BTSerial.print(epcID);
        preKey = epcID;
      }
    }
    else if (responseType == ERROR_CORRUPT_RESPONSE)
    {
      Serial.println("Bad CRC");
    }
    else
    {
      //Unknown response
      Serial.print("Unknown error");
    }
  }
}

//Gracefully handles a reader that is already configured and already reading continuously
//Because Stream does not have a .begin() we have to do this outside the library
boolean setupNano(long baudRate)
{
  nano.begin(softSerial); //Tell the library to communicate over software serial port

  //Test to see if we are already connected to a module
  //This would be the case if the Arduino has been reprogrammed and the module has stayed powered
  softSerial.begin(baudRate); //For this test, assume module is already at our desired baud rate
  while (softSerial.isListening() == false); //Wait for port to open

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while (softSerial.available()) softSerial.read();

  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
  {
    //This happens if the baud rate is correct but the module is doing a ccontinuous read
    nano.stopReading();

    Serial.println(F("Module continuously reading. Asking it to stop..."));

    delay(1500);
  }
  else
  {
    //The module did not respond so assume it's just been powered on and communicating at 115200bps
    softSerial.begin(115200); //Start software serial at 115200

    nano.setBaud(baudRate); //Tell the module to go to the chosen baud rate. Ignore the response msg

    softSerial.begin(baudRate); //Start the software serial port, this time at user's chosen baud rate

    delay(250);
  }

  //Test the connection
  nano.getVersion();
  if (nano.msg[0] != ALL_GOOD) return (false); //Something is not right

  //The M6E has these settings no matter what
  nano.setTagProtocol(0x07); //Set protocol to GEN2

  nano.setAntennaPort(); //Set TX/RX antenna ports to 1

  return (true); //We are ready to rock
}
