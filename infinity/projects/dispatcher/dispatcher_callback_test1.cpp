#include <iostream>
#include"dispatcher_callback_impl.hpp"

//using namespace std;
using namespace ilrd;

class Event
{
public:
private:
};

class Observer
{
public:
    void Notify(Event&) { std::cout << "Noyify" << std::endl; }
    void Death() { std::cout << "Death" << std::endl; }

private:
    /* data */
};

int main()
{
    Observer o;
    Callback<Event, Observer> cb(&o, &Observer::Notify, &Observer::Death);
    Dispatcher<Event> d;


    d.Subscribe(&cb);

    d.Broadcast(Event());

    sleep(2);

    return 0;
}