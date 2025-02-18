#include <Arduino.h>
#include "gameBox.h"
#include "timer.h"

enum SnakeState {
    PLAYING,
    GAME_OVER,
    WIN,
};

struct Point {
  uint8_t x;
  uint8_t y;
};

struct SnakeBit {
  struct Point point;
  SnakeBit *next;
};

class Snake {
    GameBox *gameBox;

    struct Point target;
    struct Point direction;
    SnakeState state;
    SnakeBit head;

    Timer tickTimer;
    Timer gameOverTimer;
    Timer flashTimer;
    Timer winStep;

    int score;
    long tickTime;
    bool showSnake;

    void drawTarget();
    void drawSnake();
    void updateSnake();
    void resetTarget();
    void handleInput();
    void tick();
    void reset();
    void gameOver();
    void win();
    bool checkCollision(Point point, bool ignoreHead = false);

    public:
        void setup(GameBox *gameBox);
        void update(long elapsedMs);
};
