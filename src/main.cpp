#include <Arduino.h>
#include "getTime.h"
#include "SevSeg.h"
#include "config.h"
#include "WiFi.h"
#include "alarm.h"


ClockTime timeNow;


const char* ssid = "ssid";
const char* password = "password";

SevSeg display;







void setup() {
  //---------Display variables-------//
  byte digitPins[] = {13,16, 17,18};
  byte segmentPins[] = {19,21,22,23,25,26,27};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; 
  bool updateWithDelays = false; 
  bool leadingZeros = true; 
  bool disableDecPoint = true;

  display.begin(hardwareConfig, 4, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  
  //Clock init function, defined in getTime.cpp
  initClock();

  //---------Server-----------//
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) delay(200);

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("HTTP Server started");

  
}

void loop() {
   timeNow = getClockTime();
   int joinedTime = timeNow.hour * 100 + timeNow.minute;

   display.setNumber(joinedTime,-1);
   server.handleClient();
   soundAlarm(timeNow.hour, timeNow.minute, receivedHour, receivedMinute);
  


}

