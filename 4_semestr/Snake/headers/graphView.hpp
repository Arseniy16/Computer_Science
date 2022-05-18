#ifndef _GRAPHVIEW_HPP_
#define _GRAPHVIEW_HPP_

#include <SFML/Graphics.hpp>

#include "view.hpp"
#include "option.hpp"
#include "model.hpp"

class GraphView : public View
{
public:
    GraphView();
    ~GraphView() override = default;

    void drawGameBoard() override;
    void run() override;

    void draw(const Rabbit& rabbit) override;
    void draw(const Snake& snake) override;
    void drawMood(const Point& point) override;
    void drawSpace(const Point& point) override;
    
    void clearSnake(const Snake& snake) override;
    void drawLost() override;

    void setDrawer(drawer drawerFunc) override;
    void setKeyHandler(keyHandler keyHandlerFunc) override;

    const int getMaxX() override;
    const int getMaxY() override;
    const int getMinX() override;
    const int getMinY() override;

private:

    int maxX_;
    int maxY_;
    int pixelSize_;

    sf::RenderWindow window_;

    drawer drawGame;
    keyHandler keyHandlerFunc;

};

#endif //_GRAPHVIEW_HPP_