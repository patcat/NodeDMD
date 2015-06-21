#include <SoftwareSerial.h>
#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 16
#define BUFLENGTH 512
#define DEBUG true

/*--------------------------------------------------------------------------------------
  DMD Includes
--------------------------------------------------------------------------------------*/
#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}


/*--------------------------------------------------------------------------------------
  SerialDMD
--------------------------------------------------------------------------------------*/
char buf[BUFLENGTH]; // character buffer for json message processing
int bufCount; // counter for the string buffer.

SoftwareSerial mySerial(2, 3); // RX, TX
byte note = 0; //The MIDI note value to be played
byte resetMIDI = 4; //Tied to VS1053 Reset line
byte ledPin = 13; //MIDI traffic inidicator
int instrument = 0;

void setup() {
  Serial.begin(57600);
  mySerial.begin(31250);

  //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
  Timer1.initialize(5000);           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt(ScanDMD);   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

  //clear/init the DMD pixels held in RAM
  dmd.clearScreen(true);   //true is normal (all pixels off), false is negative (all pixels on)
}

void loop() {
  if (Serial.available() > 0) {
    SerialParse();
  }
  
  delay(1);
}


void SerialParse(void) {
  bufCount = -1; // reset it
  bufCount = Serial.readBytesUntil('\n', buf, BUFLENGTH);

  if (bufCount > 0) {
    String message = String(buf);
    
    parse_message(message);
  }
}


void parse_message(String& message) {
  dmd.clearScreen(true);

  // Turn our string into an array of characters
  message.toCharArray(buf, BUFLENGTH);
  
  // Iterate over each char in the array
  char *p = buf;
  char *str;

  int i = 0;

  for (byte y = 0; y < SCREEN_HEIGHT; y++) {
    for (byte x = 0; x < SCREEN_WIDTH; x++) {
      if ((char)p[i] == '1') {
        dmd.drawFilledBox(x, y, x, y, GRAPHICS_NORMAL);
      }
      i++;
    }
  }
}

