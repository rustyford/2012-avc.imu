#ifndef Gps_h
#define Gps_h

#include "Arduino.h"
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <Streaming.h>

class Gps {
  long latitude;
  long longitude;
  float hdop;
  boolean lock;
  boolean waasLock;
  TinyGPS       gps;
  float distanceTraveled;
  unsigned long previousFixTime;
  //time of day in hundredths of a second
  unsigned long fixTime;
  unsigned long age;
  unsigned long previousMillis;
  unsigned long updateCheck;

  static float toFloat(long fixed) {
    return fixed / 1000000.0;
  }

  static void sndByte (SoftwareSerial *mySerial, byte cc) {
    mySerial->write(cc);
  }
  
  static void sendCmd (SoftwareSerial *mySerial, byte *msg) {
    sndByte(mySerial, 0xA0);
    sndByte(mySerial, 0xA1);
    sndByte(mySerial, 0x00);
    byte len = msg[0];
    sndByte(mySerial, len);
    byte crc = 0;
    for (int ii = 1; ii <= len; ii++) {
      byte cc = msg[ii];
      sndByte(mySerial, cc);
      crc ^= cc;
    }
    sndByte(mySerial, crc);
    sndByte(mySerial, 0x0D);
    sndByte(mySerial, 0x0A);
  }

  public:
  Gps();
  void checkGps(SoftwareSerial *serial);
  int getHeadingTo (Gps *dest);
  inline void testPrint() {
    Serial << "Lat: " << getLatitude() << ", Lon: " << getLongitude() << ", hdop: " <<
        getHdop() << ", Distance Traveled: " << getDistanceTraveled() << ", Elapsed Time: " <<
        getTimeDelta() << ", WAAS Lock: " << hasWaasLock() << ", Speed: " << _FLOAT(getSpeed(), 5) << " m/s" << endl;
  }

  inline void excelPrint() {
    int m = millis() - previousMillis;
    previousMillis = millis();
    Serial << 
        getLatitude() << "\t" << 
        getLongitude() << "\t" <<
        getHdop() << "\t" << 
        getDistanceTraveled() << "\t" <<
        getFixTime() * 10 << "\t" <<
        getTimeDelta() * 10 << "\t" << 
        age << "\t" << 
        m << "\t" <<
        _FLOAT(getSpeed(), 5) << "\t" << 
        hasWaasLock() <<
        endl;
  }

  inline long getLatitude() {
    return latitude;
  }
  
  inline long getLongitude() {
    return longitude;
  }
  
  inline boolean hasLock() {
    return lock;
  }

  inline boolean hasWaasLock() {
    return waasLock;
  }
  
  inline unsigned long getFixTime () {
    return fixTime;
  }
  
  inline int getTimeDelta () {
    return fixTime - previousFixTime;
  }

  inline float getHdop () {
    return hdop/100.0;
  }
  
  inline float getDistanceTraveled() {
    return distanceTraveled;
  }
  
  inline float getSpeed () {
    return getDistanceTraveled() / (getTimeDelta() / 100.0);
  }
  
  inline boolean isUpdated () {
    if (updateCheck != fixTime) {
      updateCheck = fixTime;
      return true;
    }
    return false;
  }

  static void init(SoftwareSerial *mySerial) {
    mySerial->begin(9600);
#if SET_UP_GPS
    byte Update5Hz[]  = {0x03, 0x0E, 0x05, 0x00};
    byte WAASEnable[] = {0x03, 0x37, 0x01, 0x00};
    byte GGA_RMC[10]    = {0x09, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
                                       // 1 = GGA Enable
                                       //       1 = GSA Enable (*)
                                       //             1 = GSV Enable
                                       //                   1 = GLL Enable
                                       //                         1 = RMC Enable (*)
                                       //                               1 = VTG Enable
                                       //                                     1 = ZDA Enable
                                       //                                           1 = Attributes
    sndByte(mySerial, 0x0A);
    sndByte(mySerial, 0x0D);
    // Disable all but GGA Messages
    sendCmd(mySerial, GGA_RMC);
    // Set update rate to 5Hz (Forces Reset)
    sendCmd(mySerial, Update5Hz);
    // Enable WAAS
    sendCmd(mySerial, WAASEnable);
#endif
  }  
};

#endif


