#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <EasyButton.h>

class ButtonController {
public:
    // Constructor: two button pins and output pin
    ButtonController(int btn1Pin, int btn2Pin, int outPin);

    // Initialize pins and EasyButton
    void begin();

    // Must be called in loop()
    void update();

private:
    EasyButton button1;
    EasyButton button2;
    int outputPin;
    bool outputState;
    unsigned long timerEnd;

    // Callbacks
    void handleButton1();
    void handleButton2();
};

#endif
