#ifndef AvcCompass_h
#define AvcCompass_h

#include "Arduino.h"
#include <HMC5883L.h>
#include <Streaming.h>
#include <Wire.h>

class AvcCompass {
  static const int CompassAddress = 0x1E;
  static const float xOffset = 14.0;
  static const float yOffset = -125.0;
  static const float zOffset = 0.0;
  // declination angle 10 deg 55 min
  static const float declinationAngle = 10.0 + .55 / .6;

  HMC5883L compass;

  float magnetom_x;
  float magnetom_y;
  float magnetom_z;
  int heading;
  
  boolean error;

  public:
  void init();
  void update();

  inline int getHeading() {
    return heading;
  }

  inline void print () {
    Serial << getHeading() << endl;
  }
};
#endif
