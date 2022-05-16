#include "view.hpp"
#include "textView.hpp"
#include "graphView.hpp"

View * View::viewObj = nullptr;

View * View::getView(const char * viewType)
{
    if (viewObj)
        return viewObj;
    
    if (viewType == nullptr)
        return nullptr;

    if (!strcmp(viewType, "gui"))
    {
        viewObj = new GraphView;
        return viewObj;
    }
    else
    {
        viewObj = new TextView;
        return viewObj;
    }
}