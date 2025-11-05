#ifndef __SIMULATION__
#define __SIMULATION__

#include <vector>
#include "../component/componentControl.hpp"
#include "../component/member/memberController.hpp"
#include "../component/supports/supportController.hpp"
#include "../component/force/forceController.hpp"
#include "../joints/connectionManager.hpp"
#include "../action/action.hpp"
#include "../mathHelpers/NMatrix.hpp"
#include "../mathHelpers/linearEquations.hpp"
#include "../camera/camera.hpp"
#include "../sidebar/sidebar.hpp"
#include <cassert>

#define DEBUG_MODE false

#define RESOURCES_PATH (DEBUG_MODE ? "../resources/" : "resources/")

const int NUM_COMPONENT_TYPE = 3;

class Simulation
{
    private:
        double scale;
        // TODO: free this up
        ComponentController* components[NUM_COMPONENT_TYPE] = {
            new MultiMemberController(RESOURCES_PATH),
            new MultiSupportController(RESOURCES_PATH),
            new MultiForceController(RESOURCES_PATH)
        };

        ConnectionManager connectionManager;

        Sidebar sidebar;

        int screenWidth;
        int screenHeight;

        int focusedID;
        int prevInActionID;


    public:
        Simulation(double scale, int windowWidth, int windowHeight) 
            : connectionManager(), sidebar(windowWidth, windowHeight, RESOURCES_PATH), focusedID(-1), prevInActionID(-1)
        {
            this->scale = scale;
            this->screenWidth = windowWidth;
            this->screenHeight = windowHeight;
        }

        void update()
        {
            // camera.update();
            this->sidebar.update();

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
            else
            {
                ingfokan->set_content(nullptr);
            }
            this->connectionManager.update(this->components, NUM_COMPONENT_TYPE, this->prevInActionID);

            if(requestAction.click(key_down(SPACE_KEY), 
                                   ActionType::CALCULATE_FORCE, 
                                   ActionType::CALCULATE_FORCE) &&
                this->connectionManager.read_joints().size() > 0)
            {
                std::vector<std::pair<int,ForceType::type>> label;
                std::vector<std::vector<double>> v = this->connectionManager.convert_joints(this->components, NUM_COMPONENT_TYPE, label);
                if(v.size() == 0)
                {
                    label.clear();
                    return; 
                }
                
                std::vector<double> forces;
                NMatrix nmatrix = NMatrix(v);
                if(!solve_linear_equations(nmatrix, forces))
                {}

                for(int i = 0; i < label.size(); i++)
                {
                    for(int j = 0; j < NUM_COMPONENT_TYPE; j++)
                    {
                        int id = label[i].first;
                        if(this->components[j]->get_type(label[i].first) == ComponentType::NONE)
                        {
                            continue;
                        }
                        this->components[j]->set_forces(id, { ForceType::value {forces[i], label[i].second} });
                    }
                }
            }
            textboxManager.clear();
            checkbox_works.clear();
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
            sidebar.draw();
        }
};

#endif