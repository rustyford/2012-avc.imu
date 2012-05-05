#include "Gps.h"

Gps::Gps() {
  lock = false;
  hdop = 0;
  latitude = 0;
  longitude = 0;
  waasLock = false;
  distanceTraveled = 0;
  previousFixTime = 0;
  fixTime = 0;
}

void Gps::checkGps(SoftwareSerial *serial) {
  char buf[256];
  boolean inStatus = 0;
  boolean sampled = false;
  int idx = 0;

  while (serial->available()) {
    unsigned char cc = serial->read();
    if (cc == 0xA0) {
      inStatus = 1;
      idx = 0;
    }
    if (inStatus) {
      if (cc == 0x0D) {
        inStatus = 0;
      } else {
        buf[idx++] = cc;
      }
    } else {
      if (cc != 0x0A && cc != 0x0D  &&  idx < 254)
        buf[idx++] = cc;
      if (gps.encode(cc)) {
        lock = true;
        int year;
        byte month, day, hour, minute, second, hundredths;
        gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
        previousFixTime = fixTime;
        fixTime = hundredths + second * 100 + minute * 60 * 100 + hour * 60 * 60 * 100 + day * 24 * 60 * 60 * 100;
        long lat = latitude, lon = longitude;
        gps.get_position(&latitude, &longitude, &age);
        distanceTraveled = TinyGPS::distance_between (toFloat(lat), 0.0, toFloat(latitude), toFloat(lon - longitude));
        hdop = gps.hdop();
        if (gps.fix_type() == '2') {
          waasLock = true;
        } else {
          waasLock = false;
        }
        
        idx = 0;
      } else if (cc == 0x0D) {
        idx = 0;
      }
    }
  }
}

int Gps::getHeadingTo (Gps *dest) {
  return (int) TinyGPS::course_to(toFloat(latitude), 0.0f, toFloat(dest->getLatitude()), toFloat(dest->getLongitude() - longitude));
}

