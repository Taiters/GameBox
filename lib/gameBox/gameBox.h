#include "dpad.h"
#include "screen.h"
#include "sevenSegment.h"

struct GameBox {
    DPad dpad;
    Screen screen;
    SevenSegmentDisplay sevenSegmentDisplay;
};