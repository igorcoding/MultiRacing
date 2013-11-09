#include <iostream>
#include <locale>
#include "client.h"

int main()
{
    std::setlocale(LC_ALL, "Russian");

    Client::getInstance().connect("127.0.0.1", 14882);
}

