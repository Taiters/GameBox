#include <Arduino.h>
#include "screen.h"

long BRIGHTNESS_TIMING[MAX_BRIGHTNESS] = {
    10,
    300,
    2000,
};

void pulsePin(uint8_t pin, bool inverted = false) {
    digitalWrite(pin, !inverted);
    digitalWrite(pin, inverted);
}

void Screen::attach(uint8_t rowSer, uint8_t rowClk, uint8_t colSer, uint8_t colClk, uint8_t rclk, uint8_t clr) {
    rowSerPin = rowSer;
    rowClkPin = rowClk;
    colSerPin = colSer;
    colClkPin = colClk;
    rclkPin = rclk;
    clrPin = clr;

    pinMode(rowSerPin, OUTPUT);
    pinMode(rowClkPin, OUTPUT);
    pinMode(colSerPin, OUTPUT);
    pinMode(colClkPin, OUTPUT);
    pinMode(rclkPin, OUTPUT);
    pinMode(clrPin, OUTPUT);

    digitalWrite(clrPin, HIGH);
}

void Screen::drawPoint(uint8_t x, uint8_t y, uint8_t brightness) {
    uint8_t bit = 1 << (5 - x);
    data[y] |= bit;
    for (int i = 0; i < brightness; i++) {
        brightnessMask[y][i] |= bit;
    }
}

void Screen::clear() {
    for (int i = 0; i < 4; i++) {
        data[i] = 0;
        for (int j = 0; j < MAX_BRIGHTNESS; j++) {
            brightnessMask[i][j] = 0;
        }
    }

    pulsePin(clrPin, true);
    pulsePin(rclkPin);
}

void Screen::updateColumn(uint8_t row) {
    for (int col = 0; col < 6; col++) {
        int val = row & 1 ;
        row = row >> 1;
        digitalWrite(colSerPin, 1-val);
        pulsePin(colClkPin);
    }
}

void Screen::refresh() {
    digitalWrite(rowSerPin, HIGH);
    for (int row = 0; row < 4; row++) {
        int rowData = data[row];
        long rowTime = 0;
        pulsePin(rowClkPin);
        if (row == 0) {
            digitalWrite(rowSerPin, LOW);
        }
        for (int m = 0; m < MAX_BRIGHTNESS; m++) {
            updateColumn(rowData & brightnessMask[row][m]);

            pulsePin(rclkPin);
            delayMicroseconds(BRIGHTNESS_TIMING[m] - rowTime);
            rowTime += BRIGHTNESS_TIMING[m];
        }
    }
    pulsePin(rowClkPin);
    pulsePin(rclkPin);
}