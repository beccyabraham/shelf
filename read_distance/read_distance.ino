#define DEBUG 1

// touch includes
#include <MPR121.h>
#include <MPR121_Datastream.h>
#include <Wire.h>

// MPR121 datastream behaviour constants
const bool MPR121_DATASTREAM_ENABLE = false;

// touch constants
const uint32_t BAUD_RATE = 115200;
const uint8_t MPR121_ADDR = 0x5C;  // 0x5C is the MPR121 I2C address on the Bare Touch Board
const uint8_t MPR121_INT = 4;  // pin 4 is the MPR121 interrupt on the Bare Touch Board

const byte trigger1 = 11;
const byte echo1 = 8;
const byte trigger2 = 12;
const byte echo2 = 9;
const byte trigger3 = 13;
const byte echo3 = 10;

const byte electrode1 = 0;
const byte electrode2 = 2;
const byte electrode3 = 4;

void setup() {
  // put your setup code here, to run once:
  if (DEBUG) {
    Serial.begin(BAUD_RATE);
    if (!MPR121.begin(MPR121_ADDR)) {
      Serial.println("error setting up MPR121");
      switch (MPR121.getError()) {
        case NO_ERROR:
          Serial.println("no error");
          break;
        case ADDRESS_UNKNOWN:
          Serial.println("incorrect address");
          break;
        case READBACK_FAIL:
          Serial.println("readback failure");
          break;
        case OVERCURRENT_FLAG:
          Serial.println("overcurrent on REXT pin");
          break;
        case OUT_OF_RANGE:
          Serial.println("electrode out of range");
          break;
        case NOT_INITED:
          Serial.println("not initialised");
          break;
        default:
          Serial.println("unknown error");
          break;
      }
      while (!Serial);
    }
  }    

  MPR121.setInterruptPin(MPR121_INT);
  if (MPR121_DATASTREAM_ENABLE) {
    MPR121.restoreSavedThresholds();
    MPR121_Datastream.begin(&Serial);
  } else {
    MPR121.setTouchThreshold(10);
    MPR121.setReleaseThreshold(5);
  }

  MPR121.setFFI(FFI_10);
  MPR121.setSFI(SFI_10);
  MPR121.setGlobalCDT(CDT_4US);  // reasonable for larger capacitances

  MPR121.setNumDigPins(4);
  MPR121.pinMode(trigger1, INPUT);
  MPR121.pinMode(echo1, OUTPUT);
  MPR121.pinMode(echo2, OUTPUT);
  MPR121.pinMode(echo3, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);  // switch on user LED while auto calibrating electrodes
  delay(1000);
  MPR121.autoSetElectrodes();  // autoset all electrode settings
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trigger3, OUTPUT);
  pinMode(echo3, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float distance1 = readDistanceSensor(trigger1, echo1);
  float distance2 = readDistanceSensor(trigger2, echo2);
  float distance3 = readDistanceSensor(trigger3, echo3);
  MPR121.updateTouchData();
  MPR121.updateFilteredData();
  
  // Get an analog proximity value
  Serial.print(MPR121.getFilteredData(electrode1));Serial.print(" ");
  Serial.print(MPR121.getTouchData(electrode1));Serial.print(" ");
  Serial.print(MPR121.getFilteredData(electrode2));Serial.print(" ");
  Serial.print(MPR121.getTouchData(electrode2));Serial.print(" ");
  Serial.print(MPR121.getFilteredData(electrode3));Serial.print(" ");
  Serial.print(MPR121.getTouchData(electrode3));Serial.print(" ");
  Serial.print(distance1);Serial.print(" ");
  Serial.print(distance2);Serial.print(" ");
  Serial.println(distance3);Serial.println(" ");
  delay(100);
}

float readDistanceSensor(byte triggerPin, byte echoPin) {
   digitalWrite(triggerPin, LOW);
   delayMicroseconds(2);
   digitalWrite(triggerPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(triggerPin, LOW);
   float duration = pulseIn(echoPin, HIGH);  // in microseconds
   float distance = duration * 0.0343 / 2.0;  // in centimeters
   return distance;
}