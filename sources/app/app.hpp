#ifndef __APP__
#define __APP__

#include <splashkit.h>
#include <string>
#include "../simulation/simulation.hpp"
// #include "../component/member/memberInterface.hpp"
// #include "userInput..hpp"

// class UserInput
// {
//     private:
//         ButtonLogic createMemberButton;

//     public:
//         UserInput() { this->createMemberButton = ButtonLogic(); }

//         void create_new_member(App& app)
//         {
//             // if(this->createMemberButton.isPressed(key_down(M_KEY)))
//             // {
//             //     MemberInterface* Imember = new MemberInterface(100, 2);
//             //     Imember->modify_mid_pos(mouse_position().x, mouse_position().y);
//             //     app.add_member(Imember);
//             // }
//         }
// };

class App
{
    protected:
        window appWindow;
        bool quit;
        double windowHeight;
        double windowWidth;

    public: 
        App(std::string title="Truss Simulation", int width=640, int height=480, bool isFullScreen=false);
        void run(Simulation& simulation);
        bool isQuit();
        window getWindow();
};

#endif