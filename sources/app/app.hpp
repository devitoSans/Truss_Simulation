#ifndef __APP__
#define __APP__

#include <splashkit.h>
#include <string>

class App
{
    protected:
        window appWindow;
        bool quit;
        double windowHeight;
        double windowWidth;

    public: 
        App(std::string title="Truss Simulation", int width=640, int height=480, bool isFullScreen=false);
        void run();
        bool isQuit();
        window getWindow();
};

#endif