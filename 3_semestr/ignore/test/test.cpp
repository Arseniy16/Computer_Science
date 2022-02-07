#include <iostream>

int foo(int x)
{
	return x;
}

int& foo1(int x)
{
	return x;
}
const int& foo2(int x)
{
	return x;
}
int main()
{
    int x = 0;
    int a = 5, b = 4, c = 3;
   	if (x) std::cout << "good";
    //(x += foo2(a))++;

    //a = (b = c);
    std::cout << a << " " << b << " " << c << std::endl;
    std::cout << x;
    //cout<<a<<" " << b << " " << c; 
    return 0;
}