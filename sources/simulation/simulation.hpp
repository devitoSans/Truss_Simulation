#ifndef __SIMULATION__
#define __SIMULATION__

#include <vector>
#include "../component/componentControl.hpp"
#include "../component/member/memberController.hpp"
#include "../component/supports/supportController.hpp"
#include "../component/force/forceController.hpp"

const int NUM_COMPONENT_TYPE = 3;

class Simulation
{
    private:
        double scale;
        ComponentController* components[3] = {
            new MultiMemberController(),
            new MultiSupportController(),
            new MultiForceController()
        };

    public:
        Simulation(double scale)
        {
            this->scale = scale;
        }

        void update()
        {
            static int focusedID = -1;
            for(int i = 0; i < NUM_COMPONENT_TYPE; i++)
            {
                if(this->components[i]->get_type(focusedID) != ComponentType::NONE || focusedID == -1)
                {
                    focusedID = this->components[i]->update(true);
                }
                else
                {
                    int temp = this->components[i]->update(false);
                    focusedID = (temp == -1 ? focusedID : temp);
                }
            }
        }

        void IO()
        {
            // if((key_down(LEFT_CTRL_KEY) || key_down(RIGHT_CTRL_KEY)) && key_down(EQUALS_KEY)) // plus key
            // {
            //     this->scale++;
            // }
            // else if((key_down(LEFT_CTRL_KEY) || key_down(RIGHT_CTRL_KEY)) && key_down(MINUS_KEY))
            // {
            //     this->scale--;
            // }
            // this->scale = std::max(this->scale, 1.0);

            // double speed = 2;
            // if(key_down(UP_KEY))
            // {
            //     this->components.move_all_components_by(0, speed);
            // }
            // if(key_down(RIGHT_KEY))
            // {
            //     this->components.move_all_components_by(-speed, 0);
                
            // }
            // if(key_down(LEFT_KEY))
            // {
            //     this->components.move_all_components_by(speed, 0);
                
            // }
            // if(key_down(DOWN_KEY))
            // {
            //     this->components.move_all_components_by(0, -speed);
            // }
        }

        void draw()
        {
            for(int i = 0; i < NUM_COMPONENT_TYPE; i++)
            {
                this->components[i]->draw(this->scale);
            }
        }
};

#endif