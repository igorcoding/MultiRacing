#include <iostream>
#include <locale>
#include "server.h"

int main()
{
    std::setlocale(LC_ALL, "Russian");

    while(true) //restart on error
        Server::getInstance().start();
}


