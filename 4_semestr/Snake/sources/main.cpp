#include <cstdio>
#include "control.hpp"
#include "view.hpp"
#include "model.hpp"

int main(int argc, char* argv[])
{
    View * view = nullptr;

    if (argc == 1)
        view = View::getView("text");
    else
    {
        view = View::getView(argv[1]);
    }

    Model model;
    //Human player(model.createStartSnake());
    DumbRobot dumb1(model);
    LessDumbRobot dumb2(model);
    
    view->run();

    delete view;

    return 0;
}