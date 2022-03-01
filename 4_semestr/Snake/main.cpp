#include <stdio.h>
#include "view.h"
#include <string>

int main(int argc, char * argv[])
{
    if ((argv[1] == NULL) || (std::string(argv[1]) == "text"))
    {
        View * v = View::getView("text"); //text or gui
    }
    else
    {
        View * v = View::getView(argv[1]);
    }

    return 0;
}