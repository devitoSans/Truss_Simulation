#include "setup.hpp"
#include "../simulation/simulation.hpp"

void Setup::run()
{
    set_current_window(this->appWindow);

    process_events();

    clear_screen(color_wheat());

    if(start_panel("Test", rectangle_from(this->windowWidth/2, this->windowHeight/2, 0.3*this->windowWidth, 0.3*this->windowHeight)))
    {
        static std::string s = "";
        s = text_box("Name", s);
        write_line(s);
        end_panel("Test");
    }


    draw_interface();
    refresh_screen();
}

bool Setup::drawButton(std::string label, double x, double y, double len)
{
    return button(label, rectangle_from(x-len/2, y-len/2,
                                        len, 20));
    
}

