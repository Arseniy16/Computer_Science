#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <list>
#include <utility>
#include <random>
#include <algorithm>

using botController = std::function<void ()>;

using Point = std::pair<int, int>;
using Rabbit = Point;
using drawer = std::function<bool ()>;
using keyHandler = std::function<void (int )>;

enum class Color : int
{
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    DEFAULT = 9
};

enum class Direction : int
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    NO_DIRECTION
};

enum class Status : int
{
    LOST,
    CATCH_RABBIT = 1,
    ALIVE,
    CATCH_MOOD
};

struct Snake
{
    std::list<Point> body;

    Direction direction = Direction::RIGHT;
    Direction prev_direction;

    Status status = Status::ALIVE;
    Color color = Color::DEFAULT;

    bool player = true;
};

class Model
{
public:
    Model();
    ~Model();

    bool drawGame();
    void handleKey();

    Snake& createStartSnake();
    Snake& createBotSnake();
    void setBotController(botController update);
    void getMood();

    std::list<Rabbit>& getRabbits() { return rabbits_; }
    std::list<Snake>& getSnakes() { return snakes_; }

private:
    std::list<Rabbit> rabbits_;
    std::list<Snake> snakes_;
    std::list<Point> moods_;

    std::vector<botController> botUpdaterVec_;

    const Point getPoint();
    const Point makePointValid(Point& point);

    std::random_device gen_;

    const int rabbitSpawnTime_ = 5;
    const int moodSpawnTime_ = 5;

    const int MaxRabbits = 20;

    const unsigned int MaxMoods = 150;

    void SnakeMove(Snake& snake);
    void checkCrash();
};

#endif //_MODEL_HPP_