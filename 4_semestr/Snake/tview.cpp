#include <stdio.h>
#include "tview.h"

int TView::print_value(int x, int y, int z)
{
    printf("TX = %d, TY = %d, TZ = %d\n", x, y, z);
    return 0;
}
