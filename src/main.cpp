#include <Arduino.h>
#include "getTime.h"
#include "SevSeg.h"
#include "globals.h"

SevSeg hourDisplay;
SevSeg minuteDisplay;
int hour = timeNow.hour;
int minute = timeNow.minute;



void setup() {
  void initClock();
  
  
}

void loop() {
   ClockTime time = getClocktime();


}

