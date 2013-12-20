#include <iostream>
#include <locale>
#include "server.h"

void startServer(int port)
{
    bool started = Server::getInstance().start(port);

    if(!started)
        std::cout << "Unable to start server on port " << port << "." << std::endl
                  << "Use 'starton <port>' to start on different port or 'exit' to exit" << std::endl;
    else
        std::cout << "Server started." << std::endl;
}

int main()
{
    std::setlocale(LC_ALL, "Russian");

    std::cout << "NeonHockey v0.2 Server" << std::endl << std::endl;
    std::cout << "Supported commands: {start, starton, stop, restart, exit}" << std::endl;
    std::cout << std::endl;
    int port = 14882;
    std::string command;

    startServer(port);

    do
    {
        std::cin >> command;

        if(command == "start")
        {
            startServer(port);
        }
        else if(command == "starton")
        {
            //restart on different port
            std::cin >> port;
            startServer(port);
        }
        else if(command == "restart")
        {
            Server::getInstance().stop();
            std::cout << "Server stopped." << std::endl;

            startServer(port);
        }
        else if(command == "stop" || command == "exit")
        {
            Server::getInstance().stop();
            std::cout << "Server stopped." << std::endl;
        }
        else
            std::cout << "Unrecognized command, please reenter." << std::endl;
    }
    while(command != "exit");
}


