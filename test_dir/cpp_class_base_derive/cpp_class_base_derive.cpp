#include <iostream>

class cpp_class_base
{
private:
    /* data */
public:
    cpp_class_base(const std::string& tranName = "Unknown"):isFlag(false){std::cout << "cpp_class_base "<< tranName << std::endl;};
    ~cpp_class_base(){};
    void print_cpp_class_base(){std::cout << "cpp_class_base "<< std::endl;};
    void print_isFlag(){std::cout << isFlag << std::endl;}

    virtual bool get_isFlag() const { return false; }
    void print_get_isFlag() const {
        std::cout << "isFlag: " << get_isFlag() << std::endl;
    }
protected:
    bool isFlag = false; 
    bool isFlag2 = false; // Example of another flag, not used in this context
};

class cpp_class_derive : public cpp_class_base
{
private:
    /* data */
public:
    cpp_class_derive(bool isTrue, bool isfalse){isFlag = isTrue; isFlag2 = isfalse; std::cout << "cpp_class_derive "<< std::endl;};
    ~cpp_class_derive(){};
    void print_cpp_class_derive(){std::cout << "cpp_class_derive "<< std::endl;};
    bool get_isFlag() const override { return true; } // Override to return the derived class flag
};

class cpp_class_derive2 : public cpp_class_base
{
private:
    /* data */
public:
    cpp_class_derive2(/* args */){std::cout << "cpp_class_derive2 "<< std::endl;};
    ~cpp_class_derive2(){};
    void print_cpp_class_derive2(){std::cout << "cpp_class_derive2 "<< std::endl;};
};

int main()
{
    cpp_class_base t;
    cpp_class_derive t2(true, false);
    // cpp_class_derive2 t3;
    // t.print_isFlag();
    t2.print_isFlag();
    std::cout << "t2.get_isFlag(): " << t2.get_isFlag() << " derived print_isFlag";
    t2.print_get_isFlag();

    std::cout << "t.get_isFlag(): " << t.get_isFlag() << " base print_isFlag";
    t.print_get_isFlag();
    // t3.print_isFlag();

    return 0;
}
