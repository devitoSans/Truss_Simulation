#include "app/app.hpp"

int main()
{
    App app = App();
    
    while(!app.isQuit())
    {
        app.run();
    }

    return 0;
}