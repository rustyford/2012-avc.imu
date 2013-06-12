#ifndef AvcCompass_h
#define AvcCompass_h

#include "Arduino.h"
#include <HMC5883L.h>
#include <Streaming.h>
#include <Wire.h>
#include "AvcEeprom.h"

class AvcCompass {
  static const int CompassAddress = 0x1E;
//  static const float xOffset = 14.0;
//  static const float yOffset = -125.0;
//  static const float zOffset = 0.0;
  float xOffset;
  float yOffset;
  float zOffset;
  int maxX;
  int maxY;
  int maxZ;
  int minX;
  int minY;
  int minZ;
//  phoenix declination angle 10 deg 46 min
  static const float declinationAngle = -(10.0 + .46 / .6);
//  denver co 8 deg 50 min
//  static const float declinationAngle = -(8.0 + .50 / .6);

  HMC5883L compass;

  float magnetom_x;
  float magnetom_y;
  float magnetom_z;
  float rawMagnetomX;
  float rawMagnetomY;
  float rawMagnetomZ;
  int heading;
  
  boolean error;

  public:
  void init();
  void update();
  void callibrate();
  void writeCallibration();

  inline int getHeading() {
    return heading;
  }

  inline void print () {
    Serial << getHeading() << endl;
  }
  
  inline int getXOffset () {return xOffset;}
  inline int getYOffset () {return yOffset;}
  inline int getZOffset () {return zOffset;}
  inline float getX () {return rawMagnetomX;}
  inline float getY () {return rawMagnetomY;}
  inline float getZ () {return rawMagnetomZ;}
  inline float getAdjX () {return magnetom_x;}
  inline float getAdjY () {return magnetom_y;}
  inline float getAdjZ () {return magnetom_z;}
};
#endif
