#include <iostream>
#include <map>
#include <cstdint>

class emplace_return
{
private:
    /* data */
public:
    emplace_return(/* args */){};
    ~emplace_return(){};
    void print_emplace_return(){std::cout << "emplace_return "<< std::endl;};

};
static void
fourth_db(char *buf, int32_t value)
{
    if (value < 0)
        snprintf(buf, 256, "-%.2f dB", -value/4.0);
    else
        snprintf(buf, 256, "%.2f dB", value/4.0);
}
// void
// signed_fourth_db(char *buf, int16_t value)
// {
// //   if (value & 0x80000000)
// //     value = (~value + 1) & 0xFFFFFFFF;
// //     abs
//   snprintf(buf, 256, "%s%.2f dB", (value & 0x8000) ? "-" : "", abs(value)/4.0);
// }
static void
signed16_fourth_db(char *buf, int16_t value)
{
    snprintf(buf, 256, "%.2f dB", value / 4.0);
}

int main()
{
    // emplace_return t;
    // t.print_emplace_return();
    // std::map<int, int> m;
    // bool inserted = false;
    // try
    // {

    //     auto [result, was_inserted] = m.emplace(1, 2);
    //     inserted = was_inserted;
    //     std::cout << "Inserted: " << std::boolalpha << inserted << ", Value: " << result->second << std::endl;
    //     throw std::runtime_error("Simulated exception");
    // }
    // catch(...){
    //     if (inserted)
    //     {
    //         m.erase(1);
    //         std::cout << "Rolled back insertion." << std::endl;
    //     }
    //     std::cout << "Exception occurred during emplace." << std::endl;
    // }
    char buf[256] = {};
    signed16_fourth_db(buf,(int16_t)0xffea);
    printf("buf:%s\n",buf); 
    int8_t x = -8;
    signed16_fourth_db(buf,x);
    printf("buf:%s\n",buf);

    signed16_fourth_db(buf,0xfff8);
    printf("buf:%s\n",buf);

    return 0;
}
