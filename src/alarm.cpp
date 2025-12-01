#include "getTime.h"
#include "Arduino.h"
void soundAlarm( int hour, int minute, int receivedHour, int receivedMinute){
    if (hour == receivedHour && minute == receivedMinute ){
        digitalWrite(34, HIGH);
    }
    
}



