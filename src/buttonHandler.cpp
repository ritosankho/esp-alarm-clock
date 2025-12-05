#include "EasyButton.h"
#include "buttonHandler.h"
// Constructor
ButtonController::ButtonController(int btn1Pin, int btn2Pin, int outPin)
  : button1(btn1Pin), button2(btn2Pin), outputPin(outPin), outputState(false), timerEnd(0) {}

// Initialize pins and register callbacks
void ButtonController::begin() {
    pinMode(outputPin, OUTPUT);
    digitalWrite(outputPin, outputState);

    button1.begin();
    button2.begin();

    button1.onPressed([this](){ this->handleButton1(); });
    button2.onPressed([this](){ this->handleButton2(); });
}

// Call this in loop
void ButtonController::update() {
    button1.read();
    button2.read();

    // Auto-off after 7 seconds if triggered by button1
    if (outputState && timerEnd != 0 && millis() >= timerEnd) {
        outputState = false;
        digitalWrite(outputPin, outputState);
        timerEnd = 0;
    }
}

// Button1 pressed: turn on output for 7 seconds
void ButtonController::handleButton1() {
    outputState = true;
    digitalWrite(outputPin, outputState);
    timerEnd = millis() + 7000; // 7 seconds
}

// Button2 pressed: toggle output immediately
void ButtonController::handleButton2() {
    outputState = !outputState;
    digitalWrite(outputPin, outputState);
    // If toggled manually, cancel auto-off timer
    timerEnd = 0;
}
