#define LOG_RAW_COMPASS 0

int CompassAddress = 0x1E;

HMC5883L compass;

int magnetom_x;
int magnetom_y;
int magnetom_z;
int magraw_x;
int magraw_y;
int magraw_z;

float xOffset = 14.0;
float yOffset = -125.0;
float zOffset = 0.0;

void Compass_Init()
{
//  Wire.beginTransmission(CompassAddress);
//  Wire.write((uint8_t)0x02); 
//  Wire.write((uint8_t)0x00);   // Set continouos mode (default to 10Hz)
//  Wire.endTransmission(); //end transmission
  Wire.begin();
  compass = HMC5883L();
  // Set scale to +/- 1.3 Ga
  int error = compass.SetScale(1.3);
  if (error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
  // Set measurement mode to continous
  error = compass.SetMeasurementMode(Measurement_Continuous);
  if (error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
}

void Read_Compass() {
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  magnetom_x = scaled.XAxis - xOffset;
  magnetom_y = scaled.YAxis - yOffset;
  magnetom_z = scaled.ZAxis - zOffset;
#if LOG_RAW_COMPASS == 1
  MagnetometerRaw raw = compass.ReadRawAxis();
  magraw_x = raw.XAxis;
  magraw_y = raw.YAxis;
  magraw_z = raw.ZAxis;
  Serial.print(magraw_x);
  Serial.print("\t");
  Serial.print(magraw_y);
  Serial.print("\t");
  Serial.print(magraw_z);
  Serial.print("\t");
  Serial.print(magnetom_x);
  Serial.print("\t");
  Serial.print(magnetom_y);
  Serial.print("\t");
  Serial.println(magnetom_z);
#endif
}

void Compass_Heading()
{
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(magnetom_y, magnetom_x);
  float declinationAngle = 190.5 / 1000.0;
  heading -= declinationAngle;
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  MAG_Heading = heading * 180/M_PI; 
}

