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
#include <cassert>

const int NUM_COMPONENT_TYPE = 3;

class Simulation
{
    private:
        double scale;
        // TODO: free this up
        ComponentController* components[NUM_COMPONENT_TYPE] = {
            new MultiMemberController("../resources/"),
            new MultiSupportController("../resources/"),
            new MultiForceController("../resources/")
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

            if(requestAction.click(key_down(SPACE_KEY), 
                                   ActionType::CALCULATE_FORCE, 
                                   ActionType::CALCULATE_FORCE) &&
                this->connectionManager.read_joints().size() > 0)
            {
                std::vector<std::pair<int,ForceType::type>> label;
                std::vector<std::vector<double>> v = this->connectionManager.convert_joints(this->components, NUM_COMPONENT_TYPE, label);
                // std::vector<std::vector<double>> v = {};
                fprintf(stderr, "size: %d\n", v.size());
                if(v.size() == 0)
                {
                    label.clear();
                    printf("Truss design is indeterminate!\n");  
                    return; 
                }

                
                // printf("Calculating...\n");
                // for(auto& i : v)
                // {
                //     for(auto& j : i)
                //     {
                //         printf("%f, ", j);
                //     }
                //     printf("\n");
                // }
                
                std::vector<double> forces;
                NMatrix nmatrix = NMatrix(v);
                if(!solve_linear_equations(nmatrix, forces))
                {
                    printf("Truss design is indeterminate!\n");
                }

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
                // printf("forces: ");
                // for(auto& i : forces)
                // {
                //     printf("%f, ", i);
                // }
                // printf("\n");
                // printf("label: ");
                // for(auto& [id, forceType] : label)
                // {
                //     printf("%d -- %d, ", id, forceType);
                // }
                // printf("\n");
            }
        }

        // void calculate()
        // {
            // const JointList& joints = this->connectionManager.read_joints();
            
            // int unknownVariablesTotal = 0;
            // std::set<int> temp;
            // for(auto& [_, joint] : joints)
            // {
            //     for(auto& connection : joint)
            //     {
            //         for(auto)
            //     }
            // }
            // unknownVariablesTotal = temp.size();

            // printf("uv: %d, j: %d\n", unknownVariablesTotal, joints.size());
            // if(unknownVariablesTotal < 2*joints.size())
            // {
            //     printf("Truss design is not in equilibrium state!\n");
            //     return;
            // }
            // if(unknownVariablesTotal > 2*joints.size())
            // {
            //     printf("Truss design is indeterminate!\n");
            //     return;
            // }
            // printf("Calculating...\n");
        // }

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