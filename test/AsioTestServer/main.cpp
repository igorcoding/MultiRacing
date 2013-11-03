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
        boost::system::error_code error;

        std::vector<tcp::socket> clientSockets;
        const int clientsNeeded = 2;

        for(int clientId = 0; clientId < clientsNeeded; ++clientId)
        {
            //get client
            tcp::socket socket(io_service);

            acceptor.accept(socket, error);

            //read it's name
            boost::asio::streambuf buffer;
            read_until(socket, buffer, "\n", error);

            std::istream is(&buffer);
            std::string clientName;
            std::getline(is, clientName, '\n');

            //send it's id
            std::string clientIdStr = std::to_string(clientId) + "\n";
            socket.send(boost::asio::buffer(clientIdStr));

            std::cout << "Client connected:" << std::endl
                      << " Name: " << clientName << std::endl
                      << " IP: " << socket.remote_endpoint().address() << std::endl
                      << " ID: " << clientId << std::endl << std::endl;

            clientSockets.emplace_back(std::move(socket));
        }

        //have fun
        //...

        //free clients
        for(auto &socket: clientSockets)
        {
            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
            socket.close();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
