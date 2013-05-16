#include "AvcCompass.h"

void AvcCompass::init() {
  Wire.begin();
  compass = HMC5883L();
  // Set scale to +/- 1.3 Ga
  int error = compass.SetScale(1.3);
  if (error != 0) // If there is an error, print it out.
    error = true;
    //Serial.println(compass.GetErrorText(error));
  // Set measurement mode to continous
  error = compass.SetMeasurementMode(Measurement_Continuous);
  if (error != 0) // If there is an error, print it out.
    error = true;
    //Serial.println(compass.GetErrorText(error));
  xOffset = AvcEeprom::getXOffset();
  yOffset = AvcEeprom::getYOffset();
  zOffset = AvcEeprom::getZOffset();
//  Serial <<
//      xOffset << "\t" <<
//      yOffset << "\t" <<
//      zOffset << endl;
  maxX = 0;
  maxY = 0;
  maxZ = 0;
  minX = 0;
  minY = 0;
  minZ = 0;
}

void AvcCompass::update () {
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  rawMagnetomX = scaled.XAxis;
  rawMagnetomY = scaled.YAxis;
  rawMagnetomZ = scaled.ZAxis;
  magnetom_x = rawMagnetomX - xOffset;
  magnetom_y = rawMagnetomY - yOffset;
  magnetom_z = rawMagnetomZ - zOffset;
  // Calculate lHeading when the magnetometer is level, then correct for signs of axis.
  float lHeading = atan2(magnetom_y, magnetom_x);
  // Correct for when signs are reversed.
  if(lHeading < 0)
    lHeading += 2*PI;
  // Check for wrap due to addition of declination.
  if(lHeading > 2*PI)
    lHeading -= 2*PI;
  
  // Convert radians to degrees for readability.
  float deg = lHeading * 180/M_PI;
  deg = deg - declinationAngle;
  // add 180 because the compass is backward
  heading = (int(deg) + 180) % 360;
}

void AvcCompass::callibrate () {
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  int x = scaled.XAxis;
  int y = scaled.YAxis;
  int z = scaled.ZAxis;
  if (x > 1000 || x < -1000 || y > 1000 || y < -1000 || z > 1000 || z < -1000) {
    return;
  }
  maxX = max(x, maxX);
  maxY = max(y, maxY);
  maxZ = max(z, maxZ);
  minX = min(x, minX);
  minY = min(y, minY);
  minZ = min(z, minZ);
}

void AvcCompass::writeCallibration() {
  xOffset = (maxX + minX) / 2;
  yOffset = (maxY + minY) / 2;
  zOffset = (maxZ + minZ) / 2;
  AvcEeprom::setXOffset(xOffset);
  AvcEeprom::setYOffset(yOffset);
  AvcEeprom::setZOffset(zOffset);
  maxX = maxY = maxZ = minX = minY = minZ = 0;
}

