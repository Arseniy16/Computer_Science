#include "control.hpp"
#include "model.hpp"
#include "view.hpp"

Human::Human(Snake& snake) : snake_(snake)
{
    auto view = View::getView();
    view->setKeyHandler(std::bind(&Human::handleKey, this, std::placeholders::_1));
    snake.color = Color::YELLOW;
}

void Human::handleKey(char key)
{
    key = tolower(key);

    switch(key)
    {
        case 'w': 
            if (snake_.direction != Direction::DOWN)
                snake_.direction = Direction::UP;
            break;

        case 's': 
            if (snake_.direction != Direction::UP)
                snake_.direction = Direction::DOWN;
            break;

        case 'a': 
            if (snake_.direction != Direction::RIGHT)
                snake_.direction = Direction::LEFT;
            break;

        case 'd': 
            if (snake_.direction != Direction::LEFT)
                snake_.direction = Direction::RIGHT;
            break;

        default: break;
    }

    return;
}

Human::~Human() { }


Robot::Robot(Model& model) : snake_(model.createBotSnake()), rabbits_(model.getRabbits()), snakes_(model.getSnakes())
{
    model.setBotController(std::bind(&Robot::control, this));
}


DumbRobot::DumbRobot(Model& model) : Robot::Robot(model)
{
    snake_.color = Color::CYAN;
}

void DumbRobot::control()
{
    auto rabbitToEat = rabbits_.front();
    auto snakeHead = snake_.body.begin();

    if ((rabbitToEat.first > snakeHead->first))
    {
        snake_.direction = Direction::DOWN;
        return;
    }
    if ((rabbitToEat.first < snakeHead->first))
    {
        snake_.direction = Direction::UP;
        return;
    }
    if ((rabbitToEat.second > snakeHead->second))
    {
        snake_.direction = Direction::RIGHT;
        return;
    }
    if((rabbitToEat.second < snakeHead->second))
    {
        snake_.direction = Direction::LEFT;
        return;
    }
    return;
}

LessDumbRobot::LessDumbRobot(Model& model) : Robot::Robot(model)
{
    snake_.color = Color::GREEN;
}

bool LessDumbRobot::ifDirectionOK(Direction direction)
{
    auto futureHeadPos = *(snake_.body.begin());
    switch(direction)
    {
        case Direction::UP:  
            (futureHeadPos.first)--;
            
            if (futureHeadPos.first == View::getView()->getMinX())
                return false;
            break;

        case Direction::DOWN:    
            (futureHeadPos.first)++;
            
            if (futureHeadPos.first == View::getView()->getMaxX())
                return false;
            break;

        case Direction::LEFT:  
            (futureHeadPos.second)--;

            if (futureHeadPos.second == View::getView()->getMinY())
                return false;
            break;

        case Direction::RIGHT: 
            (futureHeadPos.second)++;
            
            if (futureHeadPos.second == View::getView()->getMaxY())
                return false;
            break;

        default: return false;
    }

    for (auto snakeIt : snakes_)
    {
        for (auto bodyIt : snakeIt.body)
        {
            if (bodyIt == futureHeadPos)
                return false;
        }
    }

    return true;
}

void LessDumbRobot::control()
{
    int minDist = -1;
    auto snakeHead = snake_.body.begin();
    auto rabbitToEat = rabbits_.end();
    for (auto it = rabbits_.begin(); it != rabbits_.end(); ++it)
    {
        int dist = abs(it->first - snakeHead->first) + abs(it->second - snakeHead->second);

        if ((dist < minDist) || (minDist == -1))
        {
            rabbitToEat = it;
            minDist = dist;
        }
    }

    if (rabbitToEat == rabbits_.end())
    {
        return;
    }

    if ((rabbitToEat->first > snakeHead->first) && ifDirectionOK(Direction::DOWN))
    {
        snake_.direction = Direction::DOWN;
        return;
    }
    
    if ((rabbitToEat->first < snakeHead->first) && ifDirectionOK(Direction::UP))
    {
        snake_.direction = Direction::UP;
        return;
    }

    if ((rabbitToEat->second > snakeHead->second) && ifDirectionOK(Direction::RIGHT))
    {
        snake_.direction = Direction::RIGHT;
        return;
    }

    if((rabbitToEat->second < snakeHead->second) && ifDirectionOK(Direction::LEFT))
    {
        snake_.direction = Direction::LEFT;
        return;
    }
    
    for (int dir = (int)Direction::UP; dir != (int)Direction::NO_DIRECTION; dir++)
    {
        if (ifDirectionOK((Direction)dir))
        {
            snake_.direction = (Direction)dir;
            return;
        }
    }

    return;
}
