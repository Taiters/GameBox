#include <Arduino.h>

#ifndef MAX_BRIGHTNESS
#define MAX_BRIGHTNESS 3
#endif

class Screen {
    uint8_t rowSerPin;
    uint8_t rowClkPin;
    uint8_t colSerPin;
    uint8_t colClkPin;
    uint8_t rclkPin;
    uint8_t clrPin;
    uint8_t data[4];
    uint8_t brightnessMask[4][MAX_BRIGHTNESS];

    void updateColumn(uint8_t row);

    public:
        void attach(uint8_t rowSer, uint8_t rowClk, uint8_t colSer, uint8_t colClk, uint8_t rclk, uint8_t clr);
        void drawPoint(uint8_t x, uint8_t y, uint8_t brightness);
        void clear();
        void refresh();
};