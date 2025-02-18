#include <Arduino.h>
#include "dpad.h"

int DPad::directionToPin(DPadDirection dir) {
    switch(dir) {
        case DPadDirection::UP:
            return upPin;
        case DPadDirection::RIGHT:
            return rightPin;
        case DPadDirection::DOWN:
            return downPin;
        case DPadDirection::LEFT:
            return leftPin;
    }
}

void DPad::attach(uint8_t up, uint8_t right, uint8_t down, uint8_t left) {
    upPin = up;
    rightPin = right;
    downPin = down;
    leftPin = left;

    pinMode(up, INPUT);
    pinMode(right, INPUT);
    pinMode(down, INPUT);
    pinMode(left, INPUT);
}

bool DPad::isDown(DPadDirection dir) {
    // DOWN and RIGHT seem to be reporting HIGH from digitalRead,
    // even though the voltage is 0 when measured.
    // analogRead seems to work as expected though...
    // (Wish I knew about INPUT_PULLUP before soldering the buttons)
    return analogRead(directionToPin(dir)) > 1000;
}
