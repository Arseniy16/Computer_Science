#include <sys/ioctl.h>
#include <termios.h>
#include <signal.h>
#include <functional>
#include <poll.h>

#include "textView.hpp"
#include "model.hpp"

struct termios oldSettings;
bool final = false;

void sigHandler(int sig)
{
    final = true;

    return;
}

TextView::TextView()
{
    setvbuf (stdout, NULL, _IONBF, 0);

    winsize currentTerminalSize;

    ioctl(1, TIOCGWINSZ, &currentTerminalSize);
    ioctl(1, TIOCSWINSZ, &currentTerminalSize);

    maxX_ = currentTerminalSize.ws_row;
    maxY_ = currentTerminalSize.ws_col;

    struct termios terminalSettings;

    tcgetattr(0, &terminalSettings);
    oldSettings = terminalSettings;

    cfmakeraw(&terminalSettings);
    terminalSettings.c_lflag |= ISIG;
    tcsetattr(0, TCSANOW, &terminalSettings);

    signal(SIGINT, sigHandler);
}

void TextView::setDrawer(drawer drawFunc) 
{ 
    drawGame = drawFunc; 
}

void TextView::setKeyHandler(keyHandler keyHandler) 
{ 
    keyHandlerFunc = keyHandler; 
}

const int TextView::getMinY() { return 1; }

const int TextView::getMinX() { return 3; }

const int TextView::getMaxX() { return maxX_; }

const int TextView::getMaxY() { return maxY_; }

void TextView::run()
{
    struct pollfd fds = {0, POLLIN};

    clearField();
    drawGameBoard();

    bool lost = false;
    drawGame();

    while (!final)
    {
        int pollRes = poll (&fds, 1, speedGame_);
        if (pollRes == 1)
        {
            char keyBuf;

            if (read(0, &keyBuf, 1) > 0)
            {
                if (keyBuf == 'q')
                {
                    final = true;
                    break;
                }
                else
                {
                    keyHandlerFunc(keyBuf);
                    lost = drawGame();
                    
                    if (lost)
                    {
                        drawLost();
                        final = true;
                        usleep(500000);
                        return;
                    }
                }
            }

        }
        else
        {
            lost = drawGame();
            if (lost)
            {
                drawLost();
                final = true;
                usleep(500000);
                return;
            }
        }
 
    }

    return;
}

void TextView::draw(const Rabbit& rabbit)
{   
    setColor(Color::RED);
    setRegime(1);
    gotoxy(rabbit.first, rabbit.second);
    putchar('@');
    resetColor();

    return;
}

void TextView::draw(const Snake& snake)
{
    setRegime(1);

    auto it = snake.body.begin();

    gotoxy(*it);

    setColor(snake.color);
    switch (snake.direction)
    {
        case Direction::UP : 
            putchar('^'); break;

        case Direction::DOWN: 
            putchar('v'); break;
        
        case Direction::LEFT: 
            putchar('<'); break;
        
        case Direction::RIGHT: 
            putchar('>'); break;
        
        default : break;
    }

    auto lastToDraw = snake.body.end();
    if (snake.body.front() == snake.body.back())
        lastToDraw--;

    for (it++; it != lastToDraw; it++)
    {
        gotoxy(*it);
        putchar('#');
    }
    resetColor();

    setRegime(0);

    return;
}

void TextView::drawMood(const Point& point)
{
    setColor(Color::GREEN);
    gotoxy(point.first, point.second);
    putchar('*');
    resetColor();

    return;
}


void TextView::drawSpace(const Point& point)
{
    gotoxy(point);
    putchar(' ');

    return;
}

void TextView::clearSnake(const Snake& snake)
{
    for (auto snakeIt : snake.body)
    {
        drawSpace(snakeIt);
    }

    return;
}

void TextView::drawLost()
{
    setColor(Color::GREEN);
    setRegime(1);
    drawRectangle(maxX_/4, maxY_/4, maxX_/2, maxY_/2);
    
    setColor(Color::WHITE);
    setRegime(6);
    setRegime(7);
    gotoxy(getMaxX()/2, (getMaxY() - strlen("YOU LOST!")) / 2);
    putstr("YOU LOST!");
    
    sleep(2);
    resetColor();
    
    return;
}

void TextView::drawRectangle(const int x, const int y, const int lenX, const int lenY)
{
    hline(x, y+1, lenY);
    hline(x + lenX, y+1, lenY);
    vline(x+1, y, lenX);
    vline(x+1, y + lenY, lenX);

    gotoxy(x, y);
    putchar('+');
    usleep(10000);

    gotoxy(x + lenX, y);
    putchar('+');
    usleep(10000);

    gotoxy(x + lenX, y + lenY);
    putchar('+');
    usleep(10000);

    gotoxy(x, y + lenY);
    putchar('+');
    usleep(10000);

    return;
}

void TextView::drawGameBoard()
{      
    setRegime(1);
    setColor(Color::YELLOW);

    drawRectangle(1, 1, maxX_, maxY_);
    
    gotoxy(2, maxY_/2);

    setColor(Color::GREEN);
    puts("SNAKE");

    setColor(Color::YELLOW);
    hline(3, 2, maxY_-2);

    resetColor();

    return;
}

void TextView::gotoxy(const int x, const int y)
{
    printf("\e[%d;%dH", x, y);
    return;
}

void TextView::gotoxy(const Point& point)
{
    printf("\e[%d;%dH", point.first, point.second);
    return;
}

void TextView::putchar(char c)
{
    printf("%c", c);
    return;
}

void TextView::putstr(const char* str)
{
    printf("%s", str);
    return;
}

void TextView::setColor(Color color)
{
    //printf ("\e[m");
    printf ("\e[%dm", Option::adaptColTView(color));
    return;
}

void TextView::resetColor()
{
    printf("\e[m");
    return;
}

void TextView::setRegime(const unsigned int value)
{
    printf("\e[%dm", value);
    return;
}

void TextView::hline(const int x, int y, const int len)
{
    for (int i = y; i < y + len; ++i)
    {   
        printf("\e[%d;%dH-", x, i);
        usleep(5000);
    }

    return;
}

void TextView::vline(int x, const int y, const int len)
{
    for (int i = x; i < x + len; ++i)
    {  
        printf("\e[%d;%dH|", i, y);
        usleep(5000);
    }

    return;
}

void TextView::clearField()
{
    gotoxy(0, 0);
    printf("\eH\e[J");
    return;
}

TextView::~TextView()
{
    sleep(1);
    resetColor();
    gotoxy(1, 1);
    clearField();
    tcsetattr(0, TCSANOW, &oldSettings);
    setvbuf (stdout, NULL, _IOFBF, 0);
}