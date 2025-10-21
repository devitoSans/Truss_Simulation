#include "app/app.hpp"

int main()
{
    App app = App();
    Simulation simulation = Simulation();
    
    while(!app.isQuit())
    {
        app.run(simulation);
    }

    return 0;
}