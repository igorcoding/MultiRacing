#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

int main()
{
    using namespace boost::asio;

    io_service service;
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 14882);
    ip::tcp::socket sock(service);
    sock.open(ip::tcp::v4());
    sock.connect(ep);

    sock.write_some(buffer("Hello, from client!"));

    /*boost::asio::streambuf buffer;
    boost::system::error_code err;
    read_until(sock, buffer, "\r\n\r\n", err);

    std::istream str(&buffer);
    std::string result;

    while(!str.eof())
    {
        str >> result;
        std::cout << result << std::endl;
    }*/

    sock.shutdown(ip::tcp::socket::shutdown_receive);
    sock.close();
}

