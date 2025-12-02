#pragma once

#include <WebServer.h>

extern int receivedHour;
extern int receivedMinute;


// Extern server object (defined in config.cpp)
extern WebServer server;


// Function prototypes
void handleRoot();
void handleSet();

