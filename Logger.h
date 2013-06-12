#ifndef Logger_h
#define Logger_h

#include <TinyGPS.h>
#include "AvcMpu6000.h"

class Logger {

public:
  static void logImuData (Gps *gps, AvcCompass *compass) {
    int count = 0;
    count += Serial.print(gps->getLatitude());
    count += Serial.print(",");
    count += Serial.print(gps->getLongitude());
    count += Serial.print(",");
    count += Serial.print(gps->getHdop());
    count += Serial.print(",");
    count += Serial.print(gps->getDistanceTraveled());
    count += Serial.print(",");
    count += Serial.print(gps->getFixTime() * 10);
    count += Serial.print(",");
    count += Serial.print(gps->getSpeed(), 7);
    count += Serial.print(",");
    count += Serial.print(gps->hasWaasLock());
    count += Serial.print(",");
    count += Serial.print(compass->getHeading());
    Serial.print("*");
    Serial.println(count);
  }

  static void logGpsData (Gps *gps) {
    int count = 0;
    count += Serial.print("GPS,");
    count += Serial.print(gps->getLatitude());
    count += Serial.print(",");
    count += Serial.print(gps->getLongitude());
    count += Serial.print(",");
    count += Serial.print(gps->getHdop());
    count += Serial.print(",");
    count += Serial.print(gps->getDistanceTraveled());
    count += Serial.print(",");
    count += Serial.print(gps->getFixTime() * 10);
    count += Serial.print(",");
    count += Serial.print(gps->getSpeed(), 7);
    count += Serial.print(",");
    count += Serial.print(gps->hasWaasLock());
    Serial.print("*");
    Serial.println(count);
  }

  static void logCompassData (AvcCompass *compass) {
    int count = 0;
    count += Serial.print("COMP,");
    count += Serial.print(compass->getHeading());
    count += Serial.print(",");
    count += Serial.print(compass->getXOffset());
    count += Serial.print(",");
    count += Serial.print(compass->getYOffset());
    count += Serial.print(",");
    count += Serial.print(compass->getZOffset());
    Serial.print("*");
    Serial.println(count);
  }
  
  static void logCamera(int x1, int y1, int x2, int y2) {
    int count = 0;
    count += Serial.print("CAM,");
    count += Serial.print(x1);
    count += Serial.print(",");
    count += Serial.print(y1);
    count += Serial.print(",");
    count += Serial.print(x2);
    count += Serial.print(",");
    count += Serial.print(y2);
    Serial.print("*");
    Serial.println(count);
  }

  static void logCallibration (AvcCompass *compass) {
    Serial <<
    compass->getX() << "\t" <<
    compass->getY() << "\t" <<
    compass->getZ() << "\t" <<
    compass->getAdjX() << "\t" <<
    compass->getAdjY() << "\t" <<
    compass->getAdjZ() << "\t" <<
    compass->getXOffset() << "\t" <<
    compass->getYOffset() << "\t" <<
    compass->getZOffset() << "\t" <<
    endl;
  }

  static void logMpuData (AvcMpu6000 *mpu) {
    int count = 0;
    count += Serial.print("MPU,");
    count += Serial.print(mpu->getAccelX());
    count += Serial.print(",");
    count += Serial.print(mpu->getAccelY());
    count += Serial.print(",");
    count += Serial.print(mpu->getAccelZ());
    count += Serial.print(",");
    count += Serial.print(mpu->getGyroX());
    count += Serial.print(",");
    count += Serial.print(mpu->getGyroY());
    count += Serial.print(",");
    count += Serial.print(mpu->getGyroZ());
    count += Serial.print(",");
    count += Serial.print(mpu->getTemp());
    Serial.print("*");
    Serial.println(count);
  }
  
  static void logKalman (Gps *gps, AvcCompass *compass, AvcMpu6000 *mpu, float speed) {
    int count = 0;
    count += Serial.print("GCM,");
    count += Serial.print(gps->getLatitude());
    count += Serial.print(",");
    count += Serial.print(gps->getLongitude());
    count += Serial.print(",");
    count += Serial.print(gps->getHdop());
    count += Serial.print(",");
    count += Serial.print(gps->getDistanceTraveled());
    count += Serial.print(",");
    count += Serial.print(gps->getFixTime() * 10);
    count += Serial.print(",");
    count += Serial.print(gps->getSpeed(), 7);
    count += Serial.print(",");
    count += Serial.print(gps->hasWaasLock());
    count += Serial.print(",");
    count += Serial.print(compass->getHeading());
    count += Serial.print(",");
    count += Serial.print(compass->getXOffset());
    count += Serial.print(",");
    count += Serial.print(compass->getYOffset());
    count += Serial.print(",");
    count += Serial.print(compass->getZOffset());
    count += Serial.print(",");
    count += Serial.print(mpu->getAccelX());
    count += Serial.print(",");
    count += Serial.print(mpu->getAccelY());
    count += Serial.print(",");
    count += Serial.print(mpu->getAccelZ());
    count += Serial.print(",");
    count += Serial.print(mpu->getGyroX());
    count += Serial.print(",");
    count += Serial.print(mpu->getGyroY());
    count += Serial.print(",");
    count += Serial.print(mpu->getGyroZ());
    count += Serial.print(",");
    count += Serial.print(mpu->getTemp());
    count += Serial.print(",");
    count += Serial.print(speed);
    Serial.print("*");
    Serial.println(count);
  }

  static void logKalman (AvcCompass *compass, AvcMpu6000 *mpu, float speed) {
    int count = 0;
    count += Serial.print("CM,");
    count += Serial.print(compass->getHeading());
    count += Serial.print(",");
    count += Serial.print(compass->getXOffset());
    count += Serial.print(",");
    count += Serial.print(compass->getYOffset());
    count += Serial.print(",");
    count += Serial.print(compass->getZOffset());
    count += Serial.print(",");
    count += Serial.print(mpu->getAccelX());
    count += Serial.print(",");
    count += Serial.print(mpu->getAccelY());
    count += Serial.print(",");
    count += Serial.print(mpu->getAccelZ());
    count += Serial.print(",");
    count += Serial.print(mpu->getGyroX());
    count += Serial.print(",");
    count += Serial.print(mpu->getGyroY());
    count += Serial.print(",");
    count += Serial.print(mpu->getGyroZ());
    count += Serial.print(",");
    count += Serial.print(mpu->getTemp());
    count += Serial.print(",");
    count += Serial.print(speed);
    Serial.print("*");
    Serial.println(count);
  }
};

#endif
