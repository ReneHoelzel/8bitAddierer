#include <LedControl.h>
#include <SevSegShift.h>

// Source: https://www.woolseyworkshop.com/2021/02/18/adding-digital-io-to-your-arduino-part-2-the-74hc165/
const unsigned long SamplePeriod = 250;  // Sampling period in milliseconds

const int dOffPin = A3;  // Used to show or hide the the final byte value (3 segment display)

// Variables for the 74HC165 that reads the 8 switches
const int dataPin1 = 2;   // Connected to 74HC165 QH    (pin 9)
const int clockPin1 = 3;  // Connected to 74HC165 CLK   (pin 2)
const int latchPin1 = 4;  // Connected to 74HC165 SH/LD (pin 1)

// Variables for the MAX7219 to visualize 0 or 1 (8x 7Segment)
const int latchPin2 = 5;  // Connected to MAX7219 DIN   (pin 12)
const int clockPin2 = 6;  // Connected to MAX7219 CLK  (pin 13)
const int dataPin2 = 7;   // Connected to MAX7219 LOAD (pin 1)

// Initialize Led control library
// http://wayoda.github.io/LedControl/pages/software
LedControl lc = LedControl(dataPin2, clockPin2, latchPin2, 1);

// Variables for the 3 digit 7 segment display
const int dataPin4 = 11;   // Connected to 74HC595 SER  (pin 14)
const int latchPin4 = 12;  // Connected to 74HC595 RCLK (pin 12)
const int clockPin4 = 13;  // Connected to 74HC595 SRLCK (pin 11)

// Instantiate a seven segment controller object
/* 1 = number of shift registers used
   true = Digits are connected to Arduino directly default value = false (see SevSegShift example)
*/
SevSegShift sevsegshift(dataPin4, clockPin4, latchPin4, 1, true);

int zeroOneSegments;
int zeroOneSegmentsOld;

void setup() {
  // Serial Monitor
  //Serial.begin(115200);  // initialize serial bus
  //while (!Serial) ;  // wait for serial connection

  // Button to show/hide final byte value
  pinMode(dOffPin, INPUT);

  // 74HC165 shift register (8x buttons/switches)
  pinMode(dataPin1, INPUT);
  pinMode(latchPin1, OUTPUT);
  pinMode(clockPin1, OUTPUT);

  /* The MAX72XX is in power-saving mode on startup, we have to do a wakeup call */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values (max = 15) */
  lc.setIntensity(0, 3);
  /* Clear the display */
  lc.clearDisplay(0);

  setupSevSeg();
}

void setupSevSeg() {
  byte numDigits = 3;
  byte digitPins[] = { A0, A1, A2 };                // These are the PINS of the ** Arduino **
  byte segmentPins[] = { 0, 1, 2, 3, 4, 5, 6, 7 };  // These are the PINs of the ** Shift register **
  bool resistorsOnSegments = true;                  // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE;             // See README.md for options
  bool updateWithDelays = false;                    // Default 'false' is Recommended
  bool leadingZeros = false;                        // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true;                      // Use 'true' if your decimal point doesn't exist or isn't connected

  sevsegshift.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
                    updateWithDelays, leadingZeros, disableDecPoint);
  sevsegshift.setBrightness(90);
}

void loop() {
  // Read and print inputs at the specified sampling rate
  static unsigned long previousTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= SamplePeriod) {
    readButtonStates();
    previousTime = currentTime;

    // Draw 0 or 1 in each segment
    segmentLine_MAX7219();
  }

  // Draw the byte number (0-255)
  sevSegWrite();
}

void readButtonStates() {
  // Always reset
  zeroOneSegments = 0;

  // Read
  //Serial.print("Bits: ");
  for (int i = 0; i < 8; i++) {
    int state = isrDigitalRead(i);
    zeroOneSegments |= state << i;
  }
  //Serial.println(zeroOneSegments);
}

// Source: https://www.woolseyworkshop.com/2021/02/18/adding-digital-io-to-your-arduino-part-2-the-74hc165/
uint8_t isrReadRegister() {
  uint8_t inputs = 0;
  digitalWrite(clockPin1, HIGH);                    // preset clock to retrieve first bit
  digitalWrite(latchPin1, HIGH);                    // disable input latching and enable shifting
  inputs = shiftIn(dataPin1, clockPin1, MSBFIRST);  // capture input values
  digitalWrite(latchPin1, LOW);                     // disable shifting and enable input latching
  return inputs;
}

// Source: https://www.woolseyworkshop.com/2021/02/18/adding-digital-io-to-your-arduino-part-2-the-74hc165/
int isrDigitalRead(uint8_t pin) {
  return bitRead(isrReadRegister(), pin);
}

void segmentLine_MAX7219() {
  /* 
  * Display a character on a 7-Segment display.
  * There are only a few characters that make sense here :
  * '0','1','2','3','4','5','6','7','8','9','0',
  * 'A','b','c','d','E','F','H','L','P',
  * '.','-','_',' ' 
  * Params:
  * addr	 address of the display (0 if only one is used else index)
  * digit  the position of the character on the display (0..7)
  * value  the character to be displayed. 
  * dp     sets the decimal point.
  *
  * lc.setDigit(addr, digit, value, dp);
  */

  for (int digit = 0; digit < 8; digit++) {
    lc.setDigit(0, digit, (zeroOneSegments >> digit) & 1, false);
    //Serial.print(digit);
  }

  //Serial.println("");
  lc.shutdown(0, false);
}

void sevSegWrite() {
  if (digitalRead(dOffPin) == LOW) {
    // ToDo: Animation?
    sevsegshift.setNumber(999);
  } else {
    sevsegshift.setNumber(zeroOneSegments);
  }
  sevsegshift.refreshDisplay();  // Must run repeatedly
}
