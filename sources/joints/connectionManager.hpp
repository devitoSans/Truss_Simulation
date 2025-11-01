#ifndef __JOINTS_LIST__
#define __JOINTS_LIST__

#include <map>
#include <utility>
#include <vector>
#include <set>
#include <splashkit.h>
#include "../action/action.hpp"
#include "../mathHelpers/mathHelpers.hpp"
#include "../component/componentControl.hpp"
#include "../component/member/memberController.hpp"
#include "../component/supports/supportController.hpp"
#include "../component/force/forceController.hpp"

typedef std::map<int, std::set<Connection>> JointList;

class ConnectionManager
{
    private:
        // key: joint's id
        JointList joints;
        std::map<Connection,int> r_joints; // as a lookup table to easy to search which joint a component's part is connected to

        bool isReady;

        int get_focused_component_index(ComponentController** components, int numComponentType, int focusedID)
        {
            ComponentType::ComponentType type = ComponentType::NONE;
            int index = -1;
            for(int i = 0; i < numComponentType; i++)
            {
                if(type == ComponentType::NONE)
                {
                    type = components[i]->get_type(focusedID);
                    index = i;
                }
            }
            if(type == ComponentType::NONE) { index = -1; }
            return index;
        }

        bool is_connected(const Connection& connection) const
        {
            if(this->r_joints.find(connection) == this->r_joints.end())
            {
                return false;
            }
            return true;
        }

        void create_joint(Connection& c1, Connection& c2)
        {
            if(this->is_connected(c1) || this->is_connected(c2))
            {
                return;
            }
            int jointId = mh_random(0,MAX_ID);

            this->joints.emplace(jointId, std::set({c1, c2}));
            this->r_joints.emplace(c1, jointId);
            this->r_joints.emplace(c2, jointId);
        }

        void transfer_connection(Connection& from, Connection& to)
        {
            if(!this->is_connected(to))
            {
                fprintf(stderr, "Warning: Attempting to transfer to non-existing joint.");
                return;
            }
            this->clear_connection(from);

            int toJoint = this->r_joints.at(to);

            this->r_joints[from] = toJoint;
            this->joints.at(toJoint).insert(from);
        }

        void clear_connection(Connection& connection)
        {
            if(!this->is_connected(connection))
            {
                return;
            }
            int atJoint = this->r_joints.at(connection);

            this->r_joints.erase(connection);
            auto& temp = this->joints.at(atJoint);
            temp.erase(connection);

            if(temp.size() <= 1)
            {
                for(auto& i : temp)
                {
                    this->r_joints.erase(i);
                }
                this->joints.erase(atJoint);
            }
        }

        std::vector<Part::PartType> get_part(int id)
        {
            std::vector<Part::PartType> parts;
            for(auto& [connection, _] : this->r_joints)
            {
                if(connection.first == id)
                {
                    parts.push_back(connection.second);
                }
            }
            return parts;
        }

        void change_connection(Connection& connection1, Connection& connection2) 
        {
            if(this->is_connected(connection1))
            {
                this->transfer_connection(connection2, connection1);
            }
            else if(!this->is_connected(connection1) && !this->is_connected(connection2))
            {
                this->create_joint(connection1, connection2);
            }
            else
            {
                this->transfer_connection(connection1, connection2);
            }
        }

        ComponentController* extract_component(int id, ComponentController** components, int numComponentType)
        {
            ComponentController* componentController = nullptr;
            for(int i = 0; i < numComponentType; i++)
            {
                if(components[i]->get_type(id) != ComponentType::NONE)
                {
                    componentController = components[i];
                    break;
                }
            }
            if(componentController == nullptr)
            {
                fprintf(stderr, "Warning: Attempting to extract a non-existing component. ID is not found.");
            }
            return componentController;
        }

    public:
        ConnectionManager() : isReady(true) {}

        const JointList& read_joints() const
        {
            return this->joints;
        }

        std::vector<std::vector<double>> convert_joints(ComponentController** components, int numComponentType, std::vector<std::pair<int,ForceType::type>>& label)
        {
            int numOfEquation = 2*this->joints.size();
            std::vector<std::vector<double>> matrix(numOfEquation, 
                std::vector<double>(numOfEquation+1, 0)
            );

            int currentRow = 0, currentCol = 0;
            std::map<int,int> mapping;

            label.resize(numOfEquation, std::pair<int,ForceType::type>(0,ForceType::AXIAL));

            for(auto& [_, joint] : this->joints)
            {
                for(auto& connection : joint)
                {
                    auto [id, _] = connection;
                    ComponentController* cc = this->extract_component(id, components, numComponentType);

                    // extract each component's part's angles
                    for(auto& [angle, forceType] : cc->get_part_angles(connection))
                    {
                        // Force is not unknown variable
                        if(cc->get_type() == ComponentType::FORCE)
                        {
                            matrix[currentRow][numOfEquation] -= cc->get_forces(connection)[0] * std::sin(TO_RAD(angle));
                            matrix[currentRow+1][numOfEquation] -= cc->get_forces(connection)[0] * std::cos(TO_RAD(angle));
                            continue;
                        }

                        // Support 
                        if(cc->get_type() == ComponentType::SUPPORT)
                        {
                            matrix[currentRow][currentCol] = std::sin(TO_RAD(angle));
                            matrix[currentRow+1][currentCol] = std::cos(TO_RAD(angle));

                            // labeling
                            int id_ = id;
                            ForceType::type type_ = forceType;
                            label[currentCol] = { id_, type_ };
                            // printf("label: %d", labe/l[currentCol].first);
                            
                            currentCol++;
                            continue;
                        }
                    
                        // Check if a particular member is already been considered
                        // and assigned to the matrix
                        if(mapping.find(id) == mapping.end())
                        {
                            matrix[currentRow][currentCol] = std::sin(TO_RAD(angle));
                            matrix[currentRow+1][currentCol] = std::cos(TO_RAD(angle));

                            // labeling
                            int id_ = id;
                            ForceType::type type_ = forceType;
                            label[currentCol] = { id_, type_ };

                            // Save its id
                            mapping[id] = currentCol; currentCol++;

                            // Unknown variables (reaction forces, and members' forces)
                            // are greater than number of equations (twice of the joints).
                            // Therefore, system is indeteriminant.
                            if(currentCol > numOfEquation) { return {}; }

                            continue;
                        }
                        else
                        {
                            int col = mapping.at(id);
                            matrix[currentRow][col] = std::sin(TO_RAD(angle));
                            matrix[currentRow+1][col] = std::cos(TO_RAD(angle));
                            continue;
                        }
                    }
                }
                currentRow += 2;
            }

            return matrix;
        }

        bool update(ComponentController** components, int numComponentType, int focusedID)
        // no action is taken.
        // there must be a component in focused/selected.
        // only done once.
        // otherwise, cannot update components' connection
        {
            if(!(requestAction.is_in_action(-1, ActionType::NONE) && focusedID != -1))  
            { 
                this->isReady = true;
                return false;
            }
            if(!this->isReady) { return false; }
            this->isReady = false;

            int focusedIndex = this->get_focused_component_index(components, numComponentType, focusedID);
            if(focusedIndex == -1)
            {
                for(auto& i : this->get_part(focusedID))
                {
                    Connection temp = {focusedID, i};
                    this->clear_connection(temp);
                }
            }
            else 
            {
                std::vector<Hitbox> focusedComponentHitboxes = components[focusedIndex]->get_hit_box(focusedID);
    
                for(auto& hitbox : focusedComponentHitboxes)
                {
                    Connection focusedConnection = Connection {focusedID, hitbox.type};
                    this->clear_connection(focusedConnection);
                    for(int i = 0; i < numComponentType; i++)
                    {
                        std::vector<Connection> intersection = components[i]->get_intersection(hitbox.shape.center.x, 
                                                            hitbox.shape.center.y, 
                                                            hitbox.shape.radius);
                        
                        for(auto& connection : intersection)
                        {
                            // printf("1. connection id: %d, focused id: %d\n", connection.first, focusedID);
                            if(connection.first == focusedID)
                            {
                                continue;
                            }
                            // printf("2. connection id: %d, focused id: %d\n", connection.first, focusedID);
    
                            this->change_connection(focusedConnection, connection);
                        }
                    }
                }
            }

            // for(auto& j : this->joints)
            // {
            //     printf("joint: %d -> ", j.first);
            //     for(auto& c : j.second)
            //     {
            //         printf("%d, ", c.first);
            //     }
            //     printf("\n");
            // }
            // printf("\n\n");
            return true;
        }
};

#endif