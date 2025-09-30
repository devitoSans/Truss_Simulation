#include "simulation.hpp"

void Simulation::run()
{
    set_current_window(this->appWindow);
    process_events();
    if(quit_requested())
    {
        this->quit = true;
    }

    clear_screen(random_color());

    refresh_screen();
}