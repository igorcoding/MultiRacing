#include "game.h"
#include "client.h"
#include <iostream>
#include <thread>


int main()
{
    using namespace NeonHockey;
    std::cout << sizeof(float);
    try
    {
        //start connection
        std::string ip = "";
        std::cout << "Enter IP address: ";
        std::cin >> ip;
        std::thread clientThread([&ip]
        {
            //"172.16.55.161"
            Client::getInstance().connect(ip, 14882);
        });

        //wait for second client
        while(!Client::getInstance().isGameStarted()) {};


        Game& game = Game::getInstance();
        game.start();


        clientThread.join();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
