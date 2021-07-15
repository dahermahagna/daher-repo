#include <iostream>
#include "../utils/test.hpp"
#include "dispatcher_callback_impl.hpp"

//----------- Functions Decleration --------------

test_status_t FirstCaseTest(void);
test_status_t SecondCaseTest(void);
test_status_t ThirdCaseTest(void);
test_status_t ForthCaseTest(void);

//-------------------- Main ----------------------

using namespace ilrd;



class Event
{
public:
private:
};

Dispatcher<Event> d1;

class Observer
{
public:
    void Notify(Event&) { std::cout << "\nNoyify" << std::endl; }
    void Death() { std::cout << "Death" << std::endl; }

private:
    /* data */
};

class Observer2
{
public:
    void Notify(Event&) { std::cout << "\nNoyify2" << std::endl; }
    void Death() { std::cout << "Death2" << std::endl; }

private:
    int a;
};

class Observer3
{
public:
    void Notify(Event&) { std::cout << "\nNoyify3" << std::endl; }
    void Death() { std::cout << "Death3" << std::endl; }

private:
    /* data */
};

template <typename EVENT>
class Mediator
{
public:
    explicit Mediator()
    :m_d1()
    {
    }
    ~Mediator()
    {

    }
    void Add() 
    {
        Observer2 ob;
        m_observers.push_front(ob);

        Callback<EVENT, Observer2> cb(&(m_observers.front()), &Observer2::Notify, &Observer2::Death);
        m_callers.push_front(cb);
        m_d1.Subscribe(&(m_callers.front()));
    }
    void Add2()
    {
        Callback<EVENT, Mediator> cb(this, &Mediator::Notify, &Mediator::Death);
        m_callers_m.push_front(cb);
        m_d1.Subscribe(&(m_callers_m.front()));
    }
    void Delete()
    {
        m_callers.pop_back();
    }
    void DeleteAll()
    {
        while(!m_callers.empty())
        {
            m_callers.pop_back();
        }
        while (!m_callers_m.empty())
        {
            m_callers_m.pop_back();
        }
    }
    void Run()
    {
        m_d1.Broadcast(Event());
    }
    void Notify(Event&) 
    {
        std::cout << "\nNoyify Mediator" << std::endl;
        Add();
        Add();
        //Delete();
        DeleteAll();
    }
    void Death() { std::cout << "Death3" << std::endl; }

private:
    std::list<Callback<EVENT,Observer2> > m_callers;
    std::list<Callback<EVENT, Mediator> > m_callers_m;

    std::list<Observer2> m_observers;

    Dispatcher<EVENT> m_d1;
};
int main()
{
    RUNTEST(FirstCaseTest);
    RUNTEST(SecondCaseTest);
    RUNTEST(ThirdCaseTest);
    RUNTEST(ForthCaseTest);

    return 0;
}

//-------------Functions Definetion --------------

test_status_t FirstCaseTest()
{
    Observer o;
    Callback<Event, Observer> cb(&o, &Observer::Notify, &Observer::Death);
    Dispatcher<Event> d;

    d.Subscribe(&cb);

    d.Broadcast(Event());

    sleep(2);

    return PASSED;
}
test_status_t SecondCaseTest()
{
    Observer o1;
    Observer2 o2;

    Callback<Event, Observer> cb1(&o1, &Observer::Notify, &Observer::Death);
    Callback<Event, Observer2> cb2(&o2, &Observer2::Notify, &Observer2::Death);

    Dispatcher<Event> d;

    d.Subscribe(&cb1);
    d.Subscribe(&cb2);

    d.Broadcast(Event());

    sleep(2);

    return PASSED;
}
test_status_t ThirdCaseTest()
{
    Observer o1;
    Observer2 o2;
    Observer3 o3;

    Callback<Event, Observer> cb1(&o1, &Observer::Notify, &Observer::Death);
    Callback<Event, Observer2> cb2(&o2, &Observer2::Notify, &Observer2::Death);
    Callback<Event, Observer3> cb3(&o3, &Observer3::Notify, &Observer3::Death);

    Dispatcher<Event> d;

    d.Subscribe(&cb1);
    d.Subscribe(&cb2);
    d.Subscribe(&cb3);

    d.Broadcast(Event());

    sleep(2);

    return PASSED;
}

test_status_t ForthCaseTest()
{
    Observer2 o2;
    Mediator<Event> m1;
    m1.Add();
    m1.Add();
    m1.Add();
    m1.Add();
    m1.Add();
    m1.Add2();
    m1.Run();
    std::cout <<"------------------------------------"<<std::endl;
    m1.Run();
    std::cout << "------------------------------------" << std::endl;
    m1.Run();
    return PASSED;
}

/* test_status_t ForthCaseTest()
{
    Observer o1;
    Observer2 o2;
    Observer3 o3;
    
    Callback<Event, Observer> cb1(&o1, &Observer::Notify, &Observer::Death);
    Callback<Event, Observer2> cb2(&o2, &Observer2::Notify, &Observer2::Death);
    Callback<Event, Observer3> cb3(&o3, &Observer3::Notify, &Observer3::Death);


    d1.Subscribe(&cb1);
    d1.Subscribe(&cb2);
    d1.Subscribe(&cb3);

    d1.Broadcast(Event());
    write(STDOUT_FILENO,"-----\n",7);
    d1.Broadcast(Event());

    sleep(2);

    return PASSED;
} */
