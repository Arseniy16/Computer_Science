#ifndef _VIEW_HPP_
#define _VIEW_HPP_

#include "model.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <list>
#include <utility>
#include <functional>


inline bool operator==(const Snake& lhs, const Snake& rhs)
{
    return (lhs.body == rhs.body) && (lhs.direction == rhs.direction)
    && (lhs.status == rhs.status) && (lhs.color == rhs.color);
}

class View
{
public:
    View() = default;
    virtual ~View() = default;

    static View* getView(const char* viewType = nullptr);

    virtual void drawGameBoard() = 0;
    virtual void run() = 0;
    virtual void draw(const Rabbit& rabbit) = 0;
    virtual void draw(const Snake& snake) = 0;
    virtual void drawSpace(const Point& point) = 0;
    virtual void clearSnake(const Snake& snake) = 0;
    virtual void drawLost() = 0;

    virtual void setDrawer(drawer drawerFunc) = 0;
    virtual void setKeyHandler(keyHandler keyHandlerFunc) = 0;

    virtual const int getMaxX() = 0;
    virtual const int getMaxY() = 0;
    virtual const int getMinX() = 0;
    virtual const int getMinY() = 0;

private:
    static View* viewObj;
};

#endif //_VIEW_HPP_