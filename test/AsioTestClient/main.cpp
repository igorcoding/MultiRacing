#include <iostream>
#include <locale>
#include <thread>
#include <cstdlib>
#include "client.h"

int main()
{
    std::setlocale(LC_ALL, "Russian");

    //start connection
    std::thread clientThread([]
    {
        Client::getInstance().connect("127.0.0.1", 14882);
    });

    //wait for second client
    while(!Client::getInstance().isGameStarted()) {};

    //somewhere in the frameFunc()
    for(int i = 0; i < 10; ++i)
    {
        //onMouseMove:
        Client::getInstance().sendPaddleCoords(i*10, i*20);

        int x = 0, y = 0;
        Client::getInstance().getEnemyPaddleCoords(x, y);
        //save 'em in the model, use in render
        //... or just call that function in the render,
        //cuz it just immediately returns cached coords

        std::this_thread::sleep_for(std::chrono::milliseconds(rand()%1000 + 500));
    }

    clientThread.join(); //main thread should not be stopped before child ones do
}

