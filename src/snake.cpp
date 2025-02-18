#include <Arduino.h>
#include "snake.h"

#define INITIAL_TICK_TIME 500
#define TICK_INCREMENT 15
#define WIN_TIMER 100

void removeTail(SnakeBit *head) {
    SnakeBit *current = head->next;
    if (current == nullptr) {
        return;
    }

    SnakeBit *prev = head;
    while (current->next != nullptr) {
        prev = current;
        current = current->next;
    }

    delete current;
    prev->next = nullptr;
}

uint8_t snakeLength(SnakeBit *head) {
    int length = 0;
    SnakeBit *current = head;
    while (current != nullptr) {
        current = current->next;
        length++;
    }
    return length;
}

void Snake::reset() {
    SnakeBit *current = head.next;
    state = SnakeState::PLAYING;

    while (current != nullptr) {
        SnakeBit *toDelete = current;
        current = current->next;
        delete toDelete;
    }
    head.point.x = 2;
    head.point.y = 2;
    head.next = new SnakeBit();
    head.next->point.x = 1;
    head.next->point.y = 2;

    tickTime = INITIAL_TICK_TIME;
    tickTimer.reset();
    tickTimer.every(tickTime);


    resetTarget();
    score = 0;

    direction.x = 1;
    direction.y = 0;
}

void Snake::win() {
    state = SnakeState::WIN;
    winStep.reset();
    winStep.every(WIN_TIMER);
}

void Snake::gameOver() {
    state = SnakeState::GAME_OVER;
    flashTimer.reset();
    gameOverTimer.reset();
    flashTimer.every(300);
    gameOverTimer.every(2000);
}

void Snake::drawTarget() {
    gameBox->screen.drawPoint(target.x, target.y, 3);
}

void Snake::drawSnake()
{
    gameBox->screen.drawPoint(head.point.x, head.point.y, 2);
    SnakeBit *current = head.next;
    while (current != nullptr)
    {
        gameBox->screen.drawPoint(current->point.x, current->point.y, 1);
        current = current->next;
    }
}

void Snake::updateSnake()
{
    Point lastPoint;
    lastPoint.x = head.point.x;
    lastPoint.y = head.point.y;

    SnakeBit *prev = &head;
    SnakeBit *next = head.next;
    while (next != nullptr)
    {
        int x = next->point.x;
        int y = next->point.y;

        next->point.x = lastPoint.x;
        next->point.y = lastPoint.y;

        lastPoint.x = x;
        lastPoint.y = y;

        prev = next;
        next = next->next;
    }

    head.point.x += direction.x;
    head.point.y += direction.y;

    // < 0 wraps around as it's unsigned
    if (head.point.x == 6)
        head.point.x = 0;
    else if (head.point.x > 6)
        head.point.x = 5;

    if (head.point.y == 4)
        head.point.y = 0;
    else if (head.point.y > 4)
        head.point.y = 3;

    if (
        head.point.x == target.x &&
        head.point.y == target.y)
    {
        score++;
        tickTime -= TICK_INCREMENT;
        tickTimer.every(tickTime);
        gameBox->sevenSegmentDisplay.set(score);
        SnakeBit *end = new SnakeBit();
        end->point.x = lastPoint.x;
        end->point.y = lastPoint.y;
        prev->next = end;
        if (snakeLength(&head) == 24) {
            win();
        } else {
            resetTarget();
        }
    }
    else if (checkCollision(head.point, true)) {
        gameOver();
    }
}

bool Snake::checkCollision(Point point, bool ignoreHead)
{
    SnakeBit *current = &head;
    if (ignoreHead) {
        current = current->next;
    }

    while (current != nullptr)
    {
        if (current->point.x == point.x && current->point.y == point.y)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

void Snake::resetTarget()
{
    // Terrible approach... but it's a glorified jewellery box
    do
    {
        target.x = random(0, 5);
        target.y = random(0, 3);
    } while (checkCollision(target));
}

void Snake::handleInput()
{
    Point newDirection;
    if (gameBox->dpad.isDown(DPadDirection::UP))
    {
        newDirection.y = -1;
        newDirection.x = 0;
    }
    else if (gameBox->dpad.isDown(DPadDirection::DOWN))
    {
        newDirection.y = 1;
        newDirection.x = 0;
    }

    if (gameBox->dpad.isDown(DPadDirection::LEFT))
    {
        newDirection.x = -1;
        newDirection.y = 0;
    }
    else if (gameBox->dpad.isDown(DPadDirection::RIGHT))
    {
        newDirection.x = 1;
        newDirection.y = 0;
    }

    Point previousPart = head.next->point;
    if (
        head.point.x + newDirection.x != previousPart.x && 
        head.point.y + newDirection.y != previousPart.y
    ) {
        direction = newDirection;
    }
}

void Snake::tick()
{
    updateSnake();
    gameBox->screen.clear();
    drawSnake();
    drawTarget();
}

void Snake::setup(GameBox *gameBox)
{
    this->gameBox = gameBox;
    reset();
}

void Snake::update(long delta)
{
    if (state == SnakeState::PLAYING) {
        handleInput();

        tickTimer.update(delta);
        if (tickTimer.hasTriggered())
        {
            tick();
        }
    } else if (state == SnakeState::GAME_OVER) {
        flashTimer.update(delta);
        gameOverTimer.update(delta);

        if (gameOverTimer.hasTriggered()) {
            // Just reusing the win effect
            win();
        } else if(flashTimer.hasTriggered()) {
            gameBox->screen.clear();
            showSnake = !showSnake;
            if (showSnake) {
                drawSnake();
            }
            drawTarget();
        }
    } else if (state == SnakeState::WIN) {
        winStep.update(delta);
        if (winStep.hasTriggered()) {
            if (snakeLength(&head) == 1) {
                reset();
            } else {
                removeTail(&head);
            }
            gameBox->screen.clear();
            drawSnake();
        }
    }
}