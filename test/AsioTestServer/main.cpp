#include <iostream>
#include <locale>
#include "server.h"

int main()
{
    std::setlocale(LC_ALL, "Russian");

    Server::getInstance().start();
}


