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
}

void AvcCompass::update () {
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  magnetom_x = scaled.XAxis - xOffset;
  magnetom_y = scaled.YAxis - yOffset;
  magnetom_z = scaled.ZAxis - zOffset;
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


