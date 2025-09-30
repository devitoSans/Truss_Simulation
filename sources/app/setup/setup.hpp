#ifndef __SETUP__
#define __SETUP__

#include "../app.hpp"
#include <string>

class Setup : public App
{
    public:
        Setup() : App("Setup", 480, 360, false) {};
        void run();
        bool drawButton(std::string, double, double, double);
};

#endif