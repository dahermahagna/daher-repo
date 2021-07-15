#ifndef __DISPATCHER_CALLBACK_IMPL_HPP__
#define __DISPATCHER_CALLBACK_IMPL_HPP__

#include <algorithm> // foreach
#include <cassert>   //assert
#include "dispatcher_callback.hpp" //dispatcher_callback

namespace ilrd // callbackbase
{

template<typename EVENT>
CallbackBase<EVENT>::CallbackBase()
:m_dispatcher(NULL)
{

}

template<typename EVENT>
CallbackBase<EVENT>::~CallbackBase() noexcept
{
    if (NULL != m_dispatcher)
    {
        CallBackDeath();
    }
}
template <typename EVENT>
void CallbackBase<EVENT>::SetDispatcher(Dispatcher<EVENT> *dispatcher_)
{
    m_dispatcher = dispatcher_;
}

template <typename EVENT>
void CallbackBase<EVENT>::CallBackDeath()
{
    
    m_dispatcher->UnSubscribe(this);

}

}//callbackbase

namespace ilrd // callback
{

template<typename EVENT, typename OBSERVER>
Callback<EVENT,OBSERVER>::Callback(OBSERVER *observer_, notify_func notify_func_, notify_death_func dispatcher_death_func_)
:m_observer(observer_),
m_notify_func(notify_func_),
m_dispatcher_death_func(dispatcher_death_func_)
{
    assert(NULL != observer_);
    assert(NULL != notify_func_);
    assert(NULL != dispatcher_death_func_);
}

template<typename EVENT, typename OBSERVER>
Callback<EVENT,OBSERVER>::~Callback() noexcept
{

}

template <typename EVENT, typename OBSERVER>
void Callback<EVENT,OBSERVER>::Notify(EVENT event_)
{
    (m_observer->*m_notify_func)(event_);
}

template <typename EVENT, typename OBSERVER>
void Callback<EVENT, OBSERVER>::DispatcherDeath()
{
    (m_observer->*m_dispatcher_death_func)();

}

}//callback

namespace ilrd//dispatcher
{

template<typename EVENT>
Dispatcher<EVENT>::Dispatcher()
:m_subscribers()
{
}

template <typename EVENT>
Dispatcher<EVENT>::~Dispatcher()
{
    typename std::list<CallbackBase<EVENT> *>::iterator iter;
    for (iter = m_subscribers.begin(); iter != m_subscribers.end(); ++iter)
    {
        (*iter)->DispatcherDeath();
        (*iter)->SetDispatcher(NULL);

    }
}

template <typename EVENT>
void Dispatcher<EVENT>::Subscribe(CallbackBase<EVENT> *call_back_)
{
    call_back_->SetDispatcher(this);
    m_subscribers.push_front(call_back_);
}

template <typename EVENT>
void Dispatcher<EVENT>::Broadcast(EVENT event_)
{
    typename std::list<CallbackBase<EVENT>*>::iterator iter;
    for(iter = m_subscribers.begin(); iter != m_subscribers.end(); ++iter)
    {
        (*iter)->Notify(event_);
        if (m_subscribers.empty())
        {
            break;
        }
    } 
}


template <typename EVENT>
void Dispatcher<EVENT>::UnSubscribe(CallbackBase<EVENT> *call_back_)
{
    assert(NULL != call_back_);
    m_subscribers.remove(call_back_);
}

}//dispatcher


#endif // __DISPATCHER_CALLBACK_IMPL_HPP__