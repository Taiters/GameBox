#include <Arduino.h>

#include "snake.h"

#define DPAD_UP A4
#define DPAD_RIGHT A5
#define DPAD_DOWN A6
#define DPAD_LEFT A7

#define SCREEN_ROW_SER 2
#define SCREEN_ROW_CLK 3
#define SCREEN_COL_SER 4
#define SCREEN_COL_CLK 5
#define SCREEN_RCLK 6
#define SCREEN_CLR 7

#define SEGMENT_SER A0
#define SEGMENT_RCLK A1
#define SEGMENT_SRCLK A2

#define SEGMENT_DIGIT_0 8
#define SEGMENT_DIGIT_1 9
#define SEGMENT_DIGIT_2 10
#define SEGMENT_DIGIT_3 11

uint8_t SEGMENT_DIGITS[4] = {
    SEGMENT_DIGIT_0,
    SEGMENT_DIGIT_1,
    SEGMENT_DIGIT_2,
    SEGMENT_DIGIT_3};

GameBox gameBox;
Snake snakeGame;

void setup()
{
    gameBox.dpad.attach(
        DPAD_UP,
        DPAD_RIGHT,
        DPAD_DOWN,
        DPAD_LEFT);
    gameBox.screen.attach(
        SCREEN_ROW_SER,
        SCREEN_ROW_CLK,
        SCREEN_COL_SER,
        SCREEN_COL_CLK,
        SCREEN_RCLK,
        SCREEN_CLR);
    gameBox.sevenSegmentDisplay.attach(
        SEGMENT_SER,
        SEGMENT_RCLK,
        SEGMENT_SRCLK,
        SEGMENT_DIGITS);

    snakeGame.setup(&gameBox);
}

long previousTime = 0;
void loop()
{
    long now = millis();
    long delta = now - previousTime;
    previousTime = now;

    snakeGame.update(delta);
    gameBox.screen.refresh();
    gameBox.sevenSegmentDisplay.refresh();
}