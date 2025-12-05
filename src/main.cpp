#include <Arduino.h>
#include "getTime.h"
#include "SevSeg.h"
#include "config.h"
#include "WiFi.h"
#include "alarm.h"
#include "buttonHandler.h"
#include "handleServo.h"
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "ElegantOTA.h"
#include "audioPlayer.h"

ServoESP32 servo;

const char* ssid = "ssid";
const char* password = "password";

SevSeg display;

ButtonController buttons(4,2,5);

AsyncWebServer otaServer(80);



//---------Display variables-------//
  const int brightnessPin = 34;
  
  byte digitPins[] = {13,16, 17,18};
  byte segmentPins[] = {19,21,22,23,25,33,27};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = N_TRANSISTORS; 
  bool updateWithDelays = false; 
  bool leadingZeros = true; 
  bool disableDecPoint = true;

  //Static IP
  IPAddress local_IP(192, 168, 1, 255);

IPAddress gateway(192, 168, 1, 1);

 IPAddress subnet(255, 255, 0, 0);








void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  audioInit();
  audioPlay("/output.wav");
  
  pinMode(brightnessPin, INPUT);

  display.begin(hardwareConfig, 4, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  
  //Clock init function, defined in getTime.cpp
  initClock();

  
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) delay(200);

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("HTTP Server started");
  servo.attach(32, 0);

  otaServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  otaServer.begin();
  ElegantOTA.begin(&otaServer);
  
}

void loop() {
   ClockTime timeNow = getClockTime();
   int joinedTime = timeNow.hour * 100 + timeNow.minute;

   display.setNumber(joinedTime,-1);
   server.handleClient();
   soundAlarm(timeNow.hour, timeNow.minute, receivedHour, receivedMinute);
   int potValue = analogRead(brightnessPin);
   //display.setBrightness(map(potValue, 0, 4095, 0, 100));
   display.setBrightness(100)
  
   int day = timeNow.weekDay;

   switch(day){
    case 0:
    servo.write(20);
    break;
    case 1:
    servo.write(40);
    break;
    case 2:
    servo.write(60);
    break;
    case 3:
    servo.write(80);
    break;
    case 4:
    servo.write(100);
    break;
    case 5:
    servo.write(120);
    break;
    case 6:
    servo.write(140);
    break;
   }
  
   ElegantOTA.loop();
  


}

