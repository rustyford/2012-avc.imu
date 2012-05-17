long ledTimer = 0;

void controlGpsLed (Gps *gps) {
  long now = millis();
  int diff = now - ledTimer;
  if (gps->hasWaasLock()) {
    if (diff > WAAS_BLINK_MILLIS || diff < 0) {
      if (isLedOn == 1) {
        digitalWrite(GPS_LED, LOW);
        isLedOn = 0;
      } else {
        digitalWrite(GPS_LED, HIGH);
        isLedOn = 1;
      }
      ledTimer = now;
    }
  } else if (gps->hasLock()) {
    digitalWrite(GPS_LED, HIGH);
    isLedOn = 1;
  } else {
    digitalWrite(GPS_LED, LOW);
    isLedOn = 0;
  }
}

