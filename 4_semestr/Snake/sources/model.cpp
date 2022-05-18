#include "model.hpp"
#include "view.hpp"

Model::Model()
{
    auto view = View::getView();
    
    view->setDrawer(std::bind(&Model::drawGame, this));
    
    view->setKeyHandler(std::bind(&Model::handleKey, this));

    for (int i = 0; i < MaxRabbits; ++i)
        rabbits_.push_front(getPoint());

    for (int i = 0; i < 5; ++i)
        moods_.push_front(getPoint());

}

Model::~Model(){}

void Model::handleKey() {return;}

Snake& Model::createStartSnake()
{
    Snake snake;
    
    auto Xstart = View::getView()->getMaxX() / 2;
    auto Ystart = View::getView()->getMaxY() / 2;

    snake.body.push_back({Xstart, Ystart});
    snake.body.push_back({Xstart, Ystart - 1});
    snake.body.push_back({Xstart, Ystart - 2});
    snake.body.push_back({Xstart, Ystart - 3});

    snake.color = Color::DEFAULT;

    snake.player = true;

    snakes_.push_back(snake);

    return snakes_.back();
}

Snake& Model::createBotSnake()
{
    Snake snake;
    
    snake.body.push_back(getPoint());
    snake.color = Color::DEFAULT;
    snake.player = false;

    snakes_.push_back(snake);

    return snakes_.back();
}

#if 1
void Model::getMood()
{
    for (auto moodIt : moods_)
    {
        //Point point = std::make_pair(moodIt.first, moodIt.second);

        //if (!makePointValid(point)) continue;

        std::uniform_int_distribution<int> distribution(1, 4);

        switch (distribution(gen_))
        {
            case 1: moods_.push_front({moodIt.first + 1, moodIt.second}); break;
            case 2: moods_.push_front({moodIt.first, moodIt.second + 1}); break;
            case 3: moods_.push_front({moodIt.first - 1, moodIt.second}); break;
            case 4: moods_.push_front({moodIt.first, moodIt.second - 1}); break;
            default : break; 
        }
    }

    return ;
}
#endif

void Model::setBotController(botController MoveControl)
{
    botUpdaterVec_.push_back(MoveControl);
}

const Point Model::getPoint()
{
    std::uniform_int_distribution<int> distributionX(View::getView()->getMinX() + 1, View::getView()->getMaxX() - 1);
    std::uniform_int_distribution<int> distributionY(View::getView()->getMinY() + 1, View::getView()->getMaxY() - 1);
    
    Point newPoint = std::make_pair(distributionX(gen_), distributionY(gen_));

    return makePointValid(newPoint);
}

const Point Model::makePointValid(Point& point)
{
    auto oldPoint = point;
    bool replace = false;

    while(true)
    {
        for (auto snakeIt : snakes_)
            for (auto it : snakeIt.body)
            {
                if (point == it)
                {
                    replace = true;
                    break;
                }
            }

        if (replace)
        {
            if (point.first != 2)
                point.first--;
            else
                point.first++;

            if (oldPoint.first == point.first)
            {
                if (point.second != 2)
                    point.second--;
                else
                    point.second++;
            }

            replace = false;
        }
        else
            break;
    }

    return point;
}

bool Model::drawGame()
{
    auto view = View::getView();
    
    static int ticks = 1;

    for (auto it : rabbits_)
    {
        view->draw(it);
    }

    for (auto mood_it : moods_)
    {
        view->drawMood(mood_it);
    }

    for (auto controlFunc : botUpdaterVec_)
        (controlFunc)();

    for (auto& snakeIt : snakes_)
        SnakeMove(snakeIt);

    checkCrash();

    auto controlFuncIt = botUpdaterVec_.begin();

    for (auto snakeIt = snakes_.begin(); snakeIt != snakes_.end(); ++snakeIt)
    {
        if (snakeIt->status == Status::LOST)
        {
            if (snakeIt->player == true)
                return true;
            else
            {
                if (snakeIt->player == false)
                    {
                        auto funcToErase = controlFuncIt;
                        controlFuncIt--;
                        botUpdaterVec_.erase(funcToErase);
                        controlFuncIt++;
                    }
    
                auto snakeToErase = snakeIt;
                snakeIt--;
                view->clearSnake(*snakeToErase);
                snakes_.erase(snakeToErase);

                continue;
            }
        }
        
        view->draw(*snakeIt);

        if (snakeIt->status != Status::CATCH_RABBIT)
        {
            if (snakeIt->body.front() != snakeIt->body.back())
                view->drawSpace(snakeIt->body.back());
            snakeIt->body.pop_back();
        }
        else
            snakeIt->status = Status::ALIVE;

        if (snakeIt->player == false)
            controlFuncIt++;

    }

    ticks = (ticks + 1) % rabbitSpawnTime_;
    if (!ticks && (rabbits_.size() < static_cast<unsigned int>(MaxRabbits)))
        rabbits_.push_back(getPoint());

    ticks = (ticks + 1) % moodSpawnTime_;
    if (!ticks && (moods_.size() < MaxMoods))
        getMood();

    return false;
}

void Model::checkCrash()
{
    for (auto & currentsnakeIt : snakes_)
    {
        if (currentsnakeIt.status == Status::LOST)
            continue;

        auto head = currentsnakeIt.body.begin();
        for (auto& snakeIt : snakes_)
            if (!(currentsnakeIt == snakeIt))
                for (auto bodyIt : snakeIt.body)
                    if (*head == bodyIt)
                    {
                        currentsnakeIt.status = snakeIt.status = Status::LOST;
                    }
    }
}

void Model::SnakeMove(Snake& snake)
{
    auto head = snake.body.front();

    switch(snake.direction)
    {
        case Direction::UP: (head.first)--;

            if (head.first == View::getView()->getMinX())
                snake.status = Status::LOST;
            break;

        case Direction::DOWN: (head.first)++;

            if (head.first == View::getView()->getMaxX())
                snake.status = Status::LOST;
            break;

        case Direction::LEFT: (head.second)--;

            if (head.second == View::getView()->getMinY())
                snake.status = Status::LOST;
            break;

        case Direction::RIGHT: (head.second)++;

            if (head.second == View::getView()->getMaxY())
                snake.status = Status::LOST;
            break;

        default : break;
    }

    if (snake.status == Status::LOST)
        return;

    for (auto it : snake.body)
    {
        if (head == it)
        {
            snake.status = Status::LOST;
            
            return;
        }
    }

    snake.body.push_front(head);

    for (auto it = rabbits_.begin(); it != rabbits_.end(); it++)
    {
        if (*it == head)
        {
            snake.status = Status::CATCH_RABBIT;
            auto toErase = it;
            it--;
            rabbits_.erase(toErase);
        }
    }

    return;
}

