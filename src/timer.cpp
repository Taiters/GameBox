#include <Arduino.h>
#include "timer.h"

void Timer::every(unsigned long value) {
    everyMs = value;
}

void Timer::update(unsigned long delta) {
    elapsed += delta;
    if (elapsed > everyMs) {
        elapsed -= everyMs;
        triggered = true;
    }
}

void Timer::reset() {
    elapsed = 0;
    triggered = false;
}

bool Timer::hasTriggered() {
    bool val = triggered;
    if (val) {
        triggered = false;
    }
    return val;
}