#ifndef Logger_h
#define Logger_h

#include <TinyGPS.h>

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
//    Serial <<
//        gps->getLatitude() << "," << 
//        gps->getLongitude() << "," <<
//        gps->getHdop() << "," << 
//        gps->getDistanceTraveled() << "," <<
//        gps->getFixTime() * 10 << "," <<
//        _FLOAT(gps->getSpeed(), 7) << "," << 
//        gps->hasWaasLock() << "," <<
//        compass->getHeading() <<
//        endl;
  }
};

#endif
