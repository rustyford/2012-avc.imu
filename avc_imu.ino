#include <TinyGPS.h>
#include <HMC5883L.h>
//#include <SoftwareSerial.h>
#include <Streaming.h>
#include "Gps.h"
#include "AvcCompass.h"
#include <Wire.h>
#include "Logger.h"
#include <EEPROM.h>
#include "AvcCamera.h"
#include "AvcMpu6000.h"
#include <SPI.h>
#include <AltSoftSerial.h>

#define GPS_LED 6
#define COMPASS_CALLIBRATION_PIN 10

#define WAAS_BLINK_MILLIS 1000
#define LOOP_SPEED 20 // HERTZ

#define MULTIPLE_LOG_TYPES 1
#define LOG_CALLIBRATION 0
#define LOG_ALL_GPS_DATA 0
#define LOG_CAMERA 0

// LED vars
int isLedOn = 0;
boolean isGpsLock = false;
boolean callibrating = false;

unsigned long previousTime = 0;
unsigned long fiveHertzTime = 0;

#define RXPIN A0
#define TXPIN A1

//SoftwareSerial mySerial(RXPIN, TXPIN);
Gps location;
AltSoftSerial mySerial;
AvcCompass compass;
unsigned long previousFixTime = 0;

volatile int MPU6000_newdata;
volatile int odometerCount;
AvcMpu6000 mpu;

void setup()
{
  Serial.begin(57600);
  mySerial.begin(9600);
  pinMode(GPS_LED,OUTPUT);
  Gps::init(&mySerial);
  compass.init();
  AvcCamera::init();
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);
  delay(1000);
  digitalWrite(6,LOW);
  digitalWrite(5,LOW);
  pinMode(COMPASS_CALLIBRATION_PIN, INPUT);
  digitalWrite(COMPASS_CALLIBRATION_PIN, HIGH);
  pinMode(A0, INPUT);
  digitalWrite(A0, HIGH);

  //MPU_INT is connected to INT 1. Enable interrupt on INT1
  attachInterrupt(0,MPU6000_data_int,RISING);
  mpu.init();
  
  //interrupt
  PCICR |= (1<<PCIE1);
  PCMSK1 |= (1<<PCINT8);
  MCUCR |= (1<<ISC10);
  sei(); //enable interrupts
}

void loop() {
  if (digitalRead(COMPASS_CALLIBRATION_PIN) == LOW) {
    callibrating = true;
    compass.callibrate();
#if LOG_CALLIBRATION
    Logger::logCallibration(&compass);
#endif
  } else {
    if (callibrating) {
      callibrating = false;
      compass.writeCallibration();
    }
    location.checkGps(&mySerial);
    compass.update();
    mpu.update(MPU6000_newdata);
    if (millis() - previousTime > 1000 / LOOP_SPEED && !LOG_CALLIBRATION) {
      previousTime = millis();
//      controlGpsLed(&location);
#if MULTIPLE_LOG_TYPES
      Logger::logCompassData(&compass);
      Logger::logMpuData(&mpu);
      if (previousFixTime != location.getFixTime()) {
        previousFixTime = location.getFixTime();
        Logger::logGpsData(&location);
      }
#else
      Logger::logImuData (&location, &compass);
#endif
    }
#if LOG_CALLIBRATION
    Logger::logCallibration(&compass);
#endif
    if (millis() - fiveHertzTime > 1000 / 5) {
      fiveHertzTime = millis();
      int x1 = 0;
      int y1 = 0;
      int x2 = 0;
      int y2 = 0;
      AvcCamera::checkCamera(&x1, &y1, &x2, &y2);
#if LOG_CAMERA
      Logger::logCamera(x1, y1, x2, y2);
#endif
    }
  }
//    Serial.println(odometerCount);

}

// MPU6000 INTERRUPT ON INT1
void MPU6000_data_int()
{
  MPU6000_newdata=1;
}

ISR(PCINT1_vect) {
  odometerCount++;
}
