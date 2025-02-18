enum DPadDirection {
    UP,
    RIGHT,
    DOWN,
    LEFT,
};

class DPad {
    uint8_t upPin;
    uint8_t rightPin;
    uint8_t downPin;
    uint8_t leftPin;

    int directionToPin(DPadDirection dir);

    public:
        void attach(uint8_t up, uint8_t right, uint8_t down, uint8_t left);
        bool isDown(DPadDirection dir);
};