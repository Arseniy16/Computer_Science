#include <cstdio>
#include "control.hpp"
#include "view.hpp"
#include "model.hpp"

int main(int argc, char* argv[])
{

    View * v;
    if (argc == 1)
        v = View::getView("text");
    else
    {
        v = View::getView(argv[1]);
    }
    Model model;
    Human humanAI(model.createStartSnake());
    DumbRobot dumbest (model);
    LessDumbRobot dumb (model);
    //LessDumbRobot dumb2 (model);
    
    v->run();

    // sleep(1);

    delete v;

    return 0;
}