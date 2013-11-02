#include <iostream>
#include <string>
#include <sstream>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


int main()
{
    try
    {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 14882));

        //for client:
        tcp::socket socket(io_service);
        tcp::endpoint client;
        boost::system::error_code error;

        acceptor.accept(socket, client, error);

        boost::asio::streambuf buffer;
        boost::system::error_code err;
        read_until(socket, buffer, "\0", err);

        std::istream str(&buffer);
        std::string result;

        while(!str.eof())
        {
            str >> result;
            std::cout << result << std::endl;
        }

        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
        socket.close();

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
