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
        App(std::string, int, int, bool);
        void run();
        bool isQuit();
        window getWindow();
};

#endif