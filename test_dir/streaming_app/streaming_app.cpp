#include <iostream>

class streaming_app
{
private:
    /* data */
public:
    streaming_app(/* args */){};
    ~streaming_app(){};
    void print_streaming_app(){std::cout << "streaming_app "<< std::endl;};

};

int main()
{
    streaming_app t;
    t.print_streaming_app();
    return 0;
}
