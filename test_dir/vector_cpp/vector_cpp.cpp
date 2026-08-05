#include <iostream>
#include <vector>

using namespace std;
class vector_cpp
{
private:
    /* data */
public:
    vector_cpp(/* args */){};
    ~vector_cpp(){};
    void print_vector_cpp(){std::cout << "vector_cpp "<< std::endl;};
    int a;

};

int main()
{
    vector_cpp t,p,q;
    t.a = 24;
    p.a = 25;
    q.a = 26;
    vector<vector_cpp> v;
    v.push_back(t);
    v.push_back(p);
    v.push_back(q);
    v.back().a = 27;
    for (long unsigned int i = 0; i < v.size(); i++)
    {
        cout << v[i].a << endl;
    }
    uint8_t a = 255, b = 2;
    auto c = uint16_t(a << 8 | b);
    cout << c << endl;
    cout << (a<<8) << endl;
    cout << sizeof(c) << endl;
    cout << sizeof(bool)    << endl;
    return 0;
}
