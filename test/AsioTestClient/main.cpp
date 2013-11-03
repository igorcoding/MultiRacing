#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

int main()
{
    using namespace boost::asio;

    try
    {
        io_service service;
        ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 14882);
        ip::tcp::socket sock(service);
        sock.open(ip::tcp::v4());
        sock.connect(ep);

        sock.write_some(buffer("Hello, from client!\n"));

        boost::asio::streambuf buffer;
        boost::system::error_code err;
        read_until(sock, buffer, "\n", err);

        std::istream is(&buffer);
        std::string clientId;
        is >> clientId;

        std::cout << "Client id: " << clientId << std::endl;

        sock.shutdown(ip::tcp::socket::shutdown_receive);
            sock.close();
    }
    catch(std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Unexpected error" << std::endl;
    }

    std::cin.peek(); //prevent from Qt
}

