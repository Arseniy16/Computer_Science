#include <stdio.h>
#include "gview.h"

int GView::print_value(int x, int y, int z)
{
    printf("GX = %d, GY = %d, GZ = %d\n", x, y, z);
    return 0;
}