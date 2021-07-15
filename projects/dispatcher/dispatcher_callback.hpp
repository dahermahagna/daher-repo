#ifndef __RD94_DISPATCHER_CALLBACK_HPP__
#define __RD94_DISPATCHER_CALLBACK_HPP__

#include <list> //std::list
#include <boost/noncopyable.hpp> //boost::noncopyable
#include "../utils/utils.hpp"

namespace ilrd
{

template<typename EVENT> //EVENT must be copyable
class Dispatcher;

template<typename EVENT> //EVENT must be copyable
class CallbackBase 
{
public:
    explicit CallbackBase();
    virtual ~CallbackBase() noexcept;

private:

    friend class Dispatcher<EVENT>;

    virtual void Notify(EVENT event_) = 0;
    virtual void DispatcherDeath() = 0; 
    void SetDispatcher(Dispatcher<EVENT> *dispatcher_);

    void CallBackDeath();  //only destructor calls when callback dies
    Dispatcher<EVENT> *m_dispatcher; 
};


template<typename EVENT, typename OBSERVER> //EVENT must be copyable
class Callback: public CallbackBase<EVENT> // callback object should be bound to OBSERVER life cycle
{

public:
    typedef void(OBSERVER::*notify_func)(EVENT&);
    typedef void(OBSERVER::*notify_death_func)();
    
    explicit Callback(OBSERVER *observer_, 
                        notify_func notify_func_, 
                        notify_death_func dispatcher_death_func_);

    ~Callback() noexcept;

private:
    
    virtual void Notify(EVENT event_); //notify observer on event
    virtual void DispatcherDeath(); // action to be make when dispatcher is dead
    
    OBSERVER *m_observer;
    notify_func m_notify_func;
    notify_death_func m_dispatcher_death_func;
};


template<typename EVENT>
class Dispatcher: private boost::noncopyable
{
public:

    explicit Dispatcher();
    ~Dispatcher();
    void Subscribe(CallbackBase<EVENT> *call_back_);
    void Broadcast(EVENT event_);

private:
    friend class CallbackBase<EVENT>;
    void UnSubscribe(CallbackBase<EVENT> *call_back_);
    
    std::list<CallbackBase<EVENT>*> m_subscribers;
};

}// ilrd

#endif // __RD94_DISPATCHER_CALLBACK_HPP__