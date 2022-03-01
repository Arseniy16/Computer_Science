#ifndef _TVIEW_H_
#define _TVIEW_H_

#include "view.h"

class TView : public View
{
private:
    int tx, ty, tz;

public:
    int print_value(int x, int y, int z);

};

#endif //_TVIEW_H_