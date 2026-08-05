#include <iostream>

class draw_diagram
{
private:
    /* data */
public:
    draw_diagram(/* args */){};
    ~draw_diagram(){};
    void print_draw_diagram(){std::cout << "draw_diagram "<< std::endl;};

};

int main()
{
    draw_diagram t;
    t.print_draw_diagram();
    return 0;
}
