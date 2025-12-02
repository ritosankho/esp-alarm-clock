#include <WiFi.h>
#include <time.h>
#include "getTime.h"

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 5 * 3600 + 30 * 60;   // UTC+5:30
const int daylightOffset_sec = 0;

unsigned long lastSync = 0;
const unsigned long syncInterval = 3600 * 1000;  // 1 hour

// Struct for SevSeg display

// Sync with NTP
void syncNTP() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println("NTP synced!");
  } else {
    Serial.println("Failed to sync NTP");
  }
}

void initClock() {                    //Initialising the clock by calling the syncNTP function and recording the time of syncronysation
  syncNTP();
  lastSync = millis();
}


ClockTime getClockTime() {                //returns current time as a struct, resyncs every hour
  // Resync every hour
  if (millis() - lastSync > syncInterval) {
    syncNTP();
    lastSync = millis();
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    // return something safe if time is not available
    return {0, 0,};
  }

  ClockTime ct = {
    timeinfo.tm_hour,
    timeinfo.tm_min,
  };

  return ct;
}
