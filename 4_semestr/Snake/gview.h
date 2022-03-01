#ifndef _GVIEW_H_
#define _GVIEW_H_

#include "view.h"

class GView : public View
{
private:
    int gx, gy, gz;
public:
    int print_value(int x, int y, int z);
};

#endif //_GVIEW_H_