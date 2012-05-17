#include <TinyGPS.h>
#include <HMC5883L.h>
#include <SoftwareSerial.h>
#include <PID_v1.h>
#include <Streaming.h>
#include "Gps.h"
#include "AvcCompass.h"
#include <Wire.h>
#include "Logger.h"

#define GPS_LED 8

#define WAAS_BLINK_MILLIS 1000
#define LOOP_SPEED 50 // HERTZ

// LED vars
int isLedOn = 0;
boolean isGpsLock = false;

unsigned long previousTime = 0;

#define RXPIN A0
#define TXPIN A1

SoftwareSerial mySerial(RXPIN, TXPIN);
Gps location;
AvcCompass compass;

void setup()
{
  Serial.begin(19200);
  pinMode(GPS_LED,OUTPUT);
  Gps::init(&mySerial);
  compass.init();
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);
  delay(1000);
  digitalWrite(6,LOW);
  digitalWrite(5,LOW);
}

void loop()
{
  if (millis() - previousTime > 1000 / LOOP_SPEED) {
    previousTime = millis();
    controlGpsLed(&location);
    location.checkGps(&mySerial);
    compass.update();
    Logger::logImuData (&location, &compass);
  }
}
