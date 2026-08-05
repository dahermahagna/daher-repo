#include <iostream>
using namespace std;
class string_cpp
{
private:
    /* data */
public:
    string_cpp(/* args */){};
    ~string_cpp(){};
    void print_string_cpp(){std::cout << "string_cpp "<< std::endl;};

};

void print_string(string& t)
{
    std::cout << "string_cpp print_string: " << t << std::endl;
}

void add_string(string* t, char * arrp = NULL)
{
    char arr[] = "Array of chars";
    char arr2[] = " with more chars";
    // char * arrp="";
    char arr3[1000];
    if (arrp == NULL) {
        arrp = (char *)" and a pointer to chars";
    }
    snprintf(arr3, sizeof(arr3), "%s%s: %s", arr, arr2, arrp);
    *t = arr3;
}
int main()
{
    string_cpp t;
    t.print_string_cpp();
    string str = "Hello, World!";
    str += " from C++17";
    print_string(str);
    string arr3;
    char * arrp ={0};
    add_string(&arr3, arrp);

    printf("Formatted string: %s\n", arr3.c_str());
    string empty_str;
    arr3 = arr3 + empty_str;
    printf("After concatenation with empty string: %s\n", arr3.c_str());
    return 0;
}
