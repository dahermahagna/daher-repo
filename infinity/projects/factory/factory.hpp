#ifndef __RD94_FACTORY_HPP__
#define __RD94_FACTORY_HPP__

#include <boost/unordered_map.hpp> // unordered map
#include <boost/noncopyable.hpp> //noncopyable
#include <string> //string
#include <boost/function.hpp> // function
#include <exception> //exception
#include <boost/any.hpp> // boost::any

namespace ilrd
{

template<typename BASE, typename ARG, typename ID >
class Factory: private boost::noncopyable
{
public:
    //generated OK

    void Add(ID class_id_, 
    boost::function< boost::shared_ptr<BASE> (ARG) > create_func_); //overide existing func

    boost::shared_ptr<BASE> Create(ID class_id_, ARG arg_); // throws NotExistException
    

private:
    typedef boost::unordered_map< ID , boost::function < boost::shared_ptr<BASE> (ARG) > > FunctionMap_t;

    FunctionMap_t m_map;

    struct WrongKey : public std::exception
    {
        const char * what () const throw ()
        {
            return "Key is unknown";
        }
    };
    
};

}// ilrd

 
#endif // __RD94_FACTORY_HPP__