#ifndef AvcCam_h
#define AvcCam_h

#include "Arduino.h"
#include <Wire.h>
#include <Streaming.h>

typedef struct {
  int X;
  int Y;
} Blob;
// http://blog.makezine.com/2008/11/22/hacking-the-wiimote-ir-ca/
//Initialization with sensitivity setting:
//The author defined 5 sensitivity levels, and there are four parameters (p0, p1, p2, p3) that are adjusted for each level. Here are the settings:
//
//Level 1: p0 = 0×72, p1 = 0×20, p2 = 0x1F, p3 = 0×03
//Level 2: p0 = 0xC8, p1 = 0×36, p2 = 0×35, p3 = 0×03
//Level 3: p0 = 0xAA, p1 = 0×64, p2 = 0×63, p3 = 0×03
//Level 4: p0 = 0×96, p1 = 0xB4, p2 = 0xB3, p3 = 0×04
//Level 5: p0 = 0×96, p1 = 0xFE, p2 = 0xFE, p3 = 0×05
//
//Quoting the Wiimote Wiki IR sensor page, these parameters correspond to:
//p0: MAXSIZE: Maximum blob size. Wii uses values from 0×62 to 0xc8
//p1: GAIN: Sensor Gain. Smaller values = higher gain
//p2: GAINLIMIT: Sensor Gain Limit. Must be less than GAIN for camera to function. No other effect?
//p3: MINSIZE: Minimum blob size. Wii uses values from 3 to 5
//
//Either pick your own custom settings for the parameters, or choose them from one of the 5 levels above, then send the following data to the device:
//
//0×30, 0×01
//0×00, 0×02, 0×00, 0×00, 0×71, 0×01, 0×00, p0
//0×07, 0×00, p1
//0x1A, p2, p3
//0×33, 0×03
//0×30, 0×08
//
//The author also links to the following source, which serves as a helloworld for reading sensor data directly from the IR camera:
class AvcCamera {
  const static int    slaveAddress = 0xB0 >> 1;

  static void WriteRegister (byte reg, byte data) {
    Wire.beginTransmission(slaveAddress);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
    delay(10);
  }
  
  public:
  static void init () {
    Wire.begin();
    // IR sensor initialize
    WriteRegister(0x30,0x01);
    Wire.beginTransmission(slaveAddress);
    Wire.write(0x00);
    Wire.write(0x02);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write(0x71);
    Wire.write(0x01);
    Wire.write(0x00);
    Wire.write(0x72);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(slaveAddress);
    Wire.write(0x07);
    Wire.write(0x00);
    Wire.write(0x20);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(slaveAddress);
    Wire.write(0x1A);
    Wire.write(0x1F);
    Wire.write(0x03);
    Wire.endTransmission();
    delay(10);
//    WriteRegister(0x06,0x90);
//    WriteRegister(0x08,0xC0);
//    WriteRegister(0x1A,0x40);
    WriteRegister(0x33,0x03);
    WriteRegister(0x30,0x08);
    delay(100);
  }
  
  static void checkCamera(int *x1, int *y1, int *x2, int *y2) {
    Blob   blobs[4];
    byte   data_buf[16];
    int    ii;
    //IR sensor read
    Wire.beginTransmission(slaveAddress);
    Wire.write(0x36);
    Wire.endTransmission();
    Wire.requestFrom(slaveAddress, 16);        // Request the 2 byte heading (MSB comes first)
    for (ii = 0; ii < 16; ii++) {
      data_buf[ii] = 0;
    }
    ii = 0;
    while (Wire.available() && ii < 16) { 
        data_buf[ii++] = Wire.read();
    }
    // Read 10 bit X/Y values
    int kk = 1;
    for (int jj = 0; jj < 4; jj++) {
      blobs[jj].X = data_buf[kk++];
      blobs[jj].Y = data_buf[kk++];
      int ss = data_buf[kk++];
      blobs[jj].X += (ss & 0x30) << 4;
      blobs[jj].Y += (ss & 0xC0) << 2;
    }
//    Serial << blobs[0].X << endl;
    *x1 = blobs[0].X;
    *y1 = blobs[0].Y;
    *x2 = blobs[1].X;
    *y2 = blobs[1].Y;
  }
};

#endif
