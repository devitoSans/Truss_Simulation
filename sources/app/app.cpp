#include "app.hpp"
// #include "userInput.hpp"

App::App(std::string title, int width, int height, bool isFullScreen)
{
    this->appWindow = open_window(title, width, height);
    if(isFullScreen)
    {
        current_window_toggle_fullscreen();
    }

    this->windowWidth = current_window_width();
    this->windowHeight = current_window_height();
    this->quit = false;
}

void App::run(Simulation& simulation)
{
    set_current_window(this->appWindow);

    process_events();
    simulation.IO();
    simulation.update();

    clear_screen(color_white());
    simulation.draw();

    draw_interface();
    refresh_screen(60);
}

bool App::isQuit()
{
    return this->quit || quit_requested();
}

window App::getWindow()
{
    return this->appWindow;
}