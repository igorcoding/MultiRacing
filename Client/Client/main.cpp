#include <iostream>
#include <thread>
#include <string>
#include "game.h"
#include "client.h"
#include "exceptions.h"

int main()
{
    try
    {
        using namespace NeonHockey;
        Game::start();
    }
    catch(ResourceException &e)
    {
        std::cout << "Critical Error. Unable to load resources. Please read NeonHockey.log for details";
    }
    catch(...)
    {
        std::cout << "Unknown critical Error. Please read NeonHockey.log for details";
    }
}
