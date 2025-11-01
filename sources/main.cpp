#include "app/app.hpp"

int main()
{
    App app = App();
    Simulation simulation = Simulation(5.0);
    
    while(!app.isQuit())
    {
        app.run(simulation);
    }

    return 0;
}