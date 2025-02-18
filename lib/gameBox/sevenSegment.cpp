#include <Arduino.h>
#include "sevenSegment.h"

const uint8_t DIGITS[10] = {
    0b11111100,
    0b01100000,
    0b11011010,
    0b11110010,
    0b01100110,
    0b10110110,
    0b10111110,
    0b11100000,
    0b11111110,
    0b11110110,
};

void SevenSegmentDisplay::attach(uint8_t ser, uint8_t rclk, uint8_t srclk, uint8_t digits[4]) {
    serPin = ser;
    rclkPin = rclk;
    srclkPin = srclk;

    pinMode(ser, OUTPUT);
    pinMode(rclk, OUTPUT);
    pinMode(srclk, OUTPUT);

    for (int i = 0; i < 4; i++) {
        value[i] = 0;
        digitPins[i] = digits[i];
        pinMode(digitPins[i], OUTPUT);
        digitalWrite(digitPins[i], HIGH);
    }
}

void SevenSegmentDisplay::set(int newValue) {
    int val = newValue;
    for (int i = 0; i < 4; i++) {
        value[3-i] = val % 10;
        val = val / 10;
    }
}

void SevenSegmentDisplay::refresh() {
    for (int i = 0; i < 4; i++) {
        digitalWrite(rclkPin, LOW);
        shiftOut(serPin, srclkPin, LSBFIRST, DIGITS[value[i]]);
        digitalWrite(rclkPin, HIGH);
        digitalWrite(digitPins[i], LOW);
        delayMicroseconds(500);
        digitalWrite(digitPins[i], HIGH);
    }
}