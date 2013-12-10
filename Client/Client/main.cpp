#include <iostream>
#include <thread>
#include <string>
#include "game.h"
#include "client.h"

int main()
{
    using namespace NeonHockey;
    Game::start();
    /*try
    {
        std::thread clientThread([]
        {
            //start connection
            std::string ip;
            bool connected = false;

            do
            {
                std::cout << "Enter IP address: ";
                std::cin >> ip;
                if (ip == "0")
                    ip = "127.0.0.1";

                constexpr int port = 14882;

                srand(time(nullptr));
                std::string playerName = "Vasja" + std::to_string(rand());

                int retriesCount = 0;

                do
                {
                    connected = Client::getInstance()
                            .connect(ip, port, playerName);

                    if(!connected)
                    {
                        std::cerr << "Unable to connect to: "
                                  << ip << ":" << port
                                  << " (retry: " << retriesCount + 1 << ")"
                                  << std::endl << std::endl;

                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        retriesCount++;
                    }
                }
                while(!connected && retriesCount < 3);

                if(!connected)
                    std::cerr << "Please, re-enter ip address" << std::endl << std::endl;
            }
            while(!connected);
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
    */
}
