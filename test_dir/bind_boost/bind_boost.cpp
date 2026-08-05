#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

void print_values(int a, int b, int c)
{
    std::cout << "Values: " << a << ", " << b << ", " << c << std::endl;
}
BOOST_STATIC_CONSTEXPR boost::arg<1> _1;
BOOST_STATIC_CONSTEXPR boost::arg<2> _2;
BOOST_STATIC_CONSTEXPR boost::arg<3> _3;

int main()
{
    boost::asio::io_context io;

    // Use boost::bind with placeholders _1, _2, _3
    auto bound_func = boost::bind(print_values, _1, _3, 1000);

    // Call the bound function directly with arguments
    bound_func(10, 20, 30);

    // Or post to asio with fixed arguments
    boost::asio::post(io, boost::bind(print_values, 1, 2, 3));
    std::cout<< "daher" << std::endl;
    io.run();

    return 0;
}