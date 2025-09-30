#ifndef __SIMULATION__
#define __SIMULATION__

#include "../app.hpp"

class Simulation : public App
{
    public:
        Simulation() : App("Test", 640, 480, false) {};
        void run();
};

#endif