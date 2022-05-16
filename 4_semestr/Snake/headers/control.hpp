#ifndef _CONTROL_HPP_
#define _CONTROL_HPP_

#include <functional>
#include <cmath>
#include <algorithm>

#include "model.hpp"

class Human
{

public:
    Human(Snake& snake);
    ~Human();

private:

    Snake& snake_;

    void handleKey(char key) ;
};

class Robot
{
public:
    Robot(Model& model);
    virtual ~Robot() = default;

protected:

    Snake& snake_;
    std::list<Rabbit>& rabbits_;
    std::list<Snake>& snakes_;
    virtual void control() {return;};
};


class DumbRobot : public Robot
{
public:
    DumbRobot(Model& model);
    ~DumbRobot() override = default;

private:
    void control() override;
};

class LessDumbRobot : public Robot
{
public:
    LessDumbRobot(Model& model);
    ~LessDumbRobot() override = default;

private:
    bool ifDirectionOK(Direction direction);
    void control() override;
};

#endif //_CONTROL_HPP_