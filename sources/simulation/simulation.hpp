#ifndef __SIMULATION__
#define __SIMULATION__

#include <vector>
#include "../component/componentControl.hpp"
#include "../component/member/memberController.hpp"
#include "../component/supports/supportController.hpp"
#include "../component/force/forceController.hpp"
#include "../joints/connectionManager.hpp"
#include "../action/action.hpp"
#include <cassert>

const int NUM_COMPONENT_TYPE = 3;

class Simulation
{
    private:
        double scale;
        // TODO: free this up
        ComponentController* components[NUM_COMPONENT_TYPE] = {
            new MultiMemberController(),
            new MultiSupportController(),
            new MultiForceController()
        };

        int focusedID;
        int prevInActionID;

        ConnectionManager connectionManager;

    public:
        Simulation(double scale) : connectionManager(), focusedID(-1), prevInActionID(-1)
        {
            this->scale = scale;
        }

        void update()
        {
            for(int i = 0; i < NUM_COMPONENT_TYPE; i++)
            {
                if(this->components[i]->get_type(this->focusedID) != ComponentType::NONE || this->focusedID == -1)
                {
                    this->focusedID = this->components[i]->update(true);
                }
                else
                {
                    int temp = this->components[i]->update(false);
                    this->focusedID = (temp == -1 ? this->focusedID : temp);
                }
            }
            if(this->focusedID != -1)
            {
                this->prevInActionID = this->focusedID;
            }
            this->connectionManager.update(this->components, NUM_COMPONENT_TYPE, this->prevInActionID);
            if(requestAction.click(key_down(KEYPAD_ENTER), (ActionType::ActionType)-1234, -1234))
            {
                this->calculate();
            }
        }

        void calculate()
        {
            
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