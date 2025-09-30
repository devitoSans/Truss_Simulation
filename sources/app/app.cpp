#include "app.hpp"

App::App(std::string title, int width, int height, bool isFullScreen)
{
    this->appWindow = open_window(title, width, height);
    this->windowWidth = width; 
    this->windowHeight = height;
    this->quit = false;

    if(isFullScreen)
    {
        current_window_toggle_fullscreen();
        this->windowWidth = current_window_width();
        this->windowHeight = current_window_height();
    }
}

void App::run()
{
    set_current_window(this->appWindow);
    refresh_screen();
    process_events();
}

bool App::isQuit()
{
    return this->quit || quit_requested();
}

window App::getWindow()
{
    return this->appWindow;
}