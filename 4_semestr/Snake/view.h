#ifndef _VIEW_H_
#define _VIEW_H_

class View
{
private:
    int x, y, z;
    static View * obj;

public:
    int set_value(int x, int y, int z);

    static View * getView(const char * value);
    
};

#endif //_VIEW_H_