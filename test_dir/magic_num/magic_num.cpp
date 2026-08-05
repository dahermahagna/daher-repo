#include <iostream>

class magic_num
{
private:
    /* data */
public:
    magic_num(/* args */){};
    ~magic_num(){};
    void print_magic_num(){std::cout << "magic_num "<< std::endl;};

};

int order_digits(int a, bool from_big_to_small)
{
    int result = 0;
    // 4 digit number
    int d1 = a / 1000;
    int d2 = (a / 100) % 10;
    int d3 = (a / 10) % 10;
    int d4 = a % 10;
    int arr[4] = {d1, d2, d3, d4};
    // bubble sort
    for (int i = 0; i < 4 - 1; i++)
    {
        for (int j = 0; j < 4 - i - 1; j++)
        {
            if ((from_big_to_small && arr[j] < arr[j + 1]) ||
                (!from_big_to_small && arr[j] > arr[j + 1]))
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    result = arr[0] * 1000 + arr[1] * 100 + arr[2] * 10 + arr[3];
    return result;
}

int main()
{
    int a;
    while (true)
    {
        // input 4 digit number
        std::cout << "Input a 4 digit number: ";
        std::cin >> a;
        if (a >= 1000 && a <= 9999)
        {

            std::cout << order_digits(a, true) << std::endl;
            std::cout << order_digits(a, false) << std::endl;
            while (a != 6174)
            {
                int big = order_digits(a, true);
                int small = order_digits(a, false);
                a = big - small;
                std::cout << big << " - " << small << " = " << a << std::endl;
            }
        }
    }
    return 0;
}
