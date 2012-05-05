#include <TinyGPS.h>
#include <HMC5883L.h>
#include <SoftwareSerial.h>
#include <PID_v1.h>
#include <Streaming.h>
#include "Gps.h"

#define SET_WAYPOINT_BUTTON A2
#define RESET_BUTTON A3
#define GPS_LED 8
#define SERVO_PIN 9

#define SAMPLING_BLINK_MILLIS 200
#define WAAS_BLINK_MILLIS 1000
#define SAMPLING_COUNT 50
#define WAYPOINT_COUNT 4
#define REORIENT_THRESHOLD 20
#define MAX_PID_DEVIATION_FROM_COMPASS 120
#define MAX_STEERING_CHANGE 5

float MAG_Heading;
int currentHeading;
int normalizedHeading;
float gpsHeading;
boolean reorient = true;

Gps *waypoints[WAYPOINT_COUNT];
int waypointSamplingIndex = -1;
float samplingHdop = 0;
int sampleCount = 0;
int nextWaypoint = 0;
int numWaypointsSet = 0;

// LED vars
boolean isSamplingGps = false;
int isLedOn = 0;
boolean isGpsLock = false;

#define RXPIN A0
#define TXPIN A1

SoftwareSerial mySerial(RXPIN, TXPIN);
Gps location;

void setup()
{
  Serial.begin(57600);
  pinMode(GPS_LED,OUTPUT);
  Gps::init(&mySerial);
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
  controlGpsLed(&location);
  location.checkGps(&mySerial);
  // isUpdated can only be called once per checkGps call
  if (location.isUpdated()) {
    location.excelPrint();
  } else {
   // Serial << "skipping";
  }
  delay(50);
}
