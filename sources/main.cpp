#include "app/setup/setup.hpp"

int main()
{
    Setup setup = Setup();
    
    while(!setup.isQuit())
    {
        setup.run();
    }

    return 0;
}