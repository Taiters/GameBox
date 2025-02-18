class Timer {
    unsigned long elapsed;
    unsigned long everyMs;
    bool triggered;

    public:
        void every(unsigned long value);
        void update(unsigned long value);
        void reset();
        bool hasTriggered();
};
