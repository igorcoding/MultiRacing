#include "game.h"
#include "client.h"
#include <iostream>
#include <thread>


int main()
{
    using namespace NeonHockey;
    try
    {
        //start connection
        std::thread clientThread([]
        {
            Client::getInstance().connect("127.0.0.1", 14882);
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
