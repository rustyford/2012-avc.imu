#ifndef Eeprom_h
#define Eeprom_h

#include "Arduino.h"
#include <EEPROM.h>

#define X_OFFSET  0
#define Y_OFFSET 2
#define Z_OFFSET 4

class AvcEeprom {
public:
  static void writeInt (int idx, int *val) {
    byte *bytes = (byte *) val;
    EEPROM.write(idx, bytes[0]);
    EEPROM.write(idx + 1, bytes[1]);
  }

  static void readInt (int idx, int *val) {
    byte *bytes = (byte *) val;
    bytes[0] = EEPROM.read(idx);
    bytes[1] = EEPROM.read(idx + 1);
  }

  static int getXOffset () {
    int x;
    readInt(X_OFFSET, &x);
    return x;
  }
  
  static void setXOffset (int x) {
    AvcEeprom::writeInt(X_OFFSET, &x);
  }
  
  static int getYOffset () {
    int y;
    AvcEeprom::readInt(Y_OFFSET, &y);
    return y;
  }
  
  static void setYOffset (int x) {
    AvcEeprom::writeInt(Y_OFFSET, &x);
  }

  static int getZOffset () {
    int z;
    AvcEeprom::readInt(Z_OFFSET, &z);
    return z;
  }
  
  static void setZOffset (int x) {
    AvcEeprom::writeInt(Z_OFFSET, &x);
  }
};
#endif
