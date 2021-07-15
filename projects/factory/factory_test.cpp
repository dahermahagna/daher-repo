#include <iostream> //cout
#include <boost/make_shared.hpp> // make shared

#include "factory.hpp"
#include "factory_imp.hpp"

#include "../utils/test.hpp"

using namespace std;
using namespace ilrd;

class Animal
{
public:
    virtual ~Animal();
    virtual void eat() = 0;
    virtual void PrintMealHours() = 0;
};

Animal::~Animal()
{}

class Lion : public Animal
{
private:
    int m_lion_meal_hours;

public:
    Lion(int lion_meal_hours_);
    ~Lion();
    virtual void eat();
    void PrintMealHours() {cout << "meal every " << m_lion_meal_hours <<" hours" << endl;};
};

Lion::Lion(int lion_meal_hours_)
: m_lion_meal_hours(lion_meal_hours_)
{}

Lion::~Lion()
{}

void Lion::eat()
{
    cout << "I eating Meat every " <<m_lion_meal_hours << " hours"<< endl;
}

boost::shared_ptr<Animal> CreateLion(int lion_num_)
{
    return boost::shared_ptr<Animal>(new Lion(lion_num_));
}

boost::shared_ptr<Animal> CreateLionOverride(int lion_num_)
{
    return boost::shared_ptr<Animal>(new Lion(lion_num_ + 100));
}


static test_status_t TestAddAndCreate(void);
static test_status_t TestOverrideFunction(void);
static test_status_t TestException(void);

int main()
{

    RUNTEST(TestAddAndCreate);
    RUNTEST(TestOverrideFunction);
    RUNTEST(TestException);
 
    return 0;
}

static test_status_t TestAddAndCreate(void)
{
    const int lion_ID = 1;

    Factory<Animal, int, int> animals;

    animals.Add(lion_ID, CreateLion);


    boost::shared_ptr<Animal> lion1 = animals.Create(lion_ID, 1);
    lion1->eat();
    lion1->PrintMealHours();

    boost::shared_ptr<Animal> lion2 = animals.Create(lion_ID, 2);
    lion2->eat();
    lion2->PrintMealHours();

  
    REQUIRE(1 == 1);

    return PASSED;
}

static test_status_t TestOverrideFunction(void)
{
    const int lion_ID = 1;

    Factory<Animal, int, int> animals;

    animals.Add(lion_ID, CreateLion);

    boost::shared_ptr<Animal> lion1 = animals.Create(lion_ID, 1);
    lion1->eat();
    lion1->PrintMealHours();
    
    cout << "over ride the function of creating lion" << endl;
    animals.Add(lion_ID, CreateLionOverride);
    lion1 = animals.Create(lion_ID, 1);
    lion1->eat();
    lion1->PrintMealHours();

    REQUIRE(1 == 1);

    return PASSED;
}

static test_status_t TestException(void)
{
    const int lion_ID = 1;

    boost::function< boost::shared_ptr<Animal> (int) > fun_lion = CreateLion;

    Factory<Animal, int, int> animals;

    animals.Add(lion_ID, fun_lion);

    try
    {
        boost::shared_ptr<Animal> lion1 = animals.Create(2, 1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    REQUIRE(1 == 1);

    return PASSED;
    
}