#ifndef _TEXTVIEW_HPP_
#define _TEXTVIEW_HPP_

#include <chrono>

#include "view.hpp"
#include "option.hpp"
#include "model.hpp"

class TextView : public View
{
public:
    TextView();
    ~TextView() override;

    void drawGameBoard() override;
    void run() override;

    void drawMood(const Point& point) override;

    void draw(const Rabbit& rabbit) override;
    void draw(const Snake& snake) override;    
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

    drawer drawGame;
    keyHandler keyHandlerFunc;

    int maxX_;
    int maxY_;

    const int speedGame_ = 100;

    void gotoxy(const int x, const int y);
    void gotoxy(const Point& point);
    void drawRectangle(const int x, const int y, const int lenX, const int lenY);


    void putchar(char c);
    void putstr(const char* str);

    void setColor(Color color);
    void resetColor();
    void setRegime(const unsigned int value);

    void hline(const int x, int y, const int len);
    void vline(int x, const int y, const int len);

    void clearField();

};

#endif //_TEXTVIEW_HPP_