#include <iostream>
#include <thread>
#include <string>
#include "game.h"
#include "client.h"

int main()
{
    using namespace NeonHockey;

    try
    {
        //start connection
        std::string ip;

        std::cout << "Enter IP address: ";
        std::cin >> ip;
        std::thread clientThread([ip]
        {
            constexpr int port = 14882;

            srand(time(nullptr));
            std::string playerName = "Vasja" + std::to_string(rand());

            if(!Client::getInstance().connect(ip, port, playerName))
                std::cerr << "Unable to connect to: " << ip << ":" << port << std::endl;
        });

        //wait for second client
        while(!Client::getInstance().isGameStarted())
        {
            if(Client::getInstance().shouldStop())
            {
                clientThread.join(); //wait for it to die
                return 1;
            }
        };


        //Game& game = Game::getInstance();
        Game::start();

        Client::getInstance().stop();

        clientThread.join();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
