#include <iostream>
#include <optional>

struct isQualClass
{
private:
    /* data */
public:
    isQualClass(/* args */){};
    ~isQualClass(){};
    void print_isQualClass(){std::cout << "isQualClass "<< std::endl;};
    enum class isQualEnum
    {
        A = 0,
        B = 1,
        C = 2,
        D = 3,
        E = 4,
    };

    static std::string to_string(isQualEnum value)
    {
        switch (value)
        {
        case isQualEnum::A:
            return "A";
        case isQualEnum::B:
            return "B";
        case isQualEnum::C:
            return "C";
        case isQualEnum::D:
            return "D";
        case isQualEnum::E:
            return "E";
        default:
            return "Unknown";
        }
    }
    // isQualEnum to
    // std::optional<isQualEnum> a, b;
    std::optional<isQualEnum> a, b;
    bool operator==(const isQualClass &other) const
    {
        if (this->a == other.a && this->b == other.b)
        {
            return true;
        }
        return false;
    }
    bool operator!=(const isQualClass &other) const
    {
        if (this->a != other.a || this->b != other.b)
        {
            return true;
        }
        return false;
    }
    void operator=(const isQualClass &other)
    {
        this->a = other.a;
        this->b = other.b;
    }

};

int main()
{
    isQualClass t;
    t.print_isQualClass();
    t.a = isQualClass::isQualEnum::C;

    isQualClass p;
    p.print_isQualClass();
    p.a = isQualClass::isQualEnum::D;
    p.b = isQualClass::isQualEnum::E;

    std::cout << "t == p: " << (t == p) << std::endl;
    std::cout << "t != p: " << (t != p) << std::endl;

    std::cout << "a: " << (p.a.has_value() ? static_cast<int>(p.a.value()) : -1) 
              << ", " << (p.b.has_value() ? static_cast<int>(p.b.value()) : -1) << std::endl;
    std::cout << "p has value: " << p.b.has_value() << std::endl;
    p = t;
    std::cout << "p == t: " << (p == t) << std::endl;
    std::cout << "p != t: " << (p != t) << std::endl;
    std::cout << "p has value: " << p.b.has_value() << std::endl;
    std::cout << "a: " << (p.a.has_value() ? static_cast<int>(p.a.value()) : -1) 
              << ", " << (p.b.has_value() ? static_cast<int>(p.b.value()) : -1) << std::endl;
    // switch (p.b.value())
    // {
    // case isQualClass::isQualEnum::D:
    //     std::cout << "p.b == D";
    //     break;
    // default:
    //     std::cout << "p.b is default" << std::endl;
    //     break;
    // }
    std::string s = p.b.has_value() ? std::to_string(static_cast<int>(p.b.value())) : "no value";
    std::cout << "p.b: " << s << std::endl;
    std::optional<isQualClass> opt = isQualClass();
    opt.value().a = isQualClass::isQualEnum::D;
    opt.value().b = isQualClass::isQualEnum::E;
    std::cout << "opt has value: " << opt.has_value() << std::endl;
    std::cout << "opt.a: " << (opt.value().a.has_value() ? static_cast<int>(opt.value().a.value()) : -1) <<
                 ", opt.b: " << (opt.value().b.has_value() ? static_cast<int>(opt.value().b.value()) : -1) << std::endl;
    return 0;
}