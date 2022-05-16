#include "graphView.hpp"
#include "option.hpp"
#include "model.hpp"

GraphView::GraphView(): maxX_(60), maxY_(60), pixelSize_(15), window_(sf::VideoMode(maxX_ * pixelSize_, maxY_ * pixelSize_), "SNAKE GAME")
{
    window_.setFramerateLimit(20);
}

void GraphView::run()
{
    while (window_.isOpen())
    {
        sf::Event event;
        bool ifEnd = false;

        while (window_.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || (event.key.code == sf::Keyboard::Q))
                window_.close();

            if (!ifEnd)
            {
                if (event.type == sf::Event::KeyPressed)
                    keyHandlerFunc(Option::adaptKey(event.key.code));
            }
        }

        window_.clear();
        ifEnd = drawGame();
        window_.display();
        
        if (ifEnd)
        {
            drawLost();
            window_.display();
            sleep(2);
            window_.close();
        }  
    }

    return;
}

void GraphView::drawGameBoard()
{
    return;
}

void GraphView::draw(const Rabbit& rabbit)
{
    sf::CircleShape figure(pixelSize_ * 0.58);
    figure.setFillColor(sf::Color::Red);
    figure.setPosition(sf::Vector2f(rabbit.second * pixelSize_, rabbit.first * pixelSize_));
    window_.draw(figure);

    return;
}

void GraphView::draw(const Snake& snake)
{
    auto it = snake.body.begin();

    sf::CircleShape figure(pixelSize_ * 0.56, 3);

    if (snake.direction == Direction::UP)
    {
        figure.setRotation(0.f);
        figure.setPosition(sf::Vector2f(it->second * pixelSize_, it->first * pixelSize_));
    }    

    if (snake.direction == Direction::RIGHT)
    {
        figure.setRotation(90.f);
        figure.setPosition(sf::Vector2f((it->second + 1) * pixelSize_, it->first * pixelSize_));
    } 

    if (snake.direction == Direction::DOWN)
    {
        figure.setRotation(180.f);
        figure.setPosition(sf::Vector2f((it->second + 1) * pixelSize_, (it->first + 1) * pixelSize_));
    }    

    if (snake.direction == Direction::LEFT)
    {
        figure.setRotation(270.f);
        figure.setPosition(sf::Vector2f((it->second) * pixelSize_, (it->first + 1) * pixelSize_));
    }    


    figure.setFillColor(Option::adaptColGView(snake.color));
    window_.draw(figure);

    for (++it; it != snake.body.end(); it++)
    {
        sf::RectangleShape figure;
        figure.setSize(sf::Vector2f(pixelSize_, pixelSize_));
        figure.setFillColor(Option::adaptColGView(snake.color));
        figure.setPosition(sf::Vector2f(it->second * pixelSize_, it->first * pixelSize_));
        window_.draw(figure);
    }

    return;
}

void GraphView::drawSpace(const Point& point)
{
    sf::RectangleShape blackPoint;
    blackPoint.setSize(sf::Vector2f(pixelSize_, pixelSize_));
    blackPoint.setFillColor(Option::adaptColGView(Color::BLACK));
    blackPoint.setPosition(sf::Vector2f(point.second * pixelSize_, point.first * pixelSize_));
    window_.draw(blackPoint);
    return;
}

void GraphView::clearSnake(const Snake& snake)
{
    for (auto snakeIt : snake.body)
    {
        drawSpace(snakeIt);
    }

    return;
}

void GraphView::drawLost()
{    
    sf::RectangleShape box;
    box.setSize(sf::Vector2f(getMaxY()/2 * pixelSize_, getMaxX()/2 * pixelSize_));
    box.setFillColor(sf::Color(0, 191, 255, 128));
    box.setPosition(sf::Vector2f(getMaxY()/4 * pixelSize_, getMaxX()/4 * pixelSize_));
    window_.draw(box);
    
    sf::Font font;
    font.loadFromFile("fonts/OpenSans-Bold.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString("YOU LOST!!!");
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    text.setPosition(sf::Vector2f((getMaxY()/2 - 8) * pixelSize_, getMaxX()/2 * pixelSize_));
    
    window_.draw(text);

    return;
}

void GraphView::setDrawer(drawer drawFunc)
{
    drawGame = drawFunc;
    return;
}

void GraphView::setKeyHandler(keyHandler keyHandler)
{
    keyHandlerFunc = keyHandler;
    return;
}

const int GraphView::getMinY() { return 0; }

const int GraphView::getMinX() { return 0; }

const int GraphView::getMaxX() { return maxX_; }

const int GraphView::getMaxY() { return maxY_; }
