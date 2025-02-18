class SevenSegmentDisplay {
    uint8_t rclkPin;
    uint8_t srclkPin;
    uint8_t serPin;
    uint8_t digitPins[4];
    int value[4];

    public:
        void attach(uint8_t ser, uint8_t rclk, uint8_t srclk, uint8_t digits[4]);
        void set(int newValue);
        void refresh();
};